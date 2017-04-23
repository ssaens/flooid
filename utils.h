#ifndef UTILS_H
#define UTILS_H

#include "glm/glm/glm.hpp"
#include <iostream>

const glm::dvec3 ACCEL_GRAVITY = glm::dvec3(0, -9.8f, 0);
const double SURFACE_OFFSET = 0.000001;
const double RADIUS = 0.05;
const bool RANDOMIZE = true;

const int SOLVER_ITERS = 4;

const double PARTICLE_MASS = 1;
const double EPS = 600;
const double H = 0.1; // Kernel radius
const double REST_DENSITY = 6378;
const double C = 0.01; // Viscosity
const double K = 0.1; //Pressure Strength
const double N = 4; // Pressure Power
const glm::dvec3 DELTA_Q = glm::dvec3(0.2 * H, 0, 0); // Pressure Radius

inline void stringify_vec(glm::dvec3 vec) {
    std::cout << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")" << std::endl;
}

inline void printify(const char *label, glm::dvec3 vec) {
	std::cout << label << std::endl << "\t";
	stringify_vec(vec);
}

inline double clamp(double x, double min, double max) {
	return std::min(max, std::max(x, min));
}

#endif