#include "networked_player.h"

Mesh *NetworkedPlayer::mesh;

NetworkedPlayer::NetworkedPlayer(btDynamicsWorld* world,
                                 float mass,
                                 glm::vec3 position,
                                 glm::vec3 forward) : Entity(world,
                                                            "networkedplayer",
                                                            NetworkedPlayer::mesh,
                                                            glm::vec4(1,0,0,1),
                                                            position,
                                                            glm::vec3(0),
                                                            glm::vec3(20),
                                                            NULL), forward(forward)
{
    btTransform t;
    btVector3 inertia(0, 0, 0);

    t.setIdentity();
    t.setOrigin(btVector3(position.x, position.y, position.z));

    btCapsuleShape* capsuleShape = new btCapsuleShape(scale.x, scale.y);

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

    UserPointer *userPointer = new UserPointer();
    userPointer->type = NETWORKED_PLAYER;
    userPointer->ptrType.networkedPlayer = this;
    m_body->setUserPointer((void*)userPointer);
    world->addRigidBody(m_body);
}

void NetworkedPlayer::setPosition(float x, float y, float z) {
    btTransform transform = m_body->getCenterOfMassTransform();
    transform.setOrigin(btVector3(x, y, z));
    m_body->setCenterOfMassTransform(transform);
    Entity::setPosition(x, y, z);
}

void NetworkedPlayer::setMesh(Mesh *mesh) {
    NetworkedPlayer::mesh = mesh;
}

NetworkedPlayer::~NetworkedPlayer()
{
    delete m_body->getUserPointer();
}
