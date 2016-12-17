#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <GL/glew.h>
#include <iostream>

class Framebuffer
{
    public:
        Framebuffer(int, int);
        void bind();
        void unbind();
        GLuint getTextureId();
        virtual ~Framebuffer();
    protected:
    private:
        int width, height;
        GLuint frameBufferId;
        GLuint depthBufferId;
        GLuint textureId;
        const int TEXTURE_WIDTH, TEXTURE_HEIGHT;
};

#endif // FRAMEBUFFER_H
