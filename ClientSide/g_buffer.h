#ifndef GBUFFER_H
#define GBUFFER_H

#include <GL/glew.h>
#include <vector>
#include <iostream>

class GBuffer
{
    public:
        GBuffer(unsigned int, unsigned int);
        void generate(unsigned int, unsigned int);
        void destroy();
        GLuint getColorTexture();
        GLuint getPositionTexture();
        GLuint getNormalTexture();
        GLuint getLightAccumulationTexture();
        GLuint getDepthTexture();
        GLuint getFrameBufferObject();
        void bindForScene();
        void bindForLights();
        void bindForStencil();
        void unbind();
        unsigned int getWidth();
        unsigned int getHeight();
        virtual ~GBuffer();
    protected:
    private:
        unsigned int width, height;
        GLuint framebuffer_object;
		GLuint texture_normal;
		GLuint texture_color;
		GLuint texture_light_accumulation;
		GLuint texture_depth;
};

#endif // GBUFFER_H
