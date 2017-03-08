#include "sphere.h"

Mesh* Sphere::mesh;

Sphere::Sphere(btDynamicsWorld* world,
               float mass,
               glm::vec4 color,
               glm::vec3 position,
               glm::vec3 rotation,
               float radius,
               Texture* texture) : m_radius(radius),
                                    Entity(world, "sphere", Sphere::mesh, color, position, rotation, glm::vec3(radius), texture)
{
    btTransform t;
    btVector3 inertia(0, 0, 0);

    t.setIdentity();
    t.setOrigin(btVector3(position.x, position.y, position.z));

    btSphereShape* sphereShape = new btSphereShape(radius/2.0);
    if(mass != 0.0){
        sphereShape->calculateLocalInertia(btScalar(mass), inertia);
    }
    btMotionState* motion = new btDefaultMotionState(t);
    btRigidBody::btRigidBodyConstructionInfo info(mass, motion, sphereShape, inertia);
    info.m_restitution = 0.2f;
    info.m_friction = 0.3f;

    m_body = new btRigidBody(info);
    UserPointer *userPointer = new UserPointer();
    userPointer->type = SPHERE;
    userPointer->ptrType.sphere = this;
    m_body->setUserPointer((void*)userPointer);

    m_body->setDamping(btScalar(0), btScalar(0.3));
    world->addRigidBody(m_body);
}

void Sphere::draw(GeneralShader *shader){
    btTransform t;

    m_body->getMotionState()->getWorldTransform(t);
    float mat[16];
    t.getOpenGLMatrix(mat);

    this->setModelMatrix(glm::scale(glm::make_mat4(mat), glm::vec3(m_radius)));
    Entity::draw(shader);
}

void Sphere::setMesh(Mesh *mesh){
    Sphere::mesh = mesh;
}

void Sphere::setLinearVelocity(glm::vec3 vel){
    m_body->setLinearVelocity(btVector3(vel.x, vel.y, vel.z));
}

Sphere::~Sphere()
{
    //delete m_body;
}
