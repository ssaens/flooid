//
// Created by Dillon Yao on 4/26/17.
//

#include "Application.h"
#include "../util.h"
#include <glm/gtc/type_ptr.hpp>

Application::Application() {}

Application::~Application() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void Application::init() {
    
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);

    sphere = generate_sphere_mesh(1, 10, 10);

    shader.load("src/shaders/particle.vert", "src/shaders/particle.frag");
    solid_shader.load("src/shaders/particle.vert", "src/shaders/particle_outline.frag");
    mode = MODE_EDIT;

    last_x = screen_w / 2;
    last_y = screen_h / 2;
    first_mouse = true;
}

void Application::render() {
    shader.use();
    glm::mat4 projection = glm::perspective(camera.zoom, (float) screen_w / (float) screen_h, camera.n_clip, camera.f_clip);
    glm::mat4 view = camera.get_view_matrix();
    glm::mat4 model;

    glm::vec3 lightPos(5, 5, 5);
    glm::vec3 lightColor(1, 1, 1);
    GLint lightPosLoc = glGetUniformLocation(shader.program, "light_pos");
    glUniform3f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z);
    GLint lightColorLoc = glGetUniformLocation(shader.program, "light_color");
    glUniform3f(lightColorLoc, lightColor.r, lightColor.g, lightColor.b);

    // Get their uniform location
    glUniformMatrix4fv(glGetUniformLocation(shader.program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(glGetUniformLocation(shader.program, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shader.program, "model"), 1, GL_FALSE, glm::value_ptr(model));

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
    sphere.render(shader);
}

void Application::update(float dt) {
    move_camera(dt);
}

void Application::resize(int width, int height) {

}

void Application::keyboard_event(int key, int action, int mods) {
    if (key == GLFW_KEY_C) {
        if (mode == MODE_EDIT) {
            mode = MODE_VIEW;
        } else {
            mode = MODE_EDIT;
        }
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