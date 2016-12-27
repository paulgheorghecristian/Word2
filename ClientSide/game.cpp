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
    input = new Input();
    simpleShader = new SimpleShader("res/shaders/vertex", "res/shaders/fragment");
    simpleShaderForLights = new SimpleShader("res/shaders/vertex_light", "res/shaders/fragment_light");
    deferredLightShader = new DeferredLightShader("res/shaders/vertex_def", "res/shaders/fragment_def");
    emptyShader = new SimpleShader("res/shaders/vertex_empty", "res/shaders/fragment_empty");
    boxMesh = Mesh::loadObject("res/models/cube4.obj");
    sphereMesh = Mesh::loadObject("res/models/sphere4.obj");
    lightMesh = Mesh::loadObject("res/models/lightsphere.obj");
    turretMesh = Mesh::loadObject("res/models/turret.obj");
    baseMesh = Mesh::loadObject("res/models/base.obj");
    fanMesh = Mesh::loadObject("res/models/fan.obj");
    fanBaseMesh = Mesh::loadObject("res/models/fanBase.obj");
    textShader = new TextShader("res/shaders/text_vs", "res/shaders/text_fs");
    player = new Player(world, 30.0f, glm::vec3(0.0f, 30.0f, 0.0f), glm::vec3(10));
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

    entities.push_back(new Entity(world,
                               "surface",
                               Mesh::getSurface(500, 500),
                               glm::vec4(0.5, 0.5, 0.5, 1),
                               glm::vec3(0, 0, 0),
                               glm::vec3(0.0f, 0.0f, 0.0f),
                               glm::vec3(50000.0f, 1, 50000.0f),
                               NULL)
                        );

    entities.push_back(new Entity(world,
                               "turret",
                               turretMesh,
                               glm::vec4(0.5, 0.5, 0.5, 1),
                               glm::vec3(0, 40, 0),
                               glm::vec3(0.0f, 0.0f, 0.0f),
                               glm::vec3(100.0f),
                               NULL)
                        );

    entities.push_back(new Entity(world,
                               "base",
                               baseMesh,
                               glm::vec4(0.8, 0.8, 0.5, 1),
                               glm::vec3(0, 0.0, 0),
                               glm::vec3(0.0f, 0.0f, 0.0f),
                               glm::vec3(100.0f),
                               NULL)
                        );

    entities.push_back(new Entity(world,
                               "fan",
                               fanMesh,
                               glm::vec4(1.0, 0.5, 0.5, 1),
                               glm::vec3(200, 1.5, 0),
                               glm::vec3(0.0f, 0.0f, 0.0f),
                               glm::vec3(50.0f),
                               NULL)
                        );

    entities.push_back(new Entity(world,
                               "fanBase",
                               fanBaseMesh,
                               glm::vec4(0.8, 0.8, 0.5, 1),
                               glm::vec3(200, 0.0, 0),
                               glm::vec3(0.0f, 0.0f, 0.0f),
                               glm::vec3(50.0f),
                               NULL)
                        );


    float lightsize = 400.0f;
    /*for(int i = 0; i < 10; i++){
        for(int j = 0; j < 10; j++){
            lights.push_back(new Light(gBuffer, glm::vec3(1, 1, 1), glm::vec3(i*500, 100, j*500), lightsize));
        }
    }*/
    lights.push_back(new Light(gBuffer, glm::vec3(0.3, 0.9, 0.0), glm::vec3(0, 100, 400), lightsize));
    //lights.push_back(new Light(gBuffer, glm::vec3(1, 1, 1), glm::vec3(200, 100, 340), lightsize));
    //lights.push_back(new Light(gBuffer, glm::vec3(1, 0, 0), glm::vec3(30, 100, -100), lightsize));

    /*lights.push_back(new Light(gBuffer, glm::vec3(1, 0, 1), glm::vec3(500, 100, -10), lightsize));
    lights.push_back(new Light(gBuffer, glm::vec3(1, 1, 1), glm::vec3(500, 100, 40), lightsize));
    lights.push_back(new Light(gBuffer, glm::vec3(1, 1, 0), glm::vec3(500, 100, -100), lightsize));
    lights.push_back(new Light(gBuffer, glm::vec3(1, 0, 1), glm::vec3(30, 100, 10), lightsize));
    lights.push_back(new Light(gBuffer, glm::vec3(0.1, 1, 0.4), glm::vec3(20, 30, 100), lightsize));
    lights.push_back(new Light(gBuffer, glm::vec3(1, 0.6, 0), glm::vec3(300, 100, -100), lightsize));
    lights.push_back(new Light(gBuffer, glm::vec3(1, 0, 1), glm::vec3(10, 100, -10), lightsize));
    lights.push_back(new Light(gBuffer, glm::vec3(1, 0, 1), glm::vec3(-30, 100, 10), lightsize));
    lights.push_back(new Light(gBuffer, glm::vec3(0.1, 1, 0.4), glm::vec3(20, 30, 100), lightsize));
    lights.push_back(new Light(gBuffer, glm::vec3(1, 0.6, 0), glm::vec3(-300, 100, -10), lightsize));
    lights.push_back(new Light(gBuffer, glm::vec3(1, 0, 1), glm::vec3(10, 10, 10), lightsize));
    lights.push_back(new Light(gBuffer, glm::vec3(1, 0, 1), glm::vec3(-30, 100, -10), lightsize));
    lights.push_back(new Light(gBuffer, glm::vec3(0.1, 1, 0.4), glm::vec3(-20, 30, -100), lightsize));
    lights.push_back(new Light(gBuffer, glm::vec3(1, 0.6, 0), glm::vec3(-30, 100, -100), lightsize));
    lights.push_back(new Light(gBuffer, glm::vec3(1, 0, 1), glm::vec3(10, 10, 10), lightsize));*/

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

    isClosed = false;
    cullLightsThread = getCullLightsThread();

    particleRenderer = new ParticleRenderer(projectionMatrix, glm::vec3(500, 100, 500), 500);
}

