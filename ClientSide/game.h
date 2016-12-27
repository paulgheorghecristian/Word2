#ifndef GAME_H
#define GAME_H

#include "camera.h"
#include "display.h"
#include "input.h"
#include "simple_shader.h"
#include "entity.h"
#include <string>
#include <vector>
#include "box.h"
#include "sphere.h"
#include "player.h"
#include "text_shader.h"
#include "text.h"
#include <bullet/btBulletDynamicsCommon.h>
#include "texture.h"
#include "g_buffer.h"
#include "deferred_light_shader.h"
#include "light.h"
#include <thread>
#include <mutex>
#include <condition_variable>
#include "math_utils.h"
#include "particle_renderer.h"

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
        void stencil(Light*);
        void normal(Light*);
        void cullLights(){
            while(!isClosed){
                std::unique_lock<std::mutex> lk(m);
                cv.wait(lk, [=]{return ready;});

                Frustum *frustum;
                frustum = MathUtils::calculateFrustum(camera, near, far, fov, aspect);

                for(Light *l : lights){
                    if(MathUtils::isSphereInsideFrustum(frustum, l->getPosition(), l->getRadius())){
                        l->setRenderIt(true);
                        numOfLightsVisible++;
                    }else{
                        l->setRenderIt(false);
                    }
                }

                processed = true;
                lk.unlock();
                cv.notify_one();

                ready = false;
                delete frustum;
            }
        }
        std::thread getCullLightsThread(){
            return std::thread([=] {cullLights();});
        }

        float screenWidth, screenHeight;
        std::string title;
        Camera* camera;
        Display* display;
        Input* input;
        SimpleShader* simpleShader;
        SimpleShader* simpleShaderForLights;
        SimpleShader* emptyShader;
        DeferredLightShader* deferredLightShader;
        Player* player;
        Entity* screenRectangle;
        TextShader* textShader;
        std::vector<Entity*> entities;
        std::vector<Light*> lights;
        Mesh* boxMesh;
        Mesh* sphereMesh;
        Mesh* lightMesh;
        Mesh* turretMesh;
        Mesh* baseMesh;
        Mesh* fanMesh;
        Mesh* fanBaseMesh;
        btDynamicsWorld* world;
        btDispatcher* dispatcher;
        btBroadphaseInterface* broadsphase;
        btCollisionConfiguration* collisionConfig;
        btConstraintSolver* solver;
        long timeAccumulator;
        float discreteChunk;
        Texture *tex1, *tex2;
        Text* fpsText;
        Text* lightsText;
        GBuffer* gBuffer;
        int outputType;
        glm::mat4 projectionMatrix;
        ParticleRenderer *particleRenderer;
        std::mutex m;
        std::condition_variable cv;
        std::thread cullLightsThread;
        volatile bool ready = false;
        volatile bool processed = false;
        volatile bool isClosed;
        volatile long numOfLightsVisible;

        float near, far, aspect, fov;
};

#endif // GAME_H
