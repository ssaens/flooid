//
// Created by Dillon Yao on 4/23/17.
//

#ifndef FLOOID_PARTICLE_H
#define FLOOID_PARTICLE_H

#include <glm/glm.hpp>
#include <eng/eng.h>
#include <vector>

using namespace glm;
using namespace eng;

/*
 * Particle Struct
 */
struct Particle {
    vec3 p;            // pos vector
    vec3 pred_p;       // predicted pos vector
    vec3 d_p;          // pressure correction vector
    vec3 v;            // velocity vector

    vec3 f;            // accumulated forces
    float m;           // mass
    float lambda;      // lambda for constraint solution
    float rho;         // pressure constant for particle

    Color color;        // color or particle

    std::vector<struct Particle *> neighborhood;

    /**
     * Constructor
     */
    Particle() {}

    /**
     * Constructor
     * Creates a particle with the specified position
     */
    Particle(float x, float y, float z) : p(x, y, z) {}
    Particle(dvec3 &p) : p(p) {}

    /**
     * Constructor
     * Creates a particle with the specified position and velocity
     */
    Particle(float x,  float y,  float z,
             float dx, float dy, float dz) : p(x, y, z), v(dx, dy, dz) {}

    Particle(vec3 &p, vec3 &v) : p(p), v(p) {}
};

#endif //FLOOID_PARTICLE_H
