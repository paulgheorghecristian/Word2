#ifndef PLAYER_H
#define PLAYER_H

#include "entity.h"

class Player : public Entity
{
    public:
        Player(btDynamicsWorld*, float, glm::vec4, glm::vec3, glm::vec3, glm::vec3);
        glm::vec3 get_position_for_camera();
        void draw(Shader *);
        static void set_mesh(Mesh *);
        virtual ~Player();
    protected:
    private:
        static Mesh* mesh;
        glm::vec3 position_for_camera;
};

#endif // PLAYER_H
