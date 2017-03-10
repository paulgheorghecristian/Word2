#include "puzzle_object.h"
#include "commons.h"

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
    glm::vec3 rotation = entities[0]->getRotation();
    scale = entities[0]->getScale();
    float mass;
    if (dynamic) {
        mass = 100.0f;
    } else {
        mass = 0.0f;
    }

    t.setIdentity();
    t.setOrigin(btVector3(position.x, position.y, position.z));
    t.setRotation(btQuaternion(rotation.y, rotation.x, rotation.z));
    //t.setFromOpenGLMatrix(glm::value_ptr(entities[0]->getModelMatrix()));

    //std::cout << glm::value_ptr(entities[0]->getModelMatrix())[12] << " " << glm::value_ptr(entities[0]->getModelMatrix())[13] << " " << glm::value_ptr(entities[0]->getModelMatrix())[14] << std::endl;

    glm::vec3 maxCoordinates = entities[0]->getMesh()->getMaxCoordinates();
    glm::vec3 minCoordinates = entities[0]->getMesh()->getMinCoordinates();

    btBoxShape* boxShape = new btBoxShape(btVector3(scale.x * (maxCoordinates.x-minCoordinates.x)/2.0f,
                                                    scale.y * (maxCoordinates.y-minCoordinates.y)/2.0f,
                                                    scale.z * (maxCoordinates.z-minCoordinates.z)/2.0f));
    if(mass != 0.0){
        boxShape->calculateLocalInertia(btScalar(mass), inertia);
    }
    btMotionState* motion = new btDefaultMotionState(t);
    btRigidBody::btRigidBodyConstructionInfo info(mass, motion, boxShape, inertia);
    info.m_restitution = 0.3;
    info.m_friction = 1.0f;

    m_body = new btRigidBody(info);
    m_body->setDamping(btScalar(0.2), btScalar(0.6));
    m_body->setSleepingThresholds(0.0, 0.0);
    m_body->setAngularFactor(0.0);
    UserPointer *userPointer = new UserPointer();
    userPointer->type = PUZZLE_OBJECT;
    userPointer->ptrType.puzzleObject = this;
    m_body->setUserPointer((void*)userPointer);

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

btRigidBody* PuzzleObject::getRigidBody(){
    return m_body;
}

PuzzleObject::~PuzzleObject()
{
    for(Entity* e : entities){
        delete e;
    }
}
