

#define DELTA_T 0.004f
#define PI 3.14159265359f
#define SOLVER_ITERS 4
#define KERNEL_RADIUS 0.1001f
#define EPS_T 600.f
#define REST_DENSITY 6378.f
#define PRESSURE_STRENGTH 0.000001f
#define PRESSURE_POW 4.f;
#include "CudaPBDSolver.cuh"

const float VISCOSITY = 0.01;

const float SURFACE_OFFSET = 0.000001;

using namespace glm;

__device__ inline float poly6(glm::vec3 r_ij, float h);

__device__ inline float spiky(glm::vec3 r_ij, float h);

__device__ inline glm::vec3 spiky_grad(glm::vec3 r_ij, float h);

__device__ float rho_i(Particle *p_i, Particle *particles);

__device__ float lambda_i(Particle *p_i, Particle *particles);

__device__ float C_i(Particle *p_i, Particle *particles);

__device__ glm::vec3 grad_k_Ci(Particle *p_k, Particle *p_i, Particle *particles);

__device__ glm::vec3 delta_p(Particle *p_i, Particle *particles);

__device__ glm::vec3 vorticity(Particle *p_i, Particle *particles);

__device__ glm::vec3 f_vorticity(Particle *p_i, Particle *particles);

__device__ glm::vec3 XSPH_vel(Particle *p_i, Particle *particles);

__device__ void triangle_collide(Triangle &t, Particle &p);

__device__ void plane_collide(Plane &p, Particle &par);

__global__ void seed_position(Particle *particles, int n) {
    int index = threadIdx.x + blockIdx.x * blockDim.x;
    if (index >= n) {
        return;
    }
    Particle &p = particles[index];
    p.f = glm::vec3(0, -9.8f, 0) * p.m;
    p.v = p.v + p.f * (1.f / p.m) * DELTA_T;
    p.pred_p = p.p + p.v * DELTA_T;
}

__global__ void run_solver(Particle *particles, int n, Triangle *triangles, int num_triangles, Plane *planes, int num_planes) {
    int index = threadIdx.x + blockIdx.x * blockDim.x;
    if (index >= n) {
        return;
    }
    Particle &p = particles[index];

    for (int i = 0; i < SOLVER_ITERS; ++i) {
        __syncthreads();
        p.lambda = lambda_i(&p, particles);
        __syncthreads();

        p.dp = delta_p(&p, particles);
        p.pred_p += p.dp;
        for (int i = 0; i < num_triangles; ++i) {
            triangle_collide(triangles[i], p);
        }
        for (int i = 0; i < num_planes; ++i) {
            plane_collide(planes[i], p);
        }
    }

    if (!p.collided)
        p.v = (1.f / DELTA_T) * (p.pred_p - p.p);
    __syncthreads();

    p.f += f_vorticity(&p, particles);
    p.v = XSPH_vel(&p, particles);
    p.collided = false;
    p.p = p.pred_p;
    p.num_neighbors = 0;
}

__device__ float poly6(glm::vec3 r_ij, float h) {
    float r = length(r_ij);
    return 0 <= r && r <= h ? 315 / (64 * PI * pow(h, 9)) * pow(h * h - r * r, 3) : 0;
}

__device__ float spiky(glm::vec3 r_ij, float h) {
    float r = length(r_ij);
    return 0 <= r && r <= h ? 15 / (PI * pow(h, 6)) * pow(h - r, 3) : 0;
}

__device__ glm::vec3 spiky_grad(glm::vec3 r_ij, float h) {
    float r = length(r_ij);
    if (0 < r && r <= h) {
        glm::vec3 d = normalize(r_ij);
        float coeff = 45 / (PI * pow(h, 6)) * pow(h - r, 2);
        return coeff * d;
    } else {
        return glm::vec3();
    }
}

__device__ float rho_i(Particle *p_i, Particle *particles) {
    float rho = 0;
    for (int i = 0; i < p_i->num_neighbors; ++i) {
        int n = p_i->neighborhood[i];
        Particle *p_j = &particles[n];
        rho += p_j->m * poly6(p_i->pred_p - p_j->pred_p, KERNEL_RADIUS);
    }
    return rho;
}

__device__ float lambda_i(Particle *p_i, Particle *particles) {
    float lambda = 0;
    for (int i = 0; i < p_i->num_neighbors; ++i) {
        int n = p_i->neighborhood[i];
        Particle *p_k = &particles[n];
        glm::vec3 grad_k_ci = grad_k_Ci(p_k, p_i, particles);
        lambda += pow(length(grad_k_ci), 2);
    }
    lambda = -C_i(p_i, particles) * (1 / (lambda + EPS_T));
    return lambda;
}

__device__ float C_i(Particle *p_i, Particle *particles) {
    return rho_i(p_i, particles) / REST_DENSITY - 1;
}

