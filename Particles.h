/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Particles.h
 * Author: swl
 *
 * Created on April 15, 2016, 12:16 PM
 */

#ifndef PARTICLES_H
#define PARTICLES_H

#include "utils.h"
#include <vector>
#include <map>
#include "objects/Plane.h"
#include "solver.h"

#if defined(__APPLE_CC__)
#include <GLUT/glut.h>

#else
#include <GL/glut.h>
#include <math.h>
#endif

class Particles {
public:
    Particles();
    void render() const;
    void step(double dt, std::vector<Plane *> planes); // simulate one frame

private:
    std::vector<Particle> particles;
    glm::ivec3 bin(Particle &p);
    int hash_bin(glm::ivec3 pos);
    std::vector<Particle *> neighborhood(Particle& p);
    void collide_particles(Particle &p1, Particle &p2);

    std::map<int, std::vector<Particle *> *> spacial_map;
    std::vector<glm::dvec3> external_accels;

    ConstraintSolver solver;
};

#endif /* PARTICLES_H */

