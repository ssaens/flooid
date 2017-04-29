//
// Created by Dillon Yao on 4/26/17.
//

#ifndef FLOOID2_PARTICLEMANAGER_H
#define FLOOID2_PARTICLEMANAGER_H


#include <vector>
#include <glm/glm.hpp>
#include "Particle.h"
#include "../common.h"
#include "../scene/Shader.h"
#include <map>
#include "PBDSolver.h"
#include "../scene/Plane.h"
#include "../scene/Mesh.h"

using namespace glm;

class ParticleManager {
private:
    Mesh particle_mesh;
    Shader particle_shader;

    std::vector<Particle> particles;
    std::vector<glm::vec3> particle_positions;
    GLuint instanceVBO;

    ivec3 bin(Particle &p);
    int hash_bin(glm::ivec3 pos);
    std::vector<Particle *> neighborhood(Particle& p);
    std::vector<glm::vec3> accels;

    std::map<int, std::vector<Particle *> *> spacial_map;

    int solver_iters;
    float particle_radius;

    std::vector<Plane> planes;

public:
    ParticleManager();
    ~ParticleManager() { spacial_map.clear(); }
    void init();
    void render();
    void step(float dt);
    void set_shader(Shader &shader);
};


#endif //FLOOID2_PARTICLEMANAGER_H
