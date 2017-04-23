#ifndef UTILS_H
#define UTILS_H

#include "glm/glm/glm.hpp"
#include <iostream>

const glm::fvec3 ACCEL_GRAVITY = glm::fvec3(0, -9.8f, 0);
const double SURFACE_OFFSET = 0.000001;
const double RADIUS = 0.05;

const int SOLVER_ITERS = 6;

const double EPS = 600;
const double H = 0.1;
const double REST_DENSITY = 6378;

inline void stringify_vec(glm::dvec3 vec) {
    std::cout << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")" << std::endl;
}

#endif