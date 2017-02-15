#include "box.h"

Mesh* Box::mesh;

Box::Box(btDynamicsWorld* world,
         float mass,
         glm::vec4 color,
         glm::vec3 position,
         glm::vec3 rotation,
         glm::vec3 scale,
         Texture* texture) :  Entity(world,
                                    "box",
                                    Box::mesh,
                                    color,
                                    position,
                                    rotation,
                                    scale,
                                    texture)
{
    btTransform t;
    btVector3 inertia(0, 0, 0);

    t.setIdentity();
    t.setOrigin(btVector3(position.x, position.y, position.z));

    btBoxShape* boxShape = new btBoxShape(btVector3(scale.x/2.0, scale.y/2.0, scale.z/2.0));
    if(mass != 0.0){
        boxShape->calculateLocalInertia(btScalar(mass), inertia);
    }
    btMotionState* motion = new btDefaultMotionState(t);
    btRigidBody::btRigidBodyConstructionInfo info(mass, motion, boxShape, inertia);
    info.m_restitution = 1;
    info.m_friction = 1.0f;

    m_body = new btRigidBody(info);
    m_body->setDamping(btScalar(0.2), btScalar(0.6));

    world->addRigidBody(m_body);
}

void Box::draw(GeneralShader *shader){
    btTransform t;

    m_body->getMotionState()->getWorldTransform(t);
    float mat[16];
    t.getOpenGLMatrix(mat);

    this->setModelMatrix(glm::scale(glm::make_mat4(mat), scale));
    Entity::draw(shader);
}

void Box::setLinearVelocity(glm::vec3 vel){
    m_body->setLinearVelocity(btVector3(vel.x, vel.y, vel.z));
}

void Box::setMesh(Mesh *mesh){
    Box::mesh = mesh;
}

Box::~Box()
{
    //delete m_body;
}
