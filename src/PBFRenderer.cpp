//
// Created by Dillon Yao on 4/23/17.
//

#include "PBFRenderer.h"
#include "utils.h"

using namespace glm;

void PBFRenderer::render() {
//    scene.draw();
    particles.step(0.0083);
    particles.render();
}

void PBFRenderer::load_shader(ShaderInfo &info) {
    shader = Shader(info);
}

void PBFRenderer::load_scene(string path) {
    scene.load_scene(path);
}

