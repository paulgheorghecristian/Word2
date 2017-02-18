#ifndef POSTPROCESS_H
#define POSTPROCESS_H

#include "framebuffer.h"
#include "simple_shader.h"
#include "mesh.h"
#include <string>
#include "texture.h"

class PostProcess
{
    public:
        PostProcess(float width, float height, const std::string&, const std::string&);
        void bind();
        void process();
        GLuint getResultingTextureId();
        SimpleShader& getShader();
        GLuint getFrameBufferObject();
        virtual ~PostProcess();
    protected:
    private:
        Framebuffer fb;
        float height, width;
        SimpleShader processShader;
        Mesh* renderingQuad;
        Texture* inputTexture;
};

#endif // POSTPROCESS_H
