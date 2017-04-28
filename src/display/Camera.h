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
const GLfloat YAW        = -90.0f;
const GLfloat PITCH      =  0.0f;
const GLfloat SPEED      =  3.0f;
const GLfloat SENSITIVTY =  0.1f;
const GLfloat ZOOM       =  45.0f;
const GLfloat NCLIP      =  0.1f;
const GLfloat FCLIP      =  100.f;
const GLfloat VDIST      =  10.f;


class Camera {
public:
    // Camera Attributes
    glm::vec3 pos;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 world_up;
    // Eular Angles
    GLfloat yaw;
    GLfloat pitch;
    // Camera options
    GLfloat movement_speed;
    GLfloat mouse_sensitivity;
    GLfloat zoom;
    GLfloat n_clip, f_clip;
    GLfloat view_dist;

    // Constructor with vectors
    Camera(
            glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
            GLfloat yaw = YAW,
            GLfloat pitch = PITCH) :
            front(glm::vec3(0.0f, 0.0f, -1.0f)),
            movement_speed(SPEED),
            mouse_sensitivity(SENSITIVTY),
            zoom(ZOOM),
            n_clip(NCLIP),
            f_clip(FCLIP),
            view_dist(VDIST)
    {
        this->pos = position;
        this->world_up = up;
        this->yaw = yaw;
        this->pitch = pitch;
        this->update_vectors();
    }

    // Constructor with scalar values
    Camera(GLfloat posX, GLfloat posY, GLfloat posZ,
           GLfloat upX, GLfloat upY, GLfloat upZ,
           GLfloat yaw, GLfloat pitch) :
            front(glm::vec3(0.0f, 0.0f, -1.0f)),
            movement_speed(SPEED),
            mouse_sensitivity(SENSITIVTY),
            zoom(ZOOM),
            n_clip(NCLIP),
            f_clip(FCLIP),
            view_dist(VDIST)
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
    void keyboard_event(Camera_Movement direction, GLfloat deltaTime) {
        GLfloat velocity = this->movement_speed * deltaTime;
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
    void cursor_event(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch = true) {
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
        std::cout << xoffset << " " << yoffset << " " << this->yaw << " " << this->pitch << "\r";
        // Update Front, Right and Up Vectors using the updated Euler angles
        this->update_vectors();
    }

    // Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void scroll_event(GLfloat yoffset) {
        if (this->zoom >= 1.0f && this->zoom <= 45.0f)
            this->zoom -= yoffset;
        if (this->zoom <= 1.0f)
            this->zoom = 1.0f;
        if (this->zoom >= 45.0f)
            this->zoom = 45.0f;
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
