//
// Created by Dillon Yao on 4/30/17.
//

#ifndef FLOOID2_CUDAPBDSOLVER_H_H
#define FLOOID2_CUDAPBDSOLVER_H_H

#include "../PBD/Particle.h"
#include "../scene/triangle.h"
#include "../scene/Plane.h"

__global__ void seed_position(Particle *particles, int n);

__global__ void run_solver(Particle *particles, int n, Triangle *triangles, int num_triangles, Plane *plane, int num_planes);

#endif //FLOOID2_CUDAPBDSOLVER_H_H
