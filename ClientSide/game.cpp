#include "game.h"

#define RENDER_LIGHTS 1
#define RENDER_GEOMETRY 1
#define RENDER_PARTICLES 1
#define RENDER_EFFECTS 1
#define RENDER_GUI 1
#define RENDER_SCREENRECTANGLE 1
#define UPDATE_PARTICLES 1
#define ADD_LIGHTS 0

Game::Game(float width, float height, std::string title, Camera* camera) : screenWidth(width), screenHeight(height), title(title)
{
    this->display = new Display(screenWidth, screenHeight, title, false);
    this->camera = camera;
    this->construct();
}

Game::Game(std::string title, Camera* camera) : title(title)
{
    this->display = new Display(screenWidth, screenHeight, title, true);
    this->camera = camera;
    this->construct();
}

Game::Game(std::string title) : Game(title, new Camera(glm::vec3(0, 0, 0), 0, 0, 0))
{
}

Game::Game(float width, float height, std::string title) : Game(width, height, title, new Camera(glm::vec3(0, 0, 0), 0, 0, 0))
{
}

void Game::construct(){
    initBullet();

    timeAccumulator = 0;
    discreteChunk = 18.0f/6.0f;
    wasSpaceReleased = true;
    input = new Input();
    terrainShader = new SimpleShader("res/shaders/terrain.vs", "res/shaders/terrain.fs");
    simpleShader = new SimpleShader("res/shaders/vertex", "res/shaders/fragment");
    sunSimpleShader = new SimpleShader("res/shaders/sun_simple.vs", "res/shaders/sun_simple.fs");
    simpleShaderForLights = new SimpleShader("res/shaders/vertex_light", "res/shaders/fragment_light");
    directionalLightShader = new SimpleShader("res/shaders/vertex_directional_light.txt", "res/shaders/fragment_directional_light.txt");
    deferredLightShader = new DeferredLightShader("res/shaders/vertex_def", "res/shaders/fragment_def");
    skyShader = new DeferredLightShader("res/shaders/vertex_sky.txt", "res/shaders/fragment_sky.txt");
    sunShader = new DeferredLightShader("res/shaders/vertex_sun.txt", "res/shaders/fragment_sun.txt");
    emptyShader = new SimpleShader("res/shaders/vertex_empty", "res/shaders/fragment_empty");
    guiShader = new SimpleShader("res/shaders/GuiShader.vs", "res/shaders/GuiShader.fs");
    goalShader = new SimpleShader("res/shaders/goal.vs", "res/shaders/goal.fs");
    boxMesh = Mesh::loadObject("res/models/cube4.obj");
    sphereMesh = Mesh::loadObject("res/models/sphere4.obj");
    lightMesh = Mesh::loadObject("res/models/lightlightsphere.obj");
    turretMesh = Mesh::loadObject("res/models/turret.obj");
    baseMesh = Mesh::loadObject("res/models/base.obj");
    fanMesh = Mesh::loadObject("res/models/fan2.obj");
    fanBaseMesh = Mesh::loadObject("res/models/fanBase2.obj");
    treeTrunk = Mesh::loadObject("res/models/tree2/tree2.obj");
    treeBranch = Mesh::loadObject("res/models/tree2/branches3.obj");
    textShader = new TextShader("res/shaders/text_vs", "res/shaders/text_fs");
    player = new Player(world, 30.0f, glm::vec3(0.0f, 30.0f, -300.0f), glm::vec3(10));
    fpsText = new Text(new Font("res/fonts/myfont.fnt", "res/fonts/font7.bmp"),
                            "",
                            glm::vec3(101, 100, 0),
                            glm::vec3(0, 0, 0),
                            glm::vec3(1, 0.5, 0), 10);
    lightsText = new Text(new Font("res/fonts/myfont.fnt", "res/fonts/font7.bmp"),
                            "",
                            glm::vec3(400, 100, 0),
                            glm::vec3(0, 0, 0),
                            glm::vec3(1, 0.5, 0), 10);
    tex1 = new Texture("res/textures/154.bmp", 4);
    tex2 = new Texture("res/textures/196.bmp", 5);
    bark = new Texture("res/models/tree/Bark_Tile.bmp", 6, false);
    leaf = new Texture("res/models/tree2/leaf.bmp", 7, true);
    blendmap = new Texture("res/textures/blendmap.bmp", 0);
    dirt = new Texture("res/textures/dirt.bmp", 1);
    grass = new Texture("res/textures/grass2.bmp", 2);
    soil = new Texture("res/textures/soil.bmp", 3);
    rock = new Texture("res/textures/rock.bmp", 4);
    gBuffer = new GBuffer(this->screenWidth, this->screenHeight);
    Box::setMesh(boxMesh);
    Sphere::setMesh(sphereMesh);
    Light::setMesh(lightMesh);
    outputType = 6;

    screenRectangle = new Entity(world,
                                 "screenRectangle",
                                 Mesh::getRectangle(),
                                 glm::vec4(1),
                                 glm::vec3(0),
                                 glm::vec3(0),
                                 glm::vec3(1),
                                 NULL);

    sky = new Entity(world,
                     "screenRectangle",
                     Mesh::getDome(10,10),
                     glm::vec4(0.52, 0.8, 0.98, 1),
                     glm::vec3(0,-50,0),
                     glm::vec3(0),
                     glm::vec3(600),
                     NULL);

    sun = new Entity(world,
                     "screenRectangle",
                     Mesh::getCircle(0, 0, 150.0, 10),
                     glm::vec4(0.9, 0.7, 0.5, 1),
                     glm::vec3(this->screenWidth/2.0f+50,this->screenHeight/2.0f+300,-4000),
                     glm::vec3(0),
                     glm::vec3(1),
                    NULL);

    goal = new Entity(world,
                      "goal",
                      Mesh::getSurface(10, 10),
                      glm::vec4(0.5, 0.1, 0.5, 1),
                      glm::vec3(-30.537, 350.779, -950.82),
                      glm::vec3(glm::radians(90.0f), 0, 0),
                      glm::vec3(100.0f),
                      NULL);

    crosshair = new Entity(world,
                           "crosshair",
                           Mesh::getCircle(0, 0, 2.0f, 10),
                           glm::vec4(1,1,1,1),
                           glm::vec3(this->screenWidth/2.0, this->screenHeight/2.0, 0),
                           glm::vec3(0),
                           glm::vec3(1),
                           NULL);

    entities.push_back(new Box(world,
                                0.0f,
                                glm::vec4(1,1,1,1),
                                glm::vec3(0, 100, 200),
                                glm::vec3(0, 0, 0),
                                glm::vec3(300),
                                tex1)
                       );

    entities.push_back(new Sphere(world,
                                    100.0f,
                                    glm::vec4(1,1,1,1),
                                    glm::vec3(400, 100, 0),
                                    glm::vec3(0, 0, 0),
                                    30.0f,
                                    tex1)
                       );


    /*entities.push_back(new Entity(world,
                               "surface",
                               Mesh::getSurface(500, 500),
                               glm::vec4(0.5, 0.5, 0.5, 1),
                               glm::vec3(0, 0, 0),
                               glm::vec3(0.0f, 0.0f, 0.0f),
                               glm::vec3(50000.0f, 1, 50000.0f),
                               NULL)
                        );*/

    terrain = new Entity(world,
                           "surface",
                           terrainMesh,
                           glm::vec4(1, 1, 1, 1),
                           glm::vec3(0, 0, 0),
                           glm::vec3(0.0f, 0.0f, 0.0f),
                           glm::vec3(4800.0f, 1600.0f, 4800.0f),
                           NULL
                        );

    float lightsize = 600.0f;
    //lights.push_back(new Light(gBuffer, glm::vec3(0.9, 0.5, 0.5), glm::vec3(-30.537, 100.779, -800.82), lightsize));
    //lights.push_back(new Light(gBuffer, glm::vec3(0.2, 0.9, 0.0), glm::vec3(-30.537, 400.779, -800.82), lightsize));

    #if ADD_LIGHTS == 1
    /*for(int i = 0; i < 20; i++){
        for(int j = 0; j < 20; j++){
            lights.push_back(new Light(gBuffer, glm::vec3(1, 1, 1), glm::vec3(i*500, 100, j*500), lightsize));
        }
    }
    lights.push_back(new Light(gBuffer, glm::vec3(0.3, 0.9, 0.0), glm::vec3(0, 100, 400), lightsize));
    lights.push_back(new Light(gBuffer, glm::vec3(0.9, 0.9, 0.9), glm::vec3(0, 100, 0), lightsize));
    lights.push_back(new Light(gBuffer, glm::vec3(0.9, 0.9, 0.9), glm::vec3(100, 100, 200), lightsize));*/

    lights.push_back(new Light(gBuffer, glm::vec3(0.9, 0.3, 0.9), glm::vec3(-400, 100, -200), lightsize));
    lights.push_back(new Light(gBuffer, glm::vec3(0.4, 0.9, 0.9), glm::vec3(400, 100, -200), lightsize));
    lights.push_back(new Light(gBuffer, glm::vec3(0.1, 0.9, 0.4), glm::vec3(0, 100, -200), lightsize));
    lights.push_back(new Light(gBuffer, glm::vec3(0.4, 0.9, 0.2), glm::vec3(-400, 100, -400), lightsize));
    lights.push_back(new Light(gBuffer, glm::vec3(0.9, 0.9, 0.3), glm::vec3(400, 100, -400), lightsize));
    lights.push_back(new Light(gBuffer, glm::vec3(0.9, 0.4, 0.9), glm::vec3(0, 100, -400), lightsize));

    lights.push_back(new Light(gBuffer, glm::vec3(1, 0, 0), glm::vec3(500, 100, -100), lightsize));
    lights.push_back(new Light(gBuffer, glm::vec3(1, 0, 1), glm::vec3(500, 100, 400), lightsize));
    lights.push_back(new Light(gBuffer, glm::vec3(0.4, 0.2, 1), glm::vec3(500, 100, -1000), lightsize));
    lights.push_back(new Light(gBuffer, glm::vec3(1, 0, 1), glm::vec3(30, 100, 500), lightsize));
    lights.push_back(new Light(gBuffer, glm::vec3(0.1, 1, 0.4), glm::vec3(20, 30, 100), lightsize));
    lights.push_back(new Light(gBuffer, glm::vec3(1, 0.6, 0), glm::vec3(300, 100, -100), lightsize));
    lights.push_back(new Light(gBuffer, glm::vec3(1, 0, 0.2), glm::vec3(10, 100, -10), lightsize));
    lights.push_back(new Light(gBuffer, glm::vec3(0.7, 0, 1), glm::vec3(-30, 100, 10), lightsize));
    lights.push_back(new Light(gBuffer, glm::vec3(0.1, 1, 0.4), glm::vec3(20, 30, 100), lightsize));
    lights.push_back(new Light(gBuffer, glm::vec3(1, 0.6, 0), glm::vec3(-300, 100, -10), lightsize));
    lights.push_back(new Light(gBuffer, glm::vec3(1, 0, 1), glm::vec3(10, 10, 10), lightsize));
    lights.push_back(new Light(gBuffer, glm::vec3(1, 0, 1), glm::vec3(-30, 100, -10), lightsize));
    lights.push_back(new Light(gBuffer, glm::vec3(0.3, 1, 0.4), glm::vec3(-20, 30, -100), lightsize));
    lights.push_back(new Light(gBuffer, glm::vec3(1, 0.6, 0), glm::vec3(-30, 100, -100), lightsize));
    lights.push_back(new Light(gBuffer, glm::vec3(1, 0, 1), glm::vec3(10, 10, 10), lightsize));
    #endif
    DirectionalLight::setMesh(Mesh::getRectangle());
    sunLight = new DirectionalLight(gBuffer, glm::vec3(1.0, 0.50, 0.2), glm::vec3(1,3,-4.5));

    near = 1.0f;
    far = 5000.0f;
    fov = 75.0f;
    aspect = this->screenWidth / this->screenHeight;

    projectionMatrix = glm::perspective(glm::radians(fov), aspect, near, far);
    glm::mat4 orthographicProjectionMatrix = glm::ortho(0.0f, screenWidth, 0.0f, screenHeight);

    deferredLightShader->bind();
    deferredLightShader->loadProjectionMatrix(projectionMatrix);

    simpleShaderForLights->bind();
    simpleShaderForLights->loadProjectionMatrix(projectionMatrix);

    textShader->bind();
    textShader->loadProjectionMatrix(orthographicProjectionMatrix);

    emptyShader->bind();
    emptyShader->loadProjectionMatrix(projectionMatrix);

    skyShader->bind();
    skyShader->loadProjectionMatrix(projectionMatrix);

    sunShader->bind();
    sunShader->loadProjectionMatrix(projectionMatrix);

    sunSimpleShader->bind();
    sunSimpleShader->loadProjectionMatrix(projectionMatrix);

    guiShader->bind();
    guiShader->loadProjectionMatrix(orthographicProjectionMatrix);

    goalShader->bind();
    goalShader->loadProjectionMatrix(projectionMatrix);

    terrainShader->bind();
    terrainShader->loadProjectionMatrix(projectionMatrix);
    glUniform1i(glGetUniformLocation(terrainShader->getProgram(), "blendmap"), 0);
    glUniform1i(glGetUniformLocation(terrainShader->getProgram(), "dirt"), 1);
    glUniform1i(glGetUniformLocation(terrainShader->getProgram(), "grass"), 2);
    glUniform1i(glGetUniformLocation(terrainShader->getProgram(), "soil"), 3);
    glUniform1i(glGetUniformLocation(terrainShader->getProgram(), "rock"), 4);

    isClosed = false;
    cullLightsThread = getCullLightsThread();

    std::vector<Entity*> fanEntities;

    fanEntities.push_back(new Entity(world,
                               "fanBase",
                               fanBaseMesh,
                               glm::vec4(1, 1, 1, 1),
                               glm::vec3(183.019, 65.7267, 286.942),
                               glm::vec3(glm::radians(0.0f), 0.0f, glm::radians(0.0f)),
                               glm::vec3(30.0f),
                               new Texture("res/textures/FanBaseTexture.bmp", 0))
                        );

    fanEntities.push_back(new Entity(world,
                               "fan",
                               fanMesh,
                               glm::vec4(1, 1, 1, 1),
                               glm::vec3(10200, 1.5, 0),
                               glm::vec3(glm::radians(0.0f), 0.0f, glm::radians(.0f)),
                               glm::vec3(30.0f),
                               new Texture("res/textures/FanTexture.bmp", 0))
                        );

    fanPickableObject = new PickableObject(world, "fan", fanEntities, [](PuzzleObject* obj){
                                                                    glm::vec3 pos = obj->getEntities()[0]->getPosition();
                                                                    obj->getEntities()[1]->setPosition(pos.x, pos.y, pos.z);
                                                                    obj->getEntities()[1]->addRotation(0, 0.03, 0);
                                                                },
                                                                [](PuzzleObject* obj, unsigned int type){
                                                                    return;
                                                               }
                                    );
    fanEntities.clear();
    fanEntities.push_back(new Entity(world,
                               "fanBase",
                               fanBaseMesh,
                               glm::vec4(1, 1, 1, 1),
                               glm::vec3(0, 200.0, -500),
                               glm::vec3(glm::radians(90.0f), 0.0f, glm::radians(.0f)),
                               glm::vec3(80.0f),
                               new Texture("res/textures/FanBaseTexture.bmp", 0))
                        );

    fanEntities.push_back(new Entity(world,
                               "fan",
                               fanMesh,
                               glm::vec4(1, 1, 1, 1),
                               glm::vec3(10200, 1.5, 0),
                               glm::vec3(glm::radians(90.0f), 0.0f, glm::radians(0.0f)),
                               glm::vec3(80.0f),
                               new Texture("res/textures/FanTexture.bmp", 0))
                        );
    fanPuzzleObject2 = new PuzzleObject(world, "fan", fanEntities, [](PuzzleObject* obj){
                                                                    glm::vec3 pos = obj->getEntities()[0]->getPosition();
                                                                    obj->getEntities()[1]->setPosition(pos.x, pos.y, pos.z);
                                                                    obj->getEntities()[1]->addRotation(0, 0.5, 0);
                                                                },
                                                                false
                                    );

    fanPickableObject->boundingRectangle[0] = glm::vec3(-0.656439, 0.096338, -0.656439);
    fanPickableObject->boundingRectangle[1] = glm::vec3(0.656439, 0.096338, -0.656439);
    fanPickableObject->boundingRectangle[2] = glm::vec3(0.656439, 0.096337, 0.656439);
    fanPickableObject->boundingRectangle[3] = glm::vec3(-0.656439, 0.656439, 0.096337);

    fanPuzzleObject2->boundingRectangle[0] = glm::vec3(-0.656439, 0.096338, -0.656439);
    fanPuzzleObject2->boundingRectangle[1] = glm::vec3(0.656439, 0.096338, -0.656439);
    fanPuzzleObject2->boundingRectangle[2] = glm::vec3(0.656439, 0.096337, 0.656439);
    fanPuzzleObject2->boundingRectangle[3] = glm::vec3(-0.656439, 0.096337, 0.656439);

    particleInteractors.push_back(fanPickableObject);
    particleInteractors.push_back(fanPuzzleObject2);
    puzzleObjects.push_back(fanPuzzleObject2);
    puzzleObjects.push_back(fanPickableObject);
    pickableObjects.push_back(fanPickableObject);

    std::vector<Entity*> turretEntities;

    turretEntities.push_back(new Entity(world,
                               "base",
                               baseMesh,
                               glm::vec4(1, 1, 1, 1),
                               glm::vec3(0, 45.0, -100),
                               glm::vec3(0.0f, 0.0f, 0.0f),
                               glm::vec3(60.0f),
                               new Texture("res/textures/BaseTexture.bmp", 0))
                        );

    turretEntities.push_back(new Entity(world,
                               "turret",
                               turretMesh,
                               glm::vec4(1, 1, 1, 1),
                               glm::vec3(0, 63.0f, -100),
                               glm::vec3(0.0f, 0.0f, 0.0f),
                               glm::vec3(30.0f),
                               new Texture("res/textures/TurretTexture.bmp", 0))
                        );

    turretPuzzleObject = new PuzzleObject(world, "turret", turretEntities,
                                          [](PuzzleObject* obj){
                                                glm::vec3 pos = obj->getEntities()[0]->getPosition();
                                                obj->getEntities()[1]->setPosition(pos.x, pos.y+18.0f, pos.z);
                                            },
                                          [](PuzzleObject* obj, unsigned int type){
                                                switch(type){
                                                    case 0:{
                                                        obj->getEntities()[0]->addRotation(0.0, 0.005, 0.0);
                                                        obj->getEntities()[1]->addRotation(0.0, 0.005, 0.0);
                                                        break;
                                                    }
                                                    case 1:{
                                                        obj->getEntities()[1]->addRotation(0.005, 0, 0);
                                                        break;
                                                    }
                                                }
                                           },
                                           false
                                        );
    puzzleObjects.push_back(turretPuzzleObject);
    particleRenderer = new ParticleRenderer(projectionMatrix, turretPuzzleObject->getEntities()[1]->getPosition(), 1000);
    hBlur = new PostProcess(this->screenWidth/2.0f, this->screenHeight/2.0f, "res/shaders/hBlur.vs", "res/shaders/hBlur.fs");
    wBlur = new PostProcess(this->screenWidth/4.0f, this->screenHeight/4.0f, hBlur->getResultingTextureId(), "res/shaders/wBlur.vs", "res/shaders/wBlur.fs");
    std::vector<std::string> paths = {"res/textures/lensflare/lenscolor.bmp", "lensFlareColorSampler"};
    sunPostProcess = new PostProcess(this->screenWidth/4.0, this->screenHeight/4.0, "res/shaders/sun_postprocess.vs", "res/shaders/sun_postprocess.fs", paths);
    hBlur2 = new PostProcess(this->screenWidth/8.0f, this->screenHeight/8.0f, sunPostProcess->getResultingTextureId(), "res/shaders/hBlur.vs", "res/shaders/hBlur.fs");
    wBlur2 = new PostProcess(this->screenWidth/8.0f, this->screenHeight/8.0f, hBlur2->getResultingTextureId(), "res/shaders/wBlur.vs", "res/shaders/wBlur.fs");

    simpleShader->bind();
    glUniform1i(glGetUniformLocation(simpleShader->getProgram(), "colorSampler"), 11);
    glUniform1i(glGetUniformLocation(simpleShader->getProgram(), "depthSampler"), 12);
    glUniform1i(glGetUniformLocation(simpleShader->getProgram(), "normalSampler"), 14);
    glUniform1i(glGetUniformLocation(simpleShader->getProgram(), "lightSampler"), 15);
    glUniform1i(glGetUniformLocation(simpleShader->getProgram(), "particlesPostProcessSampler"), 10);
    glUniform1i(glGetUniformLocation(simpleShader->getProgram(), "particlesSampler"), 9);
    glUniform1i(glGetUniformLocation(simpleShader->getProgram(), "sunPostProcessSampler"), 8);

    fanEntities.clear();
    fanEntities.push_back(new Entity(world,
                               "fanBase",
                               fanBaseMesh,
                               glm::vec4(1, 1, 1, 1),
                               glm::vec3(100.6487, 100.4589, 29.2592),
                               glm::vec3(glm::radians(270.0f), 0.0f, glm::radians(0.0f)),
                               glm::vec3(30.0f),
                               new Texture("res/textures/FanBaseTexture.bmp", 0))
                        );

    fanEntities.push_back(new Entity(world,
                               "fan",
                               fanMesh,
                               glm::vec4(1, 1, 1, 1),
                               glm::vec3(10200, 1.5, 0),
                               glm::vec3(glm::radians(270.0f), 0.0f, glm::radians(0.0f)),
                               glm::vec3(30.0f),
                               new Texture("res/textures/FanTexture.bmp", 0))
                        );
    ob1 = new PickableObject(world, "pickableFan", fanEntities,
                                          [](PuzzleObject* obj){
                                                glm::vec3 pos = obj->getEntities()[0]->getPosition();
                                                obj->getEntities()[1]->setPosition(pos.x, pos.y, pos.z);
                                                obj->getEntities()[1]->addRotation(0, 0.5, 0);
                                            },
                                          [](PuzzleObject* obj, unsigned int type){
                                                return;
                                           }
                            );
    ob1->boundingRectangle[0] = glm::vec3(-0.656439, 0.096338, -0.656439);
    ob1->boundingRectangle[1] = glm::vec3(0.656439, 0.096338, -0.656439);
    ob1->boundingRectangle[2] = glm::vec3(0.656439, 0.096337, 0.656439);
    ob1->boundingRectangle[3] = glm::vec3(-0.656439, 0.096337, 0.656439);

    particleInteractors.push_back(ob1);
    pickableObjects.push_back(ob1);
    puzzleObjects.push_back(ob1);

    std::vector<Entity*> cubeEntities;
    cubeEntities.push_back(new Entity(world,
                               "fanBase",
                               boxMesh,
                               glm::vec4(1, 1, 1, 1),
                               glm::vec3(38.8719, 21.7659, 30.9866),
                               glm::vec3(glm::radians(0.0f), 0.0f, glm::radians(0.0f)),
                               glm::vec3(25.0f),
                               tex2)
                            );
    pickableObjects.push_back(new PickableObject(world, "pickableCube", cubeEntities,
                                          [](PuzzleObject* obj){
                                                return;
                                            },
                                          [](PuzzleObject* obj, unsigned int type){
                                                return;
                                           }
                            ));
    puzzleObjects.push_back(pickableObjects[pickableObjects.size()-1]);
    cubeEntities.clear();
    cubeEntities.push_back(new Entity(world,
                               "fanBase",
                               boxMesh,
                               glm::vec4(1, 1, 1, 1),
                               glm::vec3(175.001, 46.8874, 118.769),
                               glm::vec3(glm::radians(0.0f), 0.0f, glm::radians(0.0f)),
                               glm::vec3(30.0f),
                               tex2)
                            );
    pickableObjects.push_back(new PickableObject(world, "pickableCube", cubeEntities,
                                          [](PuzzleObject* obj){
                                                return;
                                            },
                                          [](PuzzleObject* obj, unsigned int type){
                                                return;
                                           }
                            ));
    puzzleObjects.push_back(pickableObjects[pickableObjects.size()-1]);
    cubeEntities.clear();
    cubeEntities.push_back(new Entity(world,
                               "fanBase",
                               boxMesh,
                               glm::vec4(1, 1, 1, 1),
                               glm::vec3(183.534, 19.3874, 104.213),
                               glm::vec3(glm::radians(0.0f), 0.0f, glm::radians(0.0f)),
                               glm::vec3(25.0f),
                               tex2)
                            );
    pickableObjects.push_back(new PickableObject(world, "pickableCube", cubeEntities,
                                          [](PuzzleObject* obj){
                                                return;
                                            },
                                          [](PuzzleObject* obj, unsigned int type){
                                                return;
                                           }
                            ));
    puzzleObjects.push_back(pickableObjects[pickableObjects.size()-1]);
    cubeEntities.clear();
    cubeEntities.push_back(new Entity(world,
                               "fanBase",
                               boxMesh,
                               glm::vec4(1, 1, 1, 1),
                               glm::vec3(103.93, 78.5056, 33.6321 ),
                               glm::vec3(glm::radians(0.0f), 0.0f, glm::radians(0.0f)),
                               glm::vec3(30.0f),
                               tex2)
                            );
    pickableObjects.push_back(new PickableObject(world, "pickableCube", cubeEntities,
                                          [](PuzzleObject* obj){
                                                return;
                                            },
                                          [](PuzzleObject* obj, unsigned int type){
                                                return;
                                           }
                            ));
    puzzleObjects.push_back(pickableObjects[pickableObjects.size()-1]);
    cubeEntities.clear();
    cubeEntities.push_back(new Entity(world,
                               "fanBase",
                               boxMesh,
                               glm::vec4(1, 1, 1, 1),
                               glm::vec3(101.981, 21.0061, 33.0398),
                               glm::vec3(glm::radians(0.0f), 0.0f, glm::radians(0.0f)),
                               glm::vec3(25.0f),
                               tex2)
                            );
    pickableObjects.push_back(new PickableObject(world, "pickableCube", cubeEntities,
                                          [](PuzzleObject* obj){
                                                return;
                                            },
                                          [](PuzzleObject* obj, unsigned int type){
                                                return;
                                           }
                            ));
    puzzleObjects.push_back(pickableObjects[pickableObjects.size()-1]);
    cubeEntities.clear();
    cubeEntities.push_back(new Entity(world,
                               "fanBase",
                               boxMesh,
                               glm::vec4(1, 1, 1, 1),
                               glm::vec3(176.793, 46.3665, 163.636 ),
                               glm::vec3(glm::radians(0.0f), 0.0f, glm::radians(0.0f)),
                               glm::vec3(30.0f),
                               tex2)
                            );
    pickableObjects.push_back(new PickableObject(world, "pickableCube", cubeEntities,
                                          [](PuzzleObject* obj){
                                                return;
                                            },
                                          [](PuzzleObject* obj, unsigned int type){
                                                return;
                                           }
                            ));
    puzzleObjects.push_back(pickableObjects[pickableObjects.size()-1]);
    cubeEntities.clear();
    cubeEntities.push_back(new Entity(world,
                               "fanBase",
                               boxMesh,
                               glm::vec4(1, 1, 1, 1),
                               glm::vec3(102.564, 51.0056, 34.9733),
                               glm::vec3(glm::radians(0.0f), 0.0f, glm::radians(0.0f)),
                               glm::vec3(25.0f),
                               tex2)
                            );
    pickableObjects.push_back(new PickableObject(world, "pickableCube", cubeEntities,
                                          [](PuzzleObject* obj){
                                                return;
                                            },
                                          [](PuzzleObject* obj, unsigned int type){
                                                return;
                                           }
                            ));
    puzzleObjects.push_back(pickableObjects[pickableObjects.size()-1]);
    cubeEntities.clear();
    cubeEntities.push_back(new Entity(world,
                               "fanBase",
                               boxMesh,
                               glm::vec4(1, 1, 1, 1),
                               glm::vec3(135.242, 18.0786, 31.411),
                               glm::vec3(glm::radians(0.0f), 0.0f, glm::radians(0.0f)),
                               glm::vec3(30.0f),
                               tex2)
                            );
    pickableObjects.push_back(new PickableObject(world, "pickableCube", cubeEntities,
                                          [](PuzzleObject* obj){
                                                return;
                                            },
                                          [](PuzzleObject* obj, unsigned int type){
                                                return;
                                           }
                            ));
    puzzleObjects.push_back(pickableObjects[pickableObjects.size()-1]);
    cubeEntities.clear();
    cubeEntities.push_back(new Entity(world,
                               "fanBase",
                               boxMesh,
                               glm::vec4(1, 1, 1, 1),
                               glm::vec3(172.28, 18.8665, 157.944),
                               glm::vec3(glm::radians(0.0f), 0.0f, glm::radians(0.0f)),
                               glm::vec3(30.0f),
                               tex2)
                            );
    pickableObjects.push_back(new PickableObject(world, "pickableCube", cubeEntities,
                                          [](PuzzleObject* obj){
                                                return;
                                            },
                                          [](PuzzleObject* obj, unsigned int type){
                                                return;
                                           }
                            ));
    puzzleObjects.push_back(pickableObjects[pickableObjects.size()-1]);
    cubeEntities.clear();
    cubeEntities.push_back(new Entity(world,
                               "fanBase",
                               boxMesh,
                               glm::vec4(1, 1, 1, 1),
                               glm::vec3(187.348, 18.5961, 212.942),
                               glm::vec3(glm::radians(0.0f), 0.0f, glm::radians(0.0f)),
                               glm::vec3(25.0f),
                               tex2)
                            );
    pickableObjects.push_back(new PickableObject(world, "pickableCube", cubeEntities,
                                          [](PuzzleObject* obj){
                                                return;
                                            },
                                          [](PuzzleObject* obj, unsigned int type){
                                                return;
                                           }
                            ));
    puzzleObjects.push_back(pickableObjects[pickableObjects.size()-1]);
    cubeEntities.clear();
    cubeEntities.push_back(new Entity(world,
                               "fanBase",
                               boxMesh,
                               glm::vec4(1, 1, 1, 1),
                               glm::vec3(143.388, 16.7918, 366.668),
                               glm::vec3(glm::radians(0.0f), 0.0f, glm::radians(0.0f)),
                               glm::vec3(30.0f),
                               tex2)
                            );
    pickableObjects.push_back(new PickableObject(world, "pickableCube", cubeEntities,
                                          [](PuzzleObject* obj){
                                                return;
                                            },
                                          [](PuzzleObject* obj, unsigned int type){
                                                return;
                                           }
                            ));
    puzzleObjects.push_back(pickableObjects[pickableObjects.size()-1]);
    cubeEntities.clear();
    cubeEntities.push_back(new Entity(world,
                               "fanBase",
                               boxMesh,
                               glm::vec4(1, 1, 1, 1),
                               glm::vec3(176.8, 18.6574, 340.072 ),
                               glm::vec3(glm::radians(0.0f), 0.0f, glm::radians(0.0f)),
                               glm::vec3(30.0f),
                               tex2)
                            );
    pickableObjects.push_back(new PickableObject(world, "pickableCube", cubeEntities,
                                          [](PuzzleObject* obj){
                                                return;
                                            },
                                          [](PuzzleObject* obj, unsigned int type){
                                                return;
                                           }
                            ));
    puzzleObjects.push_back(pickableObjects[pickableObjects.size()-1]);
    cubeEntities.clear();
    cubeEntities.push_back(new Entity(world,
                               "fanBase",
                               boxMesh,
                               glm::vec4(1, 1, 1, 1),
                               glm::vec3(195.845, 18.4358, 274.71),
                               glm::vec3(glm::radians(0.0f), 0.0f, glm::radians(0.0f)),
                               glm::vec3(30.0f),
                               tex2)
                            );
    pickableObjects.push_back(new PickableObject(world, "pickableCube", cubeEntities,
                                          [](PuzzleObject* obj){
                                                return;
                                            },
                                          [](PuzzleObject* obj, unsigned int type){
                                                return;
                                           }
                            ));
    puzzleObjects.push_back(pickableObjects[pickableObjects.size()-1]);
    cubeEntities.clear();
    cubeEntities.push_back(new Entity(world,
                               "fanBase",
                               boxMesh,
                               glm::vec4(1, 1, 1, 1),
                               glm::vec3(36.2425, 51.7658, 34.5357 ),
                               glm::vec3(glm::radians(0.0f), 0.0f, glm::radians(0.0f)),
                               glm::vec3(30.0f),
                               tex2)
                            );
    pickableObjects.push_back(new PickableObject(world, "pickableCube", cubeEntities,
                                          [](PuzzleObject* obj){
                                                return;
                                            },
                                          [](PuzzleObject* obj, unsigned int type){
                                                return;
                                           }
                            ));
    puzzleObjects.push_back(pickableObjects[pickableObjects.size()-1]);
    cubeEntities.clear();
    cubeEntities.push_back(new Entity(world,
                               "fanBase",
                               boxMesh,
                               glm::vec4(1, 1, 1, 1),
                               glm::vec3(174.599, 48.4354, 285.982),
                               glm::vec3(glm::radians(0.0f), 0.0f, glm::radians(0.0f)),
                               glm::vec3(30.0f),
                               tex2)
                            );
    pickableObjects.push_back(new PickableObject(world, "pickableCube", cubeEntities,
                                          [](PuzzleObject* obj){
                                                return;
                                            },
                                          [](PuzzleObject* obj, unsigned int type){
                                                return;
                                           }
                            ));
    puzzleObjects.push_back(pickableObjects[pickableObjects.size()-1]);
    cubeEntities.clear();
    cubeEntities.push_back(new Entity(world,
                               "fanBase",
                               boxMesh,
                               glm::vec4(1, 1, 0, 1),
                               glm::vec3(183.206, 48.6574, 333.172),
                               glm::vec3(glm::radians(0.0f), 0.0f, glm::radians(0.0f)),
                               glm::vec3(30.0f),
                               tex2)
                            );
    pickableObjects.push_back(new PickableObject(world, "pickableCube", cubeEntities,
                                          [](PuzzleObject* obj){
                                                return;
                                            },
                                          [](PuzzleObject* obj, unsigned int type){
                                                return;
                                           }
                            ));
    puzzleObjects.push_back(pickableObjects[pickableObjects.size()-1]);
    cubeEntities.clear();
    PickableObject::setCamera(camera);
    PickableObject::setPlayer(player);

    std::vector<glm::vec3> posRotScale;
    for(unsigned int i = 0; i < 5; i++){
        for(unsigned int j = 0; j < 5; j++){
            posRotScale.push_back(glm::vec3(300.0f+i*100.0f, 0.0, 165.0f+j*100.0f));
            posRotScale.push_back(glm::vec3(0));
            posRotScale.push_back(glm::vec3(50.0f));
        }
    }
    posRotScale.push_back(glm::vec3(0, 30.0, -580));
    posRotScale.push_back(glm::vec3(0));
    posRotScale.push_back(glm::vec3(50));
    treeRenderer = new TreeRenderer(posRotScale, new Tree("tree", treeTrunk, treeBranch), projectionMatrix);
}

