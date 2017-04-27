#ifndef WATERRENDERER_H
#define WATERRENDERER_H

#include "../texture.h"
#include "../mesh.h"
#include "../framebuffer.h"
#include "../general_shader.h"
#include "../display.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class WaterRenderer
{
    public:
        WaterRenderer(int width, int height, const glm::vec3& position,
                     const glm::vec3& scale, const glm::mat4& projectionMatrix);
        void bindForReflectionTexture();
        void bindForRefractionTexture();
        void unbind();
        float getWaterHeight();
        void draw(const glm::mat4& viewMatrix);
        virtual ~WaterRenderer();
    protected:
    private:
        Mesh *waterMesh;
        Texture *refractionTexture, *reflectionTexture, *DuDvTexture, *normalMapTexture, *depthMapTexture;
        float dudvOffset;
        Framebuffer *waterFramebuffer;
        GeneralShader *waterShader;
        float waterHeight;

        GLuint dudvOffsetUniformLocation;
        const float WAVE_SPEED = 0.03f;
};

#endif // WATERRENDERER_H
