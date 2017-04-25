//
// Created by Dillon Yao on 4/24/17.
//

#include "Application.h"

using namespace eng;
using namespace std;

Application::Application(AppConfig &config, bool gl) {
    gl_window = gl;
    pbf = new PBFRenderer(config);
}

Application::~Application() {
    delete pbf;
}

void Application::init() {
    if (gl_window) {
        text_color = Color(1, 1, 1);
        // Set the integer bit vector representing which keys are down.
        left_down   = false;
        right_down  = false;
        middle_down = false;

        show_coordinates = true;
        show_hud = true;

        // Lighting needs to be explicitly enabled.
        glEnable(GL_LIGHTING);

        // Enable anti-aliasing and circular points.
        glEnable(GL_LINE_SMOOTH);
        glEnable(GL_POLYGON_SMOOTH);
        glEnable(GL_POINT_SMOOTH);
        glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
        glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
        glHint(GL_POINT_SMOOTH_HINT,GL_NICEST);

        // Initialize styles (colors, line widths, etc.) that will be used
        // to draw different types of mesh elements in various situations.
        initialize_style();
    }

    mode = EDIT_MODE;
//    scene = nullptr;

    screen_w = DEFAULT_WIDTH;
    screen_h = DEFAULT_HEIGHT;
    CameraInfo camera_info;
    camera_info.hfov = 50;
    camera_info.vfov = 35;
    camera_info.n_clip = 0.01;
    camera_info.f_clip = 100;
    camera.configure(camera_info, screen_w, screen_h);
}


// TODO
void Application::initialize_style() {

}

void Application::update_style() {

}

void Application::render() {
    update_gl_camera();
    switch(mode) {
        case EDIT_MODE:
            if (show_coordinates) {
//                draw_coordinates();
            }
//            scene->render_in_opengl();
            if (show_hud) {
//                draw_hud();
            }
            break;
        case RENDER_MODE:
            break;
        case SIMULATE_MODE:
            pbf->render();
            break;
    }
}

void Application::update_gl_camera() {
    GLint view[4];
    glGetIntegerv(GL_VIEWPORT, view);
    if (view[2] != screen_w || view[3] != screen_h) {
        resize(view[2], view[3]);
    }

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    const dvec3& c = camera.position();
    const dvec3& r = camera.view_point();
    const dvec3& u = camera.up_dir();

    gluLookAt(c.x, c.y, c.z,
              r.x, r.y, r.z,
              u.x, u.y, u.z);
}

void Application::resize(size_t w, size_t h) {
    screen_w = w;
    screen_h = h;
    camera.set_screen_size(w, h);
    if (gl_window) {
//        textManager.resize(w, h);
        set_projection_matrix();
    }
    if (mode != EDIT_MODE) {
//        pbf->set_frame_size(w, h);
    }
}

void Application::set_projection_matrix() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(camera.v_fov(),
                   camera.aspect_ratio(),
                   camera.near_clip(),
                   camera.far_clip());
}

string Application::name() {
    return "Position Based Fluid Simulator";
}

string Application::info() {
    switch (mode) {
        case EDIT_MODE:
            return "Edit Mode";
            break;
        case RENDER_MODE:
        case SIMULATE_MODE:
            return "Simulating";
            break;
    }
}

// TODO
//void Application::load(SceneInfo *scene_info) {

//}

void Application::init_camera(CameraInfo &cam_info, const mat4x4 transform) {
    camera.configure(cam_info, screen_w, screen_h);
    canonical_camera.configure(cam_info, screen_w, screen_h);
    if (gl_window) {
        set_projection_matrix();
    }
}

void Application::reset_camera() {
    camera.copy_placement(canonical_camera);
}

void Application::set_scroll_rate() {
    scroll_rate = canonical_view_distance / 10;
}

void Application::cursor_event(float x, float y) {
    if (left_down && !middle_down && !right_down) {
        mouse1_dragged(x, y);
    } else if (!left_down && !middle_down && right_down) {
        mouse2_dragged(x, y);
    } else if (!left_down && !middle_down && !right_down) {
        mouse_moved(x, y);
    }
    mouseX = x;
    mouseY = y;
}

void Application::scroll_event(float offset_x, float offset_y) {
    update_style();
    switch(mode) {
        case EDIT_MODE:
        case SIMULATE_MODE:
            camera.move_forward(-offset_y * scroll_rate);
            break;
        case RENDER_MODE:
            break;
    }
}

void Application::mouse_event(int key, int event, unsigned char mods) {
    switch(event) {
        case MOUSE_BUTTON_PRESS:
            switch(key) {
                case MOUSE_BUTTON_LEFT:
                    mouse_pressed(LEFT);
                    break;
                case MOUSE_BUTTON_RIGHT:
                    mouse_pressed(RIGHT);
                    break;
                case MOUSE_BUTTON_MIDDLE:
                    mouse_pressed(MIDDLE);
                    break;
            }
            break;
        case MOUSE_BUTTON_RELEASE:
            switch(key) {
                case MOUSE_BUTTON_LEFT:
                    mouse_released(LEFT);
                    break;
                case MOUSE_BUTTON_RIGHT:
                    mouse_released(RIGHT);
                    break;
                case MOUSE_BUTTON_MIDDLE:
                    mouse_released(MIDDLE);
                    break;
            }
            break;
    }
}

void Application::keyboard_event(int key, int event, unsigned char mods) {
    // TODO
}

void Application::mouse_pressed(e_mouse_button b) {
    switch (b) {
        case LEFT:
            left_down = true;
            break;
        case RIGHT:
            right_down = true;
            break;
        case MIDDLE:
            middle_down = true;
            break;
    }
}

void Application::mouse_released(e_mouse_button b) {
    switch (b) {
        case LEFT:
            left_down = false;
            break;
        case RIGHT:
            right_down = false;
            break;
        case MIDDLE:
            middle_down = false;
            break;
    }
}

void Application::mouse1_dragged(float x, float y) {
    float dx = (x - mouseX);
    float dy = (y - mouseY);
    camera.rotate_by(-dy * (M_PI / screen_h), -dx * (M_PI / screen_w));
}

void Application::mouse2_dragged(float x, float y) {
    if (mode == RENDER_MODE) return;
    float dx = (x - mouseX);
    float dy = (y - mouseY);

    // don't negate y because up is down.
    camera.move_by(-dx, dy, canonical_view_distance);
}

void Application::mouse_moved(float x, float y) {
    if (mode != EDIT_MODE) return;
    y = screen_h - y; // Because up is down.
    // Converts x from [0, w] to [-1, 1], and similarly for y.
    dvec2 p(x * 2 / screen_w - 1, y * 2 / screen_h - 1);
//    scene->update_selection(p, get_world_to_3DH());
}


