#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <SDL2/SDL.h>
#include "input.h"
#include <iostream>
#include "display.h"
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
    public:
        Camera(glm::vec3, float, float, float);
        glm::mat4 get_view_matrix();
        void move_up(float);
        void move_sideways(float);
        void move_forward(float);
        void rotate_y(float);
        void rotate_x(float);
        void invert_forward();
        virtual ~Camera();
        void set_position(glm::vec3);
        glm::vec3 get_position();
        glm::vec3 get_right();
        glm::vec3 get_forward();
        glm::vec3 get_yaw_pitch_roll();
    protected:
    private:
        Input input;
        glm::vec3 position, forward, right;
        float yaw, pitch, roll;
        void compute_forward();
};

#endif // CAMERA_H
