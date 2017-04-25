//
// Created by Dillon Yao on 4/25/17.
//

#include "Plane.h"
#include "../common.h"

void Plane::collide(Particle& p) {
    double pos_dot = glm::dot(p.p - this->point, this->normal);
    double pred_dot = glm::dot(p.pred_p - this->point, this->normal);
    if (std::signbit(pos_dot) != std::signbit(pred_dot)) {
        glm::vec3 proj_vec = p.pred_p - this->point;
        float dist = glm::dot(proj_vec, this->normal);
        if (dist < 0) {
            dist = dist - SURFACE_OFFSET;
        } else {
            dist = dist + SURFACE_OFFSET;
        }
        glm::dvec3 proj_point = p.pred_p - this->normal * (dist);
        p.pred_p = proj_point;
        p.v = glm::dvec3();
    }

}

void Plane::render() {
}