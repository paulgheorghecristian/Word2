#include "grass_renderer.h"

GrassRenderer::GrassRenderer(const std::vector<glm::vec3>& positionsScales,
                              Mesh *mesh,
                              Texture *texture,
                              const glm::mat4& projectionMatrix) : positionsScales(positionsScales),
                                                                    mesh(mesh),
                                                                    texture(texture),
                                                                    numOfInstances(positionsScales.size()/2)
{
    shader = new SimpleShader("grass/grassShader.vs", "grass/grassShader.fs");
    shader->bind();
    shader->loadProjectionMatrix(projectionMatrix);

    matricesBuffer = new float[numOfInstances * NUM_OF_BYTES_PER_INSTANCE / sizeof(float)];
    fillBufferWithModelMatrices();

    glGenVertexArrays(1, &grassVao);
    glBindVertexArray(grassVao);

    createGrassVbo(mesh->getVertices(),
                    mesh->getIndices());
    GLuint modelMatricesVbo = createModelMatricesVbo();

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void GrassRenderer::fillBufferWithModelMatrices() {
    int offset = 0;

    for (unsigned int i = 0; i < positionsScales.size(); i += 2) {
        glm::vec3 position = positionsScales[i];
        glm::vec3 scale = positionsScales[i+1];
        glm::mat4 matrix(1.0f);
        matrix = glm::translate(matrix, position);
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

GLuint GrassRenderer::createModelMatricesVbo(){
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

void GrassRenderer::createGrassVbo(const std::vector<Vertex>& vertices,
                                    const std::vector<unsigned int>& indices)
{
    glGenBuffers(NUM_VBOS, vboGrassHandles);

    glBindBuffer(GL_ARRAY_BUFFER, vboGrassHandles[VERTEX]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*vertices.size(), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboGrassHandles[INDEX]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*indices.size(), &indices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::vec3)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(2*sizeof(glm::vec3)));
}

void GrassRenderer::draw(const glm::mat4& viewMatrix){
    shader->bind();
    shader->loadViewMatrix(viewMatrix);

    glDisable(GL_CULL_FACE);
    glBindVertexArray(grassVao);
    texture->use();
    glDrawElementsInstanced(GL_TRIANGLES, mesh->getNumberOfTriangles(), GL_UNSIGNED_INT, 0, numOfInstances);
    glBindVertexArray(0);
    glEnable(GL_CULL_FACE);
}

GrassRenderer::~GrassRenderer()
{
    delete mesh;
    delete texture;
    delete shader;
}
