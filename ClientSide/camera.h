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
        glm::mat4 getViewMatrix();
        void moveUp(float);
        void moveSideways(float);
        void moveForward(float);
        void rotateY(float);
        void rotateX(float);
        void invertForward();
        virtual ~Camera();
        void setPosition(glm::vec3);
        glm::vec3 getPosition();
        glm::vec3 getRight();
        glm::vec3 getForward();
        glm::vec3 getYawPitchRoll();
    protected:
    private:
        glm::vec3 position, forward, right;
        float yaw, pitch, roll;
        void computeForward();
};

#endif // CAMERA_H