__device__ glm::vec3 grad_k_Ci(Particle *p_k, Particle *p_i, Particle *particles) {
    if (p_k == p_i) {
        glm::vec3 grad_spiky;
        for (int i = 0; i < p_i->num_neighbors; ++i) {
            int n = p_i->neighborhood[i];
            Particle *p_j = &particles[n];
            grad_spiky += spiky_grad(p_i->pred_p - p_j->pred_p, KERNEL_RADIUS);
        }
        return (1 / REST_DENSITY) * grad_spiky;
    } else {
        return -(1 / REST_DENSITY) * spiky_grad(p_i->pred_p - p_k->pred_p, KERNEL_RADIUS);
    }
}

__device__ glm::vec3 delta_p(Particle *p_i, Particle *particles) {
    glm::vec3 delta_p;
    float s_corr;
    for (int i = 0; i < p_i->num_neighbors; ++i) {
        int n = p_i->neighborhood[i];
        Particle *p_j = &particles[n];
        s_corr = 0.001;
        glm::vec3 b = spiky_grad(p_i->pred_p - p_j->pred_p, KERNEL_RADIUS);
        float a = (p_j->lambda + p_i->lambda + s_corr);
        delta_p += a * b;
    }
    return (1 / REST_DENSITY) * delta_p;
}

__device__ glm::vec3 vorticity(Particle *p_i, Particle *particles) {
    glm::vec3 w;
    for (int i = 0; i < p_i->num_neighbors; ++i) {
        int n = p_i->neighborhood[i];
        Particle *p_j = &particles[n];
        glm::vec3 v_ij = p_j->v - p_i->v;
        w += glm::cross(v_ij, spiky_grad(p_i->pred_p - p_j->pred_p, KERNEL_RADIUS));
    }
    return w;
}

__device__ glm::vec3 f_vorticity(Particle *p_i, Particle *particles) {
    glm::vec3 force;
    glm::vec3 w = vorticity(p_i, particles);
    float rho = rho_i(p_i, particles);
    for (int i = 0; i < p_i->num_neighbors; ++i) {
        int n = p_i->neighborhood[i];
        Particle *p_j = &particles[n];
        glm::vec3 p_x = (p_i->m * p_i->pred_p + p_j->m * p_j->pred_p) * (1.f / (p_i->m + p_j->m));
        glm::vec3 eta = p_x - p_i->pred_p;
        glm::vec3 N = glm::normalize(eta);
        force += EPS_T * glm::cross(N, w) * rho;
    }
    return force;
}

__device__ glm::vec3 XSPH_vel(Particle *p_i, Particle *particles) {
    glm::vec3 v;
    for (int i = 0; i < p_i->num_neighbors; ++i) {
        int n = p_i->neighborhood[i];
        Particle *p_j = &particles[n];
        glm::vec3 v_ij = p_j->v - p_i->v;
        v += v_ij * poly6(p_i->v - p_j->v, KERNEL_RADIUS);
    }
    return p_i->v + VISCOSITY * v;
}

__device__ void triangle_collide(Triangle &t, Particle &p) {
    glm::vec3 d = p.pred_p - p.p;
    glm::vec3 e1 = t.v2 - t.v1;
    glm::vec3 e2 = t.v3 - t.v1;
    glm::vec3 s = p.p - t.v1;
    glm::vec3 s1 = glm::cross(d, e2);
    glm::vec3 s2 = glm::cross(s, e1);
    float coeff = 1 / glm::dot(s1, e1);

    // Check if on opposite sides of plane
    float pos_dot = glm::dot(p.p - t.v1, t.n);
    float pred_dot = glm::dot(p.pred_p - t.v1, t.n);
    if (pos_dot >= 0 && pred_dot >= 0 || pos_dot < 0 && pred_dot < 0) {
        return;
    }

    // Check if inside triangle
    float b1 = coeff * glm::dot(s1, s);
    float b2 = coeff * glm::dot(s2, d);
    if (b1 >= 0 && b2 >= 0 && (1 - b1 - b2) >= 0) {
        glm::vec3 tangent = b1 * t.v2 + b2 * t.v3 + (1 - b1 - b2) * t.v1;
        if (pred_dot < 0) {
            tangent += t.n * SURFACE_OFFSET;
        } else {
            tangent -= t.n * SURFACE_OFFSET;
        }
        p.pred_p = tangent;
        p.collided = true;
        p.v = glm::reflect(p.v, t.n) * 0.5f;
    }
}

__device__ void plane_collide(Plane &p, Particle &par) {
    float pos_dot = glm::dot(par.p - p.point, p.normal);
    float pred_dot = glm::dot(par.pred_p - p.point, p.normal);
    if ((pos_dot >= 0 && pred_dot < 0) || (pos_dot < 0 && pred_dot >= 0)) {
        glm::vec3 proj_vec = par.pred_p - p.point;
        float dist = glm::dot(proj_vec, p.normal);
        if (dist < 0) {
            dist = dist - SURFACE_OFFSET;
        } else {
            dist = dist + SURFACE_OFFSET;
        }
        glm::vec3 proj_point = par.pred_p - p.normal * dist;
        par.pred_p = proj_point;
        par.v = glm::reflect(-par.v, p.normal);
    }
}