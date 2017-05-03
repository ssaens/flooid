//
// Created by Dillon Yao on 4/26/17.
//


#include <glm/gtc/type_ptr.hpp>
#include "ParticleManager.h"
#include "../util.h"
#include "../display/Application.h"
#include "../cuda/CudaPBDSolver.cuh"

#define M 512

ParticleManager::ParticleManager() {}

void ParticleManager::init() {

    particle_radius = PARTICLE_RADIUS;
    shade_mode = SHADE_PARTICLE;
    skybox_id = parent->skybox.textureID;

    int nx = 30;
    int ny = 30;
    int nz = 30;

    float d = particle_radius * 2;
    for (int x = 0; x < nx; ++x) {
        for (int y = 1 / d; y < 1 / d + ny; ++y) {
            for (int z = 0; z < nz; ++z) {
                Particle par;
                par.p = vec3((x + 0.5 - nx * 0.5) * d, y * d, (z + 0.5 - nz * 0.5) * d);
                par.pred_p = glm::vec3();
                par.v = glm::vec3();
                par.m = PARTICLE_MASS;
                par.num_neighbors = 0;
                par.lambda = 0;
                par.collided = false;
                particles.push_back(par);
                initial_positions.push_back(par.p);
            }
        }
    }

    Plane ground; //BOTTOM
    ground.point = glm::vec3(0, 0, 0);
    ground.normal = glm::vec3(0, 1, 0);

    Plane side0; // RIGHT
    side0.point = glm::vec3(5, 0, 0);
    side0.normal = glm::vec3(1, 0, 0);

    Plane side1; //BACK
    side1.point = glm::vec3(0, 0, 5);
    side1.normal = glm::vec3(0, 0, 1);

    Plane side2; //LEFT
    side2.point = glm::vec3(-10, 0, 0);
    side2.normal = glm::vec3(1, 0, 0);

    Plane side3; // BACK
    side3.point = glm::vec3(0, 0, -5);
    side3.normal = glm::vec3(0, 0, 1);

    Plane side4; // TOP
    side4.point = glm::vec3(0, 10, 0);
    side4.normal = glm::vec3(0, 1, 0);
    planes.push_back(ground);
    planes.push_back(side0);
    planes.push_back(side1);
    planes.push_back(side2);
    planes.push_back(side3);
    planes.push_back(side4);

    particle_shader.load("src/shaders/particle.vert", "src/shaders/particle.frag");
    velocity_shader.load("src/shaders/particle_velocity.vert", "src/shaders/particle_velocity.frag");
    water_shader.load("src/shaders/water.vert", "src/shaders/water.frag");

    Light &light = this->parent->light;

    particle_shader.use();
    GLint lightPosLoc = glGetUniformLocation(particle_shader.program, "light_pos");
    glUniform3f(lightPosLoc, light.pos.x, light.pos.y, light.pos.z);
    GLint lightColorLoc = glGetUniformLocation(particle_shader.program, "light_color");
    glUniform3f(lightColorLoc, light.color.r, light.color.g, light.color.b);

    velocity_shader.use();
    lightPosLoc = glGetUniformLocation(particle_shader.program, "light_pos");
    glUniform3f(lightPosLoc, light.pos.x, light.pos.y, light.pos.z);
    lightColorLoc = glGetUniformLocation(particle_shader.program, "light_color");
    glUniform3f(lightColorLoc, light.color.r, light.color.g, light.color.b);

    water_shader.use();
    lightPosLoc = glGetUniformLocation(water_shader.program, "light_pos");
    glUniform3f(lightPosLoc, light.pos.x, light.pos.y, light.pos.z);
    lightColorLoc = glGetUniformLocation(water_shader.program, "light_color");
    glUniform3f(lightColorLoc, light.color.r, light.color.g, light.color.b);

    particle_mesh = generate_sphere_mesh(PARTICLE_RADIUS * 0.9f, 10, 10);
    glGenBuffers(1, &instanceVBO);
    glBindVertexArray(particle_mesh.VAO);

    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Particle) * particles.size(), NULL, GL_STREAM_DRAW);

    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (GLvoid *) offsetof(Particle, p));
    glVertexAttribDivisor(3, 1);

    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (GLvoid *) offsetof(Particle, v));
    glVertexAttribDivisor(4, 1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    triangles = &parent->test_model.triangles;

    cudaMalloc((void **) &d_particles, particles.size() * sizeof(Particle));
    cudaMalloc((void **) &d_triangles, triangles->size() * sizeof(Triangle));
    cudaMalloc((void **) &d_planes, planes.size() * sizeof(Plane));

    cudaMemcpy(d_planes, &planes[0], planes.size() * sizeof(Plane), cudaMemcpyHostToDevice);
    cudaMemcpy(d_triangles, &triangles->data()[0], triangles->size() * sizeof(Triangle), cudaMemcpyHostToDevice);
}

