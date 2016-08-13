#include "texture.h"

Texture::Texture(std::string texture_name)
{
    SDL_Surface *texture = SDL_LoadBMP(texture_name.c_str());

    if(!texture){
        std::cerr << "Loading texture error!" << std::endl;
        exit(1);
    }

		//genereaza textura
    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture->w, texture->h, 0, GL_BGR, GL_UNSIGNED_BYTE, texture->pixels);

    SDL_FreeSurface(texture);
}

Texture::Texture(GLuint texture_id) : texture_id(texture_id){

}

Texture::~Texture()
{
    glBindTexture(GL_TEXTURE_2D, 0);
    glDeleteTextures(1, &texture_id);
}

GLuint Texture::get_texture_id(){
    return texture_id;
}

void Texture::use(unsigned int n){
    glActiveTexture(GL_TEXTURE0 + n);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture_id);
}
