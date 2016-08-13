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

struct Wave{
    glm::vec2 direction, center;
    float w, f, A;
    bool circular;
};

class Shader
{
    public:
        Shader(const std::string&, const std::string&);
        std::string load_shader(const std::string&);
        void bind();
        void unbind();
        void getAllUniformLocations();
        void loadHasTexture(int);
        void loadColor(glm::vec4);
        void loadForward(glm::vec3);
        void loadLightPosition(glm::vec3);
        void loadEyePosition(glm::vec3);
        void loadVector4(GLuint, glm::vec4);
        void loadVector3(GLuint, glm::vec3);
        void loadMatrix(GLuint, glm::mat4);
        void loadProjectionMatrix(glm::mat4);
        void loadModelMatrix(glm::mat4);
        void loadViewMatrix(glm::mat4);
        void loadClipPlane(glm::vec4);
        void loadTime(float);
        void loadFloat(GLuint, float);
        void loadWaves(Wave *, int);
        void reload_shader();
        GLuint get_program();
        virtual ~Shader();
    protected:
        GLuint program;
    private:
        std::string v_path, f_path;
        GLuint vertex_handle, fragment_handle;
        GLuint projMat_location, modelMat_location, colorVector_location, viewMat_location, time_location, lightPosition_location, eyePosition_location, clip_plane_location;
        GLuint forward_location, has_texture_location;
};

#endif // SHADER_H
