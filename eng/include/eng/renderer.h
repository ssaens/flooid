//
// Created by Dillon Yao on 4/23/17.
//

#ifndef FLOOID_RENDERER_H
#define FLOOID_RENDERER_H

#include <string>

namespace eng {

    const int MOUSE_BUTTON_LEFT = 0;
    const int MOUSE_BUTTON_RIGHT = 1;
    const int MOUSE_BUTTON_MIDDLE = 2;
    const int MOUSE_BUTTON_RELEASE = 0;
    const int MOUSE_BUTTON_PRESS = 1;

    /**
     * Abstract class Renderer
     */
    class Renderer {
    protected:
        // use HDPI?
        bool use_hdpi;

    public:
        /**
         * Virtual Destructor
         * Frees renderer resources
         */
         virtual ~Renderer() {}

        /**
         * Initialize the renderer
         */
         virtual void init() = 0;

        /**
         * Render to window
         * Called on every frame update
         */
         virtual void render() = 0;

        /**
         * Get the renderer's name
         * Sets as window title
         */
         virtual std::string name() = 0;

        /**
         * Get render description
         * Shown in the renderer section of the HUD
         */
         virtual std::string info() = 0;

        // Event Handlers
        virtual void resize(unsigned int w, unsigned int h) = 0;
        virtual void key_event(char key) {}
        virtual void cursor_event(float x, float y, unsigned char keys) {}
        virtual void scroll_event(float offset_x, float offset_y) {}
        virtual void mouse_button_event(int button, int event) {}

        /**
         * View call this if the screen is HDPI
         */
        void use_hdpi_render_target() {
                this->use_hdpi = true;
        };

    };

}

#endif //FLOOID_RENDERER_H
