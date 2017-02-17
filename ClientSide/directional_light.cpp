#include "directional_light.h"

Mesh* DirectionalLight::mesh;

DirectionalLight::DirectionalLight(GBuffer* gBuffer, glm::vec3 color, glm::vec3 direction) : gBuffer(gBuffer),
                                                                                            color(color),
                                                                                            direction(direction)
{

}

void DirectionalLight::draw(GeneralShader *shader){
    glBindVertexArray(mesh->getVao());
    if(typeid(SimpleShader) == typeid(*shader)){
        SimpleShader *ss = (SimpleShader*)shader;
        ss->loadHasTexture(0);
        ss->loadColor(glm::vec4(color, 1.0));
        glUniform3f(glGetUniformLocation(ss->getProgram(), "lightDirection"), direction.x, direction.y, direction.z);
        glUniform1i(glGetUniformLocation(ss->getProgram(), "screenWidth"), gBuffer->getWidth());
        glUniform1i(glGetUniformLocation(ss->getProgram(), "screenHeight"), gBuffer->getHeight());
        glEnable(GL_TEXTURE_2D);
        glActiveTexture(GL_TEXTURE0+14);
        glBindTexture(GL_TEXTURE_2D, gBuffer->getNormalTexture());
        glUniform1i(glGetUniformLocation(ss->getProgram(), "eyeSpaceNormalSampler"), 14);
    }
    glDrawElements(GL_TRIANGLES, mesh->getNumberOfTriangles(), GL_UNSIGNED_INT, (void*)0);
    glBindVertexArray(0);
}

void DirectionalLight::setMesh(Mesh* _mesh){
    mesh = _mesh;
}

DirectionalLight::~DirectionalLight()
{

}
