#ifndef BOX_H
#define BOX_H

#include "entity.h"
#include "commons.h"

class Box : public Entity
{
    public:
        Box(btDynamicsWorld*, float, glm::vec4, glm::vec3, glm::vec3, glm::vec3, Texture* texture);
        static void setMesh(Mesh*);
        void draw(GeneralShader*);
        void setLinearVelocity(glm::vec3);
        ~Box();
    protected:
    private:
        static Mesh* mesh;
};

#endif // BOX_H
