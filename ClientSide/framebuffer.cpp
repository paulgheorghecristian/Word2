#include "framebuffer.h"

Framebuffer::Framebuffer(float width, float height, unsigned int numOfRenderTargets) : width(width), height(height), numOfRenderTargets(numOfRenderTargets), renderTargets(numOfRenderTargets)
{
    //generare framebuffer
    glGenFramebuffers(1, &frameBufferId);
    glBindFramebuffer(GL_FRAMEBUFFER, frameBufferId);

    glGenTextures(numOfRenderTargets, &renderTargets[0]);
    for(unsigned int i = 0; i < numOfRenderTargets; i++){
        glBindTexture(GL_TEXTURE_2D, renderTargets[i]);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
    }

    //selecteaza attachment-ul pentru desenare color buffer
    for(unsigned int i = 0; i < numOfRenderTargets; i++){
        glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, renderTargets[i], 0);
    }

    glGenRenderbuffers(1, &depthBufferId);
    glBindRenderbuffer(GL_RENDERBUFFER, depthBufferId);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_STENCIL, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBufferId);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
        std::cerr << "Framebuffer isn't complete!" << glCheckFramebufferStatus(GL_FRAMEBUFFER) << std::endl;
        exit(1);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Framebuffer::bindSingleRenderTarget(unsigned int index){
    if(index >= numOfRenderTargets){
        bindAllRenderTargets();
        return;
    }
    glGetIntegerv(GL_VIEWPORT, previousViewport);
    glBindFramebuffer(GL_FRAMEBUFFER, frameBufferId);
    GLenum buffer[]= {GL_COLOR_ATTACHMENT0 + index};
    glDrawBuffers(1, buffer);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBindTexture(GL_TEXTURE_2D, 0);
    glViewport(0, 0, width, height);
}

void Framebuffer::bindAllRenderTargets(){
    //bind-eaza acest framebuffer si deseneaza pe el, nu pe cel default
    glGetIntegerv(GL_VIEWPORT, previousViewport);
    glBindFramebuffer(GL_FRAMEBUFFER, frameBufferId);
    glDrawBuffers(renderTargets.size(), &renderTargets[0]);
    glClearColor(0, 0, 0, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBindTexture(GL_TEXTURE_2D, 0);
    glViewport(0, 0, width, height);
}

void Framebuffer::unbind(){
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glViewport(previousViewport[0], previousViewport[1], previousViewport[2], previousViewport[3]);
}

std::vector<GLuint>& Framebuffer::getRenderTargets(){
    return renderTargets;
}

GLuint Framebuffer::getFrameBufferObject(){
    return frameBufferId;
}

Framebuffer::~Framebuffer()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDeleteFramebuffers(1, &frameBufferId);
}
