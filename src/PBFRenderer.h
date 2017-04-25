//
// Created by Dillon Yao on 4/23/17.
//

#ifndef FLOOID_PBFRENDERER_H
#define FLOOID_PBFRENDERER_H

#include "ParticleManager.h"
#include "scene/Scene.h"
#include "AppConfig.h"

using namespace eng;
using namespace std;


struct DrawStyle {
    Color particleColor;
    Color planeColor;
};

class PBFRenderer {
private:
    ParticleManager particles;
//    Scene scene;


public:
    PBFRenderer(AppConfig &config) : particles{config} {}
    void render() {}
};



#endif //FLOOID_PBFRENDERER_H