void Game::handleInput(Game* game){
    Input* input = game->getInput();
    Display* display = game->getDisplay();
    Camera* camera = game->getCamera();
    Player* player = game->getPlayer();

    input->update(display);

    glm::vec3 forward = camera->getForward();
    glm::vec3 right = camera->getRight();

    if(input->getKeyDown(SDLK_SPACE) && game->wasSpaceReleased){
        game->wasSpaceReleased = false;
        if(!player->getIsJumping()){
            player->jump();
        }
    }

    if(input->getKeyUp(SDLK_SPACE)){
        game->wasSpaceReleased = true;
    }

    if(input->getKey(SDLK_w)){
        btVector3 fwd = btVector3(forward.x, 0, forward.z);
        fwd = fwd.normalized() * FORCE;
        player->getRigidBody()->applyCentralForce(fwd);
    }else if(input->getKey(SDLK_s)){
        btVector3 fwd = btVector3(forward.x, 0, forward.z);
        fwd = fwd.normalized() * -FORCE;
        player->getRigidBody()->applyCentralForce(fwd);
    }

    if(input->getKey(SDLK_a)){
        btVector3 rgh = btVector3(right.x, 0, right.z);
        rgh = rgh.normalized() * -FORCE;
        player->getRigidBody()->applyCentralForce(rgh);
    }else if(input->getKey(SDLK_d)){
        btVector3 rgh = btVector3(right.x, 0, right.z);
        rgh = rgh.normalized() * FORCE;
        player->getRigidBody()->applyCentralForce(rgh);
    }

    if(input->getKeyDown(SDLK_ESCAPE)){
        display->close();
        game->isClosed = true;
    }
    if(glm::abs(glm::length(input->getMouseDelta())) > 0.3f){
        camera->rotateX(input->getMouseDelta().y * 0.002f);
        camera->rotateY(input->getMouseDelta().x * 0.002f);
    }

    /*if(input->getMouseDown(1)){
        Box* box = new Box(game->getWorld(),
                                    100.0f,
                                    glm::vec4(1,1,1,1),
                                    camera->getPosition() + camera->getForward() * 100.0f,
                                    glm::vec3(0, 0, 0),
                                    glm::vec3(30.0f),
                                    game->tex2);
        box->setLinearVelocity(camera->getForward() * 100.0f);
        game->getEntities().push_back(box);
    }*/

    if(input->getKeyDown(SDLK_1)){
        game->outputType = 1;
    }else if(input->getKeyDown(SDLK_2)){
        game->outputType = 2;
    }else if(input->getKeyDown(SDLK_3)){
        game->outputType = 3;
    }else if(input->getKeyDown(SDLK_4)){
        game->outputType = 4;
    }else if(input->getKeyDown(SDLK_5)){
        game->outputType = 5;
    }else if(input->getKeyDown(SDLK_6)){
        game->outputType = 6;
    }

    if(input->getKeyDown(SDLK_q)){
        glm::vec3 pos = player->getPosition();
        float r = (float)rand()/RAND_MAX;
        float g = (float)rand()/RAND_MAX;
        float b = (float)rand()/RAND_MAX;
        game->lights.push_back(new Light(game->gBuffer, glm::vec3(r,g,b), glm::vec3(pos.x, 100, pos.z), 600.0f));
    }

    if(input->getKeyDown(SDLK_t)){
        //how to move a rigid body without breaking the engine
        btTransform transform = player->getRigidBody()->getCenterOfMassTransform();
        transform.setOrigin(btVector3(0, 300, 200));
        player->getRigidBody()->setCenterOfMassTransform(transform);
    }
}

