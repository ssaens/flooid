//
// Created by Dillon Yao on 4/23/17.
//

#ifndef FLOOID_PBDSOLVER_H
#define FLOOID_PBDSOLVER_H

#include "particle.h"

class PBDSolver {
private:
    float rest_density;
    float eps;
    float h;
    float c;
    float k;
    float n;
    vec3 dq;

    float rho_i(Particle *p_i, std::vector<Particle *> &neighborhood);
    float C_i(Particle *p_i, std::vector<Particle *> &neighborhood);
    glm::vec3 grad_k_Ci(Particle *p_k, Particle *p_i, std::vector<Particle *> &neighborhood);
    glm::vec3 vorticity(Particle *p_i, std::vector<Particle *> &neighborhood);

public:
    PBDSolver();
    PBDSolver(float rest_density, float h, float eps, float c, float k, float n, vec3 dq);

    float lambda_i(Particle *p_i, std::vector<Particle *> &neighborhood);
    glm::vec3 delta_p(Particle *p_i, std::vector<Particle *> &neighborhood);
    glm::vec3 f_vorticity(Particle *p_i, std::vector<Particle *> &neighborhood);
    glm::vec3 XSPH_vel(Particle *p_i, std::vector<Particle *> &neighborhood);
};


#endif //FLOOID_PBDSOLVER_H
