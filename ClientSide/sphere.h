#ifndef SPHERE_H
#define SPHERE_H

#include "entity.h"
#include <bullet/btBulletDynamicsCommon.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Sphere : public Entity
{
    public:
        Sphere(btDynamicsWorld*, float, glm::vec4, glm::vec3, glm::vec3, float);
        void draw(Shader *);
        static void set_mesh(Mesh *);
        void set_linear_velocity(glm::vec3);
        virtual ~Sphere();
    protected:
    private:
        static Mesh* mesh;
        btDynamicsWorld* world;
        btRigidBody *m_body;
        float m_radius;

};

#endif // SPHERE_H
