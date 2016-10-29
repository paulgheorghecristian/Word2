#include "game.h"

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
    shader = new Shader("res/shaders/vertex", "res/shaders/fragment");
    player = new Player(world, 30.0f, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(10));
    boxMesh = Mesh::loadObject("res/models/cube4.obj");
    sphereMesh = Mesh::loadObject("res/models/sphere4.obj");
    textShader = new TextShader("res/shaders/text_vs", "res/shaders/text_fs");
    fpsText = new Text(new Font("res/fonts/myfont.fnt", "res/fonts/font7.bmp"),
                            "Paul",
                            glm::vec3(101, 100, -3),
                            glm::vec3(0, 0, 0),
                            glm::vec3(1, 0.5, 0), 10);
    Box::setMesh(boxMesh);
    Sphere::setMesh(sphereMesh);

    entities.push_back(new Box(world,
                                100.0f,
                                glm::vec4(1,0,0,1),
                                glm::vec3(0, 200, 0),
                                glm::vec3(0, 0, 0),
                                glm::vec3(30))
                       );

    entities.push_back(new Sphere(world,
                                    100.0f,
                                    glm::vec4(1,1,0,1),
                                    glm::vec3(100, 100, 0),
                                    glm::vec3(0, 0, 0),
                                    30.0f)
                       );

    entities.push_back(new Entity(world,
                               "surface",
                               Mesh::getSurface(500, 500),
                               glm::vec4(0.5, 0.5, 0.5, 1),
                               glm::vec3(0, 0, 0),
                               glm::vec3(0.0f, 0.0f, 0.0f),
                               glm::vec3(50000.0f, 1, 50000.0f))
                        );

    glm::mat4 projectionMatrix = glm::perspective(glm::radians(75.0f), this->screenWidth/this->screenHeight, 1.0f, 10000.0f);

    shader->bind();
    shader->loadProjectionMatrix(projectionMatrix);

    textShader->bind();
    textShader->loadProjectionMatrix(glm::ortho(0.0f, screenWidth, 0.0f, screenHeight, 1.0f, 10.0f));
}

void Game::handleInput(Game* game){
    Input* input = game->getInput();
    Display* display = game->getDisplay();
    Camera* camera = game->getCamera();
    Player* player = game->getPlayer();

    input->update(display);

    glm::vec3 forward = camera->getForward();
    glm::vec3 right = camera->getRight();

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
    }
    if(glm::abs(glm::length(input->getMouseDelta())) > 0.5f){
        camera->rotateX(input->getMouseDelta().y * 0.004f);
        camera->rotateY(input->getMouseDelta().x * 0.004f);
    }

    if(input->getMouseDown(1)){
        Sphere* sphere = new Sphere(game->getWorld(),
                                    100.0f,
                                    glm::vec4(1,0,0,1),
                                    camera->getPosition() + camera->getForward() * 100.0f,
                                    glm::vec3(0, 0, 0),
                                    30.0f);
        sphere->setLinearVelocity(camera->getForward() * 100.0f);
        game->getEntities().push_back(sphere);
    }
}

void Game::render(){
    display->clear(1,1,1,1);

    shader->bind();
    camera->setPosition(player->getPosition() + glm::vec3(0, 20, 0));
    shader->loadViewMatrix(camera->getViewMatrix());

    for(Entity* e : entities){
        e->draw(shader);
    }

    textShader->bind();
    fpsText->displayNumber(Display::getDelta());
    fpsText->draw(textShader);

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
    std::cout << "Destroying game..." << std::endl;
    delete display;
    delete shader;
    delete input;
    delete camera;

    std::cout << "Freeing " << entities.size() << " entities..." << std::endl;

    for(Entity* e : entities){
        delete e;
    }

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
