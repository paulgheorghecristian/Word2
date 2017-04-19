#include "game.h"

#define RENDER_LIGHTS 1
#define RENDER_GEOMETRY 1
#define RENDER_PARTICLES 1
#define RENDER_EFFECTS 1
#define RENDER_GUI 1
#define RENDER_SCREENRECTANGLE 1
#define UPDATE_PARTICLES 1
#define ADD_LIGHTS 1

float Game::score;

Game::Game(float width, float height, std::string title, Camera* camera, Client *client) : screenWidth(width), screenHeight(height), title(title)
{
    this->display = new Display(screenWidth, screenHeight, title, false);
    this->camera = camera;
    this->client = client;
    this->construct();
}

Game::Game(std::string title, Camera* camera, Client *client) : title(title)
{
    this->display = new Display(screenWidth, screenHeight, title, true);
    this->camera = camera;
    this->client = client;
    this->construct();
}

Game::Game(std::string title, Client *client) : Game(title, new Camera(glm::vec3(0, 0, 0), 0, 0, 0), client)
{
}

Game::Game(float width, float height, std::string title, Client *client) : Game(width, height, title, new Camera(glm::vec3(0, 0, 0), 0, 0, 0), client)
{
}

void Game::construct(){
    initBullet();

    timeAccumulator = 0;
    discreteChunk = 18.0f/6.0f;
    wasSpaceReleased = true;
    input = new Input();
    terrainShader = new SimpleShader("res/shaders/terrain.vs", "res/shaders/terrain.fs");
    terrainShaderForWater = new SimpleShader("res/shaders/terrainForWater.vs", "res/shaders/terrainForWater.fs");
    simpleShader = new SimpleShader("res/shaders/vertex", "res/shaders/fragment");
    sunSimpleShader = new SimpleShader("res/shaders/sun_simple.vs", "res/shaders/sun_simple.fs");
    simpleShaderForLights = new SimpleShader("res/shaders/vertex_light", "res/shaders/fragment_light");
    directionalLightShader = new SimpleShader("res/shaders/vertex_directional_light.txt", "res/shaders/fragment_directional_light.txt");
    deferredLightShader = new DeferredLightShader("res/shaders/vertex_def", "res/shaders/fragment_def");
    forwardLightShaderForWater = new SimpleShader("res/shaders/forwardLightForWater.vs", "res/shaders/forwardLightForWater.fs");
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
    treeTrunkSimplified = Mesh::loadObject("res/models/tree2/tree2Simplified.obj");
    treeBranchSimplified = Mesh::loadObject("res/models/tree2/branches3Simplified.obj");
    terrainMeshSimplified = Mesh::loadObject("res/models/terrainSimplified.obj");
    textShader = new TextShader("res/shaders/text_vs", "res/shaders/text_fs");
    player = new Player(world, 30.0f, glm::vec3(0.0f, 30.0f, -300.0f), glm::vec3(10));
    fpsText = new Text(new Font("res/fonts/myfont.fnt", "res/fonts/font7.bmp"),
                            "",
                            glm::vec3(10, 500, 0),
                            glm::vec3(0, 0, 0),
                            glm::vec3(1, 0.5, 0), 3);
    lightsText = new Text(new Font("res/fonts/myfont.fnt", "res/fonts/font7.bmp"),
                            "",
                            glm::vec3(10, 480, 0),
                            glm::vec3(0, 0, 0),
                            glm::vec3(1, 0.5, 0), 3);

    scoreText = new Text(new Font("res/fonts/myfont.fnt", "res/fonts/font7.bmp"),
                            "",
                            glm::vec3(this->screenWidth/2-200, 100, 0),
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
    grassBillboard = new Texture("res/textures/billboardgrass1medium.bmp", 0, true, GL_BGR);
    gBuffer = new GBuffer(this->screenWidth, this->screenHeight);
    Box::setMesh(boxMesh);
    Sphere::setMesh(sphereMesh);
    Light::setMesh(lightMesh);
    Light::setGBuffer(gBuffer);
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
                     Mesh::getCircle(0, 0, 150.0, 36),
                     glm::vec4(0.9, 0.8, 0.6, 1),
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

    std::vector<glm::vec3> posScales;
    for(int i = 0; i < 10; i++){
        for(int j = 0; j < 10; j++){
            if (((i+j)%3*13)% 5 == 0)
                continue;
            posScales.push_back(glm::vec3(-500.0f+i*45.0f, 10, 350.0f+j*45.0f));
            posScales.push_back(glm::vec3(45));
        }
    }
    posScales.push_back(glm::vec3(350, 0, 310));
    posScales.push_back(glm::vec3(20));
    posScales.push_back(glm::vec3(379, 0, 330));
    posScales.push_back(glm::vec3(20));
    posScales.push_back(glm::vec3(320, 0, 360));
    posScales.push_back(glm::vec3(20));

    entities.push_back(new Box(world,
                                0.0f,
                                glm::vec4(1,1,1,1),
                                glm::vec3(0, 100, 200),
                                glm::vec3(0, 0, 0),
                                glm::vec3(300),
                                tex1)
                       );

    /*entities.push_back(new Sphere(world,
                                    100.0f,
                                    glm::vec4(1,1,1,1),
                                    glm::vec3(400, 100, 0),
                                    glm::vec3(0, 0, 0),
                                    30.0f,
                                    tex1)
                       );*/

    entities.push_back(new Box(world,
                               Mesh::loadObject("res/models/rock.obj"),
                               0.0f,
                               glm::vec4(1,1,1,1),
                               glm::vec3(400, 15, 400),
                               glm::vec3(0),
                               glm::vec3(30),
                               glm::vec3(65, 77, 65),
                               new Texture("res/textures/rockWithGrass.bmp",0)));

    entities.push_back(new Box(world,
                               Mesh::loadObject("res/models/rock.obj"),
                               0.0f,
                               glm::vec4(1,1,1,1),
                               glm::vec3(400, 5, 320),
                               glm::vec3(glm::radians(43.0f), glm::radians(90.0f), 0),
                               glm::vec3(15),
                               glm::vec3(35, 20, 35),
                               new Texture("res/textures/rockWithGrass.bmp",0)));

    entities.push_back(new Box(world,
                               Mesh::loadObject("res/models/rock.obj"),
                               0.0f,
                               glm::vec4(1,1,1,1),
                               glm::vec3(350, 5, 350),
                               glm::vec3(glm::radians(13.0f), glm::radians(90.0f), glm::radians(0.0f)),
                               glm::vec3(15),
                               glm::vec3(35, 30, 35),
                               new Texture("res/textures/rockWithGrass.bmp",0)));

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
                           NULL);

    terrainSimplified = new Entity(world,
                           "surface",
                           terrainMeshSimplified,
                           glm::vec4(1, 1, 1, 1),
                           glm::vec3(0, 0, 0),
                           glm::vec3(0.0f, 0.0f, 0.0f),
                           glm::vec3(4800.0f, 1600.0f, 4800.0f),
                           NULL);

    float lightsize = 600.0f;
    //lights.push_back(new Light(gBuffer, glm::vec3(0.9, 0.5, 0.5), glm::vec3(-30.537, 100.779, -800.82), lightsize));
    //lights.push_back(new Light(gBuffer, glm::vec3(0.2, 0.9, 0.0), glm::vec3(-30.537, 400.779, -800.82), lightsize));

    #if ADD_LIGHTS == 1
    lights.push_back(new Light(glm::vec3(0.3, 0.9, 0.0), glm::vec3(786, 100, -211), lightsize));
    lights.push_back(new Light(glm::vec3(0.9, 0.2, 0.9), glm::vec3(782, 100, -516), lightsize));
    lights.push_back(new Light(glm::vec3(0.5, 0.3, 0.9), glm::vec3(595, 100, -402), lightsize));
    lights.push_back(new Light(glm::vec3(0.9, 0.3, 0.4), glm::vec3(117, 100, 493), lightsize));
    //lights.push_back(new Light(glm::vec3(0.4, 0.3, 0.9), glm::vec3(-105, 100, 619), lightsize));
    lights.push_back(new Light(glm::vec3(0.1, 0.5, 0.7), glm::vec3(-636, 100, 366), lightsize));
    lights.push_back(new Light(glm::vec3(0.4, 0.3, 0.8), glm::vec3(-659, 100, 650), lightsize));
    lights.push_back(new Light(glm::vec3(0.9, 0.3, 0.7), glm::vec3(-386, 100, 481), lightsize));
    lights.push_back(new Light(glm::vec3(0.9, 0.3, 0.8), glm::vec3(4, 100, -104), lightsize));
    lights.push_back(new Light(glm::vec3(1, 0, 0), glm::vec3(11, 100, -447), lightsize));
    lights.push_back(new Light(glm::vec3(1, 0.1, 0.4), glm::vec3(-365, 100, 696), lightsize));
    lights.push_back(new Light(glm::vec3(0.4, 0.2, 1), glm::vec3(995, 100, 717), lightsize));
    lights.push_back(new Light(glm::vec3(0.8, 0, 1), glm::vec3(174, 100, 216), lightsize));
    /*for(int i = 0; i < 20; i++){
        for(int j = 0; j < 20; j++){
            lights.push_back(new Light(gBuffer, glm::vec3(1, 1, 1), glm::vec3(i*500, 100, j*500), lightsize));
        }
    }

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
    lights.push_back(new Light(gBuffer, glm::vec3(1, 0, 1), glm::vec3(10, 10, 10), lightsize));*/
    #endif
    DirectionalLight::setMesh(Mesh::getRectangle());
    sunLight = new DirectionalLight(gBuffer, glm::vec3(1.0, 0.50, 0.2), glm::vec3(1,3,-4.5));

    _near = 1.0f;
    _far = 5000.0f;
    _fov = 75.0f;
    _aspect = this->screenWidth / this->screenHeight;

    projectionMatrix = glm::perspective(glm::radians(_fov), _aspect, _near, _far);
    glm::mat4 orthographicProjectionMatrix = glm::ortho(0.0f, screenWidth, 0.0f, screenHeight);

    deferredLightShader->bind();
    deferredLightShader->loadProjectionMatrix(projectionMatrix);

    forwardLightShaderForWater->bind();
    forwardLightShaderForWater->loadProjectionMatrix(projectionMatrix);
    forwardLightShaderForWater->loadVector3(glGetUniformLocation(forwardLightShaderForWater->getProgram(), "sunLightDir"), glm::normalize(sunLight->getDirection()));
    forwardLightShaderForWater->loadVector3(glGetUniformLocation(forwardLightShaderForWater->getProgram(), "sunLightColor"), sunLight->getColor());
    clipPlaneUniformLocation2 = glGetUniformLocation(forwardLightShaderForWater->getProgram(), "clipPlane");

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

    terrainShaderForWater->bind();
    terrainShaderForWater->loadProjectionMatrix(projectionMatrix);
    glUniform1i(glGetUniformLocation(terrainShaderForWater->getProgram(), "blendmap"), 0);
    glUniform1i(glGetUniformLocation(terrainShaderForWater->getProgram(), "dirt"), 1);
    glUniform1i(glGetUniformLocation(terrainShaderForWater->getProgram(), "grass"), 2);
    glUniform1i(glGetUniformLocation(terrainShaderForWater->getProgram(), "soil"), 3);
    glUniform1i(glGetUniformLocation(terrainShaderForWater->getProgram(), "rock"), 4);

    terrainShaderForWater->loadVector3(glGetUniformLocation(terrainShaderForWater->getProgram(), "sunLightDir"), glm::normalize(sunLight->getDirection()));
    terrainShaderForWater->loadVector3(glGetUniformLocation(terrainShaderForWater->getProgram(), "sunLightColor"), sunLight->getColor());
    clipPlaneUniformLocation = glGetUniformLocation(terrainShaderForWater->getProgram(), "clipPlane");

    treeShaderForWater = new SimpleShader("res/shaders/treeShaderForWater.vs",
                                            "res/shaders/treeShaderForWater.fs");
    treeShaderForWater->bind();
    clipPlaneUniformLocation3 = glGetUniformLocation(treeShaderForWater->getProgram(), "clipPlane");
    treeShaderForWater->loadVector3(glGetUniformLocation(treeShaderForWater->getProgram(), "sunLightDir"), glm::normalize(sunLight->getDirection()));
    treeShaderForWater->loadVector3(glGetUniformLocation(treeShaderForWater->getProgram(), "sunLightColor"), sunLight->getColor());


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
    hBlur = new PostProcess(this->screenWidth/4.0f, this->screenHeight/4.0f, "res/shaders/hBlur.vs", "res/shaders/hBlur.fs");
    wBlur = new PostProcess(this->screenWidth/4.0f, this->screenHeight/4.0f, hBlur->getResultingTextureId(), "res/shaders/wBlur.vs", "res/shaders/wBlur.fs");
    std::vector<std::string> paths = {"res/textures/lensflare/lenscolor.bmp", "lensFlareColorSampler"};
    sunPostProcess = new PostProcess(this->screenWidth/4.0, this->screenHeight/4.0, "res/shaders/sun_postprocess.vs", "res/shaders/sun_postprocess.fs", paths);
    hBlur2 = new PostProcess(this->screenWidth/4.0f, this->screenHeight/4.0f, sunPostProcess->getResultingTextureId(), "res/shaders/hBlur.vs", "res/shaders/hBlur.fs");
    wBlur2 = new PostProcess(this->screenWidth/4.0f, this->screenHeight/4.0f, hBlur2->getResultingTextureId(), "res/shaders/wBlur.vs", "res/shaders/wBlur.fs");

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
                               glm::vec3(-227.019, 60.7267, 536.942),
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

    posRotScale.push_back(glm::vec3(787, 60, -516));
    posRotScale.push_back(glm::vec3(0, glm::radians(36.0f), 0));
    posRotScale.push_back(glm::vec3(50.0f));

    posRotScale.push_back(glm::vec3(602, 30, -406));
    posRotScale.push_back(glm::vec3(0, glm::radians(110.0f), 0));
    posRotScale.push_back(glm::vec3(40.0f));

    posRotScale.push_back(glm::vec3(787, 40, -216));
    posRotScale.push_back(glm::vec3(0, glm::radians(160.0f), 0));
    posRotScale.push_back(glm::vec3(60.0f));

    posRotScale.push_back(glm::vec3(-690.0f, 10.0, 302.0f));
    posRotScale.push_back(glm::vec3(0));
    posRotScale.push_back(glm::vec3(50.0f));

    posRotScale.push_back(glm::vec3(-700.0f, 20.0, 402.0f));
    posRotScale.push_back(glm::vec3(0));
    posRotScale.push_back(glm::vec3(30.0f));

    posRotScale.push_back(glm::vec3(-790.0f, 26.0, 502.0f));
    posRotScale.push_back(glm::vec3(0, glm::radians(123.0f), 0));
    posRotScale.push_back(glm::vec3(40.0f));

    posRotScale.push_back(glm::vec3(-690.0f, 30.0, 602.0f));
    posRotScale.push_back(glm::vec3(0, glm::radians(53.0f), 0));
    posRotScale.push_back(glm::vec3(50.0f));

    posRotScale.push_back(glm::vec3(-340.0f, 10.0, 302.0f));
    posRotScale.push_back(glm::vec3(0, glm::radians(102.0f), 0));
    posRotScale.push_back(glm::vec3(40.0f));

    posRotScale.push_back(glm::vec3(-690.0f, 30.0, 702.0f));
    posRotScale.push_back(glm::vec3(0, glm::radians(156.0f), 0));
    posRotScale.push_back(glm::vec3(50.0f));

    posRotScale.push_back(glm::vec3(-590.0f, 10.0, 402.0f));
    posRotScale.push_back(glm::vec3(0, glm::radians(243.0f), 0));
    posRotScale.push_back(glm::vec3(40.0f));

    posRotScale.push_back(glm::vec3(-390.0f, 10.0, 602.0f));
    posRotScale.push_back(glm::vec3(0, glm::radians(40.0f), 0));
    posRotScale.push_back(glm::vec3(50.0f));

    posRotScale.push_back(glm::vec3(-290.0f, 30.0, 702.0f));
    posRotScale.push_back(glm::vec3(0, glm::radians(303.0f), 0));
    posRotScale.push_back(glm::vec3(50.0f));

    posRotScale.push_back(glm::vec3(-490.0f, 10.0, 402.0f));
    posRotScale.push_back(glm::vec3(0, glm::radians(143.0f), 0));
    posRotScale.push_back(glm::vec3(50.0f));

    posRotScale.push_back(glm::vec3(-90.0f, 10.0, 602.0f));
    posRotScale.push_back(glm::vec3(0, glm::radians(203.0f), 0));
    posRotScale.push_back(glm::vec3(50.0f));

    posRotScale.push_back(glm::vec3(-390.0f, 10.0, 402.0f));
    posRotScale.push_back(glm::vec3(0, glm::radians(156.0f), 0));
    posRotScale.push_back(glm::vec3(50.0f));

    posRotScale.push_back(glm::vec3(110.0f, 5.0, 502.0f));
    posRotScale.push_back(glm::vec3(0));
    posRotScale.push_back(glm::vec3(50.0f));

    posRotScale.push_back(glm::vec3(590.0f, 0.0, 602.0f));
    posRotScale.push_back(glm::vec3(0));
    posRotScale.push_back(glm::vec3(50.0f));

    posRotScale.push_back(glm::vec3(990.0f, 0.0, 720.0f));
    posRotScale.push_back(glm::vec3(0, glm::radians(250.0f), 0));
    posRotScale.push_back(glm::vec3(60.0f));

    posRotScale.push_back(glm::vec3(110.0f, 10.0, 702.0f));
    posRotScale.push_back(glm::vec3(0));
    posRotScale.push_back(glm::vec3(50.0f));

    posRotScale.push_back(glm::vec3(-690.0f, 20.0, -102.0f));
    posRotScale.push_back(glm::vec3(0, glm::radians(321.0f), 0));
    posRotScale.push_back(glm::vec3(50.0f));

    posRotScale.push_back(glm::vec3(-390.0f, 30.0, 602.0f));
    posRotScale.push_back(glm::vec3(0));
    posRotScale.push_back(glm::vec3(50.0f));

    posRotScale.push_back(glm::vec3(-390.0f, 45.0, 802.0f));
    posRotScale.push_back(glm::vec3(0));
    posRotScale.push_back(glm::vec3(50.0f));

    posRotScale.push_back(glm::vec3(-290.0f, 30.0, 780.0f));
    posRotScale.push_back(glm::vec3(0));
    posRotScale.push_back(glm::vec3(50.0f));

    posRotScale.push_back(glm::vec3(0, 30.0, -580));
    posRotScale.push_back(glm::vec3(0));
    posRotScale.push_back(glm::vec3(50.0f));

    for (int i = 0; i < posRotScale.size(); i+=3) {
        glm::vec3 position = posRotScale[i];
        glm::vec3 rotation = posRotScale[i+1];
        glm::vec3 scale(posRotScale[i+2].x/6.0f, posRotScale[i+2].y*3, posRotScale[i+2].z/6.0f);

        addBoundingBox(0, position, rotation, scale);
    }

    treeRenderer = new TreeRenderer(posRotScale, new Tree("tree", treeTrunk, treeBranch), projectionMatrix);
    treeRendererSimplified = new TreeRenderer(posRotScale,
                                              new Tree("tree", treeTrunkSimplified, treeBranchSimplified),
                                              projectionMatrix,
                                              treeShaderForWater);
    grassRenderer = new GrassRenderer(posScales, Mesh::loadObject("res/models/grassTry.obj"), grassBillboard, projectionMatrix);
    waterRenderer = new WaterRenderer(this->screenWidth/8.0, this->screenHeight/8.0, glm::vec3(447, 25, -200), glm::vec3(100, 1, 100), projectionMatrix);
    Game::score = 0;

    #ifdef ONLINE
    NetworkedPlayer::setMesh(Mesh::loadObject("res/models/capsule2.obj"));
    std::vector<PlayerInfo> players = client->getOtherPlayersInfo();
    for (int i = 0; i < players.size(); i++) {
        glm::vec3 pos(players[i].posX, players[i].posY, players[i].posZ);
        glm::vec3 fwd(players[i].fwdX, players[i].fwdY, players[i].fwdZ);
        otherPlayers[players[i].id] = new NetworkedPlayer(world, 30.0f, pos, fwd);
    }
    #endif

    std::cout << "pickable=" << pickableObjects.size() << std::endl;

    glEnable(GL_CLIP_PLANE0);
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
        fwd = fwd.normalized() * player->getForce();
        player->getRigidBody()->applyCentralForce(fwd);
    }else if(input->getKey(SDLK_s)){
        btVector3 fwd = btVector3(forward.x, 0, forward.z);
        fwd = fwd.normalized() * -player->getForce();
        player->getRigidBody()->applyCentralForce(fwd);
    }

    if(input->getKey(SDLK_a)){
        btVector3 rgh = btVector3(right.x, 0, right.z);
        rgh = rgh.normalized() * -player->getForce();
        player->getRigidBody()->applyCentralForce(rgh);
    }else if(input->getKey(SDLK_d)){
        btVector3 rgh = btVector3(right.x, 0, right.z);
        rgh = rgh.normalized() * player->getForce();
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
        game->lights.push_back(new Light(glm::vec3(r,g,b), glm::vec3(pos.x, 100, pos.z), 600.0f));
    }

    if(input->getKeyDown(SDLK_t)){
        //how to move a rigid body without breaking the engine
        btTransform transform = player->getRigidBody()->getCenterOfMassTransform();
        transform.setOrigin(btVector3(0, 300, 200));
        player->getRigidBody()->setCenterOfMassTransform(transform);
    }

    if (input->getKeyDown(SDLK_p)) {
        std::cout << "set warp" << std::endl;
        input->setWarpMouse(!input->getWarpMouse());
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

    glDepthMask(GL_TRUE);
    waterRenderer->bindForReflectionTexture();
    {
        float length = glm::abs(camera->getPosition().y - waterRenderer->getWaterHeight());
        //muta camera pe OY cu -2*length, inverseaza forward si randeaza scena din acest unghi pentru reflexie
        camera->moveUp(-2*length);
        camera->invertForward();
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
        glCullFace(GL_BACK);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        blendmap->use();
        soil->use();
        rock->use();
        grass->use();
        dirt->use();
        terrainShaderForWater->bind();
        terrainShaderForWater->loadViewMatrix(camera->getViewMatrix());
        terrainShaderForWater->loadVector4(clipPlaneUniformLocation, glm::vec4(0, 1, 0, -waterRenderer->getWaterHeight()));
        terrainSimplified->draw(terrainShaderForWater);

        treeShaderForWater->loadVector4(clipPlaneUniformLocation3, glm::vec4(0, 1, 0, -waterRenderer->getWaterHeight()));
        treeRendererSimplified->draw(camera->getViewMatrix());

        forwardLightShaderForWater->bind();
        forwardLightShaderForWater->loadVector4(clipPlaneUniformLocation2, glm::vec4(0, 1, 0, -waterRenderer->getWaterHeight()));
        forwardLightShaderForWater->loadViewMatrix(camera->getViewMatrix());

        for(Entity* e : entities){
            e->draw(forwardLightShaderForWater);
        }
        for(PuzzleObject *puzzleObject : puzzleObjects){
            puzzleObject->draw(forwardLightShaderForWater);
        }

        camera->invertForward();
        camera->moveUp(2*length);
    }
    waterRenderer->unbind();

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
        grassRenderer->draw(camera->getViewMatrix());
        waterRenderer->draw(camera->getViewMatrix());

        deferredLightShader->bind();
        deferredLightShader->loadViewMatrix(camera->getViewMatrix());

        for(Entity* e : entities){
            e->draw(deferredLightShader);
        }
        for(PuzzleObject *puzzleObject : puzzleObjects){
            puzzleObject->draw(deferredLightShader);
        }
        for(auto it : otherPlayers) {
            it.second->draw(deferredLightShader);
        }
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
        glBlitFramebuffer(0, 0, this->screenWidth, this->screenHeight, 0, 0, this->screenWidth/4.0, this->screenHeight/4.0, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
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
        glm::vec3 sunPosition = calculateSunPosition(projectionMatrix,
                                                     camera->getViewMatrix(),
                                                     sun->getModelMatrix(),
                                                     this->screenWidth,
                                                     this->screenHeight);
        glUniform3f(glGetUniformLocation(sunPostProcess->getShader().getProgram(), "sunPosition"), sunPosition.x, sunPosition.y, sunPosition.z);
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
        char s[32];
        snprintf(s, 32, "ms since last frame %ld", Display::getDelta());
        fpsText->display(std::string(s));
        fpsText->draw(textShader);
        snprintf(s, 32, "lights unculled %d", numOfLightsVisible);
        lightsText->display(std::string(s));
        lightsText->draw(textShader);
        if (Game::score < 100) {
            snprintf(s, 32, "Score %d", (int)floor(Game::score));
        } else {
            strcpy(s, "Level complete!");
        }
        scoreText->display(std::string(s));
        scoreText->draw(textShader);
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
    particleRenderer->update(camera, turretPuzzleObject->getEntities()[1], particleInteractors, goal);
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
        #ifdef ONLINE
        manageUpdateFromServer();
        #endif
        resetAll();

        timeAccumulator += Display::getDelta();
        while(timeAccumulator >= discreteChunk){
            world->stepSimulation(btScalar(0.5f));
            timeAccumulator -= discreteChunk;
        }

        nonTimeCriticalInput();
        update();
        render();

        #ifdef ONLINE
        client->sendPosAndFwd(player->getPosition(), camera->getForward());
        #endif
    }
}

