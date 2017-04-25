//
// Created by Dillon Yao on 4/24/17.
//

#include "Camera.h"
#include "common.h"

void Camera::configure(const CameraInfo& info, size_t screenW, size_t screenH) {
    this->screen_w = screenW;
    this->screen_h = screenH;
    n_clip = info.n_clip;
    f_clip = info.f_clip;
    hfov = info.hfov;
    vfov = info.vfov;

    double ar1 = tan(radians(hfov) / 2) / tan(radians(vfov) / 2);
    ar = static_cast<double>(screenW) / screenH;
    if (ar1 < ar) {
        // hFov is too small
        hfov = 2 * degrees(atan(tan(radians(vfov) / 2) * ar));
    } else if (ar1 > ar) {
        // vFov is too small
        vfov = 2 * degrees(atan(tan(radians(hfov) / 2) / ar));
    }
    screen_dist = ((double) screenH) / (2.0 * tan(radians(vfov) / 2));
}

void Camera::place(const dvec3 &target_pos, const double phi, const double theta, const double r, const double min_r,
                   const double max_r) {
    this->r = std::min(max_r, std::max(min_r, r));
    this->phi = sin(phi) == 0 ? (phi + EPS_F) : phi;
    this->target_pos = target_pos;
    this->theta = theta;
    this->min_r = min_r;
    this->max_r = max_r;
    compute_position();
}

void Camera::copy_placement(const Camera &other) {
    pos = other.pos;
    target_pos = other.target_pos;
    phi = other.phi;
    theta = other.theta;
    min_r = other.min_r;
    max_r = other.max_r;
    c2w = other.c2w;
}

void Camera::set_screen_size(const size_t screen_w, const size_t screen_h) {
    this->screen_w = screen_w;
    this->screen_h = screen_h;
    ar = 1.0 * screen_w / screen_h;
    hfov = 2 * degrees(atan((double) screen_w) / (2 * screen_dist));
    vfov = 2 * degrees(atan((double) screen_h) / (2 * screen_dist));
}

void Camera::move_by(const double dx, const double dy, const double d) {
    const double scale_factor = d / screen_dist;
    const dvec3 &displacement = dvec3(c2w[0]) * (dx * scale_factor) + dvec3(c2w[1]) * (dy * scale_factor);
    pos += displacement;
    target_pos += displacement;
}

void Camera::move_forward(const double dist) {
    double new_r = std::min(max_r, std::max(r - dist, min_r));
    pos = target_pos + ((pos - target_pos) * (new_r / r));
    r = new_r;
}

void Camera::rotate_by(const double dphi, const double dtheta) {
    phi = clamp(phi + dphi, 0.0, M_PI);
    theta += dtheta;
    compute_position();
}

void Camera::compute_position() {
    double sin_phi = sin(phi);
    if (sin_phi == 0) {
        phi += EPS_F;
        sin_phi = sin(phi);
    }
    const dvec3 dir_to_camera(r * sin_phi * sin(theta), r * cos(phi), r * sin_phi * cos(theta));
    pos = target_pos + dir_to_camera;
    dvec3 up_vec(0, sin_phi > 0 ? 1 : -1, 0);
    dvec3 screen_x_dir = normalize(cross(up_vec, dir_to_camera));
    dvec3 screen_y_dir = normalize(cross(dir_to_camera, screen_x_dir));

    c2w[0] = screen_x_dir;
    c2w[1] = screen_y_dir;
    c2w[2] = normalize(dir_to_camera);
}

// TODO
void Camera::dump_settings(std::string filename) {

}

// TODO
void Camera::load_settings(std::string filename) {

}
