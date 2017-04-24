//
// Created by Dillon Yao on 4/23/17.
//

#include "view.h"

#include <iostream>

using namespace std;

const int DEFAULT_WIDTH = 1200;
const int DEFAULT_HEIGHT = 800;

namespace eng {

    // Initialize static vars
    bool View::HDPI;

    int View::framecount;
    chrono::time_point<chrono::system_clock> View::sys_last;
    chrono::time_point<chrono::system_clock> View::sys_curr;

    bool View::show_info = true;

    // Init window
    GLFWwindow *View::window = nullptr;
    unsigned int View::buffer_w;
    unsigned int View::buffer_h;

    Renderer *View::renderer;
    string View::title;

    // OSD

    // Constructor
    View::View() {
        this->title = "";
    }

    View::View(const char *title) {
        this->title = title;
    }

    View::~View() {
        glfwDestroyWindow(this->window);
        glfwTerminate();

        delete renderer;
    }

    void View::init() {
        // initialize GLFW
        glfwSetErrorCallback(this->err_callback);
        if (!glfwInit()) {
            cerr << "Error: GLFW initialization failed" << endl;
            glfwTerminate();
            exit(EXIT_FAILURE);
        }

        // create window
        string title = this->renderer ? this->renderer->name() : this->title;
        this->window = glfwCreateWindow(DEFAULT_WIDTH, DEFAULT_HEIGHT, title.c_str(), NULL, NULL);
        if (!this->window) {
            cerr << "Error: Window creation failed" << endl;
            glfwTerminate();
            exit(EXIT_FAILURE);
        }

        // set context
        glfwMakeContextCurrent(this->window);
        glfwSwapInterval(1);

        // set callbacks
        glfwSetFramebufferSizeCallback(this->window, this->resize_callback);
        glfwSetKeyCallback(this->window, this->key_callback);
        glfwSetCursorPosCallback(this->window, this->cursor_callback);
        glfwSetScrollCallback(this->window, this->scroll_callback);
        glfwSetInputMode(this->window, GLFW_STICKY_MOUSE_BUTTONS, 1);
        glfwSetMouseButtonCallback(this->window, this->mouse_button_callback);

        // init GLEW
        if (glewInit() != GLEW_OK) {
            cerr << "Error: GLEW initialization failed" << endl;
            glfwTerminate();
            exit(EXIT_FAILURE);
        }

        // enable alpha blending
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // resize to window size
        glfwGetFramebufferSize(this->window, (int *) &this->buffer_w, (int *) &this->buffer_h);
        if (this->buffer_w > DEFAULT_WIDTH)
            this->HDPI = true;

        // initialize renderer if set
        if (this->renderer) {
            if (HDPI) this->renderer->use_hdpi_render_target();
            renderer->init();
        }

        resize_callback(this->window, buffer_w, buffer_h);
        sys_last = chrono::system_clock::now();
    }

    // Starts the render loop
    void View::run() {
        // start timer
        sys_curr = chrono::system_clock::now();
        chrono::duration<double, std::milli> elapsed = sys_curr - sys_last;
        while (!glfwWindowShouldClose(window)) {
            this->update(elapsed.count());
        }
        sys_last = sys_curr;
    }

    // Set renderer
    void View::set_renderer(Renderer *renderer) {
        this->renderer = renderer;
    }

    // Update
    void View::update(double dt) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (renderer) {
            renderer->render();
        }

        if (show_info) {
            draw_info();
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Draw info
    void View::draw_info() {

    }

    // Callbacks
    void View::err_callback(int error, const char *desc) {
        cerr << "GLFW Error " << error << ": " << desc << endl;
    }

    void View::key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
        if (action == GLFW_PRESS) {
            if (key == GLFW_KEY_ESCAPE) {
                glfwSetWindowShouldClose(window, true);
            } else if (key == GLFW_KEY_GRAVE_ACCENT) {
                show_info = !show_info;
            } else {
                renderer->key_event(key);
            }
        }
    }

    void View::resize_callback(GLFWwindow *window, int width, int height) {
        unsigned int w, h;
        glfwGetFramebufferSize(window, (int *) &w, (int *) &h);

        buffer_w = w; buffer_h = h;
        glViewport(0, 0, buffer_w, buffer_h);

        if (renderer) {
            renderer->resize(buffer_w, buffer_h);
        }
    }

    void View::cursor_callback(GLFWwindow *window, double xpos, double ypos) {
        unsigned char keys = 0;
        keys |= (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS);
        keys <<= 1;
        keys |= (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS);
        keys <<= 1;
        keys |= (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS);

        if (HDPI) {
            float cursor_x = 2 * xpos;
            float cursor_y = 2 * ypos;
            renderer->cursor_event(cursor_x, cursor_y, keys);
        } else {
            float cursor_x = xpos;
            float cursor_y = ypos;
            renderer->cursor_event(cursor_x, cursor_y, keys);
        }
    }

    void View::scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
        renderer->scroll_event(xoffset, yoffset);
    }

    void View::mouse_button_callback(GLFWwindow *window, int button, int action, int mods) {
        renderer->mouse_button_event(button, action);
    }

}