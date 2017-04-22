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

Particles::Particles() :
    solver(REST_DENSITY, H, EPS) 
{
    external_accels.push_back(ACCEL_GRAVITY);
    
    int nx = 5;
    int ny = 5;
    int nz = 5;
    float d = RADIUS * 2;
    for(int x=0; x<nx; x++)
    {
        for(int y=3; y<3+ny; y++)
        {
            for(int z=0; z<nz; z++)
            {
                Particle par;
                par.pos = glm::dvec3((x+0.5-nx*0.5)*d, (y+0.5-ny*0.5)*d, (z+0.5-nz*0.5)*d);
                par.pred_pos = glm::dvec3();
                par.force = glm::dvec3();
                par.vel = glm::dvec3();
                par.mass = 0.0005233; //kg
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
    glShadeModel(GL_SMOOTH);
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
    
    for (const Particle &par : particles) {    
        
        glPushMatrix();
        glTranslatef(par.pos.x, par.pos.y, par.pos.z);
        glutSolidSphere(0.05, 10, 10);
        glPopMatrix();
    }
    
    glPopAttrib();
}

void Particles::step(double dt, Plane& plane) {
    for (Particle &p : particles) {
        stringify_vec(p.pos);
        for (auto accel : this->external_accels) {
            p.force += accel * p.mass;
        }
    }

    for (Particle &p : particles) {
        p.vel = p.vel + p.force * (1. / p.mass) * dt;
        p.pred_pos = p.pos + p.vel * dt;
    }

    spacial_map.clear();
    for (Particle &p : particles) {
        int hash = this->hash_bin(this->bin(p));
        if (spacial_map.find(hash) == spacial_map.end()) {
            spacial_map[hash] = new std::vector<Particle *>;
        }
        spacial_map[hash]->push_back(&p);
    }

    for (Particle &p : particles) {
        p.neighborhood = this->neighborhood(p);
    }

    for (int i = 0; i < SOLVER_ITERS; ++i) {
        for (Particle &p_i : particles) {
            double lambda = solver.lambda_i(&p_i, p_i.neighborhood);
            p_i.lambda = lambda;
        }
        for (Particle &p_i : particles) {
            p_i.delta_pos = solver.delta_p(&p_i, p_i.neighborhood);
            // stringify_vec(p_i.delta_pos);
            plane.collide(p_i);
        }
        for (Particle &p_i : particles) {
            p_i.pred_pos = p_i.pred_pos + p_i.delta_pos;
        }
    }

    // for (Particle &p : particles) {
    //     std::vector<Particle *> neighbors = this->neighbors(p);
    //     for (Particle *neighbor : neighbors) {
    //         this->collide_particles(p, *neighbor);
    //     }
    // }

    for (Particle& p : particles) {
        p.vel = (1.f / dt) * (p.pred_pos - p.pos);
        p.pos = p.pred_pos;
        p.force = glm::dvec3(0, 0, 0);
    }

}

glm::ivec3 Particles::bin(Particle& p) {
    double d = H;
    int bin_x = std::floor(p.pred_pos.x / d);
    int bin_y = std::floor(p.pred_pos.y / d);
    int bin_z = std::floor(p.pred_pos.z / d);
    return glm::ivec3(bin_x, bin_y, bin_z);
}

int Particles::hash_bin(glm::ivec3 pos) {
    return (pos.x * 0x9e3779b9 + pos.y) * 1610612741 + pos.z;
}

std::vector<Particle *> Particles::neighborhood(Particle& p) {
    std::vector<Particle *> neighbors;
    for (int i = -1; i <= 1; ++i) {
        for (int j = -1; j <= 1; ++j) {
            for (int k = -1; k <= 1; ++k) {
                glm::ivec3 offset = glm::ivec3(i, j, k);
                glm::ivec3 bin = this->bin(p) + offset;
                int hash = hash_bin(bin);
                if (spacial_map.find(hash) == spacial_map.end()) {
                    continue;
                }
                for (Particle *neighbor : *spacial_map[hash]) {
                    if (neighbor != &p && glm::length(neighbor->pred_pos - p.pred_pos) <= H) {
                        neighbors.push_back(neighbor);
                    }
                }
            }
        }
    }
    return neighbors;
}

void Particles::collide_particles(Particle &p1, Particle &p2) {
    p1.vel *= -1;
    p2.vel *= -1;
    glm::dvec3 d = p1.pos - p2.pos;
}
