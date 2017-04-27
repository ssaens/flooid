//
// Created by Dillon Yao on 4/26/17.
//

#ifndef FLOOID2_VIEW_H_H
#define FLOOID2_VIEW_H_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <chrono>
#include <string>
#include "Renderer.h"

namespace eng {

    class View {
    private:
        static GLFWwindow *window;
        static int width, height;
        static Renderer *renderer;

        static void update(double dt);

        static void error_callback(int error, const char *desc);
        static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
        static void resize_callback(GLFWwindow *window, int width, int height);
        static void cursor_callback(GLFWwindow *window, double xpos, double ypos);
        static void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
        static void mouse_button_callback(GLFWwindow *window, int button, int action, int mods);

        static std::string title;
        static bool HDPI;

        static int framecount;
        static std::chrono::time_point<std::chrono::system_clock> sys_last;
        static std::chrono::time_point<std::chrono::system_clock> sys_curr;


    public:
        View();
        ~View();

        void init();
        void begin();
        void set_renderer(Renderer *renderer);
    };

}

#endif //FLOOID2_VIEW_H_H
