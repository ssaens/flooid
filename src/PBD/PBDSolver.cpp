//
// Created by Dillon Yao on 4/26/17.
//

#include "PBDSolver.h"
#include "../common.h"
#include "kernels.h"

using namespace glm;
PBDSolver PBDSolver::self;

PBDSolver::PBDSolver() :
        rest_density(REST_DENSITY),
        h(KERNEL_RADIUS),
        eps_t(EPS_T),
        c(VISCOSITY),
        k(PRESSURE_STRENGTH),
        n(PRESSURE_POW),
        dq(DELTA_Q)
{}

static PBDSolver *PBDSolver::getPBDsolver() {
    return &self;
}

float PBDSolver::rho_i(Particle *p_i, std::vector<Particle *> &neighborhood) {
    float rho = 0;
    for (Particle *p_j : neighborhood) {
        rho += p_j->m * poly6(p_i->pred_p - p_j->pred_p, this->h);
    }
    return rho;
}

float PBDSolver::lambda_i(Particle *p_i, std::vector<Particle *> &neighborhood) {
    float lambda = 0;
    for (Particle *p_k : neighborhood) {
        vec3 grad_k_ci = grad_k_Ci(p_k, p_i, neighborhood);
        lambda += pow(length(grad_k_ci), 2);
    }
    lambda = -C_i(p_i, neighborhood) * (1 / (lambda + this->eps_t));
    return lambda;
}

float PBDSolver::C_i(Particle *p_i, std::vector<Particle *> &neighborhood) {
    return rho_i(p_i, neighborhood) / this->rest_density - 1;
}

vec3 PBDSolver::grad_k_Ci(Particle *p_k, Particle *p_i, std::vector<Particle *> &neighborhood) {
    if (p_k == p_i) {
        vec3 grad_spiky;
        for (Particle *p_j : neighborhood) {
            grad_spiky += spiky_grad(p_i->pred_p - p_j->pred_p, this->h);
        }
        return (1 / this->rest_density) * grad_spiky;
    } else {
        return -(1 / this->rest_density) * spiky_grad(p_i->pred_p - p_k->pred_p, this->h);
    }
}

vec3 PBDSolver::delta_p(Particle *p_i, std::vector<Particle *> &neighborhood) {
    vec3 delta_p;
    float s_corr;
    for (Particle *p_j : neighborhood) {
        s_corr = clamp(-k * pow(poly6(p_i->pred_p - p_j->pred_p, this->h) / poly6(dq, this->h), this->n), 0.0f, 0.0001f);
        vec3 b = spiky_grad(p_i->pred_p - p_j->pred_p, this->h);
        float a = (p_j->lambda + p_i->lambda + s_corr);
        delta_p += a * b;
    }
    return (1 / this->rest_density) * delta_p;
}

vec3 PBDSolver::vorticity(Particle *p_i, std::vector<Particle *> &neighborhood) {
    vec3 w;
    for (Particle *p_j : neighborhood) {
        vec3 v_ij = p_j->vel - p_i->vel;
        w += cross(v_ij, spiky_grad(p_i->pred_p - p_j->pred_p, this->h));
    }
    return w;
}

vec3 PBDSolver::f_vorticity(Particle *p_i, std::vector<Particle *> &neighborhood) {
    vec3 force;
    vec3 w = this->vorticity(p_i, neighborhood);
    float rho = this->rho_i(p_i, neighborhood);
    for (Particle *p_j : neighborhood) {
        vec3 p_x = (p_i->m * p_i->pred_p + p_j->m * p_j->pred_p) * (1.f / (p_i->m + p_j->m));
        vec3 eta = p_x - p_i->pred_p;
        vec3 N = normalize(eta);
        force += this->eps_t * cross(N, w) * rho;
    }
    return force;
}

vec3 PBDSolver::XSPH_vel(Particle *p_i, std::vector<Particle *> &neighborhood) {
    vec3 v;
    for (Particle *p_j : neighborhood) {
        vec3 v_ij = p_j->vel - p_i->vel;
        v += v_ij * poly6(p_i->vel - p_j->vel, this->h);
    }
    return p_i->vel + this->c * v;
}