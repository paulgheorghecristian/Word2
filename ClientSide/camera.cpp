#include "camera.h"

Camera::Camera(glm::vec3 position, float yaw, float pitch, float roll) : forward(0.0, 0.0, -1.0), position(position), yaw(yaw), pitch(pitch), roll(roll)
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
    right = glm::cross(forward, glm::vec3(0.0, 1.0, 0.0));
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

void Camera::compute_forward(){
		//calculeaza vectorul forward in functie de pitch si yaw
    forward = glm::normalize(glm::vec3(glm::rotate(glm::mat4(1.0), pitch, glm::vec3(1.0, 0.0, 0.0)) * glm::vec4(glm::vec3(0.0, 0.0, -1.0), 1.0)));
    forward = glm::normalize(glm::vec3(glm::rotate(glm::mat4(1.0), yaw, glm::vec3(0.0, 1.0, 0.0)) * glm::vec4(forward, 1.0)));
}

glm::vec3 Camera::get_forward(){
    compute_forward();
    return glm::normalize(forward);
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