void Game::manageUpdateFromServer() {
    char *buffer = NULL;
    size_t len;
    while(client->receivePacketNonBlocking(buffer, len) > 0) {
        if (buffer){
            if (buffer[0] == GAMESTATE) {
                int numOfPlayers;
                PlayerInfo *playersInfo;
                memcpy(&numOfPlayers, buffer+1, sizeof(int));
                playersInfo = (PlayerInfo*)malloc(sizeof(PlayerInfo) * numOfPlayers);
                memcpy(playersInfo, buffer + 1 + sizeof(int), sizeof(PlayerInfo) * numOfPlayers);
                for (int i = 0; i < numOfPlayers; i++) {
                    if (playersInfo[i].id != client->getMyId()) {
                        NetworkedPlayer *otherPlayer = otherPlayers[playersInfo[i].id];
                        if (otherPlayer) {
                            //std::cout << playersInfo[i].posX << " " << playersInfo[i].posY << " " << playersInfo[i].posZ << std::endl;
                            otherPlayer->setPosition(playersInfo[i].posX, playersInfo[i].posY, playersInfo[i].posZ);
                            otherPlayer->setForward(glm::vec3(playersInfo[i].fwdX, playersInfo[i].fwdY, playersInfo[i].fwdZ));
                        } else {
                            otherPlayers[playersInfo[i].id] = new NetworkedPlayer(world,
                                                                                  30.0f,
                                                                                  glm::vec3(playersInfo[i].posX, playersInfo[i].posY, playersInfo[i].posZ),
                                                                                  glm::vec3(playersInfo[i].fwdX, playersInfo[i].fwdY, playersInfo[i].fwdZ));
                        }
                    }
                }
                free(playersInfo);
            }
            free(buffer);
        } else {
            std::cout << "buffer is null!" << std::endl;
        }
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

glm::vec3 Game::calculateSunPosition(const glm::mat4& projectionMatrix,
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
    pos = glm::vec4(pos.x/pos.w, pos.y/pos.w, pos.z/pos.w, 0);
    glm::vec3 posXYZ = glm::vec3((pos.x + 1)/2.0, (pos.y+1)/2.0, (pos.z+1)/2.0);
    return posXYZ;
}

void Game::addBoundingBox(float mass, glm::vec3& position, glm::vec3& rotation, glm::vec3& scale) {
    btTransform t;
    btVector3 inertia(0, 0, 0);
    btRigidBody *m_body;

    t.setIdentity();
    t.setOrigin(btVector3(position.x, position.y, position.z));
    t.setRotation(btQuaternion(rotation.y, rotation.x, rotation.z));

    btBoxShape* boxShape = new btBoxShape(btVector3(scale.x, scale.y, scale.z));
    if(mass != 0.0){
        boxShape->calculateLocalInertia(btScalar(mass), inertia);
    }
    btMotionState* motion = new btDefaultMotionState(t);
    btRigidBody::btRigidBodyConstructionInfo info(mass, motion, boxShape, inertia);
    info.m_restitution = 0.3;
    info.m_friction = 1.0f;
    UserPointer *userPointer = new UserPointer();
    userPointer->type = BOX;
    userPointer->ptrType.box = NULL;
    m_body = new btRigidBody(info);
    m_body->setUserPointer((void*)userPointer);
    m_body->setDamping(btScalar(0.2), btScalar(0.6));

    world->addRigidBody(m_body);
}

Game::~Game()
{
    /*for (PickableObject *ob : pickableObjects){
        std::cout << ob->getName() << " " << ob->getEntities()[0]->getPosition().x << " " << ob->getEntities()[0]->getPosition().y << " " << ob->getEntities()[0]->getPosition().z << std::endl;
    }*/
    std::cout << player->getPosition().x << " " << player->getPosition().y << " " << player->getPosition().z << std::endl;

    /*for(Light *l : lights){
        std::cout << l->getPosition().x << " " << l->getPosition().z << std::endl;
    }*/

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
    delete grassRenderer;
    delete waterRenderer;

    delete terrainMeshSimplified;
    delete forwardLightShaderForWater;
    delete terrainSimplified;
    delete treeRendererSimplified;
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
