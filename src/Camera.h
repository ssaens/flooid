//
// Created by Dillon Yao on 4/24/17.
//

#ifndef FLOOID_CAMERA_H
#define FLOOID_CAMERA_H

#include <eng/eng.h>
#include <glm/glm.hpp>

using namespace glm;

struct CameraInfo {
    float hfov, vfov, n_clip, f_clip;
    vec3 view_dir;
    vec3 up_dir;
};

class Camera {
private:

    // Field of view aspect ratio, clipping planes.
    double hfov, vfov, ar, n_clip, f_clip;

    /**
     * Computes pos, screenXDir, screenYDir from target, r, phi, theta.
     */
    void compute_position();

    // Current position and target point (the point the camera is looking at).
    dvec3 pos, target_pos;

    // Orientation relative to target, and min & max distance from the target.
    double phi, theta, r, min_r, max_r;

    // camera-to-world rotation matrix (note: also need to translate a
    // camera-space point by 'pos' to perform a full camera-to-world
    // transform)
    mat3x3 c2w;

    // Info about screen to render to; it corresponds to the camera's full field
    // of view at some distance.
    size_t screen_w, screen_h;
    double screen_dist;

public:
    /**
      Sets the field of view to match screen screenW/H.
      NOTE: data and screenW/H will almost certainly disagree about the aspect
            ratio. screenW/H are treated as the source of truth, and the field
            of view is expanded along whichever dimension is too narrow.
      NOTE2: info.hFov and info.vFov are expected to be in DEGREES.
    */
    void configure(const CameraInfo& info, size_t screenW, size_t screenH);

    /*
    Phi and theta are in RADIANS.
  */
    void place(const dvec3& targetPos, const double phi, const double theta,
               const double r, const double minR, const double maxR);

    std::string param_string() {
        return "";
    }

    /*
      Copies just placement data from the other camera.
    */
    void copy_placement(const Camera& other);

    /*
      Updates the screen size to be the specified size, keeping screenDist
      constant.
    */
    void set_screen_size(const size_t screen_w, const size_t screen_h);

    /*
      Translates the camera such that a value at distance d directly in front of
      the camera moves by (dx, dy). Note that dx and dy are in screen coordinates,
      while d is in world-space coordinates (like pos/dir/up).
    */
    void move_by(const double dx, const double dy, const double d);

    /*
      Move the specified amount along the view axis.
    */
    void move_forward(const double dist);

    /*
      Rotate by the specified amount around the target.
    */
    void rotate_by(const double dphi, const double dtheta);

    dvec3 position() const { return pos; }
    dvec3 view_point() const { return target_pos; }
    dvec3 up_dir() const { return c2w[1]; }
    double v_fov() const { return vfov; }
    double aspect_ratio() const { return ar; }
    double near_clip() const { return n_clip; }
    double far_clip() const { return f_clip; }

    virtual void dump_settings(std::string filename);
    virtual void load_settings(std::string filename);

};


#endif //FLOOID_CAMERA_H
