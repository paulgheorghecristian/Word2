#include "light.h"

Light::Light(GBuffer* gBuffer, Mesh* mesh, glm::vec3 color, glm::vec3 position, float radius) : gBuffer(gBuffer), mesh(mesh), color(color), position(position), radius(radius)
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
        glActiveTexture(GL_TEXTURE0+13);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, gBuffer->getPositionTexture());
        glUniform1i( glGetUniformLocation(ss->getProgram(), "worldPositionSampler"), 13);
        glActiveTexture(GL_TEXTURE0+14);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, gBuffer->getNormalTexture());
        glUniform1i(glGetUniformLocation(ss->getProgram(), "worldNormalSampler"), 14);
    }
    glDrawElements(GL_TRIANGLES, mesh->getNumberOfTriangles(), GL_UNSIGNED_INT, (void*)0);
    glBindVertexArray(0);
}

Light::~Light()
{
    //dtor
}
