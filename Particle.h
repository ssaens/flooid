#ifndef PARTICLE_H
#define PARTICLE_H

#include "glm/glm/glm.hpp"
#include <vector>

struct Particle {
    glm::dvec3 pos;
    glm::dvec3 pred_pos;
    glm::dvec3 delta_pos;
    
    glm::dvec3 vel;
    glm::dvec3 force;
    
    std::vector<struct Particle *> neighborhood;
    double lambda;
    float mass;
};

#endif