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
const float SURFACE_OFFSET = 0.00001f;
const int SOLVER_ITERS = 4;

const vec3 ACCEL_GRAVITY(0, -9.8, 0);
const vector<vec3> EXTERNAL_ACCELRATIONS = {ACCEL_GRAVITY};

const float PARTICLE_RADIUS = 0.05;
const float PARTICLE_MASS = 1;
const float REST_DENSITY = 6378;
const float VISCOSITY = 0.01;

const float KERNEL_RADIUS = 0.1001;
const float EPS = 600;
const float PRESSURE_STRENGTH = 0.1;
const float PRESSURE_POW = 4;
const vec3 DELTA_Q(0.2 * KERNEL_RADIUS, 0, 0);

const float EPS_F = 0.00001f;

const int DEFAULT_WIDTH = 1200;
const int DEFAULT_HEIGHT = 800;

#endif //FLOOID_COMMON_H
