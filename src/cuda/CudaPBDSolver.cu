#include "CudaPBDSolver.h"

__global__ void seed_position(Particle *particles, int n) {
    int index = threadIdx.x + blockIdx.x * blockDim.x;
    if (index > n) {
        return;
    }
    Particle &p = particles[index];
    p.f = vec3(0, -9.8f, 0) * p.m;
    p.v = p.v + p.f * (1.f / p.m) * dt;
    p.pred_p = p.p + p.v * dt;
}

__global__ void run_solver(Particle *particles, int n, Triangle *triangles, int num_triangles, Plane *plane, int num_planes) {
    int index = threadIdx.x + blockIdx.x * blockDim.x;
    if (index > n) {
        return;
    }
    Particle &p = particles[index];

    for (int i = 0; i < SOLVER_ITERS; ++i) {
        __syncthreads();
        p.lambda = lambda_i(&p);
        __syncthreads();

        p.dp = delta_p(&p);
        p.pred_p += p_i.dp;
        for (int i = 0; i < num_triangles; ++i) {
            triangle_collide(triangles[i], p);
        }
        for (int i = 0; i < num_planes; ++i) {
            plane_collide(planes[i], p);
        }
    }

    if (!p.collided)
        p.v = (1.f / dt) * (p.pred_p - p.p);
    __syncthreads();

    p.f += f_vorticity(&p);
    p.v = XSPH_vel(&p);
    p.collided = false;
    p.p = p.pred_p;
    p.num_neighbors = 0;
}

__device__ inline float poly6(vec3 r_ij, float h) {
    float r = length(r_ij);
    return 0 <= r && r <= h ? 315 / (64 * M_PI * pow(h, 9)) * pow(h * h - r * r, 3) : 0;
}

__device__ inline float spiky(vec3 r_ij, float h) {
    float r = length(r_ij);
    return 0 <= r && r <= h ? 15 / (M_PI * pow(h, 6)) * pow(h - r, 3) : 0;
}

__device__ inline vec3 spiky_grad(vec3 r_ij, float h) {
    float r = length(r_ij);
    if (0 < r && r <= h) {
        vec3 d = normalize(r_ij);
        float coeff = 45 / (M_PI * pow(h, 6)) * pow(h - r, 2);
        return coeff * d;
    } else {
        return vec3();
    }
}

__device__ float rho_i(Particle *p_i, Particle *particles) {
    float rho = 0;
    for (int i = 0; i < p_i->num_neighbors; ++i) {
        int n = p_i->neighborhood[i];
        Particle *p_j = &particles[i];
        rho += p_j->m * poly6(p_i->pred_p - p_j->pred_p, this->h);
    }
    return rho;
}

__device__ float lambda_i(Particle *p_i, Particle *particles) {
    float lambda = 0;
    for (int i = 0; i < p_i->num_neighbors; ++i) {
        int n = p_i->neighborhood[i];
        Particle *p_k = &particles[n];
        vec3 grad_k_ci = grad_k_Ci(p_k, p_i, particles);
        lambda += pow(length(grad_k_ci), 2);
    }
    lambda = -C_i(p_i, particles) * (1 / (lambda + this->eps_t));
    return lambda;
}

__device__ float C_i(Particle *p_i, Particle *particles) {
    return rho_i(p_i, particles) / this->rest_density - 1;
}

__device__ vec3 grad_k_Ci(Particle *p_k, Particle *p_i, Particle *particles) {
    if (p_k == p_i) {
        vec3 grad_spiky;
        for (int i = 0; i < p_i->num_neighbors; ++i) {
            int n = p_i->neighborhood[i];
            Particle *p_j = &particles[n];
            grad_spiky += spiky_grad(p_i->pred_p - p_j->pred_p, this->h);
        }
        return (1 / this->rest_density) * grad_spiky;
    } else {
        return -(1 / this->rest_density) * spiky_grad(p_i->pred_p - p_k->pred_p, this->h);
    }
}

