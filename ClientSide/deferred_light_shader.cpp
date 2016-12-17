#include "deferred_light_shader.h"

DeferredLightShader::DeferredLightShader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath) : GeneralShader(vertexShaderPath, fragmentShaderPath)
{
    bind();
    getAllUniformLocations();
}

void DeferredLightShader::getAllUniformLocations(){
    GeneralShader::getAllUniformLocations();
    textureSamplerLocation = glGetUniformLocation(program, "textureSampler");
    hasTextureLocation = glGetUniformLocation(program, "hasTexture");
}

void DeferredLightShader::loadTextureSampler(int x){
    GeneralShader::loadInt(textureSamplerLocation, x);
}

void DeferredLightShader::loadHasTexture(bool hasTexture){
    GeneralShader::loadInt(hasTextureLocation, (int)hasTexture);
}

DeferredLightShader::~DeferredLightShader()
{

}
