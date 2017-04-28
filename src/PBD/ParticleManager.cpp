//
// Created by Dillon Yao on 4/26/17.
//

#include "ParticleManager.h"
#include "../util.h"

using namespace glm;

ParticleManager::ParticleManager() {
    accels.push_back(ACCEL_GRAVITY);
    int nx = 10;
    int ny = 10;
    int nz = 10;

    float d = particle_radius * 2;
    for (int x = 0; x < nx; ++x) {
        for (int y = 3; y < 3 + ny; ++y) {
            for (int z = 0; z < nz; ++z) {
                Particle par;
                par.p = dvec3((x + 0.5 - nx * 0.5) * d, y * d, (z + 0.5 - nz * 0.5) * d);
                par.pred_p = glm::dvec3();
                par.f = glm::dvec3();
                par.v = glm::dvec3();
                par.m = PARTICLE_MASS;
                particles.push_back(par);
            }
        }
    }

    Plane ground(glm::dvec3(0, 0, 0), glm::dvec3(0, 1, 0), 0);
    Plane side0(glm::dvec3(1, 0, 0), glm::dvec3(1, 0, 0), 0);
    Plane side1(glm::dvec3(0, 0, 1), glm::dvec3(0, 0, 1), 0);
    Plane side2(glm::dvec3(-1, 0, 0), glm::dvec3(1, 0, 0), 0);
    Plane side3(glm::dvec3(0, 0, -1), glm::dvec3(0, 0, 1), 0);
    Plane side4(glm::dvec3(0, 3, 0), glm::dvec3(0, 1, 0), 0);
    planes.push_back(ground);
    planes.push_back(side0);
    planes.push_back(side1);
    planes.push_back(side2);
    planes.push_back(side3);
    planes.push_back(side4);

    particle_mesh = generate_sphere_mesh(PARTICLE_RADIUS * 0.09f, 10, 10);
    particle_shader.load("src/shaders/fpscam.vert", "src/shaders/fpscam.frag");
}

void ParticleManager::render() const {
    particle_mesh.render(particle_shader);
}

void ParticleManager::step(float dt) {
    for (Particle &p : particles) {
        for (auto accel : accels) {
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
            float lambda = PBDSolver::getPBDsolver()->lambda_i(&p_i, p_i.neighborhood);
            p_i.lambda = lambda;
        }
        for (Particle &p_i : particles) {
            p_i.dp = PBDSolver::getPBDsolver()->delta_p(&p_i, p_i.neighborhood);
        }
        for (Particle &p_i : particles) {
            p_i.pred_p += p_i.dp;
            for (Plane &plane : planes)
                plane.collide(p_i);
        }
    }

    for (Particle& p : particles) {
        p.v = (1.f / dt) * (p.pred_p - p.p);
        p.f += PBDSolver::getPBDsolver()->f_vorticity(&p, p.neighborhood);
        p.v = PBDSolver::getPBDsolver()->XSPH_vel(&p, p.neighborhood);
        p.p = p.pred_p;
        p.f = glm::vec3(0, 0, 0);
    }
}

glm::ivec3 ParticleManager::bin(Particle& p) {
    double d = KERNEL_RADIUS; // TODO Change d to private var bin_size
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