void Game::stencil(){
    gBuffer->unbind();
    gBuffer->bindForStencil();

    glEnable(GL_STENCIL_TEST);
    glClear(GL_STENCIL_BUFFER_BIT);
    glStencilFunc(GL_ALWAYS, 0, 0);
    glStencilOpSeparate(GL_BACK, GL_KEEP, GL_INCR_WRAP, GL_KEEP);
    glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_DECR_WRAP, GL_KEEP);

    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);

    glDisable(GL_CULL_FACE);

    emptyShader->bind();
    for(Light* l : lights){
        if(l->getRenderIt() == true){
            l->draw(emptyShader);
        }
    }
}

void Game::normal(){
    gBuffer->unbind();
    gBuffer->bindForLights();

    directionalLightShader->bind();
    sunLight->draw(directionalLightShader);

    glStencilFunc(GL_NOTEQUAL, 0, 0xFF);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);

    glDisable(GL_DEPTH_TEST);

    glEnable(GL_BLEND);
    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_ONE, GL_ONE);

    simpleShaderForLights->bind();
    for(Light* l : lights){
        if(l->getRenderIt() == true){
            l->draw(simpleShaderForLights);
        }
    }
}

void Game::render(){
    numOfLightsVisible = 0;
    {
        std::lock_guard<std::mutex> lk(m);
        ready = true;
    }
    cv.notify_one();
    #if RENDER_GEOMETRY == 1
    gBuffer->bindForScene();
    {
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        glDepthMask(GL_TRUE);
        glDisable(GL_STENCIL_TEST);
        display->clear(0,0,0,0);

        glDepthMask(GL_FALSE);
        glDisable(GL_CULL_FACE);
        skyShader->bind();
        skyShader->loadViewMatrix(camera->getViewMatrix());
        sky->draw(skyShader);
        sunShader->bind();
        sunShader->loadViewMatrix(camera->getViewMatrix());
        sun->draw(sunShader);
        glDepthMask(GL_TRUE);
        glEnable(GL_CULL_FACE);
        blendmap->use();
        soil->use();
        rock->use();
        grass->use();
        dirt->use();
        terrainShader->bind();
        terrainShader->loadViewMatrix(camera->getViewMatrix());
        terrain->draw(terrainShader);

        treeRenderer->draw(camera->getViewMatrix());

        deferredLightShader->bind();
        deferredLightShader->loadViewMatrix(camera->getViewMatrix());

        for(Entity* e : entities){
            e->draw(deferredLightShader);
        }
        for(PuzzleObject *puzzleObject : puzzleObjects){
            puzzleObject->draw(deferredLightShader);
        }
        glDisable(GL_CULL_FACE);
    }
    #endif
    #if RENDER_LIGHTS == 1
    {
        simpleShaderForLights->bind();
        simpleShaderForLights->loadViewMatrix(camera->getViewMatrix());
        directionalLightShader->bind();
        directionalLightShader->loadViewMatrix(camera->getViewMatrix());

        emptyShader->bind();
        emptyShader->loadViewMatrix(camera->getViewMatrix());

        gBuffer->unbind();
        gBuffer->bindForLights();
        glClearColor(0,0,0,0);
        glClear(GL_COLOR_BUFFER_BIT);
        gBuffer->unbind();

        {
            std::unique_lock<std::mutex> lk(m);
            cv.wait(lk, [=]{return processed;});
        }
        stencil();
        normal();
    }
    #endif

    gBuffer->unbind();
    {
        #if RENDER_EFFECTS
        glBindFramebuffer(GL_READ_FRAMEBUFFER, gBuffer->getFrameBufferObject());
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, sunPostProcess->getFrameBufferObject());
        glBlitFramebuffer(0, 0, this->screenWidth, this->screenHeight, 0, 0, this->screenWidth/4.0, this->screenHeight/4.0, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, hBlur->getFrameBufferObject());
        glBlitFramebuffer(0, 0, this->screenWidth, this->screenHeight, 0, 0, this->screenWidth/2.0, this->screenHeight/2.0, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
        glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        hBlur->bind();
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        #endif
        #if RENDER_PARTICLES
        particleRenderer->draw();
        #endif
        #if RENDER_GEOMETRY
        glDisable(GL_CULL_FACE);
        goalShader->bind();
        glUniform1f(glGetUniformLocation(goalShader->getProgram(), "dt"), SDL_GetTicks()/1000.0f);
        goalShader->loadViewMatrix(camera->getViewMatrix());
        goal->draw(goalShader);
        #endif
        #if RENDER_EFFECTS
        glDisable(GL_DEPTH_TEST);
        hBlur->process();
        wBlur->bind();
        wBlur->process();
        sunPostProcess->bind();
        sunPostProcess->getShader().bind();
        glm::vec2 sunPosition = calculateSunPosition(projectionMatrix,
                                                     camera->getViewMatrix(),
                                                     sun->getModelMatrix(),
                                                     this->screenWidth,
                                                     this->screenHeight);
        glUniform2f(glGetUniformLocation(sunPostProcess->getShader().getProgram(), "sunPosition"), sunPosition.x, sunPosition.y);
        glEnable(GL_DEPTH_TEST);
        sunSimpleShader->bind();
        sunSimpleShader->loadViewMatrix(camera->getViewMatrix());
        sun->draw(sunSimpleShader);
        sunPostProcess->process();
        glDisable(GL_DEPTH_TEST);
        hBlur2->bind();
        hBlur2->process();
        wBlur2->bind();
        wBlur2->process();
        #endif
    }

    {
        #if RENDER_SCREENRECTANGLE
        glDisable(GL_CULL_FACE);
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_STENCIL_TEST);
		glDisable(GL_BLEND);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        simpleShader->bind();
        glActiveTexture(GL_TEXTURE0+11);
        glBindTexture(GL_TEXTURE_2D, gBuffer->getColorTexture());
        glActiveTexture(GL_TEXTURE0+12);
        glBindTexture(GL_TEXTURE_2D, gBuffer->getDepthTexture());
        glActiveTexture(GL_TEXTURE0+14);
        glBindTexture(GL_TEXTURE_2D, gBuffer->getNormalTexture());
        glActiveTexture(GL_TEXTURE0+15);
        glBindTexture(GL_TEXTURE_2D, gBuffer->getLightAccumulationTexture());
        glActiveTexture(GL_TEXTURE0+10);
        glBindTexture(GL_TEXTURE_2D, wBlur->getResultingTextureId());
        glActiveTexture(GL_TEXTURE0+9);
        glBindTexture(GL_TEXTURE_2D, hBlur->getInputTextureId());
        glActiveTexture(GL_TEXTURE0+8);
        glBindTexture(GL_TEXTURE_2D, wBlur2->getResultingTextureId());
        glUniform1i(glGetUniformLocation(simpleShader->getProgram(), "outputType"), outputType);
        screenRectangle->draw(simpleShader);
        #endif

        #if RENDER_GUI
        glDisable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        textShader->bind();
        fpsText->displayNumber(Display::getDelta());
        fpsText->draw(textShader);
        lightsText->displayNumber(numOfLightsVisible);
        lightsText->draw(textShader);
        guiShader->bind();
        crosshair->draw(guiShader);
        #endif
    }
    display->update();
}

