#ifndef GRASSRENDERER_H
#define GRASSRENDERER_H

#include <vector>
#include <glm/glm.hpp>
#include "../mesh.h"
#include "../texture.h"
#include "../simple_shader.h"
#include <glm/gtc/matrix_transform.hpp>

//FIXME make a class InstancedRenderer that this one inherits from

class GrassRenderer
{
    public:
        GrassRenderer(const std::vector<glm::vec3>& positionsScales,
                      Mesh *mesh,
                      Texture *texture,
                      const glm::mat4& projectionMatrix);
        void draw(const glm::mat4& viewMatrix);
        virtual ~GrassRenderer();
    protected:
    private:
        SimpleShader *shader;
        Texture *texture;
        Mesh *mesh;
        std::vector<glm::vec3> positionsScales;
        float *matricesBuffer;
    	const unsigned int NUM_OF_BYTES_PER_INSTANCE = 16*4;
    	unsigned int numOfInstances;
    	GLuint grassVao;
        enum VBOs{
            VERTEX, INDEX, NUM_VBOS
        };
        GLuint vboGrassHandles[NUM_VBOS];

        void fillBufferWithModelMatrices();
        GLuint createModelMatricesVbo();
        void createGrassVbo(const std::vector<Vertex>& vertices,
                                            const std::vector<unsigned int>& indices);
};

#endif // GRASSRENDERER_H
