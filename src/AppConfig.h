//
// Created by Dillon Yao on 4/25/17.
//

#ifndef FLOOID_APPCONFIG_H
#define FLOOID_APPCONFIG_H

#include <glm/glm.hpp>
#include "common.h"

struct AppConfig {
    // Physical Properties
    std::vector<vec3> external_accelerations;

    // Particle Properties
    float particle_radius;
    float particle_mass;

    // PBD Solver Configurations
    int solver_iters;
    float rest_density;
    float kernel_radius;
    float eps;
    float viscosity;
    float pressure_strength;
    float pressure_power;
    vec3 delta_q;

    AppConfig() {
        external_accelerations = {
                ACCEL_GRAVITY,
        };
        particle_radius = PARTICLE_RADIUS;
        particle_mass = PARTICLE_MASS;

        solver_iters = SOLVER_ITERS;
        rest_density = REST_DENSITY;
        kernel_radius = KERNEL_RADIUS;
        eps = EPS;
        viscosity = VISCOSITY;
        pressure_strength = PRESSURE_STRENGTH;
        pressure_power = PRESSURE_POW;
        delta_q = DELTA_Q;
    }
};

#endif //FLOOID_APPCONFIG_H
