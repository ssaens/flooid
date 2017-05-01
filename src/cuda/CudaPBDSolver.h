//
// Created by Dillon Yao on 4/30/17.
//

#ifndef FLOOID2_CUDAPBDSOLVER_H_H
#define FLOOID2_CUDAPBDSOLVER_H_H

#define GLM_FORCE_CUDA
#include <glm/glm.hpp>
#include "../PBD/Particle.h"

using namespace glm;

__global__ void seed_position(Particle *particles, int n);

__global__ void run_solver(Particle *particles, int n, Triangle *triangles, int num_triangles, Plane *plane, int num_planes);

__device__ inline float poly6(vec3 r_ij, float h);

__device__ inline float spiky(vec3 r_ij, float h);

__device__ inline vec3 spiky_grad(vec3 r_ij, float h);

__device__ float rho_i(Particle *p_i, Particle *particles);

__device__ float lambda_i(Particle *p_i, Particle *particles);

__device__ float C_i(Particle *p_i, Particle *particles);

__device__ vec3 grad_k_Ci(Particle *p_k, Particle *p_i, Particle *particles);

__device__ vec3 delta_p(Particle *p_i, Particle *particles);

__device__ vec3 vorticity(Particle *p_i, Particle *particles);

__device__ vec3 f_vorticity(Particle *p_i, Particle *particles);

__device__ vec3 XSPH_vel(Particle *p_i, Particle *particles);

__device__ void triangle_collide(Triangle &t, Particle &p);

__device__ void plane_collide(Plane &p, Particle &par);

#endif //FLOOID2_CUDAPBDSOLVER_H_H
