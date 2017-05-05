

#define DELTA_T 0.004f
#define PI 3.14159265359f
#define SOLVER_ITERS 3
#define KERNEL_RADIUS 0.106f
#define EPS_T 600.f
#define REST_DENSITY 6378.f
#include "CudaPBDSolver.cuh"

const int PRESSURE_POW = 4;
const float VISCOSITY = 0.01;
const float SURFACE_OFFSET = 0.000001;
const float PRESSURE_STRENGTH =  0.0005f;
const float PRESSURE_EPS = 1e-2f;
const float MAX_VORT_ADJ = 0.0001f;

using namespace glm;

__device__ float poly6(glm::vec3 r_ij, float h);
__device__ float spiky(glm::vec3 r_ij, float h);
__device__ glm::vec3 spiky_grad(glm::vec3 r_ij, float h);

__device__ float rho_i(Particle *p_i, Particle *particles);
__device__ float lambda_i(Particle *p_i, Particle *particles);
__device__ glm::vec3 grad_k_Ci(Particle *p_k, Particle *p_i, Particle *particles);
__device__ glm::vec3 delta_p(Particle *p_i, Particle *particles);
__device__ glm::vec3 vorticity(Particle *p_i, Particle *particles);
__device__ glm::vec3 vort_and_XSPH_vel(Particle *p_i, Particle *particles);

__device__ void triangle_collide(Triangle &t, Particle &p);
__device__ void plane_collide(Plane &p, Particle &par);
__device__ void sphere_collide(Particle &p);

__global__ void seed_position(Particle *particles, int n) {
    int index = threadIdx.x + blockIdx.x * blockDim.x;
    if (index >= n) {
        return;
    }
    Particle &p = particles[index];
    p.v = p.v + glm::vec3(0, -9.8f, 0) * DELTA_T;
    p.pred_p = p.p + p.v * DELTA_T;
}

__global__ void run_solver(Particle *particles, int n, Triangle *triangles, int num_triangles, Plane *planes, int num_planes) {
    int index = threadIdx.x + blockIdx.x * blockDim.x;
    if (index >= n) {
        return;
    }
    Particle &p = particles[index];

    for (int i = 0; i < SOLVER_ITERS; ++i) {
        p.lambda = lambda_i(&p, particles);
        __syncthreads();
        p.dp = delta_p(&p, particles);
        // for (int i = 0; i < num_triangles; ++i) {
        //     triangle_collide(triangles[i], p);
        // }
       
        // sphere_collide(p);
        glm::vec3 displacement = p.pred_p + p.dp - p.p;
        if (glm::length(displacement) > 0.1f) {
            displacement = glm::normalize(displacement) * 0.1f;
        }
        p.pred_p = p.p + displacement;
         for (int i = 0; i < num_planes; ++i) {
            plane_collide(planes[i], p);
        }
            sphere_collide(p);
        __syncthreads();
    }

    // for (int i = 0; i < num_triangles; ++i) {
    //     triangle_collide(triangles[i], p);
    // }
    for (int i = 0; i < num_planes; ++i) {
        plane_collide(planes[i], p);
    }

    p.v = (1.f / DELTA_T) * (p.pred_p - p.p);
    __syncthreads();
    p.w = vorticity(&p, particles);
    __syncthreads();

    p.v = vort_and_XSPH_vel(&p, particles);
    if (glm::length(p.v) > 7.f) {
        p.v = glm::normalize(p.v) * 7.f;
    }

    p.p = p.pred_p;
    p.num_neighbors = 0;
}

__device__ float poly6(glm::vec3 r_ij, float h) {
    float r = length(r_ij);
    float out_term = h * h - r * r;
    if (out_term < MAX_VORT_ADJ) {
        return 0.f;
    }
    return 315.f / (64.f * PI * pow(h, 9)) * pow(out_term, 3);
}

__device__ glm::vec3 spiky_grad(glm::vec3 r_ij, float h) {
    float r = length(r_ij);
    float r_2 = r * r;
    if ((r_2 >= h * h) || (r_2 <= MAX_VORT_ADJ)) {
        return glm::vec3();
    }
    glm::vec3 d = normalize(r_ij);
    float coeff = 45.f / (PI * pow(h, 6)) * pow(h - r, 2);
    return coeff * d;
}

