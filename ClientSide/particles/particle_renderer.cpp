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
        float d1 = (float)rand() / (RAND_MAX-1);
        float d2 = (float)rand() / (RAND_MAX-1);
        float d3 = (float)rand() / (RAND_MAX-1);
        glm::vec3 pos = glm::vec3(center.x + d2*10.0f, center.y + d1*10.0f, center.z + d2*10.0f);
        particles.push_back(new Particle(pos, glm::vec3(0, 0, -100*d1), 4.0f, 30));
    }

    createVao();
}

void ParticleRenderer::createVao(){
    glm::vec3 position(0);
    glGenVertexArrays(1, &vaoHandle);
    glBindVertexArray(vaoHandle);

    glGenBuffers(1, &vboHandle);

    glBindBuffer(GL_ARRAY_BUFFER, vboHandle);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3), &position, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
}

void ParticleRenderer::update(long delta, Camera *camera){
    particles.erase(
        std::remove_if(
        particles.begin(),
        particles.end(),
        [](Particle* p) -> bool {
            return !p->isAlive();
        }
        ),
        particles.end()
    );
    for(Particle *p : particles){
        p->update(delta, camera);
    }
    shader->bind();
    shader->loadViewMatrix(camera->getViewMatrix());
}

void ParticleRenderer::draw(){
    glBindVertexArray(vaoHandle);
    shader->bind();
    particleTexture.use();
    for(Particle *p : particles){
        p->draw(shader);
    }
    glBindVertexArray(0);
}

ParticleRenderer::~ParticleRenderer()
{
    glDeleteBuffers(1, &vboHandle);
    glDeleteVertexArrays(1, &vaoHandle);

    delete shader;
    for(Particle *p : particles){
        delete p;
    }
}
