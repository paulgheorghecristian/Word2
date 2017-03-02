#include "particle_renderer.h"

ParticleRenderer::ParticleRenderer(const glm::mat4& projectionMatrix,
                                   const glm::vec3& center,
                                   unsigned int numOfParticles) : numOfParticles(numOfParticles), centerPosition(center), particleTexture("particles/particle.bmp", 0)
{
    srand(time(0));
    shader = new ParticleShader("particles/particle_shader.vs", "particles/particle_shader.gs", "particles/particle_shader.fs");
    shader->bind();
    shader->loadProjectionMatrix(projectionMatrix);
    shader->loadSampler(0);

    for(unsigned int i = 0; i < numOfParticles; i++){
        float d1 = (float)(rand()) / (RAND_MAX-1);
        float d2 = (float)(rand()) / (RAND_MAX-1);
        float d3 = (float)(rand()) / (RAND_MAX-1);
        glm::vec3 pos = glm::vec3(center.x-3, center.y-3, center.z-3);
        pos.x += d1 * 6.0f;
        pos.y += d2 * 6.0f;
        pos.z += d3 * 6.0f;
        particles.push_back(new Particle(pos, glm::vec3(0, 0, -100*d3), rand()%7+3.0f, rand()%5+5));
    }

    matricesBuffer = new float[numOfParticles*16];

    createVao();
}

void ParticleRenderer::createVao(){
    glGenVertexArrays(1, &vaoHandle);
    glBindVertexArray(vaoHandle);

    createEmptyVbo();
    glBindBuffer(GL_ARRAY_BUFFER, vboHandle);

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

void ParticleRenderer::createEmptyVbo(){
    glGenBuffers(1, &vboHandle);
    glBindBuffer(GL_ARRAY_BUFFER, vboHandle);
    glBufferData(GL_ARRAY_BUFFER, NUM_OF_BYTES_PER_INSTANCE * numOfParticles, 0, GL_STREAM_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void ParticleRenderer::insertMatrixInBuffer(glm::mat4& matrix, int& offset){
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

void ParticleRenderer::updateVbo(){
    glBindBuffer(GL_ARRAY_BUFFER, vboHandle);
    glBufferData(GL_ARRAY_BUFFER, NUM_OF_BYTES_PER_INSTANCE*numOfParticles, (void*)matricesBuffer, GL_STREAM_DRAW);
    //glBufferSubData(GL_ARRAY_BUFFER, 0, NUM_OF_BYTES_PER_INSTANCE*numOfParticles, (void*)matricesBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void ParticleRenderer::update(long delta, Camera *camera, Entity *particleGenerator, const std::vector<PuzzleObject*>& particleInteractors){
    /*particles.erase(
        std::remove_if(
        particles.begin(),
        particles.end(),
        [](Particle* p) -> bool {
            return !p->isAlive();
        }
        ),
        particles.end()
    );*/
    for(PuzzleObject *particleInteractor : particleInteractors){
        glm::vec3 rot = particleInteractor->getEntities()[0]->getRotation();
        glm::vec3 velocity;
        float sinX = glm::sin(rot.x);
        float cosX = glm::cos(rot.x);
        float sinY = glm::sin(rot.y);
        float cosY = glm::cos(rot.y);

        velocity.x = -cosX * sinY;
        velocity.y = sinX;
        velocity.z = -cosX * cosY;

        glm::vec3 p1 = glm::vec3(particleInteractor->getEntities()[0]->getModelMatrix()*glm::vec4(particleInteractor->boundingRectangle[0], 1.0));
        glm::vec3 p2 = glm::vec3(particleInteractor->getEntities()[0]->getModelMatrix()*glm::vec4(particleInteractor->boundingRectangle[1], 1.0));
        glm::vec3 p3 = glm::vec3(particleInteractor->getEntities()[0]->getModelMatrix()*glm::vec4(particleInteractor->boundingRectangle[3], 1.0));

        for(Particle *p : particles){
            if(p->isAlive()){
                if(MathUtils::isPointInsideRectangle(p1, p2, p3, p->getPosition())){
                    p->addVelocity(velocity * 0.5f * particleInteractor->getEntities()[0]->getScale().x * (float)Display::getDelta());
                }
            }
        }
    }
    int offset = 0;
    for(Particle *p : particles){
        if (!p->isAlive()) {
            p->reset(particleGenerator->getPosition(), particleGenerator->getRotation());
        } else {
            p->update(delta, camera);
        }

        insertMatrixInBuffer(p->getViewModelMatrix(), offset);
    }
    updateVbo();
    shader->bind();
    shader->loadViewMatrix(camera->getViewMatrix());
}

void ParticleRenderer::draw(){
    glBindVertexArray(vaoHandle);
    shader->bind();
    particleTexture.use();
    glDrawArraysInstanced(GL_POINTS, 0, 1, numOfParticles);
    glBindVertexArray(0);
}

std::vector<Particle*>& ParticleRenderer::getParticles(){
    return particles;
}


ParticleRenderer::~ParticleRenderer()
{
    glDeleteBuffers(1, &vboHandle);
    glDeleteVertexArrays(1, &vaoHandle);

    delete shader;
    for(Particle *p : particles){
        delete p;
    }

    delete[] matricesBuffer;
}
