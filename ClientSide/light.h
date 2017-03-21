#ifndef LIGHT_H
#define LIGHT_H

#include "mesh.h"
#include <glm/glm.hpp>
#include "general_shader.h"
#include "simple_shader.h"
#include <typeinfo>
#include <glm/gtc/matrix_transform.hpp>
#include "g_buffer.h"

class Light
{
    public:
        Light(glm::vec3, glm::vec3, float);
        void draw(GeneralShader*);
        void setRenderIt(bool);
        bool getRenderIt();
        void setPosition(glm::vec3 position);
        glm::vec3 getPosition();
        float getRadius();
        static void setMesh(Mesh*);
        static void setGBuffer(GBuffer* buffer);
        virtual ~Light();
    protected:
    private:
        static Mesh* mesh;
        static GBuffer* gBuffer;

        glm::vec3 position, color;
        float radius;
        bool renderIt;
};

#endif // LIGHT_H
