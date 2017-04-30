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
#include "../display/Camera.h"


class Application;

using namespace glm;

enum {
    SHADE_PARTICLE,
    SHADE_VELOCITY,
    SHADE_WATER,
    SHADERS_TOTAL,
};

class ParticleManager {
private:
    Mesh particle_mesh;
    Shader particle_shader, velocity_shader, water_shader;
    int shade_mode;
    void bind_shader(glm::mat4 &projection, glm::mat4 &view, glm::vec3 &view_pos);

    std::vector<glm::vec3> initial_positions;
    std::vector<Particle> particles;
    GLuint instanceVBO, skybox_id;

    ivec3 bin(Particle &p);

    int hash_bin(glm::ivec3 pos);

    std::vector<Particle *> neighborhood(Particle &p);

    std::vector<glm::vec3> accels;

    std::map<int, std::vector<Particle *> *> spacial_map;

    float particle_radius;

    std::vector<Plane> planes;

    Application *parent;

public:
    ParticleManager();

    ~ParticleManager() { spacial_map.clear(); }

    void init();

    void render(Camera &c, glm::mat4 &project, glm::mat4 &view);

    void step(float dt);

    void next_mode();

    void set_parent(Application *app);

    void reset();
};


#endif //FLOOID2_PARTICLEMANAGER_H
