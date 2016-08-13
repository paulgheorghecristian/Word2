#include <iostream>
#include "display.h"
#include "input.h"
#include "shader.h"
#include "mesh.h"
#include "entity.h"
#include <glm/gtc/matrix_transform.hpp>
#include "camera.h"
#include "texture.h"
#include <ode/ode.h>

using namespace std;

int main()
{
    //rezolutia ferestrei
    const float WIDTH = 1080.0f;
    const float HEIGHT = 720.0f;

    Display display((int)WIDTH, (int)HEIGHT, "OpenGL");
    Shader general_shader("res/shaders/vertex", "res/shaders/fragment");

    Input input;
    Camera camera(glm::vec3(0.0f, HEIGHT/2.0f-200.0f, -150.0f), 0.0, 0.0, 0.0);

    //meshe
    Mesh *surface = Mesh::get_surface(500, 500);
    Mesh *sphere = Mesh::load_object("res/models/sphere.obj");
    Mesh *cube = Mesh::load_object("res/models/cube.obj");

    //entitati
    Entity *e_sphere = new Entity("water", sphere, glm::vec4(1, 0, 0, 1), glm::vec3(-5.5, 12.1, 0.6), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(100.0f, 100.0f, 100.0f));
    Entity *e_cube = new Entity("terrain", cube, glm::vec4(0, 1, 0, 1), glm::vec3(0.0f, 0.0f, -600.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(200.0f, 200.0f, 200.0f));

    glm::mat4 projection_matrix = glm::perspective(75.0f, WIDTH/HEIGHT, 0.1f, 4000.0f);

    general_shader.bind();
    general_shader.loadProjectionMatrix(projection_matrix);

    while(!display.isClosed()){
        input.update();
        display.clear(1, 1, 1, 1);

        //miscare lumina sau camera
        if(input.GetKey(SDLK_w)){
            camera.move_forward(400.0f * Display::get_delta());
        }else if(input.GetKey(SDLK_s)){
            camera.move_forward(-400.0f * Display::get_delta());
        }

        if(input.GetKey(SDLK_a)){
            camera.move_sideways(-400.0f * Display::get_delta());
        }else if(input.GetKey(SDLK_d)){
            camera.move_sideways(400.0f * Display::get_delta());
        }

        if(glm::abs(glm::length(input.GetMouseDelta())) > 0.1f){
            camera.rotate_x(input.GetMouseDelta().y * 0.05f);
            camera.rotate_y(input.GetMouseDelta().x * 0.05f);
        }

        general_shader.bind();
        general_shader.loadViewMatrix(camera.get_view_matrix());

        e_sphere->draw(&general_shader);
        e_cube->draw(&general_shader);

        display.update();
    }

    return 0;
}
