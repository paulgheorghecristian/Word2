#include "texture.h"

Texture::Texture(std::string textureFilename, int textureUnit) : Texture(textureFilename, textureUnit, false, GL_BGR){}

Texture::Texture(std::string textureFilename, int textureUnit, bool hasAlpha, GLenum format) : textureUnit(textureUnit), hasAlpha(hasAlpha)
{
    SDL_Surface* texture = SDL_LoadBMP(textureFilename.c_str());

    if(!texture){
        std::cerr << "Loading texture error! " << textureFilename << std::endl;
        exit(1);
    }

    //genereaza textura
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture->w, texture->h, 0, format, GL_UNSIGNED_BYTE, texture->pixels);

    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, -0.7f);

    glBindTexture(GL_TEXTURE_2D, 0);

    SDL_FreeSurface(texture);
}

Texture::Texture(std::string textureFilename, int textureUnit, bool hasAlpha) : Texture(textureFilename, textureUnit, hasAlpha, GL_BGR){

}

Texture::Texture(GLuint textureId, int textureUnit) : textureId(textureId), textureUnit(textureUnit)
{

}

Texture::~Texture()
{
    glBindTexture(GL_TEXTURE_2D, 0);
    glDeleteTextures(1, &textureId);
}

bool Texture::getHasAlpha(){
    return hasAlpha;
}

GLuint Texture::getTextureId(){
    return textureId;
}

int Texture::getTextureUnit(){
    return textureUnit;
}

void Texture::use(){
    glActiveTexture(GL_TEXTURE0 + textureUnit);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureId);
}
