#ifndef PLAYER_H
#define PLAYER_H

#include <bullet/btBulletDynamicsCommon.h>
#include <glm/glm.hpp>
#include <iostream>
#include "commons.h"

#define DOWN_LENGTH 1000
#define JUMP_SPEED 100000
#define EPS 0.003

class Player
{
    public:
        Player(btDynamicsWorld*, float, glm::vec3, glm::vec3);
        glm::vec3 getPosition();
        btRigidBody* getRigidBody();
        void performRayTest();
        bool getIsTouchingGround();
        bool getIsJumping();
        bool getHasPickedUp(){
            return hasPickedUp;
        }
        void setHasPickedUp(bool hasPickedUp){
            this->hasPickedUp = hasPickedUp;
        }
        float getForce() {
            return force;
        }
        void jump();
        virtual ~Player();
    protected:
    private:
        btDynamicsWorld* world;
        btRigidBody* m_body;
        bool isTouchingGround;
        bool isJumping;
        bool hasPickedUp;
        const btVector3 down;
        float radius;
        float force;
};

#endif // PLAYER_H
