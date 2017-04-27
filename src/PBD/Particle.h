//
// Created by Dillon Yao on 4/26/17.
//

#ifndef FLOOID2_PARTICLE_H
#define FLOOID2_PARTICLE_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>

using namespace glm;

struct Particle {
    vec3 p;
    vec3 pred_p;
    vec3 dp;
    vec3 v;
    vec3 f;

    GLfloat m;
    GLfloat rho;
    GLfloat lambda;

    std::vector<struct Particle *> neighborhood;
};

#endif //FLOOID2_PARTICLE_H
