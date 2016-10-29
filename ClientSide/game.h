#ifndef GAME_H
#define GAME_H

#include "camera.h"
#include "display.h"
#include "input.h"
#include "shader.h"
#include "entity.h"
#include <string>
#include <vector>
#include "box.h"
#include "sphere.h"
#include "player.h"
#include "textShader.h"
#include "text.h"
#include <bullet/btBulletDynamicsCommon.h>

#define GRAVITY -20
#define FORCE 1200

class Game
{
    public:
        Game(float, float, std::string, Camera*);
        Game(std::string, Camera*);
        Game(std::string);
        Game(float, float, std::string);
        void run();
        virtual ~Game();
    protected:
    private:
        void construct();
        void render();
        void initBullet();
        static void bulletTickCallback(btDynamicsWorld*, btScalar);
        static void handleInput(Game*);
        Input* getInput();
        Camera* getCamera();
        Display* getDisplay();
        btDynamicsWorld* getWorld();
        Player* getPlayer();
        std::vector<Entity*>& getEntities();

        float screenWidth, screenHeight;
        std::string title;
        Camera* camera;
        Display* display;
        Input* input;
        Shader* shader;
        Player* player;
        TextShader* textShader;
        Text* fpsText;
        std::vector<Entity*> entities;
        Mesh* boxMesh;
        Mesh* sphereMesh;
        btDynamicsWorld* world;
        btDispatcher* dispatcher;
        btBroadphaseInterface* broadsphase;
        btCollisionConfiguration* collisionConfig;
        btConstraintSolver* solver;
        long timeAccumulator;
        float discreteChunk;
};

#endif // GAME_H
