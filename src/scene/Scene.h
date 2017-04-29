//
// Created by Dillon Yao on 4/25/17.
//

#ifndef FLOOID_SCENE_H
#define FLOOID_SCENE_H

#include <assimp/scene.h>
#include "Model.h"
#include "Light.h"

using namespace std;

class Scene {
private:
    Model model;
    Light light;
    std::vector<glm::vec3> particle_positions;

public:
    Scene();
    void load(std::string path);
//    void save(std::string path);
};


#endif //FLOOID_SCENE_H
