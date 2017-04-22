
#include "solver.h"
#include "kernel.h"
#include "utils.h"

ConstraintSolver::ConstraintSolver() {

}

ConstraintSolver::ConstraintSolver(double rest_density, double h, double eps) :
	rest_density(rest_density), h(h), eps(eps) {

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
	for (Particle *p_j : neighborhood) {
		delta_p += (p_j->lambda + p_i->lambda) * spiky_grad(p_i->pred_pos - p_j->pred_pos, this->h);
	}
	return (1 / this->rest_density) * delta_p;
}


