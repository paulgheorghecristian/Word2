#include "game.h"

#define RENDER_LIGHTS 1
#define RENDER_GEOMETRY 1

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
    boxMesh = Mesh::loadObject("res/models/cube4.obj");
    sphereMesh = Mesh::loadObject("res/models/sphere4.obj");
    lightMesh = Mesh::loadObject("res/models/lightsphere.obj");
    turretMesh = Mesh::loadObject("res/models/turret3.obj");
    baseMesh = Mesh::loadObject("res/models/base3.obj");
    fanMesh = Mesh::loadObject("res/models/fan2.obj");
    fanBaseMesh = Mesh::loadObject("res/models/fanBase2.obj");
    treeTrunk = Mesh::loadObject("res/models/tree2/tree.obj");
    treeBranch = Mesh::loadObject("res/models/tree2/branch1.obj");
    treeBranch2 = Mesh::loadObject("res/models/tree2/branch2.obj");
    treeBranch3 = Mesh::loadObject("res/models/tree2/branch3.obj");
    treeBranch4 = Mesh::loadObject("res/models/tree2/branch4.obj");
    treeBranch5 = Mesh::loadObject("res/models/tree2/branch5.obj");
    treeBranch6 = Mesh::loadObject("res/models/tree2/branch6.obj");
    treeBranch7 = Mesh::loadObject("res/models/tree2/branch7.obj");
    treeBranch8 = Mesh::loadObject("res/models/tree2/branch8.obj");
    treeBranch9 = Mesh::loadObject("res/models/tree2/branch9.obj");
    treeBranch10 = Mesh::loadObject("res/models/tree2/branch10.obj");
    treeBranch11 = Mesh::loadObject("res/models/tree2/branch11.obj");
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
                     Mesh::getCircle(0, 0, 300.0, 50),
                     glm::vec4(0.9, 0.7, 0.5, 1),
                     glm::vec3(this->screenWidth/2.0f+50,this->screenHeight/2.0f+300,-4000),
                     glm::vec3(0),
                     glm::vec3(1),
                    NULL);

    entities.push_back(new Box(world,
                                1000.0f,
                                glm::vec4(1,0,1,1),
                                glm::vec3(0, 200, 200),
                                glm::vec3(0, 0, 0),
                                glm::vec3(300),
                                tex1)
                       );

    entities.push_back(new Sphere(world,
                                    100.0f,
                                    glm::vec4(1,1,0,1),
                                    glm::vec3(100, 100, 0),
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
                           glm::vec4(0.5, 0.5, 0.5, 1),
                           glm::vec3(0, 0, 0),
                           glm::vec3(0.0f, 0.0f, 0.0f),
                           glm::vec3(4800.0f, 1600.0f, 4800.0f),
                           NULL
                        );

    entities.push_back(new Entity(world,
                                  "tree",
                                  treeTrunk,
                                  glm::vec4(1,1,1,1),
                                  glm::vec3(300, 10, 0),
                                  glm::vec3(0),
                                  glm::vec3(50),
                                  bark)
                       );

    branches.push_back(new Entity(world,
                                  "branch",
                                  treeBranch,
                                  glm::vec4(1,1,1,1),
                                  glm::vec3(300, 10, 0),
                                  glm::vec3(0),
                                  glm::vec3(50),
                                  leaf)
                       );
     branches.push_back(new Entity(world,
                                  "branch",
                                  treeBranch2,
                                  glm::vec4(1,1,1,1),
                                  glm::vec3(300, 10, 0),
                                  glm::vec3(0),
                                  glm::vec3(50),
                                  leaf)
                       );
     branches.push_back(new Entity(world,
                                  "branch",
                                  treeBranch3,
                                  glm::vec4(1,1,1,1),
                                  glm::vec3(300, 10, 0),
                                  glm::vec3(0),
                                  glm::vec3(50),
                                  leaf)
                       );
     branches.push_back(new Entity(world,
                                  "branch",
                                  treeBranch4,
                                  glm::vec4(1,1,1,1),
                                  glm::vec3(300, 10, 0),
                                  glm::vec3(0),
                                  glm::vec3(50),
                                  leaf)
                       );
     branches.push_back(new Entity(world,
                                  "branch",
                                  treeBranch5,
                                  glm::vec4(1,1,1,1),
                                  glm::vec3(300, 10, 0),
                                  glm::vec3(0),
                                  glm::vec3(50),
                                  leaf)
                       );
     branches.push_back(new Entity(world,
                                  "branch",
                                  treeBranch6,
                                  glm::vec4(1,1,1,1),
                                  glm::vec3(300, 10, 0),
                                  glm::vec3(0),
                                  glm::vec3(50),
                                  leaf)
                       );
     branches.push_back(new Entity(world,
                                  "branch",
                                  treeBranch7,
                                  glm::vec4(1,1,1,1),
                                  glm::vec3(300, 10, 0),
                                  glm::vec3(0),
                                  glm::vec3(50),
                                  leaf)
                       );
     branches.push_back(new Entity(world,
                                  "branch",
                                  treeBranch8,
                                  glm::vec4(1,1,1,1),
                                  glm::vec3(300, 10, 0),
                                  glm::vec3(0),
                                  glm::vec3(50),
                                  leaf)
                       );
     branches.push_back(new Entity(world,
                                  "branch",
                                  treeBranch9,
                                  glm::vec4(1,1,1,1),
                                  glm::vec3(300, 10, 0),
                                  glm::vec3(0),
                                  glm::vec3(50),
                                  leaf)
                       );
     branches.push_back(new Entity(world,
                                  "branch",
                                  treeBranch10,
                                  glm::vec4(1,1,1,1),
                                  glm::vec3(300, 10, 0),
                                  glm::vec3(0),
                                  glm::vec3(50),
                                  leaf)
                       );
     branches.push_back(new Entity(world,
                                  "branch",
                                  treeBranch11,
                                  glm::vec4(1,1,1,1),
                                  glm::vec3(300, 10, 0),
                                  glm::vec3(0),
                                  glm::vec3(50),
                                  leaf)
                       );



    float lightsize = 600.0f;
    /*for(int i = 0; i < 10; i++){
        for(int j = 0; j < 10; j++){
            lights.push_back(new Light(gBuffer, glm::vec3(1, 1, 1), glm::vec3(i*500, 100, j*500), lightsize));
        }
    }*/
    /*lights.push_back(new Light(gBuffer, glm::vec3(0.3, 0.9, 0.0), glm::vec3(0, 100, 400), lightsize));
    lights.push_back(new Light(gBuffer, glm::vec3(0.9, 0.9, 0.9), glm::vec3(0, 100, 0), lightsize));
    lights.push_back(new Light(gBuffer, glm::vec3(0.9, 0.9, 0.9), glm::vec3(100, 100, 200), lightsize));*/

    /*lights.push_back(new Light(gBuffer, glm::vec3(0.9, 0.3, 0.9), glm::vec3(-400, 100, -200), lightsize));
    lights.push_back(new Light(gBuffer, glm::vec3(0.4, 0.9, 0.9), glm::vec3(400, 100, -200), lightsize));
    lights.push_back(new Light(gBuffer, glm::vec3(0.1, 0.9, 0.4), glm::vec3(0, 100, -200), lightsize));
    lights.push_back(new Light(gBuffer, glm::vec3(0.4, 0.9, 0.2), glm::vec3(-400, 100, -400), lightsize));
    lights.push_back(new Light(gBuffer, glm::vec3(0.9, 0.9, 0.3), glm::vec3(400, 100, -400), lightsize));
    lights.push_back(new Light(gBuffer, glm::vec3(0.9, 0.4, 0.9), glm::vec3(0, 100, -400), lightsize));

    lights.push_back(new Light(gBuffer, glm::vec3(1, 0, 1), glm::vec3(500, 100, -100), lightsize));
    lights.push_back(new Light(gBuffer, glm::vec3(1, 1, 1), glm::vec3(500, 100, 400), lightsize));
    lights.push_back(new Light(gBuffer, glm::vec3(1, 1, 0), glm::vec3(500, 100, -1000), lightsize));
    lights.push_back(new Light(gBuffer, glm::vec3(1, 0, 1), glm::vec3(30, 100, 500), lightsize));
    lights.push_back(new Light(gBuffer, glm::vec3(0.1, 1, 0.4), glm::vec3(20, 30, 100), lightsize));
    lights.push_back(new Light(gBuffer, glm::vec3(1, 0.6, 0), glm::vec3(300, 100, -100), lightsize));
    lights.push_back(new Light(gBuffer, glm::vec3(1, 0, 1), glm::vec3(10, 100, -10), lightsize));
    /*lights.push_back(new Light(gBuffer, glm::vec3(1, 0, 1), glm::vec3(-30, 100, 10), lightsize));
    lights.push_back(new Light(gBuffer, glm::vec3(0.1, 1, 0.4), glm::vec3(20, 30, 100), lightsize));
    lights.push_back(new Light(gBuffer, glm::vec3(1, 0.6, 0), glm::vec3(-300, 100, -10), lightsize));
    lights.push_back(new Light(gBuffer, glm::vec3(1, 0, 1), glm::vec3(10, 10, 10), lightsize));
    lights.push_back(new Light(gBuffer, glm::vec3(1, 0, 1), glm::vec3(-30, 100, -10), lightsize));
    lights.push_back(new Light(gBuffer, glm::vec3(0.1, 1, 0.4), glm::vec3(-20, 30, -100), lightsize));
    lights.push_back(new Light(gBuffer, glm::vec3(1, 0.6, 0), glm::vec3(-30, 100, -100), lightsize));
    lights.push_back(new Light(gBuffer, glm::vec3(1, 0, 1), glm::vec3(10, 10, 10), lightsize));*/

    DirectionalLight::setMesh(Mesh::getRectangle());
    sunLight = new DirectionalLight(gBuffer, glm::vec3(1.0, 0.50, 0.2), glm::vec3(1,5,-4.5));

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
                               glm::vec4(0.8, 0.8, 0.5, 1),
                               glm::vec3(0, 100.0, 0),
                               glm::vec3(glm::radians(90.0f), 0.0f, glm::radians(90.0f)),
                               glm::vec3(30.0f),
                               NULL)
                        );

    fanEntities.push_back(new Entity(world,
                               "fan",
                               fanMesh,
                               glm::vec4(1.0, 0.5, 0.5, 1),
                               glm::vec3(10200, 1.5, 0),
                               glm::vec3(glm::radians(90.0f), 0.0f, glm::radians(90.0f)),
                               glm::vec3(30.0f),
                               NULL)
                        );

    fanPuzzleObject = new PuzzleObject(world, "fan", fanEntities, [](PuzzleObject* obj){
                                                                    glm::vec3 pos = obj->getEntities()[0]->getPosition();
                                                                    obj->getEntities()[1]->setPosition(pos.x, pos.y+0.9f, pos.z);
                                                                    obj->getEntities()[1]->addRotation(0, 0, 0.5);
                                                                }
                                    );

    fanPuzzleObject->boundingRectangle[0] = glm::vec3(-0.656439, -0.656439, 0.096338);
    fanPuzzleObject->boundingRectangle[1] = glm::vec3(0.656439, -0.656439, 0.096338);
    fanPuzzleObject->boundingRectangle[2] = glm::vec3(0.656439, 0.656439, 0.096337);
    fanPuzzleObject->boundingRectangle[3] = glm::vec3(-0.656439, 0.656439, 0.096337);

    std::vector<Entity*> turretEntities;

    turretEntities.push_back(new Entity(world,
                               "base",
                               baseMesh,
                               glm::vec4(0.8, 0.8, 0.5, 1),
                               glm::vec3(0, 100.0, -100),
                               glm::vec3(0.0f, 0.0f, 0.0f),
                               glm::vec3(60.0f),
                               NULL)
                        );

    turretEntities.push_back(new Entity(world,
                               "turret",
                               turretMesh,
                               glm::vec4(0.5, 0.9, 0.5, 1),
                               glm::vec3(0, 68.0f, -100),
                               glm::vec3(0.0f, 0.0f, 0.0f),
                               glm::vec3(30.0f),
                               NULL)
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
                                           }
                                        );
    particleRenderer = new ParticleRenderer(projectionMatrix, turretPuzzleObject->getEntities()[1]->getPosition(), 1000);
    particlePostProcess = new PostProcess(this->screenWidth, this->screenHeight, "particles/post_process.vs", "particles/post_process.fs");
    hBlur = new PostProcess(this->screenWidth/4.0f, this->screenHeight/4.0f, particlePostProcess->getResultingTextureId(), "res/shaders/hBlur.vs", "res/shaders/hBlur.fs");
    wBlur = new PostProcess(this->screenWidth/4.0f, this->screenHeight/4.0f, hBlur->getResultingTextureId(), "res/shaders/wBlur.vs", "res/shaders/wBlur.fs");
    sunPostProcess = new PostProcess(this->screenWidth/4.0f, this->screenHeight/4.0f, "res/shaders/sun_postprocess.vs", "res/shaders/sun_postprocess.fs");
    hBlur2 = new PostProcess(this->screenWidth/4.0f, this->screenHeight/4.0f, sunPostProcess->getResultingTextureId(), "res/shaders/hBlur.vs", "res/shaders/hBlur.fs");
    wBlur2 = new PostProcess(this->screenWidth/4.0f, this->screenHeight/4.0f, hBlur2->getResultingTextureId(), "res/shaders/wBlur.vs", "res/shaders/wBlur.fs");
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

    if(input->getMouseDown(1)){
        Box* box = new Box(game->getWorld(),
                                    100.0f,
                                    glm::vec4(1,0,0,1),
                                    camera->getPosition() + camera->getForward() * 100.0f,
                                    glm::vec3(0, 0, 0),
                                    glm::vec3(30.0f),
                                    game->tex2);
        box->setLinearVelocity(camera->getForward() * 100.0f);
        game->getEntities().push_back(box);
    }

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

    if(input->getKey(SDLK_e)){
        float distance = glm::distance(player->getPosition(), game->turretPuzzleObject->getEntities()[0]->getPosition());
        if(distance < 80.0f){
            game->turretPuzzleObject->executeAction(0);
        }
    }

    if(input->getKey(SDLK_r)){
        float distance = glm::distance(player->getPosition(), game->turretPuzzleObject->getEntities()[0]->getPosition());
        if(distance < 80.0f){
            game->turretPuzzleObject->executeAction(1);
        }
    }

    /*if(input->getKeyDown(SDLK_1)){
        //how to move a rigid body without breaking the engine
        btTransform transform = player->getRigidBody()->getCenterOfMassTransform();
        transform.setOrigin(btVector3(0, 300, 200));
        player->getRigidBody()->setCenterOfMassTransform(transform);
    }*/
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

        blendmap->use();
        soil->use();
        rock->use();
        grass->use();
        dirt->use();
        terrainShader->bind();
        terrainShader->loadViewMatrix(camera->getViewMatrix());
        terrain->draw(terrainShader);

        deferredLightShader->bind();
        deferredLightShader->loadViewMatrix(camera->getViewMatrix());

        for(Entity* e : entities){
            e->draw(deferredLightShader);
        }
        glDisable(GL_CULL_FACE);
        for(Entity *b : branches){
            b->draw(deferredLightShader);
        }

        fanPuzzleObject->draw(deferredLightShader);
        turretPuzzleObject->draw(deferredLightShader);
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
        glBindFramebuffer(GL_READ_FRAMEBUFFER, gBuffer->getFrameBufferObject());
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, sunPostProcess->getFrameBufferObject());
        glBlitFramebuffer(0, 0, this->screenWidth, this->screenHeight, 0, 0, this->screenWidth/4.0f, this->screenHeight/4.0f, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, particlePostProcess->getFrameBufferObject());
        glBlitFramebuffer(0, 0, this->screenWidth, this->screenHeight, 0, 0, this->screenWidth, this->screenHeight, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
        glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        particlePostProcess->bind();
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        particleRenderer->draw();
        particlePostProcess->process();
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);
        hBlur->bind();
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
    }

    {
        glDisable(GL_CULL_FACE);
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_STENCIL_TEST);
		glDisable(GL_BLEND);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        simpleShader->bind();
        glActiveTexture(GL_TEXTURE0+11);
        glBindTexture(GL_TEXTURE_2D, gBuffer->getColorTexture());
        glUniform1i(glGetUniformLocation(simpleShader->getProgram(), "colorSampler"), 11);
        glActiveTexture(GL_TEXTURE0+12);
        glBindTexture(GL_TEXTURE_2D, gBuffer->getDepthTexture());
        glUniform1i(glGetUniformLocation(simpleShader->getProgram(), "depthSampler"), 12);
        glActiveTexture(GL_TEXTURE0+14);
        glBindTexture(GL_TEXTURE_2D, gBuffer->getNormalTexture());
        glUniform1i(glGetUniformLocation(simpleShader->getProgram(), "normalSampler"), 14);
        glActiveTexture(GL_TEXTURE0+15);
        glBindTexture(GL_TEXTURE_2D, gBuffer->getLightAccumulationTexture());
        glUniform1i(glGetUniformLocation(simpleShader->getProgram(), "lightSampler"), 15);
        glActiveTexture(GL_TEXTURE0+10);
        glBindTexture(GL_TEXTURE_2D, wBlur->getResultingTextureId());
        glUniform1i(glGetUniformLocation(simpleShader->getProgram(), "particlesPostProcessSampler"), 10);
        glActiveTexture(GL_TEXTURE0+9);
        glBindTexture(GL_TEXTURE_2D, particlePostProcess->getResultingTextureId());
        glUniform1i(glGetUniformLocation(simpleShader->getProgram(), "particlesSampler"), 9);
        glActiveTexture(GL_TEXTURE0+8);
        glBindTexture(GL_TEXTURE_2D, wBlur2->getResultingTextureId());
        glUniform1i(glGetUniformLocation(simpleShader->getProgram(), "sunPostProcessSampler"), 8);
        glUniform1i(glGetUniformLocation(simpleShader->getProgram(), "outputType"), outputType);
        screenRectangle->draw(simpleShader);

        glDisable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        textShader->bind();
        fpsText->displayNumber(Display::getDelta());
        fpsText->draw(textShader);
        lightsText->displayNumber(numOfLightsVisible);
        lightsText->draw(textShader);
    }
    display->update();
}