void Game::performPickRayTest(){
    if (!player->getHasPickedUp()){
        glm::vec3 startPosition = player->getPosition() + glm::vec3(0, 20, 0);
        glm::vec3 endPosition = camera->getForward()*100.0f + startPosition;

        btVector3 from = btVector3(startPosition.x, startPosition.y, startPosition.z);
        btVector3 to = btVector3(endPosition.x, endPosition.y, endPosition.z);

        btCollisionWorld::ClosestRayResultCallback rayCallback(from, to);
        world->rayTest(from, to, rayCallback);
        if(rayCallback.hasHit()){
            UserPointer *p = (UserPointer*)rayCallback.m_collisionObject->getUserPointer();
            if (p->type == PICKABLE_OBJECT){
                p->ptrType.pickableObject->setIsTouched(true);
            }
        }
    }
}

void Game::update(){
    camera->setPosition(player->getPosition() + glm::vec3(0, 20, 0));

    player->performRayTest();

    for(PuzzleObject *puzzleObject : puzzleObjects){
        puzzleObject->update();
    }
    #if UPDATE_PARTICLES == 1
    particleRenderer->update(camera, turretPuzzleObject->getEntities()[1], particleInteractors);
    #endif
}

void Game::nonTimeCriticalInput(){
    performPickRayTest();

    if (input->getKey(SDLK_e)) {
        for (PickableObject *pickableObject : pickableObjects){
            if (pickableObject->getIsTouched()){
                pickableObject->pickUp();
                player->setHasPickedUp(true);
                break;
            }
        }
    }

    if (input->getMouse(1)){
        for (PickableObject *pickableObject : pickableObjects){
            if (pickableObject->getIsPickedUp() && !pickableObject->getIsColliding()) {
                pickableObject->release();
                player->setHasPickedUp(false);
                break;
            }
        }
    }
}

