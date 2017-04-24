//
// Created by Dillon Yao on 4/23/17.
//

#include "PBFRenderer.h"
#include "GL/glew.h"
#include <cmath>
#include <iostream>

using namespace glm;

void PBFRenderer::init() {

//    // SHADER STUFF
//    smoothShading = false;
//    shadingMode = false;
//    shaderProgID = loadShaders("shader/vert", "shader/frag");

    // Set keys
    left_down = false;
    right_down = false;
    middle_down = false;
    mouse_rotate = false;

    camera_angles = dvec3(0, 0, 0);

    glEnable(GL_DEPTH_TEST);

    // Temp working lights
    GLfloat light_diffuse[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat light_ambient[] = {0.2, 0.2, 0.2, 1.0};

    GLfloat light_position[] = {0.2, 0.5, 1.0, 0.0};
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);

    // Enable antialiasing
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_LINE_SMOOTH);

    glEnable(GL_POINT_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);

    up = Z_UP;
    hfov = M_PI;
    vfov = M_PI;
    nearClip = 0;
    farClip = -10;
    min_view_distance = 0;
    max_view_distance = 5;
    canonical_view_distance = 2;
    reset_camera();
}

void PBFRenderer::render() {
    update_camera();
    particles.step(0.0084);
    particles.render();
}

void PBFRenderer::update_camera() {
    GLint view[4];
    glGetIntegerv(GL_VIEWPORT, view);
    resize(view[2], view[3]);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Camera Location
    double cx = 0, cy = 0, cz = 0;

    double up_x, up_y, up_z;
    up_x = up_y = up_z = 0.0;

    // Center of view
    double vx, vy, vz;
    vx = view_focus.x;
    vy = view_focus.y;
    vz = view_focus.z;

    switch(up) {
        case Z_UP:
            up_z = 1.0;
            cx = vx + view_distance * sin(camera_angles.x) * cos(camera_angles.y * 0.99);
            cy = vy + view_distance * cos(camera_angles.x) * cos(camera_angles.y * 0.99);
            cz = vz + view_distance*sin(camera_angles.y);
            break;
        default:
            cout << "Can't do non Z_UP" << endl;
    }

    cout << cx << " " << cy << " " << cz << endl;

    gluLookAt(cx, cy, cz,
              vx, vy, vz,
              up_x, up_y, up_z);

    float eyePos[] = {(float) cx, (float) cy, (float) cz};
    //glUseProgram(shaderProgID);
    //glUniform3fv(glGetUniformLocation(shaderProgID, "eyePos"), 1, eyePos);
    //glUniform1i(glGetUniformLocation(shaderProgID, "envmap"), 1);
    glUseProgram(0);
}

void PBFRenderer::resize(unsigned int w, unsigned int h) {
    screen_w = w;
    screen_h = h;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(vfov, 1.0 * screen_w / screen_h, nearClip, farClip);
}

string PBFRenderer::name() {
    return "Position Based Fluid Simulation";
}

string PBFRenderer::info() {
    return "";
}

void PBFRenderer::key_event(char key) {
    if (key >= '0' && key <= '9') {

    }
    switch(key) {
        case ' ':
            reset_camera();
            break;
        default:
            break;
    }
}

void PBFRenderer::cursor_event(float x, float y, unsigned char keys) {
    if (left_down || middle_down || right_down) {
        mouse_dragged(x, y);
    } else {
        mouse_moved(x, y);
    }
    update_mouse_coords(x, y);
}

void PBFRenderer::scroll_event(float offset_x, float offset_y) {
    if (offset_y > 0) {
        view_distance -= offset_y * (view_distance / 4);
    } else if (offset_y < 0) {
        view_distance -= offset_y * (view_distance / 4);
    }

    view_distance = clamp(view_distance, min_view_distance, max_view_distance);
    std::cout << view_distance << std::endl;
}

