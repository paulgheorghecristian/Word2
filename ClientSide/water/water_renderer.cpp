#include "water_renderer.h"

WaterRenderer::WaterRenderer(int width, int height, const glm::vec3& position,
                             const glm::vec3& scale, const glm::mat4& projectionMatrix) : waterHeight(position.y)
{
    waterFramebuffer = new Framebuffer(width, height, 2);
    reflectionTexture = new Texture(waterFramebuffer->getRenderTargets()[0], 0);
    refractionTexture = new Texture(waterFramebuffer->getRenderTargets()[1], 1);
    DuDvTexture = new Texture("water/waterDUDV.bmp", 2);
    normalMapTexture = new Texture("water/waterNormalMap.bmp", 3);
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
    glUniform1i(glGetUniformLocation(waterShader->getProgram(), "DuDvTextureSampler"), 2);
    glUniform1i(glGetUniformLocation(waterShader->getProgram(), "normalMapTextureSampler"), 3);
    glUniform1i(glGetUniformLocation(waterShader->getProgram(), "width"), width);
    glUniform1i(glGetUniformLocation(waterShader->getProgram(), "height"), height);
    dudvOffsetUniformLocation = glGetUniformLocation(waterShader->getProgram(), "dudvOffset");

    dudvOffset = 0;
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
    dudvOffset += WAVE_SPEED * Display::getDelta()/1000.0f;
    if (dudvOffset > 1) {
        dudvOffset = 0;
    }
    reflectionTexture->use();
    refractionTexture->use();
    DuDvTexture->use();
    normalMapTexture->use();
    waterShader->bind();
    waterShader->loadFloat(dudvOffsetUniformLocation, dudvOffset);
    waterShader->loadViewMatrix(viewMatrix);
    waterMesh->draw();
}

WaterRenderer::~WaterRenderer()
{
    delete waterFramebuffer;
    delete reflectionTexture;
    delete refractionTexture;
    delete DuDvTexture;
    delete normalMapTexture;
    delete waterMesh;
    delete waterShader;
}
