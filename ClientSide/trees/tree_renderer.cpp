#include "tree_renderer.h"

TreeRenderer::TreeRenderer(const std::vector<glm::vec3>& positionsAndRotations, Tree *wantedTreeModel) : positionsAndRotations(positionsAndRotations),
                                                                                                         tree(wantedTreeModel)
{
    glGenVertexArrays(1, &trunkVao);
    glBindVertexArray(trunkVao);

    GLuint trunkVbo = createTrunkVbo(wantedTreeModel->getTrunkMesh()->getVertices(),
                                     wantedTreeModel->getTrunkMesh()->getIndices());
    glBindBuffer(GL_ARRAY_BUFFER, trunkVbo);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, NUM_OF_BYTES_PER_INSTANCE, (void*)0);
    glVertexAttribDivisor(0, 1);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, NUM_OF_BYTES_PER_INSTANCE, (void*)(4*4));
    glVertexAttribDivisor(1, 1);

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, NUM_OF_BYTES_PER_INSTANCE, (void*)(8*4));
    glVertexAttribDivisor(2, 1);

    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, NUM_OF_BYTES_PER_INSTANCE, (void*)(12*4));
    glVertexAttribDivisor(3, 1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

glm::mat4

GLuint TreeRenderer::createTrunkVbo(const std::vector<Vertex>& vertices,
                                    const std::vector<unsigned int>& indices)
{
    GLuint vboHandle;
    glGenBuffers(1, &vboHandle);
    glBindBuffer(GL_ARRAY_BUFFER, vboHandle);
    glBufferData(GL_ARRAY_BUFFER, NUM_OF_BYTES_PER_INSTANCE * numOfParticles, 0, GL_STREAM_DRAW);

    return vboHandle;
}

GLuint TreeRenderer::createCrownVbo(const std::vector<Vertex>& vertices,
                                    const std::vector<unsigned int>& indices) {

}

TreeRenderer::~TreeRenderer()
{
    delete wantedTreeModel;
}
