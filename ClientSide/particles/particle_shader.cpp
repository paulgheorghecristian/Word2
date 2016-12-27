#include "particle_shader.h"

ParticleShader::ParticleShader(const std::string& vertexShaderPath, const std::string& geometryShaderPath, const std::string& fragmentShaderPath) : GeneralShader(vertexShaderPath, geometryShaderPath, fragmentShaderPath)
{
    bind();
    getAllUniformLocations();
}

void ParticleShader::getAllUniformLocations(){
    GeneralShader::getAllUniformLocations();
    viewModelMatrixLocation = glGetUniformLocation(program, "viewModelMatrix");
    particleSamplerLocation = glGetUniformLocation(program , "particleSampler");
}

void ParticleShader::loadViewModelMatrix(const glm::mat4& viewModelMatrix){
    loadMatrix(viewModelMatrixLocation, viewModelMatrix);
}

void ParticleShader::loadSampler(int x){
    loadInt(particleSamplerLocation, x);
}

ParticleShader::~ParticleShader()
{
    //dtor
}
