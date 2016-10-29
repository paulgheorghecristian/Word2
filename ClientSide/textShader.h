#ifndef TEXTSHADER_H
#define TEXTSHADER_H

#include "shader.h"
#include <glm/gtc/type_ptr.hpp>

class TextShader : public Shader
{
    public:
        TextShader(const std::string&, const std::string&);
        void getAllUniformLocations();
        virtual ~TextShader();
    protected:
    private:
};

#endif // TEXTSHADER_H
