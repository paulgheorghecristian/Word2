#include "game.h"

Game::Game(float width, float height, std::string title, Camera* camera) : screenHeight(height), screenWidth(width), title(title)
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
    input = new Input();
    shader = new Shader("res/shaders/vertex", "res/shaders/fragment");
    boxMesh = Mesh::loadObject("res/models/cube4.obj");

    entities.push_back(new Entity("entity",
                                    boxMesh,
                                    glm::vec4(1,0,0,1),
                                    glm::vec3(0, 0, 0),
                                    glm::vec3(0, 0, 0),
                                    glm::vec3(30))
                       );

    glm::mat4 projectionMatrix = glm::perspective(glm::radians(75.0f), this->screenWidth/this->screenHeight, 1.0f, 10000.0f);

    shader->bind();
    shader->loadProjectionMatrix(projectionMatrix);
}

void Game::handleInput(){
    input->update(display);

    if(input->getKeyDown(SDLK_ESCAPE)){
        display->close();
    }
    if(glm::abs(glm::length(input->getMouseDelta())) > 0.5f){
        camera->rotateX(input->getMouseDelta().y * 0.004f);
        camera->rotateY(input->getMouseDelta().x * 0.004f);
    }
}

void Game::render(){
    display->clear(1,1,1,1);

    shader->bind();
    shader->loadViewMatrix(camera->getViewMatrix());

    for(Entity* e : entities){
        e->draw(shader);
    }

    display->update();
}

void Game::run(){
    while(!display->isClosed()){
        display->setLastFrameTime(SDL_GetTicks());

        handleInput();
        render();
    }
}

Game::~Game()
{
    delete display;
    delete shader;
    delete input;
    delete camera;
    for(Entity* e : entities){
        delete e;
    }
}
