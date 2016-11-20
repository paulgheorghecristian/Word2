#ifndef TEXTSHADER_H
#define TEXTSHADER_H

#include "general_shader.h"
#include <glm/gtc/type_ptr.hpp>

class TextShader : public GeneralShader
{
    public:
        TextShader(const std::string&, const std::string&);
        void getAllUniformLocations();
        virtual ~TextShader();
    protected:
    private:
        GLuint  modelMatLocation, projMatLocation, colorVectorLocation;
};

#endif // TEXTSHADER_H
