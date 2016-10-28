#include "player.h"

Player::Player(btDynamicsWorld* world, float mass, glm::vec3 position, glm::vec3 scale)
{
    btTransform t;
    btVector3 inertia(0, 0, 0);

    t.setIdentity();
    t.setOrigin(btVector3(position.x, position.y, position.z));

    btCapsuleShape* capsuleShape = new btCapsuleShape(scale.x*2.0f, scale.y*2.0f);

    if(mass != 0.0){
        capsuleShape->calculateLocalInertia(btScalar(mass), inertia);
    }
    btMotionState* motion = new btDefaultMotionState(t);
    btRigidBody::btRigidBodyConstructionInfo info(mass, motion, capsuleShape, inertia);
    info.m_restitution = 0.5f;
    info.m_friction = 0.3f;

    m_body = new btRigidBody(info);

    m_body->setDamping(btScalar(0.5), btScalar(1.0));
    m_body->setSleepingThresholds(0.0, 0.0);
    m_body->setAngularFactor(0.0);

    world->addRigidBody(m_body);
}

glm::vec3 Player::getPosition(){
    btTransform t;

    m_body->getMotionState()->getWorldTransform(t);
    float mat[16];
    t.getOpenGLMatrix(mat);

    return glm::vec3(mat[12], mat[13], mat[14]);
}

btRigidBody* Player::getRigidBody(){
    return m_body;
}

Player::~Player()
{
    delete m_body;
}
