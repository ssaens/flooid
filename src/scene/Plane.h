//
// Created by Dillon Yao on 4/25/17.
//

#ifndef FLOOID_PLANE_H
#define FLOOID_PLANE_H

#include "../particle.h"

struct Plane {
    Plane(const glm::vec3 point, const glm::vec3 normal, double friction) :
            point(point), normal(normal), friction(friction) {}

    void collide(Particle& p);
    void render();

    glm::vec3 point;
    glm::vec3 normal;
    double friction;
};

#endif //FLOOID_PLANE_H
