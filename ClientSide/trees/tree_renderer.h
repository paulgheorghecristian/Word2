#ifndef TREERENDERER_H
#define TREERENDERER_H

#include <vector>
#include <glm/glm.hpp>

#include "tree.h"
#include "../simple_shader.h"

class TreeRenderer
{
    public:
        TreeRenderer(const std::vector<glm::vec3>& positionsRotationsScales, Tree *wantedTreeModel, const glm::vec3& projectionMatrix);
    	void draw(const glm::vec3& viewMatrix);
	virtual ~TreeRenderer();
    protected:
    private:
    	void fillBufferWithModelMatrices(float *matricesBuffer);
	void createTrunkVbo(const std::vector<Vertex>& vertices,
			      const std::vector<unsigned int>& indices);
    	void createCrownVbo(const std::vector<Vertex>& vertices,
			      const std::vector<unsigned int>& indices);
    	void createModelMatricesVbo();
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
};

#endif // TREERENDERER_H
