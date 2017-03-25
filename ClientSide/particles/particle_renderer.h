#ifndef PARTICLERENDERER_H
#define PARTICLERENDERER_H

#include "particle_shader.h"
#include <vector>
#include "../camera.h"
#include "particle.h"
#include <algorithm>
#include <stdlib.h>
#include <time.h>
#include "../entity.h"
#include "../puzzle_object.h"
#include "../texture.h"
#include "../math_utils.h"
#include "../game.h"

class ParticleRenderer
{
    public:
        ParticleRenderer(const glm::mat4& projectionMatrix,
                         const glm::vec3&,
                         unsigned int numOfParticles);
        void update(Camera *, Entity *, const std::vector<PuzzleObject*>&, Entity *goal);
        void draw();
        std::vector<Particle*>& getParticles();
        virtual ~ParticleRenderer();
    protected:
    private:
        void createVao();
        void createEmptyVbo();
        void updateVbo();
        void insertMatrixInBuffer(glm::mat4&, int&);

        glm::mat4 projectionMatrix;
        ParticleShader *shader;
        unsigned int numOfParticles;
        std::vector<Particle*> particles;
        glm::vec3 centerPosition;
        const int NUM_OF_BYTES_PER_INSTANCE = 16*4;
        float *matricesBuffer;

        GLuint vaoHandle, vboHandle;
        Texture particleTexture;
};

#endif // PARTICLERENDERER_H
