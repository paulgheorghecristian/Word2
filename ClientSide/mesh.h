#ifndef MESH_H
#define MESH_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>

struct Vertex{
    glm::vec3 positionCoords;
    glm::vec3 normalCoords;
    glm::vec2 textureCoords;

    Vertex() : positionCoords(0.0f), normalCoords(0.0f), textureCoords(0.0f){}

    Vertex(glm::vec3 positionCoords, glm::vec3 normalCoords, glm::vec2 tc){
        this->positionCoords = positionCoords;
        this->normalCoords = normalCoords;
        this->textureCoords = tc;
    }
    Vertex(glm::vec3 positionCoords) : Vertex(positionCoords, glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f)){}
    Vertex(glm::vec3 positionCoords, glm::vec3 normalCoords) : Vertex(positionCoords, normalCoords, glm::vec2(0.0f, 0.0f)){}
    Vertex(glm::vec3 positionCoords, glm::vec2 textureCoords) : Vertex(positionCoords, glm::vec3(0.0f, 1.0f, 0.0f), textureCoords){}
};

class Mesh
{
    public:
        Mesh(std::vector<Vertex>&, std::vector<unsigned int>&);
        GLuint getVao();
        void draw();
        GLsizei getNumberOfTriangles();
        static Mesh* loadObject(std::string);
        static Mesh* getSurface(int, int);
        virtual ~Mesh();
    protected:
    private:
            enum VBOs{
                VERTEX, INDEX, NUM_VBOS
            };
            GLuint vaoHandle;
            GLuint vboHandles[NUM_VBOS];
            GLsizei numberOfTriangles;
            static float _stringToFloat(const std::string &source);
            static unsigned int _stringToUint(const std::string &source);
            static int _stringToInt(const std::string &source);
            static void _stringTokenize(const std::string &source, std::vector<std::string> &tokens);
            static void _faceTokenize(const std::string &source, std::vector<std::string> &tokens);
};

#endif // MESH_H
