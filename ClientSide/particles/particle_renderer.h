#ifndef PARTICLERENDERER_H
#define PARTICLERENDERER_H

#include "particle_shader.h"
#include <vector>
#include "../camera.h"
#include "particle.h"
#include <algorithm>
#include <stdlib.h>
#include <time.h>
#include "../texture.h"

class ParticleRenderer
{
    public:
        ParticleRenderer(const glm::mat4& projectionMatrix,
                         const glm::vec3&,
                           unsigned int numOfParticles);
        void update(long, Camera *);
        void draw();
        virtual ~ParticleRenderer();
    protected:
    private:
        void createVao();

        glm::mat4 projectionMatrix;
        ParticleShader *shader;
        unsigned int numOfParticles;
        std::vector<Particle*> particles;
        glm::vec3 centerPosition;

        GLuint vaoHandle, vboHandle;
        Texture particleTexture;
};

#endif // PARTICLERENDERER_H
