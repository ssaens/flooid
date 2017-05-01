//
// Created by Dillon Yao on 4/26/17.
//

#ifndef FLOOID2_APPLICATION_H
#define FLOOID2_APPLICATION_H

#include <eng/eng.h>
#include "Camera.h"
#include "../util.h"
#include "../scene/Shader.h"
#include "../scene/Mesh.h"
#include "../PBD/ParticleManager.h"
#include "../scene/Model.h"
#include "../scene/Light.h"
#include "../Skybox/Skybox.h"

using namespace eng;

class Application : public Renderer {
    friend class ParticleManager;
public:
    Application();
    ~Application();

    virtual void init();
    virtual void render();
    virtual void update(float dt);

    virtual void resize(int width, int height);
    virtual void keyboard_event(int key, int action, int mods);
    virtual void cursor_event(double cursor_x, double cursor_y);
    virtual void scroll_event(double offset_x, double offset_y);
    virtual void mouse_event(int button, int action, int mods);

    virtual std::string name() { return "Position Based Fluid Simulator"; }

    void set_cube_path(std::string cube_path);

private:
    vector<std::string> faces;
    Skybox skybox;
    Camera camera;
    Shader light_shader;
    Mesh light_mesh;
    Light light;
    Model test_model;

    ParticleManager pm;

    // Hacky Camera Stuff
    bool keys[1024];
    float last_x, last_y;
    bool first_mouse;

    void move_camera(float dt);
};


#endif //FLOOID2_APPLICATION_H
