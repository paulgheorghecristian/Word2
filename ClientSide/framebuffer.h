#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <vector>
#include <GL/glew.h>
#include <iostream>

class Framebuffer
{
    public:
        Framebuffer(float, float, unsigned int);
        void bindAllRenderTargets();
        void bindSingleRenderTarget(unsigned int);
        void unbind();
        std::vector<GLuint>& getRenderTargets();
        GLuint getDepthTextureId();
        GLuint getFrameBufferObject();
        virtual ~Framebuffer();
    protected:
    private:
        float width, height;
        GLuint frameBufferId, depthBufferId, colorBufferId;
        GLint previousViewport[4];
        std::vector<GLuint> renderTargets;
        unsigned int numOfRenderTargets;
};

#endif // FRAMEBUFFER_H
