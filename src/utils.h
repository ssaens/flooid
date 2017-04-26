//
// Created by Dillon Yao on 4/23/17.
//

#ifndef FLOOID_UTILS_H
#define FLOOID_UTILS_H

#include <algorithm>
#include <glm/glm.hpp>

using namespace glm;

void draw_sphere(const glm::vec3& p, double r);

void draw_sphere(double radius, unsigned int lats, unsigned int longs);

void draw_triangle(dvec3& p1, dvec3& p2, dvec3& p3, dvec3& normal, fvec3& color);

#endif //FLOOID_UTILS_H
