#include "entity.h"

Entity::Entity(btDynamicsWorld* world, std::string name, Mesh *mesh, glm::vec4 color, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, Texture *texture) : world(world), name(name), mesh(mesh), color(color), position(position), rotation(rotation), scale(scale), model_matrix(1.0f), texture(texture)
{
    calc_model_matrix = true;
    compute_model_matrix();
}

void Entity::set_model_matrix(glm::mat4 matrix){
    model_matrix = matrix;
}

glm::mat4 Entity::get_model_matrix(){
    return model_matrix;
}

void Entity::compute_model_matrix(){
    glm::mat4 matrix(1.0f);
    model_matrix = glm::translate(matrix, position);
    model_matrix = glm::rotate(model_matrix, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
    model_matrix = glm::rotate(model_matrix, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
    model_matrix = glm::rotate(model_matrix, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
    model_matrix = glm::scale(model_matrix, scale);
    calc_model_matrix = false;
}

void Entity::draw(Shader* shader){
    glBindVertexArray(mesh->get_vao());
    if(calc_model_matrix == true){
        compute_model_matrix();
    }
    if(!texture){
        shader->load_has_texture(0);
    } else {
        shader->load_has_texture(1);
        texture->use(0);
    }
    shader->loadModelMatrix(model_matrix);
    shader->loadColor(color);
    glDrawElements(GL_TRIANGLES, mesh->get_no_triangles(), GL_UNSIGNED_INT, (void*)0);
    glBindVertexArray(0);
}

void Entity::set_rotation(float rotX, float rotY, float rotZ){
    rotation.x = rotX;
    rotation.y = rotY;
    rotation.z = rotZ;
    calc_model_matrix = true;
}

void Entity::set_scale(float sx, float sy, float sz){
    scale.x = sx;
    scale.y = sy;
    scale.z = sz;
    calc_model_matrix = true;
}

void Entity::set_position(float tx, float ty, float tz){
    position.x = tx;
    position.y = ty;
    position.z = tz;
    calc_model_matrix = true;
}

void Entity::set_position(glm::vec3 pos){
    position = pos;
}

void Entity::add_rotation(float rx, float ry, float rz){
    rotation.x += rx;
    rotation.y += ry;
    rotation.z += rz;

    if(rotation.z > 360.0f){
        rotation.z = 0.0f;
    }else if(rotation.z < 0.0f){
        rotation.z = 360.0f;
    }

    calc_model_matrix = true;
}
void Entity::add_position(float px, float py, float pz){
    position.x += px;
    position.y += py;
    position.z += pz;
    calc_model_matrix = true;
}
void Entity::add_scale(float sx, float sy, float sz){
    scale.x += sx;
    scale.y += sy;
    scale.z += sz;
    calc_model_matrix = true;
}

void Entity::set_color(glm::vec4 color){
    this->color = color;
}

std::string Entity::get_name(){
    return name;
}

glm::vec3 Entity::get_position(){
    return position;
}

glm::vec3 Entity::get_rotation(){
    return rotation;
}

glm::vec3 Entity::get_scale(){
    return scale;
}

glm::vec4 Entity::get_color(){
    return color;
}

btRigidBody* Entity::get_rigid_body(){
    return m_body;
}

Entity::~Entity()
{
}
