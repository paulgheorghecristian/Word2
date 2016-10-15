#ifndef SHADER_H
#define SHADER_H

#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <fstream>
#include <iostream>
#include <GL/glew.h>
#include <string.h>
#include <glm/glm.hpp>

class Shader
{
    public:
        Shader(const std::string&, const std::string&);
        std::string load_shader(const std::string&);
        void bind();
        void unbind();
        void getAllUniformLocations();
        void loadColor(glm::vec4);
        void loadVector4(GLuint, glm::vec4);
        void loadVector3(GLuint, glm::vec3);
        void loadMatrix(GLuint, glm::mat4);
        void loadProjectionMatrix(glm::mat4);
        void loadModelMatrix(glm::mat4);
        void loadViewMatrix(glm::mat4);
        void loadCubePosition(glm::vec3);
        void loadInt(GLuint, int);
        void load_has_texture(int);
        void reload_shader();
        GLuint get_program();
        virtual ~Shader();
    protected:
        GLuint program;
    private:
        std::string v_path, f_path;
        GLuint vertex_handle, fragment_handle;
        GLuint projMat_location, modelMat_location, colorVector_location, viewMat_location;
        GLuint has_texture_location;
};

#endif // SHADER_H
