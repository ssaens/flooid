//
// Created by Dillon Yao on 4/24/17.
//

#ifndef FLOOID_APPLICATION_H
#define FLOOID_APPLICATION_H

#include <eng/eng.h>
#include <glm/glm.hpp>
#include <vector>
#include "common.h"
#include "PBFRenderer.h"
#include "Camera.h"

using namespace eng;
using namespace glm;

struct AppConfig {
    // Physical Properties
    std::vector<dvec3> EXTERNAL_ACCELERATIONS;

    // Particle Properties
    double particle_radius;
    double particle_mass;

    // PBD Solver Configurations
    bool solver_iters;
    double rest_density;
    double kernel_radius;
    double eps;
    double pressure_strength;
    double pressure_power;
    dvec3 delta_q;

    AppConfig() {
        EXTERNAL_ACCELERATIONS = {
                ACCEL_GRAVITY,
        };
        particle_radius = PARTICLE_RADIUS;
        particle_mass = PARTICLE_MASS;

        solver_iters = SOLVER_ITERS;
        rest_density = REST_DENSITY;
        kernel_radius = KERNEL_RADIUS;
        eps = EPS;
        pressure_strength = PRESSURE_STRENGTH;
        pressure_power = PRESSURE_POW;
        delta_q = DELTA_Q;
    }
};

class Application : public Renderer {
private:

    enum Mode {
        EDIT_MODE,
        RENDER_MODE,
        SIMULATE_MODE,
    };
    Mode mode;

    PBFRenderer *pbf;

    Camera camera;
    Camera canonical_camera;

    size_t screen_w;
    size_t screen_h;

    // Length of diagonal of bounding box for the mesh.
    double canonical_view_distance;

    double scroll_rate;

    /**
     * Called when camera FOV or screen dimensions change
     */
    void set_projection_matrix();

    /**
     * Fills drawstyle struct
     */
    void initialize_style();

    /**
     * Updates drawsytle struct
     */
    void update_style();

    /**
     * Combines modelview and projection matrices
     * @return
     */
    mat4x4 world_to_3DH();

    void init_camera(CameraInfo &cam_info, const mat4x4 transform);
    void set_scroll_rate();
    void reset_camera();
    void update_gl_camera();

    // Internal event system //

    float mouseX, mouseY;
    enum e_mouse_button {
        LEFT   = MOUSE_BUTTON_LEFT,
        RIGHT  = MOUSE_BUTTON_RIGHT,
        MIDDLE = MOUSE_BUTTON_MIDDLE,
    };

    bool left_down;
    bool right_down;
    bool middle_down;

    // Event handling //

    void mouse_pressed(e_mouse_button b);   // Mouse pressed.
    void mouse_released(e_mouse_button b);  // Mouse Released.
    void mouse1_dragged(float x, float y);  // Left Mouse Dragged.
    void mouse2_dragged(float x, float y);  // Right Mouse Dragged.
    void mouse_moved(float x, float y);     // Mouse Moved.

    // OSD text manager // TODO
//    OSDText textManager;
    Color text_color;
    vector<int> messages;

    // Coordinate System //
    bool show_coordinates;
    void draw_coordinates();

    // HUD //
    bool show_hud;
    void draw_hud();
    inline void draw_string(float x, float y,
                            string str, size_t size, const Color& c);

    bool gl_window;

    std::string filename;

public:
    /**
     * Constructor
     * @param config : application configuration
     * @param gl :
     */
    Application(AppConfig config, bool gl=true);

    ~Application();

    virtual void init();
    virtual void render();
    virtual void resize(size_t w, size_t h);

    std::string name();
    std::string info();

    virtual void cursor_event(float x, float y);
    virtual void scroll_event(float offset_x, float offset_y);
    virtual void mouse_event(int key, int event, unsigned char mods);
    virtual void keyboard_event(int key, int event, unsigned char mods);

//    void load(SceneInfo *sceneInfo);
    void render_to_file(std::string path, size_t x, size_t y, size_t dx, size_t dy);
    void load_camera(std::string filename);
};


#endif //FLOOID_APPLICATION_H
