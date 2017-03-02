#include "puzzle_object.h"

PuzzleObject::PuzzleObject(btDynamicsWorld* world,
                           const std::string& name,
                           const std::vector<Entity*>& entities,
                           const std::function<void(PuzzleObject*)>& entitiesRelationshipFunction,
                           const std::function<void(PuzzleObject*, unsigned int)>& actionFunction, bool dynamic) : name(name),
                                                                                        entities(entities),
                                                                                        entitiesRelationshipFunction(entitiesRelationshipFunction),
                                                                                        actionFunction(actionFunction)
{
    btTransform t;
    btVector3 inertia(0, 0, 0);

    glm::vec3 position = entities[0]->getPosition();
    scale = entities[0]->getScale();
    float mass;
    if (dynamic) {
        mass = 20.0f;
    } else {
        mass = 0.0f;
    }

    t.setIdentity();
    t.setOrigin(btVector3(position.x, position.y, position.z));

    btBoxShape* boxShape = new btBoxShape(btVector3(scale.x/1.5f, scale.y/1.5f, scale.z/1.5f));
    if(mass != 0.0){
        boxShape->calculateLocalInertia(btScalar(mass), inertia);
    }
    btMotionState* motion = new btDefaultMotionState(t);
    btRigidBody::btRigidBodyConstructionInfo info(mass, motion, boxShape, inertia);
    info.m_restitution = 1;
    info.m_friction = 1.0f;

    m_body = new btRigidBody(info);
    m_body->setDamping(btScalar(0.2), btScalar(0.6));
    m_body->setSleepingThresholds(0.0, 0.0);
    m_body->setAngularFactor(0.0);

    world->addRigidBody(m_body);
}

PuzzleObject::PuzzleObject(btDynamicsWorld* world,
                           const std::string& name,
                           const std::vector<Entity*>& entities,
                           const std::function<void(PuzzleObject*)>& entitiesRelationshipFunction,
                           bool dynamic) : PuzzleObject(world,
                                              name,
                                              entities,
                                              entitiesRelationshipFunction,
                                              [](PuzzleObject* obj, int type){
                                                    return;
                                              },
                                              dynamic
                                              )
{
}

void PuzzleObject::executeAction(unsigned int type){
    actionFunction(this, type);
}

void PuzzleObject::update(){
    btTransform t;

    m_body->getMotionState()->getWorldTransform(t);
    float mat[16];
    t.getOpenGLMatrix(mat);
    modelMatrix = glm::mat4(glm::scale(glm::make_mat4(mat), entities[0]->getScale()));

    entities[0]->setPosition(mat[12], mat[13], mat[14]);
    entitiesRelationshipFunction(this);
}

void PuzzleObject::draw(GeneralShader* shader){
    for(Entity* e : entities){
        e->draw(shader);
    }
}

std::vector<Entity*>& PuzzleObject::getEntities(){
    return entities;
}

glm::mat4& PuzzleObject::getModelMatrix(){
    return modelMatrix;
}

PuzzleObject::~PuzzleObject()
{
    for(Entity* e : entities){
        delete e;
    }
}
