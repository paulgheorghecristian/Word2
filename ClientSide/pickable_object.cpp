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
    player->getRigidBody()->setCollisionFlags(m_body->getCollisionFlags() |
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
        glm::vec3 position = player->getPosition() + glm::vec3(0,20,0) + camera->getForward() * 50.0f;
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
