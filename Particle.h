#ifndef PARTICLE_H
#define PARTICLE_H

struct Particle {
    glm::dvec3 pos;
    glm::dvec3 pred_pos;
    glm::dvec3 vel;
    glm::dvec3 force;
    float mass;
};

#endif