//
// Created by Dillon Yao on 4/26/17.
//

#ifndef FLOOID2_APPLICATION_H
#define FLOOID2_APPLICATION_H

#include <eng/eng.h>
#include "Camera.h"
#include "../model/Shader.h"

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
    virtual void update();

    virtual void resize(int width, int height);
    virtual void keyboard_event(int key, int action, int mods);
    virtual void cursor_event(double cursor_x, double cursor_y);
    virtual void scroll_event(double offset_x, double offset_y);
    virtual void mouse_event(int button, int action, int mods);

    virtual std::string name() { return "Test"; }

private:
    GLuint VAO, VBO;
    Camera camera;
    Shader shader, solid_shader;
    Mode mode;
};


#endif //FLOOID2_APPLICATION_H