__device__ float rho_i(Particle *p_i, Particle *particles) {
    float rho = 0;
    for (int i = 0; i < p_i->num_neighbors; ++i) {
        int n = p_i->neighborhood[i];
        Particle *p_j = &particles[n];
        rho += p_j->m * poly6(p_i->pred_p - p_j->pred_p, KERNEL_RADIUS);
    }
    p_i->rho = rho;
    return rho;
}

__device__ float lambda_i(Particle *p_i, Particle *particles) {
    float C_i = rho_i(p_i, particles) / REST_DENSITY - 1;
    float grad_sum = 0;
    glm::vec3 grad_k_ci;

    for (int i = 0; i < p_i->num_neighbors; ++i) {
        int n = p_i->neighborhood[i];
        Particle *p_k = &particles[n];
        grad_k_ci = grad_k_Ci(p_k, p_i, particles);
        grad_sum += pow(glm::length(grad_k_ci), 2);
    }
    return -C_i * (1 / (grad_sum + EPS_T));
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
    float s_corr = 0.f;
    for (int i = 0; i < p_i->num_neighbors; ++i) {
        int n = p_i->neighborhood[i];
        Particle *p_j = &particles[n];
        glm::vec3 dq(0.03, 0.03, 0.03);
        float inv_sqrt_3 = 0.57735f;
        double base = poly6(p_i->pred_p - p_j->pred_p, KERNEL_RADIUS) / poly6(inv_sqrt_3 * dq, KERNEL_RADIUS);
        s_corr = -PRESSURE_STRENGTH * pow(base, PRESSURE_POW);
        glm::vec3 b = spiky_grad(p_i->pred_p - p_j->pred_p, KERNEL_RADIUS);
        float a = (p_j->lambda + p_i->lambda + s_corr);
        delta_p += a * b;
    }
    delta_p = (1 / REST_DENSITY) * delta_p;
    if (glm::length(delta_p) > 0.1) {
        delta_p = glm::normalize(delta_p) * 0.1f;
    }
    return delta_p;
}

__device__ glm::vec3 vorticity(Particle *p_i, Particle *particles) {
    glm::vec3 w;
    for (int i = 0; i < p_i->num_neighbors; ++i) {
        int n = p_i->neighborhood[i];
        Particle *p_j = &particles[n];
        glm::vec3 v_ij = p_j->v - p_i->v;
        w += glm::cross(spiky_grad(p_i->pred_p - p_j->pred_p, KERNEL_RADIUS), v_ij);
    }
    return w;
}

__device__ glm::vec3 vort_and_XSPH_vel(Particle *p_i, Particle *particles) {
    glm::vec3 eta;
    glm::vec3 neighbor_vs;
    for (int i = 0; i < p_i->num_neighbors; ++i) {
        int n = p_i->neighborhood[i];
        Particle *p_j = &particles[n];
        eta += (1 / max(p_j->rho, 100.f)) * glm::length(p_j->w) * spiky_grad(p_i->pred_p - p_j->pred_p, KERNEL_RADIUS);
        glm::vec3 v_ji = p_j->v - p_i->v;
        neighbor_vs += (1 / max(p_j->rho, 100.f)) * v_ji * poly6(p_i->pred_p - p_j->pred_p, KERNEL_RADIUS); // MAY need to flip v
    }
    if (length(eta) > MAX_VORT_ADJ) {
        eta = glm::normalize(eta);
    } else {
        eta = glm::vec3();
    }
    glm::vec3 f_vorticity = PRESSURE_EPS * glm::cross(eta, p_i->w);
    return p_i->v + VISCOSITY * neighbor_vs + DELTA_T * f_vorticity;
    // return p_i->v + VISCOSITY * neighbor_vs;
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
        // p.v = glm::reflect(p.v, t.n) * 0.5f;
        p.v = glm::vec3();
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
        // par.v = glm::reflect(-par.v, p.normal);
        par.v = glm::vec3();          
    }
}

__device__ void sphere_collide(Particle &p) {
    if (p.p.y > 0) return;
    glm::vec3 origin;
    float radius = 1.5;
    float pos_dist = glm::length(p.p - origin);
    float pred_dist = glm::length(p.pred_p - origin);
    glm::vec3 normal = glm::normalize(p.pred_p - origin); // o to pred_p
    glm::vec3 ray = p.pred_p - p.p;
    if (pred_dist >=radius && pos_dist <= radius) {   
        glm::vec3 proj_point = normal * radius + origin - normal * SURFACE_OFFSET;
        p.pred_p = proj_point;
    }
}