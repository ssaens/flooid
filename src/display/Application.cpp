//
// Created by Dillon Yao on 4/26/17.
//

#include "Application.h"
#include <glm/gtc/type_ptr.hpp>

Application::Application() {}

Application::~Application() {}

void Application::init() {
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_CULL_FACE);

    light_shader.load("src/shaders/basic.vert", "src/shaders/basic.frag");

    last_x = screen_w / 2;
    last_y = screen_h / 2;
    first_mouse = true;
    light.pos = glm::vec3(0, 3, 2);
    light.color = glm::vec3(1, 1, 1);
    light_mesh = generate_cube_mesh(.2);
    skybox.load_cube_map(faces);

    // glm::vec3 offset(-0.5, -0.5, 0.5);
    // test_model.set_offset(offset);
    // test_model.load("obj/cube.obj");

    pm.set_parent(this);

    pm.init();

    for (int i = 0; i < 1024; ++i) {
        keys[i] = 0;
    }
}

void Application::render() {
    glm::mat4 projection = glm::perspective(camera.zoom, (float) screen_w / (float) screen_h, camera.n_clip, camera.f_clip);
    glm::mat4 view = camera.get_view_matrix();

    pm.render(camera, projection, view);

    light_shader.use();
    glm::mat4 model;
    glUniformMatrix4fv(glGetUniformLocation(light_shader.program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(glGetUniformLocation(light_shader.program, "view"), 1, GL_FALSE, glm::value_ptr(view));
    model = glm::translate(model, light.pos);
    glUniformMatrix4fv(glGetUniformLocation(light_shader.program, "model"), 1, GL_FALSE, glm::value_ptr(model));
    light_mesh.render(light_shader);

    // model = glm::mat4();
    // glUniformMatrix4fv(glGetUniformLocation(light_shader.program, "model"), 1, GL_FALSE, glm::value_ptr(model));
    // test_model.render(light_shader);

    skybox.render(view, projection);
    
}

void Application::update(float dt) {
    move_camera(dt);
    pm.step(dt);
    pm.step(dt);
}

void Application::resize(int width, int height) {
    screen_w = width;
    screen_h = height;
}

void Application::keyboard_event(int key, int action, int mods) {
    if (key == GLFW_KEY_M) {
        pm.next_mode();
    }
    if (key == GLFW_KEY_R) {
        pm.reset();
    }
    if (key >= 0 && key < 1024) {
        if (action == GLFW_PRESS) {
            keys[key] = true;
        } else if (action == GLFW_RELEASE) {
            keys[key] = false;
        }
    }
}

void Application::cursor_event(double cursor_x, double cursor_y) {
    if (first_mouse) {
        last_x = cursor_x;
        last_y = cursor_y;
        first_mouse = false;
    }
    GLfloat xoffset = cursor_x - last_x;
    GLfloat yoffset = last_y - cursor_y;

    last_x = cursor_x;
    last_y = cursor_y;
    camera.cursor_event(xoffset, yoffset);
}

void Application::scroll_event(double offset_x, double offset_y) {
    camera.scroll_event(offset_y);
}

void Application::mouse_event(int button, int action, int mods) {

}

void Application::move_camera(float dt) {
    if (keys[GLFW_KEY_W]) {
        camera.keyboard_event(FORWARD, dt);
    }
    if (keys[GLFW_KEY_S]) {
        camera.keyboard_event(BACKWARD, dt);
    }
    if (keys[GLFW_KEY_A]) {
        camera.keyboard_event(LEFT, dt);
    }
    if (keys[GLFW_KEY_D]) {
        camera.keyboard_event(RIGHT, dt);
    }
    if (keys[GLFW_KEY_SPACE]) {
        camera.keyboard_event(UP, dt);
    }
    if (keys[GLFW_KEY_LEFT_SHIFT]) {
        camera.keyboard_event(DOWN, dt);
    }
}

void Application::set_cube_path(std::string cube_path) {
    faces.push_back(cube_path + "right");
    faces.push_back(cube_path + "left");
    faces.push_back(cube_path + "top");
    faces.push_back(cube_path + "bottom");
    faces.push_back(cube_path + "back");
    faces.push_back(cube_path + "front");

}
