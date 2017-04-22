#ifndef SOLVER_H
#define SOLVER_H

#include "Particle.h"
#include <vector>

class ConstraintSolver {
private:
	double rest_density;
	double eps;
	double h;

	double rho_i(Particle *p_i, std::vector<Particle *> &neighborhood);
	double C_i(Particle *p_i, std::vector<Particle *> &neighborhood);
	glm::dvec3 grad_k_Ci(Particle *p_k, Particle *p_i, std::vector<Particle *> &neighborhood);

public:
	ConstraintSolver();
	ConstraintSolver(double rest_density, double h, double eps);

	double lambda_i(Particle *p_i, std::vector<Particle *> &neighborhood);
	glm::dvec3 delta_p(Particle *p_i, std::vector<Particle *> &neighborhood);
};

#endif