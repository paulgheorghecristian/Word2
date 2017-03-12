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
    userPointer = new UserPointer();
    userPointer->type = PICKABLE_OBJECT;
    userPointer->ptrType.pickableObject = this;
    m_body->setUserPointer((void*)userPointer);
    isTouched = false;
    isPickedUp = false;
    isColliding = false;
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
    player->getRigidBody()->setCollisionFlags(player->getRigidBody()->getCollisionFlags() |
                                              btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
}

void PickableObject::update(){
    if (isColliding) {
        for (Entity *e : entities){
            e->setColor(glm::vec4(1,0,0,1));
        }
    }

    if (!isPickedUp) {
        PuzzleObject::update();
    } else {
        m_body->clearForces();
        m_body->setLinearVelocity(btVector3(0,0,0));
        m_body->setAngularVelocity(btVector3(0,0,0));
        glm::vec3 dPosition = player->getPosition() + glm::vec3(0,20,0) + camera->getForward() * 50.0f;
        btTransform transform;
        m_body->getMotionState()->getWorldTransform(transform);
        transform.setOrigin(btVector3(dPosition.x, dPosition.y, dPosition.z));
        m_body->setCenterOfMassTransform(transform);

        entities[0]->setPosition(dPosition);
        entitiesRelationshipFunction(this);
    }
}

PickableObject::~PickableObject()
{
    //dtor
}
