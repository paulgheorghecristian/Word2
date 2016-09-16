#ifndef TEXTSHADER_H
#define TEXTSHADER_H

#include "shader.h"
#include <glm/gtc/type_ptr.hpp>

class TextShader : public Shader
{
    public:
        TextShader(const std::string&, const std::string&);
        void getAllUniformLocations();
        void load_model_matrix(glm::mat4);
        void load_view_matrix(glm::mat4);
        void load_projection_matrix(glm::mat4);
        void load_color(glm::vec3);
        virtual ~TextShader();
    protected:
    private:
        GLuint modelMatrix_location, viewMatrix_location, projectionMatrix_location, color_location;
};

#endif // TEXTSHADER_H
