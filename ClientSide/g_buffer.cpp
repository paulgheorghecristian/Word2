#include "g_buffer.h"

GBuffer::GBuffer(unsigned int width, unsigned int height) : width(width), height(height)
{
    generate(width, height);
}

void GBuffer::generate(unsigned int width, unsigned int height){

    //genereaza un obiect de tip framebuffer si apoi leaga-l la pipeline
    glGenFramebuffers(1, &framebuffer_object);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_object);

    //genereaza textura de culoare, format RGB8 (3 canale unsigned char), FARA date, fara filtrare
    glGenTextures(1, &texture_color);
    glBindTexture(GL_TEXTURE_2D, texture_color);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

    //genereaza textura de culoare, format RGB32F (3 canale float), FARA date, fara filtrare
    glGenTextures(1, &texture_normal);
    glBindTexture(GL_TEXTURE_2D, texture_normal);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB10_A2, width, height, 0, GL_RGB, GL_FLOAT, 0);

    //genereaza textura de culoare, format RGB8 (3 canale float), FARA date, fara filtrare
    glGenTextures(1, &texture_light_accumulation);
    glBindTexture(GL_TEXTURE_2D, texture_light_accumulation);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

    //genereaza textura de adancime, format DEPTH (un singur canal), FARA date, fara filtrare
    glGenTextures(1, &texture_depth);
    glBindTexture(GL_TEXTURE_2D, texture_depth);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, 0);

    //leaga texturi la framebuffer , 0 de la sfarsit se refera la ce nivel din mipmap, 0 fiind cel mai de sus/mare.
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+0, texture_color,0);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+1, texture_normal,0);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+2, texture_light_accumulation,0);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, texture_depth,0);

    //verifica stare
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER)!=GL_FRAMEBUFFER_COMPLETE){
        std::cout<<"EROARE!!! Framebuffer-ul nu este complet. Apasati orice tasta pentru a inchide programul."<<std::endl;
        std::cin.get();
        std::terminate();
    }

    //nu sunt legat la pipeline
    glBindFramebuffer(GL_FRAMEBUFFER,0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void GBuffer::destroy(){
    glDeleteFramebuffers(1, &framebuffer_object);
    glDeleteTextures(1, &texture_normal);
    glDeleteTextures(1, &texture_color);
    glDeleteTextures(1, &texture_light_accumulation);
    glDeleteTextures(1, &texture_depth);
}

GLuint GBuffer::getColorTexture(){
    return texture_color;
}

GLuint GBuffer::getNormalTexture(){
    return texture_normal;
}

GLuint GBuffer::getLightAccumulationTexture(){
    return texture_light_accumulation;
}

GLuint GBuffer::getDepthTexture(){
    return texture_depth;
}

void GBuffer::bindForScene(){
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_object);
    glViewport(0.0f, 0.0f, width, height);
    GLenum buffersGeometry[]= {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
    glDrawBuffers(2, buffersGeometry);
}

void GBuffer::bindForLights(){
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_object);
    GLenum buffersGeometry[]= {GL_COLOR_ATTACHMENT2};
    glDrawBuffers(1,buffersGeometry);
}

void GBuffer::bindForStencil(){
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_object);
    glDrawBuffer(GL_NONE);
}

void GBuffer::unbind(){
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

unsigned int GBuffer::getWidth(){
    return width;
}

unsigned int GBuffer::getHeight(){
    return height;
}

GLuint GBuffer::getFrameBufferObject(){
    return framebuffer_object;
}

GBuffer::~GBuffer()
{
    destroy();
}