__device__ vec3 delta_p(Particle *p_i, Particle *particles) {
    vec3 delta_p;
    float s_corr;
    for (int i = 0; i < p_i->num_neighbors; ++i) {
        int n = p_i->neighborhood[i];
        Particle *p_j = &particles[n];
        s_corr = clamp(-k * pow(poly6(p_i->pred_p - p_j->pred_p, this->h) / poly6(dq, this->h), this->n), 0.0f, 0.0001f);
        vec3 b = spiky_grad(p_i->pred_p - p_j->pred_p, this->h);
        float a = (p_j->lambda + p_i->lambda + s_corr);
        delta_p += a * b;
    }
    return (1 / this->rest_density) * delta_p;
}

__device__ vec3 vorticity(Particle *p_i, Particle *particles) {
    vec3 w;
    for (int i = 0; i < p_i->num_neighbors; ++i) {
        int n = p_i->neighbors[i];
        Particle *p_j = particles[n];
        vec3 v_ij = p_j->v - p_i->v;
        w += cross(v_ij, spiky_grad(p_i->pred_p - p_j->pred_p, this->h));
    }
    return w;
}

__device__ vec3 f_vorticity(Particle *p_i, Particle *particles) {
    vec3 force;
    vec3 w = this->vorticity(p_i, particles);
    float rho = this->rho_i(p_i, particles);
    for (int i = 0; i < p_i->num_neighbors; ++i) {
        int n = p_i->neighborhood[i];
        Particle *p_j = particles[n];
        vec3 p_x = (p_i->m * p_i->pred_p + p_j->m * p_j->pred_p) * (1.f / (p_i->m + p_j->m));
        vec3 eta = p_x - p_i->pred_p;
        vec3 N = normalize(eta);
        force += this->eps_t * cross(N, w) * rho;
    }
    return force;
}

__device__ vec3 XSPH_vel(Particle *p_i, Particle *particles) {
    vec3 v;
    for (int i = 0; i < p_i->num_neighbors; ++i) {
        int n = p_i->neighborhood[i];
        Particle *p_j = particles[n];
        vec3 v_ij = p_j->v - p_i->v;
        v += v_ij * poly6(p_i->v - p_j->v, this->h);
    }
    return p_i->v + this->c * v;
}

__device__ void triangle_collide(Triangle &t, Particle &p) {
    vec3 d = p.pred_p - p.p;
    vec3 e1 = t.v2 - t.v1;
    vec3 e2 = t.v3 - t.v1;
    vec3 s = p.p - t.v1;
    vec3 s1 = cross(d, e2);
    vec3 s2 = cross(s, e1);
    float coeff = 1 / dot(s1, e1);

    // Check if on opposite sides of plane
    float pos_dot = glm::dot(p.p - t.v1, t.n);
    float pred_dot = glm::dot(p.pred_p - t.v1, t.n);
    if (std::signbit(pos_dot) == std::signbit(pred_dot)) {
        return;
    }

    // Check if inside triangle
    float b1 = coeff * dot(s1, s);
    float b2 = coeff * dot(s2, d);
    if (b1 >= 0 && b2 >= 0 && (1 - b1 - b2) >= 0) {
        vec3 tangent = b1 * t.v2 + b2 * t.v3 + (1 - b1 - b2) * t.v1;
        if (std::signbit(pred_dot) > 0) {
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
    float pos_dot = glm::dot(p.p - this->point, this->normal);
    float pred_dot = glm::dot(p.pred_p - this->point, this->normal);
    if (std::signbit(pos_dot) != std::signbit(pred_dot)) {
        glm::vec3 proj_vec = p.pred_p - this->point;
        float dist = glm::dot(proj_vec, this->normal);
        if (dist < 0) {
            dist = dist - SURFACE_OFFSET;
        } else {
            dist = dist + SURFACE_OFFSET;
        }
        glm::vec3 proj_point = p.pred_p - this->normal * dist;
        p.pred_p = proj_point;
        p.v = glm::reflect(-p.v, normal);
    }
}