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
    	void loadHasAlpha(bool hasAlpha);
	virtual ~SimpleShader();
    protected:
    private:
        GLuint textureSamplerLocation, hasTextureLocation, hasAlphaLocation;
};

#endif // SIMPLESHADER_H
