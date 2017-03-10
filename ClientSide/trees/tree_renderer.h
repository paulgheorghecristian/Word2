#ifndef TREERENDERER_H
#define TREERENDERER_H

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "tree.h"
#include "../simple_shader.h"
#include "../texture.h"

class TreeRenderer
{
    public:
        TreeRenderer(const std::vector<glm::vec3>& positionsRotationsScales,
                     Tree *wantedTreeModel,
                     const glm::mat4& projectionMatrix);
    	void draw(const glm::mat4& viewMatrix);
	virtual ~TreeRenderer();
    protected:
    private:
    	void fillBufferWithModelMatrices();
        void createCrownVbo(const std::vector<Vertex>& vertices,
                            const std::vector<unsigned int>& indices);
        void createTrunkVbo(const std::vector<Vertex>& vertices,
                            const std::vector<unsigned int>& indices);
    	GLuint createModelMatricesVbo();
        std::vector<glm::vec3> positionsRotationsScales;
        Tree *tree;
        GLuint trunkVao, crownVao;
        enum VBOs{
            VERTEX, INDEX, NUM_VBOS
        };
        GLuint vboTrunkHandles[NUM_VBOS];
    	GLuint vboCrownHandles[NUM_VBOS];
    	float *matricesBuffer;
    	const unsigned int NUM_OF_BYTES_PER_INSTANCE = 16*4;
    	unsigned int numOfInstances;
    	SimpleShader *shader;
    	Texture *branchTexture, *crownTexture;
};

#endif // TREERENDERER_H
