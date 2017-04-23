
#include "solver.h"
#include "kernel.h"
#include "utils.h"

ConstraintSolver::ConstraintSolver() {

}

ConstraintSolver::ConstraintSolver(double rest_density, double h, double eps, double c) :
	rest_density(rest_density), h(h), eps(eps), c(c) {

}

double ConstraintSolver::rho_i(Particle *p_i, std::vector<Particle *> &neighborhood) {
	double rho = 0;
	for (Particle *p_j : neighborhood) {
		rho += p_j->mass * poly6(p_i->pred_pos - p_j->pred_pos, this->h);
	}
	return rho;
}

double ConstraintSolver::lambda_i(Particle *p_i, std::vector<Particle *> &neighborhood) {
	double lambda = 0;
	for (Particle *p_k : neighborhood) {
		glm::dvec3 grad_k_ci = grad_k_Ci(p_k, p_i, neighborhood);
		lambda += std::pow(glm::length(grad_k_ci), 2);
	}
	lambda = -C_i(p_i, neighborhood) * (1 / (lambda + this->eps));
	return lambda;
}

double ConstraintSolver::C_i(Particle *p_i, std::vector<Particle *> &neighborhood) {
	return rho_i(p_i, neighborhood) / this->rest_density - 1;
}

glm::dvec3 ConstraintSolver::grad_k_Ci(Particle *p_k, Particle *p_i, std::vector<Particle *> &neighborhood) {
	if (p_k == p_i) {
		glm::dvec3 grad_spiky;
		for (Particle *p_j : neighborhood) {
			grad_spiky += spiky_grad(p_i->pred_pos - p_j->pred_pos, this->h);
		}
		return (1 / this->rest_density) * grad_spiky;
	} else {
		return -(1 / this->rest_density) * spiky_grad(p_i->pred_pos - p_k->pred_pos, this->h);
	}
}

glm::dvec3 ConstraintSolver::delta_p(Particle *p_i, std::vector<Particle *> &neighborhood) {
	glm::dvec3 delta_p;
	double s_corr;
	for (Particle *p_j : neighborhood) {
		s_corr = clamp(-K * std::pow(poly6(p_i->pred_pos - p_j->pred_pos, this->h) / poly6(DELTA_Q, this->h), N), 0, -0.0001);
		delta_p += (p_j->lambda + p_i->lambda + s_corr) * spiky_grad(p_i->pred_pos - p_j->pred_pos, this->h);
	}
	return (1 / this->rest_density) * delta_p;
}

glm::dvec3 ConstraintSolver::vorticity(Particle *p_i, std::vector<Particle *> &neighborhood) {
	glm::dvec3 w;
	for (Particle *p_j : neighborhood) {
		glm::dvec3 v_ij = p_j->vel - p_i->vel;
		w += glm::cross(v_ij, spiky_grad(p_i->pred_pos - p_j->pred_pos, this->h));
	}
	return w;
}

glm::dvec3 ConstraintSolver::f_vorticity(Particle *p_i, std::vector<Particle *> &neighborhood) {
	glm::dvec3 force;
	glm::dvec3 w = this->vorticity(p_i, neighborhood);
	double rho = this->rho_i(p_i, neighborhood);
	for (Particle *p_j : neighborhood) {
		glm::dvec3 p_x = (p_i->mass * p_i->pred_pos + p_j->mass * p_j->pred_pos) * (1.f / (p_i->mass + p_j->mass));
		glm::dvec3 eta = p_x - p_i->pred_pos;
		glm::dvec3 N = glm::normalize(eta);
		force += this->eps * (glm::cross(N, w)) * rho;
	}
	return force;
}

glm::dvec3 ConstraintSolver::XSPH_vel(Particle *p_i, std::vector<Particle *> &neighborhood) {
	glm::dvec3 vel;
	for (Particle *p_j : neighborhood) {
		glm::dvec3 v_ij = p_j->vel - p_i->vel;
		vel += v_ij * poly6(p_i->vel - p_j->vel, this->h);
	}
	return p_i->vel + this->c * vel;
}