void Game::update(){
    camera->setPosition(player->getPosition() + glm::vec3(0, 20, 0));

    player->performRayTest();
    fanPuzzleObject->update();
    turretPuzzleObject->update();
    particleRenderer->update(Display::getDelta(), camera, turretPuzzleObject->getEntities()[1], fanPuzzleObject);
}

void Game::run(){
    while(!display->isClosed()){
        display->setLastFrameTime(SDL_GetTicks());

        timeAccumulator += Display::getDelta();
        while(timeAccumulator >= discreteChunk){
            world->stepSimulation(btScalar(0.5f));
            timeAccumulator -= discreteChunk;
        }
        update();
        render();
    }
}

void Game::bulletTickCallback(btDynamicsWorld *world, btScalar timeStep) {
    Game* thisGame = (Game*)world->getWorldUserInfo();
    handleInput(thisGame);
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

    /*btTransform t;

    t.setIdentity();
    t.setOrigin(btVector3(0, 0, 0));

    btStaticPlaneShape* plane = new btStaticPlaneShape(btVector3(0, 1, 0), btScalar(0));
    btMotionState* motion = new btDefaultMotionState(t);
    btRigidBody::btRigidBodyConstructionInfo info(0.0, motion, plane);
    info.m_restitution = 0.3;
    info.m_friction = 1.0;
    btRigidBody *body = new btRigidBody(info);
    world->addRigidBody(body);*/

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
    isClosed = true;
    cullLightsThread.detach();
    std::cout << "Destroying game..." << std::endl;
    delete display;
    delete simpleShader;
    delete simpleShaderForLights;
    delete emptyShader;
    delete deferredLightShader;
    delete input;
    delete camera;
    delete particleRenderer;
    delete particlePostProcess;
    delete hBlur;
    delete wBlur;

    std::cout << "Freeing " << entities.size() << " entities..." << std::endl;

    for(Entity* e : entities){
        delete e;
    }
    for(Light* l : lights){
        delete l;
    }
    delete fanPuzzleObject;
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
