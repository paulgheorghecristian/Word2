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
#include "puzzle_object.h"
#include "directional_light.h"
#include "post_process.h"
#include "pickable_object.h"
#include "commons.h"
#include "trees/tree_renderer.h"
#include "grass/grass_renderer.h"
#include <unordered_map>
#include "networking/client.h"
#include "networking/networked_player.h"

#define GRAVITY -30
#define FORCE 800

class ParticleRenderer;

class Game
{
    public:
        Game(float, float, std::string, Camera*, Client*);
        Game(std::string, Camera*, Client*);
        Game(std::string, Client*);
        Game(float, float, std::string, Client*);
        void run();
        virtual ~Game();
        static float score;
    protected:
    private:
        void construct();
        void render();
        void update();
        void initBullet();
        void addBoundingBox(float mass, glm::vec3& position, glm::vec3& rotation, glm::vec3& scale);
        static void bulletTickCallback(btDynamicsWorld*, btScalar);
        static bool bulletCollisionCallback(btManifoldPoint& cp, const btCollisionObjectWrapper *obj1, int id1, int index1,
                                            const btCollisionObjectWrapper *obj2, int id2, int index2);
        static void handleInput(Game*);
        Input* getInput();
        Camera* getCamera();
        Display* getDisplay();
        btDynamicsWorld* getWorld();
        Player* getPlayer();
        std::vector<Entity*>& getEntities();
        void stencil();
        void normal();
        glm::vec3 calculateSunPosition(const glm::mat4&,
                                       const glm::mat4&,
                                       const glm::mat4&,
                                       float,
                                       float);
        void cullLights(){
            while(!isClosed){
                std::unique_lock<std::mutex> lk(m);
                cv.wait(lk, [=]{return ready;});

                Frustum *frustum;
                frustum = MathUtils::calculateFrustum(camera, _near, _far-1500.0f, _fov, _aspect);

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

        void nonTimeCriticalInput();
        void resetAll();
        void performPickRayTest();
        void manageUpdateFromServer();

        float screenWidth, screenHeight;
        std::string title;
        Camera *camera;
        Display *display;
        Input *input;
        SimpleShader *simpleShader;
        SimpleShader *simpleShaderForLights;
        SimpleShader *directionalLightShader;
        SimpleShader *emptyShader, *sunSimpleShader;
        SimpleShader *terrainShader;
        SimpleShader *guiShader, *goalShader;
        SimpleShader *grassShader;
        PostProcess *sunPostProcess;
        PostProcess *hBlur, *wBlur, *hBlur2, *wBlur2;
        DeferredLightShader *deferredLightShader;
        DeferredLightShader *skyShader, *sunShader;
        Player *player;
        Entity *screenRectangle;
        Entity *sky;
        Entity *sun, *terrain, *crosshair, *goal;
        DirectionalLight *sunLight;
        TextShader* textShader;
        std::vector<Entity*> entities;
        std::vector<Light*> lights;
        Mesh *boxMesh;
        Mesh *sphereMesh;
        Mesh *lightMesh;
        Mesh *turretMesh;
        Mesh *baseMesh;
        Mesh *fanMesh;
        Mesh *fanBaseMesh;
        Mesh *treeTrunk;
        Mesh *treeBranch;
        Mesh *terrainMesh;
        btDynamicsWorld* world;
        btDispatcher* dispatcher;
        btBroadphaseInterface* broadsphase;
        btCollisionConfiguration* collisionConfig;
        btConstraintSolver* solver;
        long timeAccumulator;
        float discreteChunk;
        Texture *tex1, *tex2;
        Texture *bark, *leaf;
        Texture *blendmap, *dirt, *grass, *soil, *rock;
        Texture *grassBillboard;
        Text *fpsText;
        Text *lightsText;
        Text *scoreText;
        GBuffer *gBuffer;
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
        bool wasSpaceReleased;

        PuzzleObject *turretPuzzleObject, *fanPuzzleObject2;
        std::vector<PuzzleObject*> particleInteractors;
        std::vector<PuzzleObject*> puzzleObjects;
        std::vector<PickableObject*> pickableObjects;

        float _near;
        float _far;
        float _aspect;
        float _fov;

        PickableObject *ob1, *fanPickableObject;
        TreeRenderer *treeRenderer;
        GrassRenderer *grassRenderer;
        std::unordered_map<int, NetworkedPlayer*> otherPlayers;
        Client *client;
};

#endif // GAME_H
