#include "player.h"

Player::Player(btDynamicsWorld* world, float mass, glm::vec3 position, glm::vec3 scale) : world(world), down(0, -1 * DOWN_LENGTH, 0), isTouchingGround(false), radius(scale.x*2.0f), force(800.0f)
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
    m_body->setCollisionFlags(m_body->getCollisionFlags() |
                              btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);

    UserPointer *userPointer = new UserPointer();
    userPointer->type = PLAYER;
    userPointer->ptrType.player = this;
    m_body->setUserPointer((void*)userPointer);

    world->addRigidBody(m_body);

    hasPickedUp = false;
}

void Player::performRayTest(){
    btVector3 playerVel = m_body->getLinearVelocity();
    glm::vec3 playerPosition = getPosition();
    float closestHitFraction = 1000.0f;
    float values[5][2]={{0, 0.6},
                      {0, -0.6},
                      {0.6, 0},
                      {-0.6, 0},
                      {0,0}
                     };

    for(int i = 0; i < 5; i++){
        float xPlayerPosition = playerPosition.x+radius*values[i][0];
        float zPlayerPosition = playerPosition.z+radius*values[i][1];
        btVector3 from = btVector3(xPlayerPosition, playerPosition.y, zPlayerPosition);
        btVector3 to = btVector3(xPlayerPosition, -10000, zPlayerPosition);

        btCollisionWorld::ClosestRayResultCallback rayCallback(from, to);
        world->rayTest(from, to, rayCallback);
        if(rayCallback.hasHit()){
            UserPointer *p = (UserPointer*)rayCallback.m_collisionObject->getUserPointer();
            if (p->type == PICKABLE_OBJECT && p->ptrType.pickableObject->getIsPickedUp()){
                continue;
            }
            if(rayCallback.m_closestHitFraction < closestHitFraction){
                closestHitFraction = rayCallback.m_closestHitFraction;
            }
        }
    }

    if (playerVel.y() < -0.1) {
        m_body->setDamping(btScalar(0.1), btScalar(0));
        force = 200;
    }

    if(closestHitFraction < EPS && playerVel.y() <= 0){
        isJumping = false;
        isTouchingGround = true;
        m_body->setDamping(btScalar(0.5), btScalar(0));
        force = 800;
    }else{
        isTouchingGround = false;
    }
}

void Player::jump(){
    isJumping = true;
    m_body->applyCentralForce(btVector3(0, JUMP_SPEED, 0));
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

bool Player::getIsTouchingGround(){
    return isTouchingGround;
}

bool Player::getIsJumping(){
    return isJumping;
}

Player::~Player()
{
    delete m_body;
}