void Game::resetAll(){
    for(PickableObject *pickableObject : pickableObjects){
        pickableObject->setIsTouched(false);
        pickableObject->setIsColliding(false);

        for (Entity *e : pickableObject->getEntities()){
            e->setColor(glm::vec4(1,1,1,1));
        }
    }
}

void Game::run(){
    while(!display->isClosed()){
        display->setLastFrameTime(SDL_GetTicks());
        resetAll();
        //printf("%d\n", Display::getDelta());
        timeAccumulator += Display::getDelta();
        while(timeAccumulator >= discreteChunk){
            world->stepSimulation(btScalar(0.1f));
            timeAccumulator -= discreteChunk;
        }
        world->stepSimulation(btScalar(0.1f * (float)timeAccumulator/discreteChunk));
        timeAccumulator = 0;
        nonTimeCriticalInput();
        update();
        render();
    }
}

void Game::bulletTickCallback(btDynamicsWorld *world, btScalar timeStep) {
    Game* thisGame = (Game*)world->getWorldUserInfo();
    handleInput(thisGame);
}

bool Game::bulletCollisionCallback(btManifoldPoint& cp, const btCollisionObjectWrapper *obj1, int id1, int index1,
                                    const btCollisionObjectWrapper *obj2, int id2, int index2)
{
    if(obj1->getCollisionObject()->getUserPointer() == 0 ||
       obj2->getCollisionObject()->getUserPointer() == 0){
        return false;
    }

    //std::cout << ((UserPointer*)obj1->getCollisionObject()->getUserPointer())->type << " " << ((UserPointer*)obj2->getCollisionObject()->getUserPointer())->type << std::endl;

    UserPointer *up1, *up2;
    up1 = (UserPointer*)obj1->getCollisionObject()->getUserPointer();
    up2 = (UserPointer*)obj2->getCollisionObject()->getUserPointer();

    if (up1->type == PICKABLE_OBJECT &&
        up1->ptrType.pickableObject->getIsPickedUp() &&
        up2->type != PLAYER) {
        up1->ptrType.pickableObject->setIsColliding(true);
        return false;
    } else if (up2->type == PICKABLE_OBJECT &&
               up2->ptrType.pickableObject->getIsPickedUp() &&
               up1->type != PLAYER){
        up2->ptrType.pickableObject->setIsColliding(true);
        return false;
    }

    return false;
}

