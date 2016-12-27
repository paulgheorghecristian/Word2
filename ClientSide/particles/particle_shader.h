#ifndef PARTICLESHADER_H
#define PARTICLESHADER_H

#include "general_shader.h"
#include <string>

class ParticleShader : public GeneralShader
{
    public:
        ParticleShader(const std::string&, const std::string&, const std::string&);
        void getAllUniformLocations();
        void loadViewModelMatrix(const glm::mat4&);
        void loadSampler(int);
        virtual ~ParticleShader();
    protected:
    private:
        GLuint viewModelMatrixLocation, particleSamplerLocation;
};

#endif // PARTICLESHADER_H
