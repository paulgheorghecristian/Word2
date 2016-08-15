#include <iostream>
#include "display.h"
#include "input.h"
#include "shader.h"
#include "mesh.h"
#include "entity.h"
#include <glm/gtc/matrix_transform.hpp>
#include "camera.h"
#include "texture.h"

using namespace std;

int main()
{
    //rezolutia ferestrei
    const float WIDTH = 1080.0f;
    const float HEIGHT = 720.0f;

    Display display((int)WIDTH, (int)HEIGHT, "OpenGL");
    Shader general_shader("res/shaders/vertex", "res/shaders/fragment");

    Input input;
    Camera camera(glm::vec3(0.0f, 0, 350.0f), 0.0, 0.0, 0.0);

    //meshe
    Mesh *surface = Mesh::get_surface(500, 500);
    Mesh *sphere = Mesh::load_object("res/models/sphere.obj");
    Mesh *cube = Mesh::load_object("res/models/cube.obj");

    //entitati
    Entity *e_sphere = new Entity("sphere",
                                   sphere,
                                   glm::vec4(1, 0, 0, 1),
                                   glm::vec3(400, 100, 0),
                                   glm::vec3(0.0f, 0.0f, 0.0f),
                                   glm::vec3(20.0f, 20.0f, 20.0f));

    Entity *e_cube = new Entity("cube",
                                 cube,
                                 glm::vec4(0, 1, 0, 1),
                                 glm::vec3(0, 100, 0),
                                 glm::vec3(0.0f, 0.0f, 0.0f),
                                 glm::vec3(20.0f, 20.0f, 20.0f));

    Entity *e_surface = new Entity("surface",
                                    surface,
                                    glm::vec4(0.5, 0.5, 0.5, 1),
                                    glm::vec3(0, 0, 0),
                                    glm::vec3(0.0f, 0.0f, 0.0f),
                                    glm::vec3(500.0f, 1, 500.0f));

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
        e_surface->draw(&general_shader);

        display.update();
    }

    destroy_ode();

    return 0;
}
