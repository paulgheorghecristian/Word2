#include "pickable_object.h"

Player* PickableObject::player;
Camera* PickableObject::camera;

PickableObject::PickableObject(btDynamicsWorld* world,
                               const std::string& name,
                               const std::vector<Entity*>& entities,
                               const std::function<void(PuzzleObject*)>& entitiesRelationshipFunction,
                               const std::function<void(PuzzleObject*, unsigned int)>& actionFunction) :
                                PuzzleObject(world, name, entities, entitiesRelationshipFunction, actionFunction, true)
{
    btTransform t;
    btVector3 inertia(0, 0, 0);
    glm::vec3 mainPosition = entities[0]->getPosition();

    t.setIdentity();
    t.setOrigin(btVector3(mainPosition.x, mainPosition.y, mainPosition.z));

    btSphereShape* sphereShape = new btSphereShape(PICK_RADIUS);
    sphereShape->calculateLocalInertia(btScalar(0.0f), inertia);
    btMotionState* motion = new btDefaultMotionState(t);
    btRigidBody::btRigidBodyConstructionInfo info(0.0f, motion, sphereShape, inertia);

    interactionSphereBody = new btRigidBody(info);
    interactionSphereBody->setCollisionFlags(interactionSphereBody->getCollisionFlags() |
                                              btCollisionObject::CF_NO_CONTACT_RESPONSE);
    world->addRigidBody(interactionSphereBody);
}

void PickableObject::pickUp(){
    isPickedUp = true;
    m_body->setCollisionFlags(m_body->getCollisionFlags() |
                              btCollisionObject::CF_NO_CONTACT_RESPONSE |
                              btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
    player->getRigidBody()->setCollisionFlags(player->getRigidBody()->getCollisionFlags() &
                                              ~btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);

}

void PickableObject::release(){
    isPickedUp = false;

    m_body->setCollisionFlags(m_body->getCollisionFlags() &
                              ~btCollisionObject::CF_NO_CONTACT_RESPONSE &
                              ~btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
    player->getRigidBody()->setCollisionFlags(m_body->getCollisionFlags() |
                                              btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
}

void PickableObject::update(){
    if (!isPickedUp) {
        PuzzleObject::update();
        btTransform transform = interactionSphereBody->getCenterOfMassTransform();
        glm::vec3 mainPosition = entities[0]->getPosition();
        transform.setOrigin(btVector3(mainPosition.x, mainPosition.y, mainPosition.z));
        interactionSphereBody->setCenterOfMassTransform(transform);
    } else {
        glm::vec3 position = player->getPosition() + camera->getForward() * 70.0f;
        btTransform transform = m_body->getCenterOfMassTransform();
        transform.setOrigin(btVector3(position.x, position.y, position.z));
        m_body->setCenterOfMassTransform(transform);

        entities[0]->setPosition(position);
        entitiesRelationshipFunction(this);
    }
}

PickableObject::~PickableObject()
{
    //dtor
}
