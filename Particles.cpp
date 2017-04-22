/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Particles.cpp
 * Author: swl
 * 
 * Created on April 15, 2016, 12:16 PM
 */

#include "Particles.h"
#include <iostream>

void stringify_vec(glm::dvec3 vec);

Particles::Particles() {

    external_accels.push_back(ACCEL_GRAVITY);
    
    int nx = 3;
    int ny = 3;
    int nz = 3;
    float d = 0.2;
    for(int x=0; x<nx; x++)
    {
        for(int y=0; y<ny; y++)
        {
            for(int z=0; z<nz; z++)
            {
                Particle par;
                par.pos = glm::dvec3((x+0.5-nx*0.5)*d, (y+0.5)*d-1.0, (z+0.5-nz*0.5)*d);
                par.force = glm::dvec3(0, 0, 0);
                par.vel = glm::dvec3(0, 0, 0);
                par.mass = 1;
                particles.push_back(par);
            }
        }
    }
}

void Particles::render() const
{
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = { 50.0 };
    GLfloat light_position[] = { 10.0, 10.0, 10.0, 0.0 };
    glShadeModel (GL_SMOOTH);
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_DIFFUSE);
    glColor3f(0.2, 0.5, 0.8);
    glColorMaterial(GL_FRONT, GL_SPECULAR);
    glColor3f(0.9, 0.9, 0.9);
    glColorMaterial(GL_FRONT, GL_AMBIENT);
    glColor3f(0.2, 0.5, 0.8);
    
    for(const Particle &par : particles)
    {    
        
        glPushMatrix();
        glTranslatef(par.pos.x, par.pos.y, par.pos.z);
        glutSolidSphere(0.05, 10, 10);
        glPopMatrix();
    }
    
    glPopAttrib();
}

void Particles::step(double dt) {
    for (Particle& p : particles) {
        for (auto accel : this->external_accels) {
            p.force += accel * p.mass;
        }
    }

    for (Particle& p : particles) {
        p.pos += p.vel * dt;
        p.vel = p.vel + p.force * (1. / p.mass) * dt;
        // std::cout << "(" << p.pos.x << ", " << p.pos.y << ", " << p.pos.z << ")" << " ";
        // std::cout << "(" << p.vel.x << ", " << p.vel.y << ", " << p.vel.z << ")" << std::endl;
    }
}

void stringify_vec(glm::dvec3 vec) {
    std::cout << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")" << std::endl;
}