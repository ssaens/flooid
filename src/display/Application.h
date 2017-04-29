//
// Created by Dillon Yao on 4/26/17.
//

#ifndef FLOOID2_APPLICATION_H
#define FLOOID2_APPLICATION_H

#include <eng/eng.h>
#include "Camera.h"
#include "../scene/Shader.h"
#include "../scene/Mesh.h"
#include "../PBD/ParticleManager.h"

using namespace eng;

enum Mode {
    MODE_EDIT,
    MODE_VIEW,
};

class Application : public Renderer {
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

private:
    Camera camera;
    Shader particle_shader;
    Mode mode;

    ParticleManager pm;

    // Hacky Camera Stuff
    bool keys[1024];
    float last_x, last_y;
    bool first_mouse;

    void move_camera(float dt);
};


#endif //FLOOID2_APPLICATION_H
