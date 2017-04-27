//
// Created by Dillon Yao on 4/26/17.
//

#include "eng/View.h"
#include <iostream>

const int DEFAULT_WIDTH = 960;
const int DEFAULT_HEIGHT = 600;

using namespace std;

namespace eng {

    GLFWwindow *View::window = nullptr;
    int View::width, View::height;
    Renderer *View::renderer = nullptr;

    int View::framecount = 0;
    float View::sys_last;
    float View::sys_curr;

    string View::title = "";
    bool View::HDPI = false;

    View::View() {}

    View::~View() {
        glfwDestroyWindow(window);
        glfwTerminate();
    }

    void View::init() {

        if (!renderer) {
            cerr << "Error: No renderer set for view" << endl;
            exit(EXIT_FAILURE);
        }

        // Init GLFW
        glfwSetErrorCallback(error_callback);
        if (!glfwInit()) {
            cerr << "Error: Failed to init GLFW" << endl;
            glfwTerminate();
            exit(EXIT_FAILURE);
        }

        // Create GLFW window
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_SAMPLES, 4);

        string title = this->renderer->name();
        this->window = glfwCreateWindow(DEFAULT_WIDTH, DEFAULT_HEIGHT, title.c_str(), NULL, NULL);
        if (!this->window) {
            cerr << "Error: Failed to create GLFWwindow" << endl;
            glfwTerminate();
            exit(EXIT_FAILURE);
        }

        // Set OpenGL Context
        glfwMakeContextCurrent(this->window);

        glfwSetFramebufferSizeCallback(this->window, this->resize_callback);
        glfwSetKeyCallback(this->window, this->key_callback);
        glfwSetCursorPosCallback(this->window, this->cursor_callback);
        glfwSetScrollCallback(this->window, this->scroll_callback);
        glfwSetInputMode(this->window, GLFW_STICKY_MOUSE_BUTTONS, 1);
        glfwSetMouseButtonCallback(this->window, this->mouse_button_callback);

        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        glewExperimental = GL_TRUE;
        if (glewInit() != GLEW_OK) {
            cerr << "Error: Failed to create GLFWwindow" << endl;
            exit(EXIT_FAILURE);
        }

        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);

        if (width > DEFAULT_WIDTH) {
            HDPI = true;
            renderer->use_hdpi();
        }

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        renderer->set_frame_size(width, height);
        renderer->init();
        resize_callback(this->window, width, height);
    }

    void View::begin() {
        while (!glfwWindowShouldClose(window)) {
            sys_curr = glfwGetTime();
            float dt = sys_curr - sys_last;
            update(dt);
            sys_last = sys_curr;
        }
    }

    void View::update(double dt) {
        glfwPollEvents();

        renderer->update(dt);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        renderer->render();

        glfwSwapBuffers(window);
    }

    void View::set_renderer(Renderer *renderer) {
        this->renderer = renderer;
    }

    void View::error_callback(int error, const char *desc) {
        cerr << "GLFW Error " << error << ": " << desc << endl;
    }

    void View::key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
        switch(action) {
            case GLFW_PRESS:
                if (key == GLFW_KEY_ESCAPE) {
                    glfwSetWindowShouldClose(window, GL_TRUE);
                }
                break;
            default:
                break;
        }
        renderer->keyboard_event(key, action, mods);
    }

    void View::resize_callback(GLFWwindow *window, int width, int height) {
        int w, h;
        glfwGetFramebufferSize(window, &w, &h);
        View::width = w;
        View::height = h;
        glViewport(0, 0, w, h);
        if (renderer) {
            renderer->resize(w, h);
        }
    }

    void View::cursor_callback(GLFWwindow *window, double xpos, double ypos) {
        if (HDPI) {
            double cursor_x = 2 * xpos;
            double cursor_y = 2 * ypos;
            renderer->cursor_event(cursor_x, cursor_y);
        } else {
            double cursor_x = xpos;
            double cursor_y = ypos;
            renderer->cursor_event(cursor_x, cursor_y);
        }
    }

    void View::scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
        renderer->scroll_event(xoffset, yoffset);
    }

    void View::mouse_button_callback(GLFWwindow *window, int button, int action, int mods) {
        renderer->mouse_event(button, action, mods);
    }
}