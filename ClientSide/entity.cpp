#include "entity.h"

Entity::Entity(btDynamicsWorld* world,
               std::string name,
               Mesh *mesh,
               glm::vec4 color,
               glm::vec3 position,
               glm::vec3 rotation,
               glm::vec3 scale,
               Texture* texture) : world(world),
                                    name(name),
                                    mesh(mesh),
                                    color(color),
                                    position(position),
                                    rotation(rotation),
                                    scale(scale),
                                    modelMatrix(1.0f),
                                    texture(texture)
{
    isModelMatrixModified = true;
    computeModelMatrix();
}

void Entity::setModelMatrix(glm::mat4 matrix){
    modelMatrix = matrix;
    position = glm::vec3(matrix[3][0], matrix[3][1], matrix[3][2]);
}

glm::mat4& Entity::getModelMatrix(){
    if(isModelMatrixModified){
        computeModelMatrix();
    }
    return modelMatrix;
}

void Entity::computeModelMatrix(){
    glm::mat4 matrix(1.0f);
    modelMatrix = glm::translate(matrix, position);
    modelMatrix = glm::rotate(modelMatrix, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
    modelMatrix = glm::rotate(modelMatrix, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
    modelMatrix = glm::rotate(modelMatrix, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
    modelMatrix = glm::scale(modelMatrix, scale);
    isModelMatrixModified = false;
}

void Entity::draw(GeneralShader* shader){
    glBindVertexArray(mesh->getVao());
    if(isModelMatrixModified){
        computeModelMatrix();
    }
    if(typeid(SimpleShader) == typeid(*shader)){
        SimpleShader *ss = (SimpleShader*)shader;
        if(texture){
            ss->loadHasTexture(1);
            ss->loadTextureSampler(texture->getTextureUnit());
            texture->use();
        }else{
            ss->loadHasTexture(0);
        }
    }else if(typeid(DeferredLightShader) == typeid(*shader)){
        DeferredLightShader *dls = (DeferredLightShader*)shader;
        if(texture){
            dls->loadHasTexture(1);
            dls->loadTextureSampler(texture->getTextureUnit());
            texture->use();
        }else{
            dls->loadHasTexture(0);
        }
    }
    shader->loadModelMatrix(modelMatrix);
    shader->loadColor(color);
    glDrawElements(GL_TRIANGLES, mesh->getNumberOfTriangles(), GL_UNSIGNED_INT, (void*)0);
    glBindVertexArray(0);
}

void Entity::setRotation(float rotX, float rotY, float rotZ){
    rotation.x = rotX;
    rotation.y = rotY;
    rotation.z = rotZ;
    isModelMatrixModified = true;
}

void Entity::setScale(float sx, float sy, float sz){
    scale.x = sx;
    scale.y = sy;
    scale.z = sz;
    isModelMatrixModified = true;
}

void Entity::setPosition(float tx, float ty, float tz){
    position.x = tx;
    position.y = ty;
    position.z = tz;
    isModelMatrixModified = true;
}

void Entity::setPosition(glm::vec3 pos){
    position = pos;
    isModelMatrixModified = true;
}

void Entity::addRotation(float rx, float ry, float rz){
    rotation.x += rx;
    rotation.y += ry;
    rotation.z += rz;

    if(rotation.z > 360.0f){
        rotation.z = 0.0f;
    }else if(rotation.z < 0.0f){
        rotation.z = 360.0f;
    }

    if(rotation.y > 360.0f){
        rotation.y = 0.0f;
    }else if(rotation.y < 0.0f){
        rotation.y = 360.0f;
    }

    isModelMatrixModified = true;
}
void Entity::addPosition(float px, float py, float pz){
    position.x += px;
    position.y += py;
    position.z += pz;
    isModelMatrixModified = true;
}
void Entity::addScale(float sx, float sy, float sz){
    scale.x += sx;
    scale.y += sy;
    scale.z += sz;
    isModelMatrixModified = true;
}

void Entity::setColor(glm::vec4 color){
    this->color = color;
}

std::string Entity::getName(){
    return name;
}

glm::vec3 Entity::getPosition(){
    return position;
}

glm::vec3 Entity::getRotation(){
    return rotation;
}

glm::vec3 Entity::getScale(){
    return scale;
}

glm::vec4 Entity::getColor(){
    return color;
}

Entity::~Entity()
{
}
