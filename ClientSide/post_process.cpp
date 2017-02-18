#include "post_process.h"

PostProcess::PostProcess(float width, float height, const std::string& vertexShaderFile, const std::string& fragmentShaderFile) : width(width), height(height), fb(width, height, 2), processShader(vertexShaderFile, fragmentShaderFile)
{
    renderingQuad = Mesh::getRectangle();
    inputTexture = new Texture(fb.getRenderTargets()[0], 0);
    processShader.loadFloat(glGetUniformLocation(processShader.getProgram(), "pixelWidth"), width);
    processShader.loadFloat(glGetUniformLocation(processShader.getProgram(), "pixelHeight"), height);
}

void PostProcess::bind(){
    fb.bindSingleRenderTarget(0);
}

void PostProcess::process(){
    fb.unbind();
    fb.bindSingleRenderTarget(1);
    processShader.bind();
    processShader.loadTextureSampler(0);
    inputTexture->use();
    renderingQuad->draw();
    fb.unbind();
}

SimpleShader& PostProcess::getShader(){
    return processShader;
}

GLuint PostProcess::getResultingTextureId(){
    return fb.getRenderTargets()[1];
}

GLuint PostProcess::getFrameBufferObject(){
    return fb.getFrameBufferObject();
}

PostProcess::~PostProcess()
{
    delete renderingQuad;
    delete inputTexture;
}
