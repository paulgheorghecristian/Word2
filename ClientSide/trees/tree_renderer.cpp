#include "tree_renderer.h"

TreeRenderer::TreeRenderer(const std::vector<glm::vec3>& positionsRotationsScales,
                           Tree *wantedTreeModel,
                           const glm::mat4& projectionMatrix,
                           SimpleShader *shader) : positionsRotationsScales(positionsRotationsScales),
                                                                    tree(wantedTreeModel),
                                                                    numOfInstances(positionsRotationsScales.size()/3)
{
    this->shader = shader;
    shader->bind();
    shader->loadProjectionMatrix(projectionMatrix);
    shader->loadTextureSampler(0);
    shader->loadHasTexture(1);

    matricesBuffer = new float[numOfInstances * NUM_OF_BYTES_PER_INSTANCE / sizeof(float)];
    fillBufferWithModelMatrices();

    glGenVertexArrays(1, &trunkVao);
    glBindVertexArray(trunkVao);

    createTrunkVbo(wantedTreeModel->getTrunkMesh()->getVertices(),
                    wantedTreeModel->getTrunkMesh()->getIndices());
    GLuint modelMatricesVbo = createModelMatricesVbo();

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glGenVertexArrays(1, &crownVao);
    glBindVertexArray(crownVao);

    createCrownVbo(wantedTreeModel->getCrownMesh()->getVertices(),
                    wantedTreeModel->getCrownMesh()->getIndices());
    glBindBuffer(GL_ARRAY_BUFFER, modelMatricesVbo);
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, NUM_OF_BYTES_PER_INSTANCE, (void*)0);
    glVertexAttribDivisor(3, 1);

    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, NUM_OF_BYTES_PER_INSTANCE, (void*)(4*4));
    glVertexAttribDivisor(4, 1);

    glEnableVertexAttribArray(5);
    glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, NUM_OF_BYTES_PER_INSTANCE, (void*)(8*4));
    glVertexAttribDivisor(5, 1);

    glEnableVertexAttribArray(6);
    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, NUM_OF_BYTES_PER_INSTANCE, (void*)(12*4));
    glVertexAttribDivisor(6, 1);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    branchTexture = new Texture("res/models/tree/Bark_Tile.bmp", 0);
    crownTexture = new Texture("res/models/tree2/leaf.bmp", 0);
}

TreeRenderer::TreeRenderer(const std::vector<glm::vec3>& positionsRotationsScales,
                           Tree *wantedTreeModel,
                           const glm::mat4& projectionMatrix) : TreeRenderer(positionsRotationsScales,
                                                                             wantedTreeModel,
                                                                             projectionMatrix,
                                                                             new SimpleShader("trees/treeShader.vs", "trees/treeShader.fs"))
{}

GLuint TreeRenderer::createModelMatricesVbo(){
    GLuint vboHandle;

    glGenBuffers(1, &vboHandle);
    glBindBuffer(GL_ARRAY_BUFFER, vboHandle);
    glBufferData(GL_ARRAY_BUFFER, NUM_OF_BYTES_PER_INSTANCE * numOfInstances, (void*)matricesBuffer, GL_STATIC_DRAW);

    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, NUM_OF_BYTES_PER_INSTANCE, (void*)0);
    glVertexAttribDivisor(3, 1);

    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, NUM_OF_BYTES_PER_INSTANCE, (void*)(4*4));
    glVertexAttribDivisor(4, 1);

    glEnableVertexAttribArray(5);
    glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, NUM_OF_BYTES_PER_INSTANCE, (void*)(8*4));
    glVertexAttribDivisor(5, 1);

    glEnableVertexAttribArray(6);
    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, NUM_OF_BYTES_PER_INSTANCE, (void*)(12*4));
    glVertexAttribDivisor(6, 1);

    delete matricesBuffer;
    return vboHandle;
}

void TreeRenderer::fillBufferWithModelMatrices(){
    int offset = 0;

    for (unsigned int i = 0; i < positionsRotationsScales.size(); i += 3) {
        glm::vec3 position = positionsRotationsScales[i];
        glm::vec3 rotation = positionsRotationsScales[i+1];
        glm::vec3 scale = positionsRotationsScales[i+2];
        glm::mat4 matrix(1.0f);
        matrix = glm::translate(matrix, position);
        matrix = glm::rotate(matrix, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
        matrix = glm::rotate(matrix, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
        matrix = glm::rotate(matrix, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
        matrix = glm::scale(matrix, scale);

        matricesBuffer[offset++] = matrix[0][0];
        matricesBuffer[offset++] = matrix[0][1];
        matricesBuffer[offset++] = matrix[0][2];
        matricesBuffer[offset++] = matrix[0][3];
        matricesBuffer[offset++] = matrix[1][0];
        matricesBuffer[offset++] = matrix[1][1];
        matricesBuffer[offset++] = matrix[1][2];
        matricesBuffer[offset++] = matrix[1][3];
        matricesBuffer[offset++] = matrix[2][0];
        matricesBuffer[offset++] = matrix[2][1];
        matricesBuffer[offset++] = matrix[2][2];
        matricesBuffer[offset++] = matrix[2][3];
        matricesBuffer[offset++] = matrix[3][0];
        matricesBuffer[offset++] = matrix[3][1];
        matricesBuffer[offset++] = matrix[3][2];
        matricesBuffer[offset++] = matrix[3][3];
    }
}

void TreeRenderer::draw(const glm::mat4& viewMatrix){
    shader->bind();
    shader->loadViewMatrix(viewMatrix);

    glEnable(GL_CULL_FACE);
    glBindVertexArray(trunkVao);
    shader->loadHasAlpha(0);
    branchTexture->use();
    glDrawElementsInstanced(GL_TRIANGLES, tree->getTrunkMesh()->getNumberOfTriangles(), GL_UNSIGNED_INT, 0, numOfInstances);
    glBindVertexArray(0);

    glDisable(GL_CULL_FACE);
    glBindVertexArray(crownVao);
    shader->loadHasAlpha(1);
    crownTexture->use();
    glDrawElementsInstanced(GL_TRIANGLES, tree->getCrownMesh()->getNumberOfTriangles(), GL_UNSIGNED_INT, 0, numOfInstances);
    glBindVertexArray(0);
}

void TreeRenderer::createTrunkVbo(const std::vector<Vertex>& vertices,
                                    const std::vector<unsigned int>& indices)
{
    glGenBuffers(NUM_VBOS, vboTrunkHandles);

    glBindBuffer(GL_ARRAY_BUFFER, vboTrunkHandles[VERTEX]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*vertices.size(), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboTrunkHandles[INDEX]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*indices.size(), &indices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::vec3)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(2*sizeof(glm::vec3)));
}

void TreeRenderer::createCrownVbo(const std::vector<Vertex>& vertices,
                                    const std::vector<unsigned int>& indices)
{
    glGenBuffers(NUM_VBOS, vboCrownHandles);

    glBindBuffer(GL_ARRAY_BUFFER, vboCrownHandles[VERTEX]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*vertices.size(), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboCrownHandles[INDEX]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*indices.size(), &indices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::vec3)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(2*sizeof(glm::vec3)));
}

TreeRenderer::~TreeRenderer()
{
    delete tree;
    delete shader;
    delete branchTexture;
    delete crownTexture;
}
