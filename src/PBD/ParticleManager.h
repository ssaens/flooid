//
// Created by Dillon Yao on 4/26/17.
//

#ifndef FLOOID2_PARTICLEMANAGER_H
#define FLOOID2_PARTICLEMANAGER_H


#include <vector>
#include <glm/glm.hpp>
#include "Particle.h"
#include "../common.h"
#include "../model/Shader.h"

using namespace glm;

class ParticleManager {
private:
    std::vector<Particle> particles;
    std::vector<vec3> particle_positions;

    Shader particle_shader;

    float dt;

public:
    ParticleManager();
    ~ParticleManager();

    void render();
    void step();
};


#endif //FLOOID2_PARTICLEMANAGER_H
