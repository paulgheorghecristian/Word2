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
        Light(GBuffer*, Mesh*, glm::vec3, glm::vec3, float);
        void draw(GeneralShader*);
        virtual ~Light();
    protected:
    private:
        Mesh* mesh;
        glm::vec3 position, color;
        GBuffer* gBuffer;
        float radius;
};

#endif // LIGHT_H
