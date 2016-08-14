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

#define DENSITY 0.5
#define MAX_CONTACTS 10

using namespace std;

dWorldID world;
dSpaceID space;
dJointGroupID contact_group;
dBodyID body_sphere;
dGeomID geom_sphere;
dBodyID body_cube;
dGeomID geom_cube;

static void nearCallback (void *data, dGeomID o1, dGeomID o2){

    int i;

    dBodyID b1 = dGeomGetBody(o1);
    dBodyID b2 = dGeomGetBody(o2);

    dContact contact[MAX_CONTACTS];

    for (i = 0; i < MAX_CONTACTS; i++){
        contact[i].surface.mode = dContactBounce | dContactSoftCFM;
        contact[i].surface.mu = dInfinity;
        contact[i].surface.mu2 = 0;
        contact[i].surface.bounce = 0.9;
        contact[i].surface.bounce_vel = 0.1;
        contact[i].surface.soft_cfm = 0.01;
    }

    int numc;

    if ((numc = dCollide(o1, o2, MAX_CONTACTS, &contact[0].geom, sizeof(dContact))) > 0){
        for (i = 0; i < numc; i++){
            dJointID c = dJointCreateContact(world, contact_group, contact + i);
            dJointAttach(c, b1, b2);

        }

    }

}

void sim_loop(){
    dSpaceCollide(space, 0, &nearCallback);
    dWorldQuickStep(world, 0.1);
    dJointGroupEmpty(contact_group);
}

void init_ode(){
    dInitODE();

    dMass m_sphere;
    dMass m_cube;
    dReal sides[3];
    float s = 2;
    sides[0] = s;
    sides[1] = s;
    sides[2] = s;

    dMassSetBox(&m_cube, DENSITY, sides[0], sides[1], sides[2]);
    dMassSetSphere(&m_sphere, DENSITY, 1);

    world = dWorldCreate();
    space = dSimpleSpaceCreate(0);
    contact_group = dJointGroupCreate(0);
    dCreatePlane(space, 0, 1, 0, 0);
    dWorldSetGravity(world, 0, -5.0, 0);
    dWorldSetERP(world, 0.2);
    dWorldSetCFM(world, 1e-5);
    dWorldSetContactMaxCorrectingVel(world, 0.9);
    dWorldSetContactSurfaceLayer(world, 0.001);
    dWorldSetAutoDisableFlag(world, 1);

    body_sphere = dBodyCreate(world);
    body_cube = dBodyCreate(world);

    dBodySetPosition(body_sphere, 400, 100, 0);
    dBodySetPosition(body_cube, 0, 100, 0);

    dBodySetLinearVel(body_sphere, -200, 0, 0);
    dBodySetLinearVel(body_cube, 0, 0, 0);

    dBodySetMass(body_sphere, &m_sphere);
    dBodySetMass(body_cube, &m_cube);

    geom_sphere = dCreateSphere(space, 20);
    dGeomSetBody(geom_sphere, body_sphere);
    geom_cube = dCreateBox(space,sides[0], sides[1], sides[2]);
    dGeomSetBody(geom_cube, body_cube);
}

void destroy_ode(){
    dJointGroupDestroy(contact_group);
    dSpaceDestroy(space);
    dWorldDestroy(world);
}

int main()
{
    init_ode();

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
        sim_loop();
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

        const dReal* sphere_pos = dGeomGetPosition(geom_sphere);
        const dReal* cube_pos = dGeomGetPosition(geom_cube);

        e_sphere->set_position(sphere_pos[0], sphere_pos[1], sphere_pos[2]);
        e_cube->set_position(cube_pos[0], cube_pos[1]+20, cube_pos[2]);

        e_sphere->draw(&general_shader);
        e_cube->draw(&general_shader);
        e_surface->draw(&general_shader);

        display.update();
    }

    destroy_ode();

    return 0;
}
