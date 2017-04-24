//
// Created by Dillon Yao on 4/23/17.
//

#ifndef FLOOID_PBFRENDERER_H
#define FLOOID_PBFRENDERER_H

#include <eng/eng.h>
#include <glm/glm.hpp>
#include "ParticleManager.h"

using namespace eng;
using namespace std;


struct DrawStyle {
    Color particleColor;
    Color planeColor;
};

class PBFRenderer : public Renderer {
private:
    void initialize_style();

    float hfov;
    float vfov;
    float nearClip;
    float farClip;

    unsigned int screen_w;
    unsigned int screen_h;

    GLuint shaderProgID;

    glm::dvec3 view_focus;
    enum e_up {X_UP, Y_UP, Z_UP};
    e_up up;
    glm::dvec3 camera_angles;

    double view_distance;
    double canonical_view_distance;

    double min_view_distance;
    double max_view_distance;

    void update_camera();
    void draw_meshes();

    void reset_camera();

    DrawStyle defaultStyle;

    bool mouse_rotate;
    float mouse_x, mouse_y;

    bool left_down;
    bool right_down;
    bool middle_down;

    void mouse_pressed(int b);
    void mouse_released(int b);
    void mouse_dragged(float x, float y);
    void mouse_moved(float x, float y);
    void update_mouse_coords(float x, float y);

    void drag_position(float screen_x_offset, float screen_y_offset, glm::dvec3 &position);

    inline glm::dvec2 unit_cube_to_screen(glm::dvec4 &in);

    ParticleManager particles;

public:

    ~PBFRenderer() {}

    virtual void init();
    virtual void render();
    virtual void resize(unsigned int w, unsigned int h);

    virtual std::string name();
    virtual std::string info();

    virtual void key_event(char key);
    virtual void cursor_event(float x, float y, unsigned char keys);
    virtual void scroll_event(float offset_x, float offset_y);
    virtual void mouse_button_event(int button, int event);
};



#endif //FLOOID_PBFRENDERER_H
