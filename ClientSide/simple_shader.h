#ifndef SIMPLESHADER_H
#define SIMPLESHADER_H

#include "general_shader.h"

class SimpleShader : public GeneralShader
{
    public:
        SimpleShader(const std::string&, const std::string&);
        void getAllUniformLocations();
        void loadTextureSampler(int);
        void loadHasTexture(bool);
        virtual ~SimpleShader();
    protected:
    private:
        GLuint textureSamplerLocation, hasTextureLocation;
};

#endif // SIMPLESHADER_H
