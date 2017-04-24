//
// Created by Dillon Yao on 4/23/17.
//

#ifndef FLOOID_KERNELS_H
#define FLOOID_KERNELS_H

#include <glm/glm.hpp>

using namespace std;
using namespace glm;

inline double poly6(dvec3 r_ij, double h) {
    double r = length(r_ij);
    return 0 <= r && r <= h ? 315 / (64 * M_PI * pow(h, 9)) * pow(h * h - r * r, 3) : 0;
}

inline double spiky(dvec3 r_ij, double h) {
    double r = length(r_ij);
    return 0 <= r && r <= h ? 15 / (M_PI * pow(h, 6)) * pow(h - r, 3) : 0;
}

inline glm::dvec3 spiky_grad(dvec3 r_ij, double h) {
    double r = length(r_ij);
    if (0 < r && r <= h) {
        return (45 / (M_PI * pow(h, 6))) * pow(h - r, 2) * glm::normalize(r_ij);
    } else {
        return dvec3();
    }
}

#endif //FLOOID_KERNELS_H
