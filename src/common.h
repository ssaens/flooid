//
// Created by Dillon Yao on 4/26/17.
//

#ifndef FLOOID2_COMMON_H
#define FLOOID2_COMMON_H

#include <glm/glm.hpp>

const float DELTA_T = 0.0083;
const glm::vec3 ACCEL_GRAVITY(0, -9.8f, 0);

const int SOLVER_ITERS = 4;
const float PARTICLE_RADIUS = 0.05;
const float PARTICLE_MASS = 1;

const float KERNEL_RADIUS = 0.1001;
const float REST_DENSITY = 6378;
const float EPS_T = 600;

const float PRESSURE_STRENGTH = 0.000001;
const float PRESSURE_POW = 4;
const glm::vec3 DELTA_Q(0.2f * KERNEL_RADIUS, 0.f, 0.f);

const float VISCOSITY = 0.01;

const float SURFACE_OFFSET = 0.000001;

#endif //FLOOID2_COMMON_H