void PBFRenderer::mouse_button_event(int button, int event) {
    switch(event) {
        case MOUSE_BUTTON_RELEASE: {
            switch(button) {
                case MOUSE_BUTTON_LEFT:
                    mouse_released(MOUSE_BUTTON_LEFT);
                    left_down = false;
                    break;
                case MOUSE_BUTTON_MIDDLE:
                    mouse_released(MOUSE_BUTTON_MIDDLE);
                    middle_down = false;
                    break;
                case MOUSE_BUTTON_RIGHT:
                    mouse_released(MOUSE_BUTTON_RIGHT);
                    right_down = false;
                    break;
            }
            break;
        }
        case MOUSE_BUTTON_PRESS: {
            switch(button) {
                case MOUSE_BUTTON_LEFT:
                    mouse_pressed(MOUSE_BUTTON_LEFT);
                    left_down = true;
                    break;
                case MOUSE_BUTTON_MIDDLE:
                    mouse_pressed(MOUSE_BUTTON_MIDDLE);
                    middle_down = true;
                    break;
                case MOUSE_BUTTON_RIGHT:
                    mouse_pressed(MOUSE_BUTTON_RIGHT);
                    right_down = true;
                    break;
            }
            break;
        }
    }
}

void PBFRenderer::mouse_pressed(int b) {
    switch(b) {
        case MOUSE_BUTTON_LEFT:
            mouse_rotate = true;
            break;
        case MOUSE_BUTTON_MIDDLE:
            break;
        case MOUSE_BUTTON_RIGHT:
            mouse_rotate = true;
            break;
        default:
            break;
    }
}

void PBFRenderer::mouse_released(int b) {
    switch(b) {
        case MOUSE_BUTTON_LEFT:
            if (mouse_rotate) mouse_rotate = false;
            break;
        case MOUSE_BUTTON_MIDDLE:
            break;
        case MOUSE_BUTTON_RIGHT:
            mouse_rotate = false;
            break;
        default:
            break;
    }
}

void PBFRenderer::mouse_dragged(float x, float y) {
    float dx = (x - mouse_x);
    float dy = (y - mouse_y);

    if (mouse_rotate) {
        double &cx = camera_angles.x;
        double &cy = camera_angles.y;

        cx += dx * 2 * M_PI / screen_w;
        cy += dy * M_PI / screen_h;

        cx = cx >= 0 ? std::min(cx, cx - 2 * M_PI) : (cx + 2 * M_PI);
        camera_angles.y = clamp(camera_angles.y, -M_PI / 2, M_PI / 2);
    }
}

void PBFRenderer::mouse_moved(float x, float y) {
    // TODO
}

void PBFRenderer::update_mouse_coords(float x, float y) {
    mouse_x = x;
    mouse_y = y;
}

inline dvec2 PBFRenderer::unit_cube_to_screen(glm::dvec4 &in) {
    return dvec2(screen_w * (in.x + 1) / 2, screen_h * (in.y + 1) / 2);
}

void PBFRenderer::drag_position(float screen_x_offset, float screen_y_offset, glm::dvec3 &position) {
    GLdouble projMatrix[16];
    GLdouble modelMatrix[16];

    for (int i = 0; i < 16; ++i) {
        projMatrix[i] = 0.0;
        modelMatrix[i] = 0.0;
    }

    glGetDoublev(GL_PROJECTION_MATRIX, projMatrix);
    glGetDoublev(GL_MODELVIEW_MATRIX, modelMatrix);

    dmat4x4 P;
    dmat4x4 M;
    for (int r = 0; r < 4; ++r) {
        for (int c = 0; c < 4; ++c) {
            P[r][c] = projMatrix[4 * c + r];
            M[r][c] = modelMatrix[4 * c + r];
        }
    }

    dvec4 pos = dvec4(position.x, position.y, position.z, 1.0);
    pos = P * M * pos;
    pos *= 1 / pos.w;

    dvec4 screen_offset(screen_x_offset * 2 / screen_w, -screen_y_offset * 2 / screen_h, 0.0, 0.0);
    pos += screen_offset;
    pos *= pos.w;
    pos = inverse(M) * inverse(P) * pos;
    position.x = pos.x;
    position.y = pos.y;
    position.z = pos.z;
}

void PBFRenderer::reset_camera() {
    view_distance = canonical_view_distance * 2.0;
    camera_angles = dvec3(0, 0, 0);
}


