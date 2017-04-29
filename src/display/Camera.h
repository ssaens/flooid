//
// Created by Dillon Yao on 4/26/17.
//

#ifndef FLOOID2_CAMERA_H
#define FLOOID2_CAMERA_H

#include <glm/glm.hpp>

using namespace glm;

// Std. Includes
#include <vector>

// GL Includes
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN,
};

// Default camera values
const float YAW        = -90.0f;
const float PITCH      =  -20.0f;
const float SPEED      =  3.0f;
const float SENSITIVTY =  0.1f;
const float ZOOM       =  M_PI / 4;
const float MIN_ZOOM   =  glm::radians(20.f);
const float MAX_ZOOM   =  ZOOM;
const float NCLIP      =  0.1f;
const float FCLIP      =  100.f;
const float VDIST      =  10.f;

const glm::vec3 DEFAULT_POS = glm::vec3(0.f, 1.5f, 3.f);
const glm::vec3 DEFAULT_UP = glm::vec3(0.f, 1.f, 0.f);
const glm::vec3 DEFAULT_FRONT = glm::vec3(0.f, 0.f, -1.f);

class Camera {
public:
    // Camera Attributes
    glm::vec3 pos;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 world_up;

    // Eular Angles
    float yaw;
    float pitch;
    // Camera options
    float movement_speed;
    float mouse_sensitivity;
    float zoom;
    float min_zoom, max_zoom;
    float n_clip, f_clip;
    float view_dist;

    // Constructor with vectors
    Camera(
            glm::vec3 position = DEFAULT_POS,
            glm::vec3 up = DEFAULT_UP,
            float yaw = YAW,
            float pitch = PITCH) :
            pos(position),
            world_up(up),
            yaw(yaw),
            pitch(pitch),
            front(DEFAULT_FRONT),
            movement_speed(SPEED),
            mouse_sensitivity(SENSITIVTY),
            zoom(ZOOM),
            n_clip(NCLIP),
            f_clip(FCLIP),
            view_dist(VDIST),
            min_zoom(MIN_ZOOM),
            max_zoom(MAX_ZOOM)
    {
        this->update_vectors();
    }

    // Constructor with scalar values
    Camera(float posX, float posY, float posZ,
           float upX, float upY, float upZ,
           float yaw, float pitch) :
            front(DEFAULT_FRONT),
            movement_speed(SPEED),
            mouse_sensitivity(SENSITIVTY),
            zoom(ZOOM),
            n_clip(NCLIP),
            f_clip(FCLIP),
            view_dist(VDIST),
            min_zoom(MIN_ZOOM),
            max_zoom(MAX_ZOOM)
    {
        this->pos = glm::vec3(posX, posY, posZ);
        this->world_up = glm::vec3(upX, upY, upZ);
        this->yaw = yaw;
        this->pitch = pitch;
        this->update_vectors();
    }

    // Returns the view matrix calculated using Euler Angles and the LookAt Matrix
    glm::mat4 get_view_matrix() {
        return glm::lookAt(this->pos, this->pos + this->front, this->up);
    }

    // Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined      ENUM (to abstract it from windowing systems)
    void keyboard_event(Camera_Movement direction, float deltaTime) {
        float velocity = this->movement_speed * deltaTime;
        if (direction == FORWARD)
            this->pos += this->front * velocity;
        if (direction == BACKWARD)
            this->pos -= this->front * velocity;
        if (direction == LEFT)
            this->pos -= this->right * velocity;
        if (direction == RIGHT)
            this->pos += this->right * velocity;
        if (direction == UP)
            this->pos += this->up * velocity;
        if (direction == DOWN)
            this->pos -= this->up * velocity;
    }

    // Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void cursor_event(float xoffset, float yoffset, GLboolean constrainPitch = true) {
        xoffset *= this->mouse_sensitivity;
        yoffset *= this->mouse_sensitivity;

        this->yaw   = mod(this->yaw + xoffset, 360.f);
        this->pitch += yoffset;

        // Make sure that when pitch is out of bounds, screen doesn't get flipped
        if (constrainPitch)
        {
            if (this->pitch > 89.0f)
                this->pitch = 89.0f;
            if (this->pitch < -89.0f)
                this->pitch = -89.0f;
        }
        // Update Front, Right and Up Vectors using the updated Euler angles
        this->update_vectors();
    }

    // Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void scroll_event(float yoffset) {
        if (this->zoom >= min_zoom && this->zoom <= max_zoom)
            this->zoom -= yoffset;
        if (this->zoom <= min_zoom)
            this->zoom = min_zoom;
        if (this->zoom >= max_zoom)
            this->zoom = max_zoom;
    }

private:
    // Calculates the front vector from the Camera's (updated) Euler Angles
    void update_vectors() {
        // Calculate the new Front vector
        glm::vec3 front;
        front.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
        front.y = sin(glm::radians(this->pitch));
        front.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
        this->front = glm::normalize(front);
        // Also re-calculate the Right and Up vector
        this->right = glm::normalize(glm::cross(this->front, this->world_up));  // Normalize the vectors, because their             length gets closer to 0 the more you look up or down which results in slower movement.
        this->up    = glm::normalize(glm::cross(this->right, this->front));
    }
};


#endif //FLOOID2_CAMERA_H
