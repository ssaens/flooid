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
    void step(); // simulate one frame
private:
    struct Particle
    {
        glm::dvec3 pos;
        glm::dvec3 pred_pos;
        glm::dvec3 vel;
        glm::dvec3 force;
        float mass;
    };
    
    std::vector<Particle> particles;
    float hash_particle(Particle &p);
    std::map<int, std::vector<Particle *>> spacial_map;
    std::vector<glm::fvec3&> external_forces;
};

#endif /* PARTICLES_H */

