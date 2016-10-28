#ifndef PLAYER_H
#define PLAYER_H

#include "entity.h"

class Player
{
    public:
        Player(btDynamicsWorld*, float, glm::vec3, glm::vec3);
        glm::vec3 getPosition();
        btRigidBody* getRigidBody();
        virtual ~Player();
    protected:
    private:
        btDynamicsWorld* world;
        btRigidBody* m_body;
};

#endif // PLAYER_H
