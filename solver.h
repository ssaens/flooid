#ifndef SOLVER_H
#define SOLVER_H

#include "Particle.h"
#include <vector>

class ConstraintSolver {
private:
	double rest_density;
	double eps;
	double h;
	double c;

	double rho_i(Particle *p_i, std::vector<Particle *> &neighborhood);
	double C_i(Particle *p_i, std::vector<Particle *> &neighborhood);
	glm::dvec3 grad_k_Ci(Particle *p_k, Particle *p_i, std::vector<Particle *> &neighborhood);
	glm::dvec3 vorticity(Particle *p_i, std::vector<Particle *> &neighborhood);

public:
	ConstraintSolver();
	ConstraintSolver(double rest_density, double h, double eps, double c);

	double lambda_i(Particle *p_i, std::vector<Particle *> &neighborhood);
	glm::dvec3 delta_p(Particle *p_i, std::vector<Particle *> &neighborhood);
	glm::dvec3 f_vorticity(Particle *p_i, std::vector<Particle *> &neighborhood);
	glm::dvec3 XSPH_vel(Particle *p_i, std::vector<Particle *> &neighborhood);
};

#endif