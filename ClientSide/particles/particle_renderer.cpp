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
        glm::vec3 pos = glm::vec3(center.x + d2*10.0f, center.y, center.z + d2*10.0f);
        particles.push_back(new Particle(pos, glm::vec3(0, 0, -600*d3), rand()%5+4.0f, rand()%5+2));
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

void ParticleRenderer::update(long delta, Camera *camera, Entity *particleGenerator, PuzzleObject *particleInteractor){
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
    glm::vec3 rot = particleInteractor->getEntities()[0]->getRotation();
    glm::vec3 velocity;
    float sinX = glm::sin(rot.x);
    float cosX = glm::cos(rot.x);
    float sinY = glm::sin(rot.y);
    float cosY = glm::cos(rot.y);

    velocity.x = -cosX * sinY;
    velocity.y = sinX;
    velocity.z = -cosX * cosY;
    for(Particle *p : particles){
        if(!p->isAlive()){
            p->reset(particleGenerator->getPosition(), particleGenerator->getRotation());
        }else{
            glm::vec3 p1 = glm::vec3(particleInteractor->getEntities()[0]->getModelMatrix()*glm::vec4(particleInteractor->boundingRectangle[0], 1.0));
            glm::vec3 p2 = glm::vec3(particleInteractor->getEntities()[0]->getModelMatrix()*glm::vec4(particleInteractor->boundingRectangle[1], 1.0));
            glm::vec3 p3 = glm::vec3(particleInteractor->getEntities()[0]->getModelMatrix()*glm::vec4(particleInteractor->boundingRectangle[3], 1.0));
            if(MathUtils::isPointInsideRectangle(p1, p2, p3, p->getPosition())){
                p->addVelocity(velocity * 15.0f * (float)Display::getDelta());
            }
        }
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
}
