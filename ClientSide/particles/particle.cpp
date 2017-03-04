#include "particle.h"

Particle::Particle(glm::vec3 position,
                   glm::vec3 velocity,
                   glm::vec3 acceleration,
                   float scale,
                   unsigned int liveForInS) : position(position),
                                              velocity(velocity),
                                              initialPosition(position),
                                              initialVelocity(velocity),
                                              acceleration(acceleration),
                                              scale(scale),
                                              liveForInS(liveForInS),
                                              aliveForInMs(0)
{

}

Particle::Particle(glm::vec3 position,
                   glm::vec3 velocity,
                   float scale,
                   unsigned int liveForInS) : Particle(position, velocity, glm::vec3(0, -GRAVITYP, 0), scale, liveForInS)
{

}

void Particle::reset(glm::vec3 pos, glm::vec3 rot){
    float d1 = (float)(rand()) / (RAND_MAX-1);
    float d2 = (float)(rand()) / (RAND_MAX-1);
    float d3 = (float)(rand()) / (RAND_MAX-1);
    position = glm::vec3(pos.x-3, pos.y-3, pos.z-3);
    position.x += d1 * 6.0f;
    position.y += d2 * 6.0f;
    position.z += d3 * 6.0f;
    glm::vec3 vel;
    float sinX = glm::sin(rot.x);
    float cosX = glm::cos(rot.x);
    float sinY = glm::sin(rot.y);
    float cosY = glm::cos(rot.y);

    vel.x = -cosX * sinY;
    vel.y = sinX;
    vel.z = -cosX * cosY;

    float d = (float)(rand()) / (RAND_MAX-1);
    velocity = glm::normalize(vel) * d * 100.0f;
    aliveForInMs = 0;
}

void Particle::generateViewModelMatrix(Camera *camera){
    glm::mat4 viewMatrix = camera->getViewMatrix();
    glm::mat4 modelMatrix(1.0);

    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            modelMatrix[i][j] = viewMatrix[j][i];
        }
    }

    modelMatrix[3][0] = position.x;
    modelMatrix[3][1] = position.y;
    modelMatrix[3][2] = position.z;

    modelMatrix = glm::scale(modelMatrix, glm::vec3(scale));
    viewModelMatrix = viewMatrix * modelMatrix;
}

void Particle::update(long delta, Camera *camera){
    aliveForInMs += delta;

    float deltaF = delta/1000.0f;

    position = position + velocity*deltaF + acceleration*(deltaF*deltaF/2.0f);
    velocity = velocity + acceleration*deltaF;

    generateViewModelMatrix(camera);
}

void Particle::draw(ParticleShader *shader){
    shader->loadViewModelMatrix(viewModelMatrix);
    glDrawArrays(GL_POINTS, 0, 1);
}

bool Particle::isAlive(){
    return aliveForInMs < liveForInS*1000 && position.y > 0 && position.y < 2000;
}

glm::vec3 Particle::addVelocity(glm::vec3 vel){
    velocity += vel;
}

glm::vec3 Particle::getPosition(){
    return position;
}

glm::mat4& Particle::getViewModelMatrix(){
    return viewModelMatrix;
}

Particle::~Particle()
{
}
