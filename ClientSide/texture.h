#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <iostream>

class Texture
{
    public:
        Texture(std::string, int);
        Texture(std::string, int, bool);
        Texture(GLuint, int);
        Texture(std::string, int, bool, GLenum);
        GLuint getTextureId();
        int getTextureUnit();
        bool getHasAlpha();
        void use();
        virtual ~Texture();
    protected:
    private:
        GLuint textureId;
        int textureUnit;
        bool hasAlpha;
};

#endif // TEXTURE_H