void Game::initBullet(){
    collisionConfig = new btDefaultCollisionConfiguration();
    dispatcher = new btCollisionDispatcher(collisionConfig);
    broadsphase = new btDbvtBroadphase();
    solver = new btSequentialImpulseConstraintSolver();

    world = new btDiscreteDynamicsWorld(dispatcher,
                                        broadsphase,
                                        solver,
                                        collisionConfig);
    world->setGravity(btVector3(0, GRAVITY, 0));
    world->setInternalTickCallback(Game::bulletTickCallback, (void*)this, true);
    gContactAddedCallback = Game::bulletCollisionCallback;

    terrainMesh = Mesh::loadObject("res/models/terrain.obj");
    int *indices = (int *)malloc(sizeof(int) * terrainMesh->getIndices().size());
    btScalar *vertices = (btScalar *)malloc(sizeof(btScalar) * terrainMesh->getVertices().size()*3);

    for(int i = 0; i < terrainMesh->getIndices().size(); i++){
        indices[i] = terrainMesh->getIndices()[i];
    }

    for(int i = 0; i < terrainMesh->getVertices().size()*3; i+=3){
        vertices[i] = btScalar(terrainMesh->getVertices()[i/3].positionCoords.x * 4800.0f);
        vertices[i+1] = btScalar(terrainMesh->getVertices()[i/3].positionCoords.y * 1600.0f);
        vertices[i+2] = btScalar(terrainMesh->getVertices()[i/3].positionCoords.z * 4800.0f);
    }

    btTransform t;
    t.setIdentity();
    t.setOrigin(btVector3(0, 0, 0));
    btMotionState* motion = new btDefaultMotionState(t);

    btTriangleIndexVertexArray *_terrain = new btTriangleIndexVertexArray(terrainMesh->getNumberOfTriangles()/3,
                                                                         indices,
                                                                         3 * sizeof(int),
                                                                         terrainMesh->getVertices().size(),
                                                                         vertices,
                                                                         3*sizeof(float));
    btBvhTriangleMeshShape *shape = new btBvhTriangleMeshShape(_terrain, true, true);
    btRigidBody::btRigidBodyConstructionInfo info2(0.0, motion, shape, btVector3(0,0,0));
    info2.m_restitution = 0.3;
    info2.m_friction = 1.0;
    btRigidBody *m_body = new btRigidBody(info2);
    UserPointer *userPointer = new UserPointer();
    userPointer->type = GROUND;
    m_body->setUserPointer((void*)userPointer);
    world->addRigidBody(m_body);
}

