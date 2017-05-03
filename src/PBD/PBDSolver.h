//
// Created by Dillon Yao on 4/26/17.
//

#ifndef FLOOID2_PBDSOLVER_H
#define FLOOID2_PBDSOLVER_H

#include "Particle.h"

class PBDSolver {
private:
    float rest_density;
    float eps_t;
    float h;
    float c;
    float k;
    float n;
    float max_vort_adj;
    float pressure_eps;

    static PBDSolver self;
    PBDSolver();

    float rho_i(Particle *p_i, std::vector<Particle *> &neighborhood);
    glm::vec3 grad_k_Ci(Particle *p_k, Particle *p_i, std::vector<Particle *> &neighborhood);

public:
    static PBDSolver *getPBDsolver();

    float lambda_i(Particle *p_i, std::vector<Particle *> &neighborhood);
    glm::vec3 delta_p(Particle *p_i, std::vector<Particle *> &neighborhood);
    glm::vec3 vort_and_XSPH_vel(Particle *p_i, std::vector<Particle *> &neighborhood);
    glm::vec3 vorticity(Particle *p_i, std::vector<Particle *> &neighborhood);
};



#endif //FLOOID2_PBDSOLVER_H
