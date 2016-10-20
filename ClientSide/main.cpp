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
#include <time.h>
#include "player.h"
#include "font.h"
#include "text.h"
#include "textshader.h"
#include <windows.h>
#include <typeinfo>

#define GRAVITY -10

btDynamicsWorld* world;
btDispatcher* dispatcher;
btBroadphaseInterface* broadsphase;
btCollisionConfiguration* collisionConfig;
btConstraintSolver* solver;

std::vector<Entity*> entities;

struct InputArgs{
    Input* input;
    Display* display;
    Camera* camera;
    Player* player;
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
    world->setGravity(btVector3(0, GRAVITY, 0));
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
}

void cleanBullet(){
    delete collisionConfig;
    delete dispatcher;
    delete broadsphase;
    delete solver;
    //delete world;

    std::cout << entities.size() << " entities" << std::endl;

    for(Entity *e : entities){
        delete e;
    }

    std::cout << "Cleaning finished!" << std::endl;
}

void processInput(InputArgs* args, Texture* brick){
    Input* input = args->input;
    Display* display = args->display;
    Camera* camera = args->camera;
    Player* player = args->player;

    input->update(display->getWindow());

    if(input->GetKeyDown(SDLK_SPACE)){
        player->get_rigid_body()->setLinearVelocity(btVector3(player->get_rigid_body()->getLinearVelocity().x(), 30.0f, player->get_rigid_body()->getLinearVelocity().z()));
    }

    glm::vec3 forward = camera->get_forward();
    glm::vec3 right = camera->get_right();

    if(input->GetKey(SDLK_w)){
        //glm::vec3 forward = camera.get_forward();
        player->get_rigid_body()->setLinearVelocity(btVector3(forward.x*30.0f, player->get_rigid_body()->getLinearVelocity().y(), forward.z*30.0f));
    }else if(input->GetKey(SDLK_s)){
        //glm::vec3 forward = camera.get_forward();
        player->get_rigid_body()->setLinearVelocity(btVector3(forward.x*-30.0f, player->get_rigid_body()->getLinearVelocity().y(), forward.z*-30.0f));
    }

    if(input->GetKey(SDLK_a)){
        player->get_rigid_body()->setLinearVelocity(btVector3(right.x*-30.0f, player->get_rigid_body()->getLinearVelocity().y(), right.z*-30.0f));
    }else if(input->GetKey(SDLK_d)){
        player->get_rigid_body()->setLinearVelocity(btVector3(right.x*30.0f, player->get_rigid_body()->getLinearVelocity().y(), right.z*30.0f));
    }

    if(glm::abs(glm::length(input->GetMouseDelta())) > 0.5f){
        camera->rotate_x(input->GetMouseDelta().y * 0.004f);
        camera->rotate_y(input->GetMouseDelta().x * 0.004f);
    }

    if(input->GetKeyDown(SDLK_ESCAPE)){
        display->close();
    }

    if(input->GetKeyDown(SDLK_x)){
        Box *b = new Box(world,
                           100.0f,
                           glm::vec4(0, 1, 0, 1),
                           camera->get_position() + forward * 50.0f,
                           glm::vec3(0.0f, 0.0f, 0.0f),
                           glm::vec3(20.0f),
                           brick);
        b->set_linear_velocity(forward * 100.0f);
        entities.push_back(b);
    }
}

int main(int argc, char *argv[])
{
    //rezolutia ferestrei
    const float WIDTH = 1080.0f;
    const float HEIGHT = 720.0f;

    initBullet();
    initObjects();

    InputArgs inputArgs;

    Display* display = new Display((int)WIDTH, (int)HEIGHT, "OpenGL");
    Shader shader("res/shaders/vertex", "res/shaders/fragment");
    TextShader *text_shader = new TextShader("res/shaders/text_vs", "res/shaders/text_fs");
    Font font("res/fonts/myfont.fnt", "res/fonts/font7.bmp");
    Text *text = new Text(&font, "Paul", glm::vec3(101, 100, -10), glm::vec3(0, 0, 0), glm::vec3(1, 0.5, 0), 10);
    Texture *wood = new Texture("res/textures/Tileable_wood_texture.bmp");
    Texture *brick = new Texture("res/textures/brick.bmp");

    Input *input = new Input();
    Camera *camera = new Camera(glm::vec3(0, 100, 1000), 0, 0, 0);

    Mesh *capsule = Mesh::load_object("res/models/capsule2.obj");
    Mesh *sphere = Mesh::load_object("res/models/sphere4.obj");
    Mesh *box = Mesh::load_object("res/models/cube4.obj");

    Sphere::set_mesh(sphere);
    Box::set_mesh(box);
    Player::set_mesh(capsule);

    //meshe
    Mesh *surface = Mesh::get_surface(500, 500);

    Entity *e_surface = new Entity(world,
                                   "surface",
                                   surface,
                                   glm::vec4(0.5, 0.5, 0.5, 1),
                                   glm::vec3(0, 0, 0),
                                   glm::vec3(0.0f, 0.0f, 0.0f),
                                   glm::vec3(50000.0f, 1, 50000.0f),
                                   NULL);

    Sphere *dynamic_sphere = new Sphere(world,
                                        20.0f,
                                        glm::vec4(1, 0, 0, 1),
                                        glm::vec3(400, 200, 0),
                                        glm::vec3(0.0f, 0.0f, 0.0f),
                                        20.0f,
                                        wood);

    Box *dynamic_box = new Box(world,
                               10000.0f,
                               glm::vec4(0, 1, 0, 1),
                               glm::vec3(0, 500, 0),
                               glm::vec3(0.0f, 0.0f, 0.0f),
                               glm::vec3(100.0f),
                               brick);

    Player *player = new Player(world,
                                100.0f,
                                glm::vec4(0.0, 1.0, 1.0, 1),
                                glm::vec3(400, 0, 0),
                                glm::vec3(0.0f, 0.0f, 0.0f),
                                glm::vec3(10));

    entities.push_back(player);
    entities.push_back(dynamic_sphere);
    entities.push_back(e_surface);
    entities.push_back(dynamic_box);

    glm::mat4 projection_matrix = glm::perspective(glm::radians(75.0f), WIDTH/HEIGHT, 1.0f, 10000.0f);

    shader.bind();
    shader.loadProjectionMatrix(projection_matrix);

    text_shader->bind();
    text_shader->load_view_matrix(glm::mat4(1.0));
    text_shader->load_projection_matrix(glm::ortho(0.0f, WIDTH, 0.0f, HEIGHT, 1.0f, 1000.0f));

    long accumulator = 0;
    float dt = 18.0f/6.0f;

    inputArgs.input = input;
    inputArgs.display = display;
    inputArgs.camera = camera;
    inputArgs.player = player;

    while(!display->isClosed()){
        display->setLastFrameTime(SDL_GetTicks());
        processInput(&inputArgs, brick);

        accumulator += Display::get_delta();
        while(accumulator >= dt){
            world->stepSimulation(btScalar(0.5f));
            accumulator -= dt;
        }

        camera->set_position(player->get_position_for_camera() + glm::vec3(0, 10, 0));

        display->clear(1,1,1,1);
        shader.bind();
        shader.loadViewMatrix(camera->get_view_matrix());
        for(Entity *e : entities){
            e->draw(&shader);
        }
        text_shader->bind();
        text->display_number(Display::get_delta());
        text->draw(text_shader);
        display->update();
    }

    delete inputArgs.camera;
    delete inputArgs.display;
    delete inputArgs.input;
    cleanBullet();

    return 0;
}
