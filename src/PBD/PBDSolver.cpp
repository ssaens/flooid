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
        max_vort_adj(MAX_VORT_ADJ),
        pressure_eps(PRESSURE_EPS)
        {}

PBDSolver *PBDSolver::getPBDsolver() {
    return &self;
}

float PBDSolver::rho_i(Particle *p_i, std::vector<Particle *> &neighborhood) {
    float rho = 0;
    for (Particle *p_j : neighborhood) {
        rho += p_j->m * poly6(p_i->pred_p - p_j->pred_p, this->h);
    }
    p_i->rho = rho;
    return rho;
}

float PBDSolver::lambda_i(Particle *p_i, std::vector<Particle *> &neighborhood) {
    float C_i = rho_i(p_i, neighborhood) / this->rest_density - 1;
    float grad_sum = 0;
    glm::vec3 grad_k_ci;
    for (Particle *p_k : neighborhood) {
        grad_k_ci = grad_k_Ci(p_k, p_i, neighborhood);
        grad_sum += pow(length(grad_k_ci), 2);
    }
    return -C_i * (1 / (grad_sum + this->eps_t));
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
    float s_corr = 0.f;
    for (Particle *p_j : neighborhood) {
        glm::vec3 dq(0.03, 0.03, 0.03);
        float inv_sqrt_3 = 0.57735f;
        double base = poly6(p_i->pred_p - p_j->pred_p, this->h) / poly6(inv_sqrt_3 * dq, this->h);
        s_corr = -this->k * pow(base, this->n);
        glm::vec3 b = spiky_grad(p_i->pred_p - p_j->pred_p, this->h);
        float a = (p_j->lambda + p_i->lambda + s_corr);
        delta_p += a * b;
    }
    delta_p = (1 / REST_DENSITY) * delta_p;
    if (glm::length(delta_p) > 0.1) {
        delta_p = glm::normalize(delta_p) * 0.1f;
    }
    return delta_p;
}

vec3 PBDSolver::vorticity(Particle *p_i, std::vector<Particle *> &neighborhood) {
    vec3 w;
    for (Particle *p_j : neighborhood) {
        vec3 v_ij = p_j->v - p_i->v;
        w += cross(spiky_grad(p_i->pred_p - p_j->pred_p, this->h), v_ij);
    }
    return w;
}


glm::vec3 PBDSolver::vort_and_XSPH_vel(Particle *p_i, std::vector<Particle *> &neighborhood) {
    glm::vec3 eta;
    glm::vec3 neighbor_vs;
    for (Particle *p_j : neighborhood) {
        eta += (1 / std::max(p_j->rho, 100.f)) * glm::length(p_j->w) * spiky_grad(p_i->pred_p - p_j->pred_p, this->h);
        glm::vec3 v_ji = p_j->v - p_i->v;
        neighbor_vs += (1 / std::max(p_j->rho, 100.f)) * v_ji * poly6(p_i->pred_p - p_j->pred_p, this->h); // MAY need to flip v
    }
    if (length(eta) > this->max_vort_adj) {
        eta = glm::normalize(eta);
    } else {
        eta = glm::vec3();
    }
    glm::vec3 f_vorticity = this->pressure_eps * glm::cross(eta, p_i->w);
    return p_i->v + VISCOSITY * neighbor_vs + DELTA_T * f_vorticity;
}