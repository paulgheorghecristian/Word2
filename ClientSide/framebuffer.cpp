#include "framebuffer.h"

Framebuffer::Framebuffer(int width, int height) : width(width), height(height), TEXTURE_WIDTH(1080), TEXTURE_HEIGHT(720)
{
    //generare framebuffer
    glGenFramebuffers(1, &frameBufferId);
    glBindFramebuffer(GL_FRAMEBUFFER, frameBufferId);
    //selecteaza attachment-ul pentru desenare color buffer
    glDrawBuffer(GL_COLOR_ATTACHMENT0);

    //genereaza textura pe care desenez scena
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);

    //alocare memorie pentru textura
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, TEXTURE_WIDTH, TEXTURE_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, (GLvoid *)0);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, textureId, 0);

    //ataseaza un depth buffer
    glGenRenderbuffers(1, &depthBufferId);
    glBindRenderbuffer(GL_RENDERBUFFER, depthBufferId);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBufferId);
}

void Framebuffer::bind(){
    //bind-eaza acest framebuffer si deseneaza pe el, nu pe cel default
    glBindFramebuffer(GL_FRAMEBUFFER, frameBufferId);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBindTexture(GL_TEXTURE_2D, 0);
    glViewport(0, 0, width, height);
}

void Framebuffer::unbind(){
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

GLuint Framebuffer::getTextureId(){
    return textureId;
}

Framebuffer::~Framebuffer()
{

}
