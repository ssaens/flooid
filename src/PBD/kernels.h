//
// Created by Dillon Yao on 4/26/17.
//

#ifndef FLOOID2_KERNELS_H
#define FLOOID2_KERNELS_H

#include <glm/glm.hpp>
#include <math.h>

using namespace std;
using namespace glm;

inline float poly6(vec3 r_ij, float h) {
    float r = length(r_ij);
    return 0 <= r && r <= h ? 315 / (64 * M_PI * pow(h, 9)) * pow(h * h - r * r, 3) : 0;
}

inline float spiky(vec3 r_ij, float h) {
    float r = length(r_ij);
    return 0 <= r && r <= h ? 15 / (M_PI * pow(h, 6)) * pow(h - r, 3) : 0;
}

inline vec3 spiky_grad(vec3 r_ij, float h) {
    float r = length(r_ij);
    if (0 < r && r <= h) {
        vec3 d = normalize(r_ij);
        float coeff = 45 / (M_PI * pow(h, 6)) * pow(h - r, 2);
        return coeff * d;
    } else {
        return vec3();
    }
}

#endif //FLOOID2_KERNELS_H
