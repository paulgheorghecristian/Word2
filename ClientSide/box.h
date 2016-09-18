#ifndef BOX_H
#define BOX_H

#include "entity.h"

class Box : public Entity
{
    public:
        Box(btDynamicsWorld*, float, glm::vec4, glm::vec3, glm::vec3, glm::vec3, Texture*);
        static void set_mesh(Mesh *);
        void draw(Shader *);
        void set_linear_velocity(glm::vec3);
        ~Box();
    protected:
    private:
        static Mesh* mesh;
};

#endif // BOX_H
