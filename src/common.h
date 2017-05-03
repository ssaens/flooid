//
// Created by Dillon Yao on 4/26/17.
//

#ifndef FLOOID2_COMMON_H
#define FLOOID2_COMMON_H

#include <glm/glm.hpp>

const float DELTA_T = 0.004;
const glm::vec3 ACCEL_GRAVITY(0, -9.8f, 0);

const int SOLVER_ITERS = 3;
const float PARTICLE_RADIUS = 0.05;
const float PARTICLE_MASS = 1;

const float KERNEL_RADIUS = 0.105f;
const float REST_DENSITY = 6378.f;
const float EPS_T = 600.f;

const float PRESSURE_STRENGTH = 0.0005f;
const float PRESSURE_POW = 4.f;

const float VISCOSITY = 0.01f;

const float SURFACE_OFFSET = 0.000001f;

const float MAX_VORT_ADJ = 0.0001f;
const float PRESSURE_EPS = 1e-2f;

#endif //FLOOID2_COMMON_H
