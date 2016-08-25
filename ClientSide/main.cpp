#include <iostream>
#include "display.h"
#include "input.h"
#include "shader.h"
#include "mesh.h"
#include "entity.h"
#include <glm/gtc/matrix_transform.hpp>
#include "camera.h"
#include "texture.h"
#include <vector>
#include <bullet/btBulletDynamicsCommon.h>
#include <glm/gtc/type_ptr.hpp>
#include "sphere.h"
#include "box.h"
#include <pthread.h>
#include <time.h>

btDynamicsWorld* world;
btDispatcher* dispatcher;
btBroadphaseInterface* broadsphase;
btCollisionConfiguration* collisionConfig;
btConstraintSolver* solver;

std::vector<Entity*> entities;

struct Args{
    Camera *cam;
    Input *input;
    SDL_Window *window;
};

void initBullet(){
    collisionConfig = new btDefaultCollisionConfiguration();
    dispatcher = new btCollisionDispatcher(collisionConfig);
    broadsphase = new btDbvtBroadphase();
    solver = new btSequentialImpulseConstraintSolver();

    world = new btDiscreteDynamicsWorld(dispatcher,
                                        broadsphase,
                                        solver,
                                        collisionConfig);
    world->setGravity(btVector3(0, -10, 0));
}

void initObjects(){
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

    //bodies.push_back(body);

    /*addSphere(20, 400, 200, 0, 50);
    addBox(40, 40, 40, 0, 180, 0, 50);*/
}

void cleanBullet(){
    delete collisionConfig;
    delete dispatcher;
    delete broadsphase;
    delete solver;
    delete world;

    for(Entity *e : entities){
        delete e;
    }
}

void *input_thread_func(void *parm){
    Args *args = (Args*)parm;

    Camera *camera = args->cam;
    Input *input = args->input;
    SDL_Window *window = args->window;

    struct timespec ts = {0, 0};

    ts.tv_sec = 0;
    ts.tv_nsec = 10000000;

    while(1){
        input->update(window);

        if(glm::abs(glm::length(input->GetMouseDelta())) > 0.3f){
            camera->rotate_x(input->GetMouseDelta().y * 0.1f);
            camera->rotate_y(input->GetMouseDelta().x * 0.1f);
        }

        if(input->GetKey(SDLK_w)){
            camera->move_forward(400.0f * Display::get_delta());
        }else if(input->GetKey(SDLK_s)){
            camera->move_forward(-400.0f * Display::get_delta());
        }

        if(input->GetKey(SDLK_a)){
            camera->move_sideways(-400.0f * Display::get_delta());
        }else if(input->GetKey(SDLK_d)){
            camera->move_sideways(400.0f * Display::get_delta());
        }

        nanosleep(&ts, NULL);
    }
}

int main()
{
    //rezolutia ferestrei
    const float WIDTH = 1080.0f;
    const float HEIGHT = 720.0f;

    pthread_t input_thread_id;


    initBullet();
    initObjects();

    Display display((int)WIDTH, (int)HEIGHT, "OpenGL");
    Shader shader("res/shaders/vertex", "res/shaders/fragment");

    Input *input = new Input();
    Camera *camera = new Camera(glm::vec3(0.0f, 50, 800.0f), 0.0, 0.0, 0.0);

    Args *args = new Args();
    args->input = input;
    args->cam = camera;

    Mesh *sphere = Mesh::load_object("res/models/sphere.obj");
    Mesh *box = Mesh::load_object("res/models/cube.obj");
    Sphere::set_mesh(sphere);
    Box::set_mesh(box);

    //meshe
    Mesh *surface = Mesh::get_surface(500, 500);


    /*Entity *e_cube = new Entity("cube",
                                 cube,
                                 glm::vec4(0, 1, 0, 1),
                                 glm::vec3(0, 180, 0),
                                 glm::vec3(0.0f, 0.0f, 0.0f),
                                 glm::vec3(20.0f, 20.0f, 20.0f));
    */

    Entity *e_surface = new Entity(world,
                                   "surface",
                                   surface,
                                   glm::vec4(0.5, 0.5, 0.5, 1),
                                   glm::vec3(0, 0, 0),
                                   glm::vec3(0.0f, 0.0f, 0.0f),
                                   glm::vec3(5000.0f, 1, 5000.0f));


    Sphere *dynamic_sphere = new Sphere(world,
                                        20.0f,
                                        glm::vec4(1, 0, 0, 1),
                                        glm::vec3(400, 200, 0),
                                        glm::vec3(0.0f, 0.0f, 0.0f),
                                        20.0f);

    Box *dynamic_box = new Box(world,
                               100.0f,
                               glm::vec4(0, 1, 0, 1),
                               glm::vec3(0, 200, 0),
                               glm::vec3(0.0f, 0.0f, 0.0f),
                               glm::vec3(20.0f));

    entities.push_back(dynamic_sphere);
    entities.push_back(e_surface);
    entities.push_back(dynamic_box);

    glm::mat4 projection_matrix = glm::perspective(75.0f, WIDTH/HEIGHT, 0.1f, 4000.0f);

    shader.bind();
    shader.loadProjectionMatrix(projection_matrix);

    for(int i = 0; i < 100; i++){
         Box *new_box = new Box(world,
                                500.0f,
                                glm::vec4(0, 1, 0, 1),
                                glm::vec3(i+50.0f, 900.0f, 0),
                                glm::vec3(0.0f, 0.0f, 0.0f),
                                glm::vec3(20.0f));
       // new_box->set_linear_velocity(camera.get_forward()*100.0f);
        entities.push_back(new_box);
        Sphere *new_sphere = new Sphere(world,
                                        1000.0f,
                                        glm::vec4(1, 0, 0, 1),
                                        glm::vec3(i+50.0f, 1000.0f, 0),
                                        glm::vec3(0.0f, 0.0f, 0.0f),
                                        20.0f);
        //new_sphere->set_linear_velocity(camera.get_forward()*100.0f);
        entities.push_back(new_sphere);
    }

    int rc = pthread_create(&input_thread_id, NULL, input_thread_func, (void*)args);

    while(!display.isClosed()){
        display.clear(1, 1, 1, 1);
        world->stepSimulation(btScalar(0.05f), 1, btScalar(0.1f));

        if(input->GetKeyDown(SDLK_z)){
            Sphere *new_sphere = new Sphere(world,
                                            40.0f,
                                            glm::vec4(1, 0, 0, 1),
                                            camera->get_position(),
                                            glm::vec3(0.0f, 0.0f, 0.0f),
                                            20.0f);
            new_sphere->set_linear_velocity(camera->get_forward()*100.0f);
            entities.push_back(new_sphere);
        }

        if(input->GetKeyDown(SDLK_x)){
            Box *new_box = new Box(world,
                                        200.0f,
                                        glm::vec4(0, 1, 0, 1),
                                        camera->get_position(),
                                        glm::vec3(0.0f, 0.0f, 0.0f),
                                        glm::vec3(20.0f));
            new_box->set_linear_velocity(camera->get_forward()*100.0f);
            entities.push_back(new_box);
        }

        shader.bind();
        shader.loadViewMatrix(camera->get_view_matrix());

        for(Entity *e : entities){
            e->draw(&shader);
        }

        display.update();
    }

    cleanBullet();
    return 0;
}
