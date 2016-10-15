#ifndef SPHERE_H
#define SPHERE_H

#include "entity.h"

class Sphere : public Entity
{
    public:
        Sphere(btDynamicsWorld*, float, glm::vec4, glm::vec3, glm::vec3, float, Texture*);
        void draw(Shader *);
        static void set_mesh(Mesh *);
        void set_linear_velocity(glm::vec3);
        ~Sphere();
    protected:
    private:
        static Mesh* mesh;
        float m_radius;

};

#endif // SPHERE_H
