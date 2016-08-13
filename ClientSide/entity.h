#ifndef ENTITY_H
#define ENTITY_H

#include "mesh.h"
#include <glm/glm.hpp>
#include "shader.h"
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <vector>

class Entity
{
    public:
        Entity(std::string, Mesh*, glm::vec4, glm::vec3, glm::vec3, glm::vec3);
        void draw(Shader*);
        void set_rotation(float, float, float);
        void set_scale(float, float, float);
        void set_position(float, float, float);
        void set_position(glm::vec3);
        void add_rotation(float, float, float);
        void add_scale(float, float, float);
        void add_position(float, float, float);
        glm::mat4 get_model_matrix();
        void set_model_matrix(glm::mat4);
        void set_color(glm::vec4);
        std::string get_name();
        glm::vec4 get_color();
        glm::vec3 get_position();
        glm::vec3 get_rotation();
        glm::vec3 get_scale();
        std::vector<glm::vec2> get_normals();
        virtual ~Entity();
    protected:
    private:
        void compute_model_matrix();
        std::string name;
        Mesh *mesh;
        glm::vec3 position, rotation;
        glm::vec3 scale;
        glm::vec4 color;
        glm::mat4 model_matrix;
        bool calc_model_matrix;
};

#endif // ENTITY_H
