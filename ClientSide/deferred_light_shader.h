#ifndef DEFERREDLIGHTSHADER_H
#define DEFERREDLIGHTSHADER_H

#include "general_shader.h"

class DeferredLightShader : public GeneralShader
{
    public:
        DeferredLightShader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
        void getAllUniformLocations();
        void loadTextureSampler(int);
        void loadHasTexture(bool);
        void loadHasAlpha(bool);
        virtual ~DeferredLightShader();
    protected:
    private:
        GLuint textureSamplerLocation, hasTextureLocation, hasAlphaLocation;
};

#endif // DEFERREDLIGHTSHADER_H
