//
// Created by Dillon Yao on 4/23/17.
//

#ifndef FLOOID_PBDSOLVER_H
#define FLOOID_PBDSOLVER_H

#include "particle.h"

class PBDSolver {
private:
    double rest_density;
    double eps;
    double h;
    double c;
    double k;
    double n;
    dvec3 dq;

    double rho_i(Particle *p_i, std::vector<Particle *> &neighborhood);
    double C_i(Particle *p_i, std::vector<Particle *> &neighborhood);
    glm::dvec3 grad_k_Ci(Particle *p_k, Particle *p_i, std::vector<Particle *> &neighborhood);
    glm::dvec3 vorticity(Particle *p_i, std::vector<Particle *> &neighborhood);

public:
    PBDSolver();
    PBDSolver(double rest_density, double h, double eps, double c, double k, double n, dvec3 dq);

    double lambda_i(Particle *p_i, std::vector<Particle *> &neighborhood);
    glm::dvec3 delta_p(Particle *p_i, std::vector<Particle *> &neighborhood);
    glm::dvec3 f_vorticity(Particle *p_i, std::vector<Particle *> &neighborhood);
    glm::dvec3 XSPH_vel(Particle *p_i, std::vector<Particle *> &neighborhood);
};


#endif //FLOOID_PBDSOLVER_H
