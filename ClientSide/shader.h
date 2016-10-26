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
        std::string loadShader(const std::string&);
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
        void loadHasTexture(int);
        void reloadShader();
        GLuint getProgram();
        virtual ~Shader();
    protected:
        GLuint program;
    private:
        std::string vertexShaderPath, fragmentShaderPath;
        GLuint vertexHandle, fragmentHandle;
        GLuint projMatLocation, modelMatLocation, colorVectorLocation, viewMatLocation;
        GLuint hasTextureLocation;
};

#endif // SHADER_H
