#ifndef DIRECTIONALLIGHT_H
#define DIRECTIONALLIGHT_H

#include "mesh.h"
#include <glm/glm.hpp>
#include "general_shader.h"
#include "simple_shader.h"
#include <typeinfo>
#include <glm/gtc/matrix_transform.hpp>
#include "g_buffer.h"

class DirectionalLight
{
    public:
        DirectionalLight(GBuffer*, glm::vec3, glm::vec3);
        void draw(GeneralShader*);
        glm::vec3 getDirection();
        glm::vec3 getColor();
        static void setMesh(Mesh*);
        virtual ~DirectionalLight();
    protected:
    private:
        static Mesh* mesh;
        glm::vec3 direction, color;
        GBuffer* gBuffer;
};

#endif // DIRECTIONALLIGHT_H
