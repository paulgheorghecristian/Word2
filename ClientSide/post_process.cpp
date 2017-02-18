#include "post_process.h"

PostProcess::PostProcess(float width, float height, const std::string& vertexShaderFile, const std::string& fragmentShaderFile) :  width(width),
                                                                                                                                    height(height),
                                                                                                                                    processShader(vertexShaderFile, fragmentShaderFile),
                                                                                                                                    fb(width, height, 2)
{
    renderingQuad = Mesh::getRectangle();
    inputTexture = new Texture(fb.getRenderTargets()[0], 0);
    processShader.loadFloat(glGetUniformLocation(processShader.getProgram(), "pixelWidth"), width);
    processShader.loadFloat(glGetUniformLocation(processShader.getProgram(), "pixelHeight"), height);
}

PostProcess::PostProcess(float width, float height, GLuint inputTextureId, const std::string& vertexShaderFile, const std::string& fragmentShaderFile) : width(width),
                                                                                                                                                        height(height),
                                                                                                                                                        processShader(vertexShaderFile, fragmentShaderFile),
                                                                                                                                                        fb(width, height, 1)
{
    renderingQuad = Mesh::getRectangle();
    inputTexture = new Texture(inputTextureId, 0);
    processShader.loadFloat(glGetUniformLocation(processShader.getProgram(), "pixelWidth"), width);
    processShader.loadFloat(glGetUniformLocation(processShader.getProgram(), "pixelHeight"), height);
}

void PostProcess::bind(){
    if(fb.getRenderTargets().size() > 1){
        fb.bindSingleRenderTarget(0);
    }
}

void PostProcess::process(){
    if(fb.getRenderTargets().size() > 1){
        fb.unbind();
    }
    fb.bindSingleRenderTarget(fb.getRenderTargets().size()-1);
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
    return fb.getRenderTargets()[fb.getRenderTargets().size()-1];
}

GLuint PostProcess::getFrameBufferObject(){
    return fb.getFrameBufferObject();
}

PostProcess::~PostProcess()
{
    delete renderingQuad;
    delete inputTexture;
}
