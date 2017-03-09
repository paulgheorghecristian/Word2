#include "simple_shader.h"

SimpleShader::SimpleShader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath) : GeneralShader(vertexShaderPath, fragmentShaderPath)
{
    bind();
    getAllUniformLocations();
}

void SimpleShader::getAllUniformLocations(){
    GeneralShader::getAllUniformLocations();
    textureSamplerLocation = glGetUniformLocation(program, "textureSampler");
    hasTextureLocation = glGetUniformLocation(program, "hasTexture");
    hasAlphaLocation = glGetUniformLocation(program, "hasAlpha");
}

void SimpleShader::loadTextureSampler(int x){
    GeneralShader::loadInt(textureSamplerLocation, x);
}

void SimpleShader::loadHasTexture(bool hasTexture){
    GeneralShader::loadInt(hasTextureLocation, (int)hasTexture);
}

void SimpleShader::loadHasAlpha(bool hasAlpha){
    GeneralShader::loadInt(hasAlphaLocation, (int)hasAlpha);
}

SimpleShader::~SimpleShader()
{
    //dtor
}