glm::vec2 Game::calculateSunPosition(const glm::mat4& projectionMatrix,
                               const glm::mat4& viewMatrix,
                               const glm::mat4& modelMatrix,
                               float WIDTH,
                               float HEIGHT){
    glm::mat4 newModelMatrix = modelMatrix;
    newModelMatrix[0][0] = viewMatrix[0][0];
    newModelMatrix[0][1] = viewMatrix[1][0];
    newModelMatrix[0][2] = viewMatrix[2][0];
    newModelMatrix[1][0] = viewMatrix[0][1];
    newModelMatrix[1][1] = viewMatrix[1][1];
    newModelMatrix[1][2] = viewMatrix[2][1];
    newModelMatrix[2][0] = viewMatrix[0][2];
    newModelMatrix[2][1] = viewMatrix[1][2];
    newModelMatrix[2][2] = viewMatrix[2][2];
    glm::vec4 pos = (projectionMatrix *
                     (glm::mat4(glm::mat3(viewMatrix)) *
                      newModelMatrix) *
                     glm::vec4(glm::vec3(0), 1.0));
    pos = glm::vec4(pos.x/pos.w, pos.y/pos.w, 0, 0);
    glm::vec2 posXY = glm::vec2((pos.x + 1)/2.0, (pos.y+1)/2.0);
    return posXY;
}


