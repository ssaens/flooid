//
// Created by Dillon Yao on 4/26/17.
//

#ifndef FLOOID2_RENDERER_H
#define FLOOID2_RENDERER_H

namespace eng {

    class Renderer {
    public:
        virtual void init() = 0;
        virtual void render() = 0;
        virtual void update(float dt) = 0;

        virtual void resize(int width, int height) {}
        virtual void keyboard_event(int key, int action, int mods) {}
        virtual void cursor_event(double cursor_x, double cursor_y) {}
        virtual void scroll_event(double offset_x, double offset_y) {}
        virtual void mouse_event(int button, int action, int mods) {}

        virtual std::string name() { return ""; }
        virtual void use_hdpi() { HDPI = true; }
        virtual void set_frame_size(int screen_w, int screen_h) {
            this->screen_w = screen_w;
            this->screen_h = screen_h;
        }

    protected:
        bool HDPI;
        int screen_w, screen_h;

    };

}

#endif //FLOOID2_RENDERER_H
