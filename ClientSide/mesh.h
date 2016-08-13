#ifndef MESH_H
#define MESH_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>

#define PI 3.1415926535

struct Vertex{
    glm::vec3 position_coords;
    glm::vec3 normal_coords;
    glm::vec2 texture_coords;

    Vertex() : position_coords(0.0f), normal_coords(0.0f), texture_coords(0.0f){}

    Vertex(glm::vec3 position_coords, glm::vec3 normal_coords, glm::vec2 tc){
        this->position_coords = position_coords;
        this->normal_coords = normal_coords;
        this->texture_coords = tc;
    }
    Vertex(glm::vec3 position_coords) : Vertex(position_coords, glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f)){}
    Vertex(glm::vec3 position_coords, glm::vec3 normal_coords) : Vertex(position_coords, normal_coords, glm::vec2(0.0f, 0.0f)){}
    Vertex(glm::vec3 position_coords, glm::vec2 text_coords) : Vertex(position_coords, glm::vec3(0.0f, 1.0f, 0.0f), text_coords){}
};

class Mesh
{
    public:
        Mesh(std::vector<Vertex>&, std::vector<unsigned int>&);
        GLuint get_vao();
        void draw();
        GLsizei get_no_triangles();
        static Mesh* load_object(std::string);
        static Mesh* get_surface(int, int);
        virtual ~Mesh();
    protected:
    private:
            enum VBOs{
                VERTEX, INDEX, NUM_VBOS
            };
            GLuint vao_handle;
            GLuint vbo_handles[NUM_VBOS];
            GLsizei no_triangles;
            static float _stringToFloat(const std::string &source);
            static unsigned int _stringToUint(const std::string &source);
            static int _stringToInt(const std::string &source);
            static void _stringTokenize(const std::string &source, std::vector<std::string> &tokens);
            static void _faceTokenize(const std::string &source, std::vector<std::string> &tokens);
};

#endif // MESH_H
