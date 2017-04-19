#include "water_renderer.h"

WaterRenderer::WaterRenderer(int width, int height, const glm::vec3& position,
                             const glm::vec3& scale, const glm::mat4& projectionMatrix) : waterHeight(position.y)
{
    waterFramebuffer = new Framebuffer(width, height, 2);
    reflectionTexture = new Texture(waterFramebuffer->getRenderTargets()[0], 0);
    refractionTexture = new Texture(waterFramebuffer->getRenderTargets()[1], 1);
    waterMesh = Mesh::getRectangleYUp();
    waterShader = new GeneralShader("water/waterShader.vs", "water/waterShader.fs");

    glm::mat4 modelMatrix(1.0f);
    modelMatrix = glm::translate(modelMatrix, position);
    modelMatrix = glm::scale(modelMatrix, scale);
    waterShader->bind();
    waterShader->loadModelMatrix(modelMatrix);
    waterShader->loadProjectionMatrix(projectionMatrix);
    waterShader->loadColor(glm::vec4(0,0,1,1));
    glUniform1i(glGetUniformLocation(waterShader->getProgram(), "reflectionSampler"), 0);
    glUniform1i(glGetUniformLocation(waterShader->getProgram(), "refractionSampler"), 1);
    glUniform1i(glGetUniformLocation(waterShader->getProgram(), "width"), width);
    glUniform1i(glGetUniformLocation(waterShader->getProgram(), "height"), height);
}

float WaterRenderer::getWaterHeight() {
    return waterHeight;
}

void WaterRenderer::bindForReflectionTexture() {
    waterFramebuffer->bindSingleRenderTarget(0);
}

void WaterRenderer::bindForRefractionTexture() {
    waterFramebuffer->bindSingleRenderTarget(1);
}

void WaterRenderer::unbind() {
    waterFramebuffer->unbind();
}

void WaterRenderer::draw(const glm::mat4& viewMatrix) {
    reflectionTexture->use();
    refractionTexture->use();
    waterShader->bind();
    waterShader->loadViewMatrix(viewMatrix);
    waterMesh->draw();
}

WaterRenderer::~WaterRenderer()
{
    delete waterFramebuffer;
    delete reflectionTexture;
    delete refractionTexture;
    delete waterMesh;
    delete waterShader;
}
