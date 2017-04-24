//
// Created by Dillon Yao on 4/23/17.
//

#ifndef FLOOID_COMMON_H
#define FLOOID_COMMON_H

#include <glm/glm.hpp>
#include <vector>

using namespace std;
using namespace glm;

const bool RADOMIZE_OFFSET = false;
const int SOLVER_ITERS = 4;

const dvec3 ACCEL_GRAVITY(0, -9.8, 0);
const vector<dvec3> EXTERNAL_ACCELRATIONS = {ACCEL_GRAVITY};

const double PARTICLE_RADIUS = 0.05;
const double PARTICLE_MASS = 1;
const double REST_DENSITY = 6378;
const double VISCOSITY = 0.01;

const double KERNEL_RADIUS = 0.1;
const double EPS = 600;
const double PRESSURE_STRENGTH = 0.1;
const double PRESSURE_POW = 4;
const dvec3 DELTA_Q(0.2 * KERNEL_RADIUS, 0, 0);


#endif //FLOOID_COMMON_H
