#include "camera.h"

Camera::Camera(glm::vec3 position, float yaw, float pitch, float roll) : position(position), yaw(yaw), pitch(pitch), roll(roll), forward(0.0, 0.0, -1.0)
{

}

Camera::~Camera()
{

}

glm::mat4 Camera::get_view_matrix(){
    glm::mat4 matrix(1.0f);

    matrix = glm::rotate(matrix, -pitch, glm::vec3(1.0f, 0.0f, 0.0f));
    matrix = glm::rotate(matrix, -yaw, glm::vec3(0.0f, 1.0f, 0.0f));

    glm::vec3 negPos(-position.x, -position.y, -position.z);

    matrix = glm::translate(matrix, negPos);

    return matrix;
}

void Camera::move_up(float dx){
    position.y += dx;
}
void Camera::move_sideways(float dx){
    compute_forward();
    right = glm::normalize(glm::cross(forward, glm::vec3(0.0, 1.0, 0.0)));
    position = position + right * dx;
}

void Camera::move_forward(float distance){
    compute_forward();
    position = position + forward * distance;
}

void Camera::rotate_y(float dx){
    yaw += dx;
}
void Camera::rotate_x(float dx){
    pitch += dx;
}
void Camera::invert_forward(){
    pitch *= -1;
}

/* contains the y rotation also
void Camera::compute_forward(){
    float sinPitch = glm::sin(pitch);
    float cosPitch = glm::cos(pitch);
    float sinYaw = glm::sin(yaw);
    float cosYaw = glm::cos(yaw);

    forward.x = -cosPitch * sinYaw;
    forward.y = sinPitch;
    forward.z = -cosPitch * cosYaw;

    forward = glm::normalize(forward);
}*/

void Camera::compute_forward(){
    forward.x = -glm::sin(yaw);
    forward.y = 0;
    forward.z = -glm::cos(yaw);

    forward = glm::normalize(forward);
}

glm::vec3 Camera::get_forward(){
    compute_forward();
    return forward;
}

glm::vec3 Camera::get_right(){
    compute_forward();
    return glm::normalize(glm::cross(forward, glm::vec3(0.0, 1.0, 0.0)));
}

void Camera::set_position(glm::vec3 pos){
    position = pos;
}

glm::vec3 Camera::get_position(){
    return position;
}

glm::vec3 Camera::get_yaw_pitch_roll(){
    return glm::vec3(yaw, pitch, roll);
}
