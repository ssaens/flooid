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
    dvec3 p;            // pos vector
    dvec3 pred_p;       // predicted pos vector
    dvec3 d_p;          // pressure correction vector
    dvec3 v;            // velocity vector

    dvec3 f;            // accumulated forces
    double m;           // mass
    double lambda;      // lambda for constraint solution
    double rho;         // pressure constant for particle

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
    Particle(double x, double y, double z) : p(x, y, z) {}
    Particle(dvec3 &p) : p(p) {}

    /**
     * Constructor
     * Creates a particle with the specified position and velocity
     */
    Particle(double x,  double y,  double z,
             double dx, double dy, double dz) : p(x, y, z), v(dx, dy, dz) {}

    Particle(dvec3 &p, dvec3 &v) : p(p), v(p) {}
};

#endif //FLOOID_PARTICLE_H