void Game::handleInput(Game* game){
    Input* input = game->getInput();
    Display* display = game->getDisplay();
    Camera* camera = game->getCamera();
    Player* player = game->getPlayer();

    input->update(display);

    glm::vec3 forward = camera->getForward();
    glm::vec3 right = camera->getRight();

    if(input->getKey(SDLK_SPACE)){
        if(!player->getIsJumping()){
            player->jump();
        }
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
    if(glm::abs(glm::length(input->getMouseDelta())) > 0.5f){
        camera->rotateX(input->getMouseDelta().y * 0.004f);
        camera->rotateY(input->getMouseDelta().x * 0.004f);
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
        game->lights.push_back(new Light(game->gBuffer, glm::vec3(1, 0.7, 0.6), glm::vec3(pos.x, 100, pos.z), 400.0f));
    }

    /*if(input->getKeyDown(SDLK_1)){
        //how to move a rigid body without breaking the engine
        btTransform transform = player->getRigidBody()->getCenterOfMassTransform();
        transform.setOrigin(btVector3(0, 300, 200));
        player->getRigidBody()->setCenterOfMassTransform(transform);
    }*/
}

void Game::stencil(Light* l){
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
    l->draw(emptyShader);
}

void Game::normal(Light* l){
    gBuffer->unbind();
    gBuffer->bindForLights();

    glStencilFunc(GL_NOTEQUAL, 0, 0xFF);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);

    glDisable(GL_DEPTH_TEST);

    glEnable(GL_BLEND);
    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_ONE, GL_ONE);

    simpleShaderForLights->bind();
    l->draw(simpleShaderForLights);
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

        deferredLightShader->bind();
        camera->setPosition(player->getPosition() + glm::vec3(0, 20, 0));
        deferredLightShader->loadViewMatrix(camera->getViewMatrix());

        for(Entity* e : entities){
            e->draw(deferredLightShader);
        }
        particleRenderer->draw();
    }
    #endif
    #if RENDER_LIGHTS == 1
    {
        simpleShaderForLights->bind();
        simpleShaderForLights->loadViewMatrix(camera->getViewMatrix());
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

        for(Light* l : lights){
            if(l->getRenderIt() == true){
                stencil(l);
                normal(l);
            }
        }
    }
    #endif

    gBuffer->unbind();

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

void Game::run(){
    while(!display->isClosed()){
        display->setLastFrameTime(SDL_GetTicks());

        timeAccumulator += Display::getDelta();
        while(timeAccumulator >= discreteChunk){
            world->stepSimulation(btScalar(0.5f));
            timeAccumulator -= discreteChunk;
        }
        player->performRayTest();
        particleRenderer->update(Display::getDelta(), camera);
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

    btTransform t;

    t.setIdentity();
    t.setOrigin(btVector3(0, 0, 0));

    btStaticPlaneShape* plane = new btStaticPlaneShape(btVector3(0, 1, 0), btScalar(0));
    btMotionState* motion = new btDefaultMotionState(t);
    btRigidBody::btRigidBodyConstructionInfo info(0.0, motion, plane);
    info.m_restitution = 0.3;
    info.m_friction = 1.0;
    btRigidBody *body = new btRigidBody(info);
    world->addRigidBody(body);
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

    std::cout << "Freeing " << entities.size() << " entities..." << std::endl;

    for(Entity* e : entities){
        delete e;
    }
    for(Light* l : lights){
        delete l;
    }

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
