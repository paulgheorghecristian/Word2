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
        PostProcess(float, float, const std::string&, const std::string&);
        PostProcess(float, float, GLuint, const std::string&, const std::string&);
        PostProcess(float,
                     float,
                     const std::string&,
                     const std::string&,
                     std::vector<std::string>&);
        PostProcess (float,
                     float,
                     GLuint,
                     const std::string&,
                     const std::string&,
                     std::vector<std::string>&);
        void bind();
        void process();
        GLuint getResultingTextureId();
        SimpleShader& getShader();
        GLuint getFrameBufferObject();
        virtual ~PostProcess();
    protected:
    private:
        void constructTextures(std::vector<std::string>&);

        Framebuffer fb;
        float width, height;
        SimpleShader processShader;
        Mesh* renderingQuad;
        Texture* inputTexture;
        std::vector<Texture*> additionalTextures;
};

#endif // POSTPROCESS_H
