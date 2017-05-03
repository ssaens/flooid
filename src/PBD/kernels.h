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
    float out_term = h * h - r * r;
    if (out_term < MAX_VORT_ADJ) {
        return 0.f;
    }
    return 315.f / (64.f * M_PI * pow(h, 9)) * pow(out_term, 3);
}

inline vec3 spiky_grad(vec3 r_ij, float h) {
    float r = length(r_ij);
    float r_2 = r * r;
    if ((r_2 >= h * h) || (r_2 <= MAX_VORT_ADJ)) {
        return glm::vec3();
    }
    glm::vec3 d = normalize(r_ij);
    float coeff = 45.f / (M_PI * pow(h, 6)) * pow(h - r, 2);
    return coeff * d;
}

#endif //FLOOID2_KERNELS_H
