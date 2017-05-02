//
// Created by Dillon Yao on 4/26/17.
//

#ifndef FLOOID2_PARTICLE_H
#define FLOOID2_PARTICLE_H

// #include <glm/glm.hpp>
#include <glm/glm.hpp>

struct Particle {
    glm::vec3 p;
    glm::vec3 pred_p;
    glm::vec3 dp;
    glm::vec3 v;
    glm::vec3 f;

    float m;
    float lambda;

    bool collided;

    int neighborhood[50];
    int num_neighbors;
};

#endif //FLOOID2_PARTICLE_H
