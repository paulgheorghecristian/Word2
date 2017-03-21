#include "light.h"

Mesh* Light::mesh;
GBuffer* Light::gBuffer;

Light::Light(glm::vec3 color, glm::vec3 position, float radius) :   color(color),
                                                                    position(position),
                                                                    radius(radius),
                                                                    renderIt(true)
{

}

void Light::draw(GeneralShader* shader){
    glBindVertexArray(mesh->getVao());
    if(typeid(SimpleShader) == typeid(*shader)){
        SimpleShader *ss = (SimpleShader*)shader;
        ss->loadHasTexture(0);
        ss->loadColor(glm::vec4(color, 1.0));
        glm::mat4 modelMatrix(1.0f);
        modelMatrix = glm::translate(modelMatrix, position);
        modelMatrix = glm::scale(modelMatrix, glm::vec3(radius));
        ss->loadModelMatrix(modelMatrix);
        glUniform3f(glGetUniformLocation(ss->getProgram(), "lightPosition"), position.x, position.y, position.z);
        glUniform1i(glGetUniformLocation(ss->getProgram(), "screenWidth"), gBuffer->getWidth());
        glUniform1i(glGetUniformLocation(ss->getProgram(), "screenHeight"), gBuffer->getHeight());
        glUniform1f(glGetUniformLocation(ss->getProgram(), "cutOff"), radius-100.0f);
        glEnable(GL_TEXTURE_2D);
        glActiveTexture(GL_TEXTURE0+14);
        glBindTexture(GL_TEXTURE_2D, gBuffer->getNormalTexture());
        glUniform1i(glGetUniformLocation(ss->getProgram(), "eyeSpaceNormalSampler"), 14);
        glActiveTexture(GL_TEXTURE0+15);
        glBindTexture(GL_TEXTURE_2D, gBuffer->getDepthTexture());
        glUniform1i(glGetUniformLocation(ss->getProgram(), "depthSampler"), 15);
    }
    glDrawElements(GL_TRIANGLES, mesh->getNumberOfTriangles(), GL_UNSIGNED_INT, (void*)0);
    glBindVertexArray(0);
}

void Light::setRenderIt(bool r){
    renderIt = r;
}

bool Light::getRenderIt(){
    return renderIt;
}

void Light::setPosition(glm::vec3 position) {
    this->position = position;
}

glm::vec3 Light::getPosition(){
    return position;
}

float Light::getRadius(){
    return radius;
}

void Light::setMesh(Mesh* _mesh){
    mesh = _mesh;
}

void Light::setGBuffer(GBuffer* buffer) {
    gBuffer = buffer;
}

Light::~Light()
{
    //dtor
}
