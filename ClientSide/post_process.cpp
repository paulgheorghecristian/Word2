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

PostProcess::PostProcess(float width,
                         float height,
                         const std::string& vertexShaderFile,
                         const std::string& fragmentShaderFile,
                         std::vector<std::string>& texturePaths) : PostProcess(width, height, vertexShaderFile, fragmentShaderFile)
{
    constructTextures(texturePaths);
}

PostProcess::PostProcess(float width,
                         float height,
                         GLuint inputTextureId,
                         const std::string& vertexShaderFile,
                         const std::string& fragmentShaderFile,
                         std::vector<std::string>& texturePaths) : PostProcess(width, height, inputTextureId, vertexShaderFile, fragmentShaderFile)
{
    constructTextures(texturePaths);
}

void PostProcess::constructTextures(std::vector<std::string>& paths){
    processShader.bind();
    for(int i = 0; i < paths.size()/2; i+=2){
        additionalTextures.push_back(new Texture(paths[i].c_str(), i/2+1));
        glUniform1i(glGetUniformLocation(processShader.getProgram(), paths[i+1].c_str()), i/2+1);
    }
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
    for(Texture *t : additionalTextures){
        t->use();
    }
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

GLuint PostProcess::getInputTextureId(){
    return inputTexture->getTextureId();
}

GLuint PostProcess::getFrameBufferObject(){
    return fb.getFrameBufferObject();
}

PostProcess::~PostProcess()
{
    delete renderingQuad;
    delete inputTexture;
}
