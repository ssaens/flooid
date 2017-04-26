//
// Created by Dillon Yao on 4/23/17.
//

#ifndef ENG_VIEW_H
#define ENG_VIEW_H

#include "color.h"
#include "renderer.h"
#include <chrono>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace eng {

    /**
     * Creates and manages an openGL context and window and handles callbacks.
     * An application is drawn using a provided renderer.
     */
    class View {
    private:
        /**
         * Update loop
         */
        static void update(double dt);

        /**
         * Draw HUD
         */
        static void draw_info();

        // window callback functinos
        static void err_callback(int error, const char *desc);
        static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
        static void resize_callback(GLFWwindow *window, int width, int height);
        static void cursor_callback(GLFWwindow *window, double xpos, double ypos);
        static void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
        static void mouse_button_callback(GLFWwindow *window, int button, int action, int mods);

        // HDPI display
        static bool HDPI;

        // framerate timers
        static int framecount;
        static std::chrono::time_point<std::chrono::system_clock> sys_last;
        static std::chrono::time_point<std::chrono::system_clock> sys_curr;

        // info toggle
        static bool show_info;

        // window and window properties
        static GLFWwindow *window;
        static unsigned int buffer_w;
        static unsigned int buffer_h;

        // renderer
        static Renderer *renderer;

        // OSD
//        static OSDText *osd_text;
        static int line_id_renderer;
        static int line_id_framerate;

        static std::string title;

    public:
        /**
         * Constructor
         */
        View();

        /**
         * Constructor
         * Creates a view with the given title
         */
        View(const char *title);

        /**
         * Destructor
         * Destroys the viewer and frees memeory
         */
         ~View();

        /**
         * Initializes the viewer
         */
        void init();

        /**
         * Starts the drawing loop
         */
        void run();

        /**
         * Set the viewer's renderer
         */
         void set_renderer(Renderer *renderer);
    };

}
#endif //ENG_VIEW_H
