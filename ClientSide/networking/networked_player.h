#ifndef NETWORKEDPLAYER_H
#define NETWORKEDPLAYER_H

#include "../commons.h"
#include "../entity.h"

class NetworkedPlayer : public Entity
{
    public:
        NetworkedPlayer(btDynamicsWorld* world,
                                 float mass,
                                 glm::vec3 position,
                                 glm::vec3 forward);
        static void setMesh(Mesh *mesh);
        void setPosition(float x, float y, float z);
        void setForward(const glm::vec3& forward) {
            this->forward = forward;
        }
        virtual ~NetworkedPlayer();
    protected:
    private:
        btDynamicsWorld* world;
        btRigidBody* m_body;
        static Mesh *mesh;
        glm::vec3 forward;
};

#endif // NETWORKEDPLAYER_H
