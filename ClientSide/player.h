#ifndef PLAYER_H
#define PLAYER_H

#include <bullet/btBulletDynamicsCommon.h>
#include <glm/glm.hpp>
#include <iostream>

#define DOWN_LENGTH 1000
#define JUMP_SPEED 100000
#define EPS 0.03

class Player
{
    public:
        Player(btDynamicsWorld*, float, glm::vec3, glm::vec3);
        glm::vec3 getPosition();
        btRigidBody* getRigidBody();
        void performRayTest();
        bool getIsTouchingGround();
        bool getIsJumping();
        void jump();
        virtual ~Player();
    protected:
    private:
        btDynamicsWorld* world;
        btRigidBody* m_body;
        bool isTouchingGround;
        bool isJumping;
        const btVector3 down;
};

#endif // PLAYER_H