Game::~Game()
{
    /*for (PickableObject *ob : pickableObjects){
        std::cout << ob->getName() << " " << ob->getEntities()[0]->getPosition().x << " " << ob->getEntities()[0]->getPosition().y << " " << ob->getEntities()[0]->getPosition().z << std::endl;
    }*/
    std::cout << player->getPosition().x << " " << player->getPosition().y << " " << player->getPosition().z << std::endl;

    isClosed = true;
    cullLightsThread.detach();
    std::cout << "Destroying game..." << std::endl;
    delete display;
    delete simpleShader;
    delete simpleShaderForLights;
    delete emptyShader;
    delete guiShader;
    delete deferredLightShader;
    delete input;
    delete camera;
    delete particleRenderer;
    delete hBlur;
    delete wBlur;

    std::cout << "Freeing " << entities.size() << " entities..." << std::endl;

    for(Entity* e : entities){
        delete e;
    }
    for(Light* l : lights){
        delete l;
    }
    delete fanPickableObject;
    delete ob1;
    delete fanPuzzleObject2;
    delete turretPuzzleObject;

    delete boxMesh;
    delete sphereMesh;
    delete lightMesh;
    delete turretMesh;
    delete baseMesh;
    delete fanMesh;
    delete fanBaseMesh;

    delete world;
    delete dispatcher;
    delete broadsphase;
    delete collisionConfig;
    delete solver;

    delete treeRenderer;
}

Input* Game::getInput(){
    return input;
}

Camera* Game::getCamera(){
    return camera;
}

Display* Game::getDisplay(){
    return display;
}

btDynamicsWorld* Game::getWorld(){
    return world;
}

Player* Game::getPlayer(){
    return player;
}

std::vector<Entity*>& Game::getEntities(){
    return entities;
}
