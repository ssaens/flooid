//
// Created by Dillon Yao on 4/26/17.
//

#ifndef FLOOID2_PBDSOLVER_H
#define FLOOID2_PBDSOLVER_H

#include "particle.h"

class PBDSolver {
private:
    float rest_density;
    float eps_t;
    float h;
    float c;
    float k;
    float n;
    vec3 dq;
    static PBDSolver self;

    PBDSolver();
    float rho_i(Particle *p_i, std::vector<Particle *> &neighborhood);
    float C_i(Particle *p_i, std::vector<Particle *> &neighborhood);
    glm::vec3 grad_k_Ci(Particle *p_k, Particle *p_i, std::vector<Particle *> &neighborhood);
    glm::vec3 vorticity(Particle *p_i, std::vector<Particle *> &neighborhood);

public:
    static PBDSolver *getPBDsolver();

    float lambda_i(Particle *p_i, std::vector<Particle *> &neighborhood);
    glm::vec3 delta_p(Particle *p_i, std::vector<Particle *> &neighborhood);
    glm::vec3 f_vorticity(Particle *p_i, std::vector<Particle *> &neighborhood);
    glm::vec3 XSPH_vel(Particle *p_i, std::vector<Particle *> &neighborhood);
};


#endif //FLOOID2_PBDSOLVER_H
