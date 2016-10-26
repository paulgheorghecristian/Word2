#include "display.h"
#include "input.h"
#include "shader.h"
#include "mesh.h"
#include "entity.h"
#include "camera.h"
#include <iostream>

int main(int argc, char *argv[])
{
    //rezolutia ferestrei
    float WIDTH;
    float HEIGHT;

    Display* display = new Display(WIDTH, HEIGHT, "Worlds-ver 0.0001");
    Shader *shader = new Shader("res/shaders/vertex", "res/shaders/fragment");
    Input *input = new Input();
    Camera *camera = new Camera(glm::vec3(-100, 0, 0), 0, 0, 0);
    Mesh *box = Mesh::loadObject("res/models/cube4.obj");

    Entity *entity = new Entity("entity",
                                box,
                                glm::vec4(1,0,0,1),
                                glm::vec3(0, 0, 0),
                                glm::vec3(0, 0, 0),
                                glm::vec3(30));

    glm::mat4 projectionMatrix = glm::perspective(glm::radians(75.0f), WIDTH/HEIGHT, 1.0f, 10000.0f);

    shader->bind();
    shader->loadProjectionMatrix(projectionMatrix);

    while(!display->isClosed()){
        display->setLastFrameTime(SDL_GetTicks());
        display->clear(1,1,1,1);

        input->update(display);
        if(glm::abs(glm::length(input->getMouseDelta())) > 0.5f){
            camera->rotateX(input->getMouseDelta().y * 0.004f);
            camera->rotateY(input->getMouseDelta().x * 0.004f);
        }

        shader->bind();
        shader->loadViewMatrix(camera->getViewMatrix());

        entity->draw(shader);

        display->update();
    }

    delete display;
    delete shader;
    delete input;
    delete camera;
    delete box;
    delete entity;

    return 0;
}
