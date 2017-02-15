#ifndef PARTICLE_H
#define PARTICLE_H

#include <glm/glm.hpp>
#include "../camera.h"
#include "particle_shader.h"
#define GRAVITYP 0.5

class Particle
{
    public:
        Particle(glm::vec3, glm::vec3, glm::vec3, float, unsigned int);
        Particle(glm::vec3, glm::vec3, float, unsigned int);
        void generateViewModelMatrix(Camera *);
        bool isAlive();
        void update(long, Camera *);
        void draw(ParticleShader *);
        void reset(glm::vec3, glm::vec3);
        glm::vec3 getPosition();
        glm::vec3 addVelocity(glm::vec3);
        virtual ~Particle();
    protected:
    private:
        glm::vec3 position, velocity, acceleration, initialPosition, initialVelocity;
        float scale;
        glm::mat4 viewModelMatrix;
        unsigned int liveForInS;
        long aliveForInMs;
};

#endif // PARTICLE_H