void ParticleManager::render(Camera &c, mat4 &projection, mat4 &view) {
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Particle) * particles.size(), NULL, GL_STREAM_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, offsetof(Particle, p), particles.size() * sizeof(Particle), &particles[0]);

    bind_shader(projection, view, c.pos);
    glBindVertexArray(particle_mesh.VAO);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skybox_id);
    glDrawElementsInstanced(GL_TRIANGLES, particle_mesh.indices.size(), GL_UNSIGNED_INT, 0, particles.size());
    glBindVertexArray(0);
}

void ParticleManager::step(float dt) {
    cudaMemcpy(d_particles, &particles[0], particles.size() * sizeof(Particle), cudaMemcpyHostToDevice);
    seed_position<<<(particles.size() + M-1) / M, M>>>(d_particles, particles.size());
    cudaThreadSynchronize();    
    cudaMemcpy(&particles[0], d_particles, particles.size() * sizeof(Particle), cudaMemcpyDeviceToHost);

    spacial_map.clear();
    for (int i = 0; i < particles.size(); ++i) {
        int hash = hash_bin(bin(particles[i]));
        if (spacial_map.find(hash) == spacial_map.end()) {
            spacial_map[hash] = new std::vector<int>();
        }
        spacial_map[hash]->push_back(i);
    }

    for (Particle &p : particles) {
        for (int n : this->neighborhood(p)) {
            if (p.num_neighbors < 50)
                p.neighborhood[p.num_neighbors++] = n;
        }
    }

    cudaMemcpy(d_particles, &particles[0], particles.size() * sizeof(Particle), cudaMemcpyHostToDevice);
    run_solver<<<(particles.size() + M-1) / M, M>>>(d_particles, particles.size(), d_triangles, triangles->size(), d_planes, planes.size());
    cudaThreadSynchronize();
    cudaMemcpy(&particles[0], d_particles, particles.size() * sizeof(Particle), cudaMemcpyDeviceToHost);
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

std::vector<int> ParticleManager::neighborhood(Particle& p) {
    std::vector<int> neighbors;
    glm::ivec3 original_bin = this->bin(p);
    for (int i = -1; i <= 1; ++i) {
        for (int j = -1; j <= 1; ++j) {
            for (int k = -1; k <= 1; ++k) {
                glm::ivec3 bin = original_bin + glm::ivec3(i, j, k);
                int hash = hash_bin(bin);
                if (spacial_map.find(hash) == spacial_map.end()) {
                    continue;
                }
                for (int n : *spacial_map[hash]) {
                    Particle &neighbor = particles[n];
                    if (&neighbor != &p && glm::length(neighbor.pred_p - p.pred_p) <= KERNEL_RADIUS) {
                        neighbors.push_back(n);
                    }
                }
            }
        }
    }
    return neighbors;
}

void ParticleManager::next_mode() {
    shade_mode = (shade_mode + 1) % SHADERS_TOTAL;
}

void ParticleManager::bind_shader(glm::mat4 &projection, glm::mat4 &view, glm::vec3 &view_pos) {
    switch(shade_mode) {
        case SHADE_PARTICLE: {
            particle_shader.use();
            glUniform3f(glGetUniformLocation(particle_shader.program, "view_pos"), view_pos.x, view_pos.y, view_pos.z);
            glUniformMatrix4fv(glGetUniformLocation(particle_shader.program, "projection"), 1, GL_FALSE, value_ptr(projection));
            glUniformMatrix4fv(glGetUniformLocation(particle_shader.program, "view"), 1, GL_FALSE, value_ptr(view));
            break;
        }

        case SHADE_VELOCITY: {
            velocity_shader.use();
            glUniform3f(glGetUniformLocation(velocity_shader.program, "view_pos"), view_pos.x, view_pos.y, view_pos.z);
            glUniformMatrix4fv(glGetUniformLocation(velocity_shader.program, "projection"), 1, GL_FALSE, value_ptr(projection));
            glUniformMatrix4fv(glGetUniformLocation(velocity_shader.program, "view"), 1, GL_FALSE, value_ptr(view));
            break;
        }

        case SHADE_WATER: {
            water_shader.use();
            glUniform3f(glGetUniformLocation(water_shader.program, "view_pos"), view_pos.x, view_pos.y, view_pos.z);
            glUniformMatrix4fv(glGetUniformLocation(water_shader.program, "projection"), 1, GL_FALSE, value_ptr(projection));
            glUniformMatrix4fv(glGetUniformLocation(water_shader.program, "view"), 1, GL_FALSE, value_ptr(view));
            glActiveTexture(GL_TEXTURE0);
            glUniform1i(glGetUniformLocation(water_shader.program, "skybox"), 0);
            break;
        }
    }
}

void ParticleManager::set_parent(Application *app) {
    this->parent = app;
}

void ParticleManager::reset() {
    auto par = particles.begin();
    auto pos = initial_positions.begin();
    for (;par != particles.end();) {
        par->p = *pos;
        par->pred_p = glm::dvec3();
        par->v = glm::dvec3();
        par->num_neighbors = 0;
        par->lambda = 0;
        ++par;
        ++pos;
    }
}
