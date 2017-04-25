//
// Created by Dillon Yao on 4/23/17.
//

#include "ParticleManager.h"
#include "common.h"
#include "utils.h"

using namespace glm;

ParticleManager::ParticleManager(AppConfig &config) :
    solver_iters(config.solver_iters),
    particle_radius(config.particle_radius),
    particle_mass(config.particle_mass),
    solver(
        config.rest_density,
        config.kernel_radius,
        config.eps,
        config.viscosity,
        config.pressure_strength,
        config.pressure_power,
        config.delta_q
    ) {

    int nx = 5;
    int ny = 10;
    int nz = 5;

    float d = particle_radius * 2;
    for (int x = 0; x < nx; ++x) {
        for (int y = 3; y < 3 + ny; ++y) {
            for (int z = 0; z < nz; ++z) {
                Particle par;
                par.p = dvec3((x + 0.5 - nx * 0.5) * d, y * d, (z + 0.5 - nz * 0.5) * d);
                par.m = PARTICLE_MASS;
                particles.push_back(par);
            }
        }
    }

    Plane ground(glm::dvec3(0, 0, 0), glm::dvec3(0, 1, 0), 0);
    Plane side0(glm::dvec3(1, 0, 0), glm::dvec3(1, 0, 0), 0);
    Plane side1(glm::dvec3(0, 0, 1), glm::dvec3(0, 0, 1), 0);
    Plane side2(glm::dvec3(-2, 0, 0), glm::dvec3(1, 0, 0), 0);
    Plane side3(glm::dvec3(0, 0, -1), glm::dvec3(0, 0, 1), 0);
    Plane side4(glm::dvec3(0, 3, 0), glm::dvec3(0, 1, 0), 0);
    planes.push_back(ground);
    planes.push_back(side0);
    planes.push_back(side1);
    planes.push_back(side2);
    planes.push_back(side3);
    planes.push_back(side4);
}

void ParticleManager::render() const {
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
        glTranslatef(par.p.x, par.p.y, par.p.z);
        draw_sphere(PARTICLE_RADIUS, 10, 10);
        glPopMatrix();
//        draw_sphere(par.p, PARTICLE_RADIUS);
    }
    glPopAttrib();
}

void ParticleManager::step(float dt) {
    for (Particle &p : particles) {
        for (auto accel : EXTERNAL_ACCELRATIONS) {
            p.f += accel * p.m;
        }
    }

    for (Particle &p : particles) {
        p.v = p.v + p.f * (1.f / p.m) * dt;
        p.pred_p = p.p + p.v * dt;
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

    for (int i = 0; i < solver_iters; ++i) {
        for (Particle &p_i : particles) {
            float lambda = solver.lambda_i(&p_i, p_i.neighborhood);
            p_i.lambda = lambda;
        }
        for (Particle &p_i : particles) {
            p_i.d_p = solver.delta_p(&p_i, p_i.neighborhood);
        }
        for (Particle &p_i : particles) {
            p_i.pred_p += p_i.d_p;
            for (Plane &plane : planes)
                plane.collide(p_i);
        }
    }

    for (Particle& p : particles) {
        p.v = (1.f / dt) * (p.pred_p - p.p);
        p.f += solver.f_vorticity(&p, p.neighborhood);
        p.v = solver.XSPH_vel(&p, p.neighborhood);
        p.p = p.pred_p;
        p.f = glm::dvec3(0, 0, 0);
    }
}

glm::ivec3 ParticleManager::bin(Particle& p) {
    double d = KERNEL_RADIUS;
    int bin_x = std::floor(p.pred_p.x / d);
    int bin_y = std::floor(p.pred_p.y / d);
    int bin_z = std::floor(p.pred_p.z / d);
    return glm::ivec3(bin_x, bin_y, bin_z);
}

int ParticleManager::hash_bin(glm::ivec3 pos) {
    return (pos.x * 0x9e3779b9 + pos.y) * 1610612741 + pos.z;
}

std::vector<Particle *> ParticleManager::neighborhood(Particle& p) {
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
                    if (neighbor != &p && glm::length(neighbor->pred_p - p.pred_p) <= KERNEL_RADIUS) {
                        neighbors.push_back(neighbor);
                    }
                }
            }
        }
    }
    return neighbors;
}
