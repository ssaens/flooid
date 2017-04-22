#ifndef KERNEL_H
#define KERNEL_H

#include "Particle.h"
#include <math.h>

using namespace std;

inline double poly6(glm::dvec3 r_ij, double h) {
	double r = glm::length(r_ij);
	return 0 <= r && r <= h ? 315 / (64 * M_PI * pow(h, 9)) * pow(h * h - r * r, 3) : 0;
}

inline double spiky(glm::dvec3 r_ij, double h) {
	double r = glm::length(r_ij);
	return 0 <= r && r <= h ? 15 / (M_PI * pow(h, 6)) * pow(h - r, 3) : 0;
}

inline glm::dvec3 spiky_grad(glm::dvec3 r_ij, double h) {
	double r = glm::length(r_ij);
	if (0 < r && r <= h) {
		return (-45 / (M_PI * pow(h, 6))) * pow(h - r, 2) * glm::normalize(r_ij);
	} else {
		return glm::dvec3();
	}
}

#endif