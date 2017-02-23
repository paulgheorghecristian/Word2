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
    hasAlphaLocation = glGetUniformLocation(program, "hasAlpha");
}

void DeferredLightShader::loadTextureSampler(int x){
    GeneralShader::loadInt(textureSamplerLocation, x);
}

void DeferredLightShader::loadHasTexture(bool hasTexture){
    GeneralShader::loadInt(hasTextureLocation, (int)hasTexture);
}

void DeferredLightShader::loadHasAlpha(bool hasAlpha){
    GeneralShader::loadInt(hasAlphaLocation, (int)hasAlpha);
}

DeferredLightShader::~DeferredLightShader()
{

}
