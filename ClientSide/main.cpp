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

btDynamicsWorld* world;
btDispatcher* dispatcher;
btBroadphaseInterface* broadsphase;
btCollisionConfiguration* collisionConfig;
btConstraintSolver* solver;

std::vector<Entity*> entities;

btRigidBody* addSphere(float radius, float x, float y, float z, float mass){
    btTransform t;
    btVector3 inertia(0, 0, 0);

    t.setIdentity();
    t.setOrigin(btVector3(x,y,z));

    btSphereShape* sphereShape = new btSphereShape(radius);
    if(mass != 0.0){
        sphereShape->calculateLocalInertia(btScalar(mass), inertia);
    }
    btMotionState* motion = new btDefaultMotionState(t);
    btRigidBody::btRigidBodyConstructionInfo info(mass, motion, sphereShape, inertia);
    info.m_restitution = 0.4;
    info.m_friction = 1.0f;

    btRigidBody *body = new btRigidBody(info);

    body->setDamping(btScalar(0), btScalar(0.7));
    body->setLinearVelocity(btVector3(-100, 0, 0));
    world->addRigidBody(body);
   // bodies.push_back(body);

    return body;
}

btRigidBody* addBox(float width, float height, float depth, float x, float y, float z, float mass){
    btTransform t;
    btVector3 inertia(0, 0, 0);

    t.setIdentity();
    t.setOrigin(btVector3(x,y,z));

    btBoxShape* boxShape = new btBoxShape(btVector3(width/2.0, height/2.0, depth/2.0));
    if(mass != 0.0){
        boxShape->calculateLocalInertia(btScalar(mass), inertia);
    }
    btMotionState* motion = new btDefaultMotionState(t);
    btRigidBody::btRigidBodyConstructionInfo info(mass, motion, boxShape, inertia);
    info.m_restitution = 1;
    info.m_friction = 1.0f;

    btRigidBody *body = new btRigidBody(info);
    world->addRigidBody(body);
    //bodies.push_back(body);

    return body;
}

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

int main()
{
    //rezolutia ferestrei
    const float WIDTH = 1080.0f;
    const float HEIGHT = 720.0f;

    initBullet();
    initObjects();

    Display display((int)WIDTH, (int)HEIGHT, "OpenGL");
    Shader shader("res/shaders/vertex", "res/shaders/fragment");

    Input input;
    Camera camera(glm::vec3(0.0f, 50, 400.0f), 0.0, 0.0, 0.0);


    Mesh *sphere = Mesh::load_object("res/models/sphere.obj");
    Sphere::set_mesh(sphere);

    //meshe
    Mesh *surface = Mesh::get_surface(500, 500);


    /*Entity *e_cube = new Entity("cube",
                                 cube,
                                 glm::vec4(0, 1, 0, 1),
                                 glm::vec3(0, 180, 0),
                                 glm::vec3(0.0f, 0.0f, 0.0f),
                                 glm::vec3(20.0f, 20.0f, 20.0f));
    */

    Entity *e_surface = new Entity("surface",
                                    surface,
                                    glm::vec4(0.5, 0.5, 0.5, 1),
                                    glm::vec3(0, 0, 0),
                                    glm::vec3(0.0f, 0.0f, 0.0f),
                                    glm::vec3(500.0f, 1, 500.0f));


    Sphere *dynamic_sphere = new Sphere(world,
                                20.0f,
                                glm::vec4(1, 0, 0, 1),
                                glm::vec3(400, 200, 0),
                                glm::vec3(0.0f, 0.0f, 0.0f),
                                20.0f);

    entities.push_back(dynamic_sphere);
    entities.push_back(e_surface);

    glm::mat4 projection_matrix = glm::perspective(75.0f, WIDTH/HEIGHT, 0.1f, 4000.0f);

    shader.bind();
    shader.loadProjectionMatrix(projection_matrix);

    while(!display.isClosed()){
        display.clear(1, 1, 1, 1);
        world->stepSimulation(btScalar(0.05), 1, btScalar(0.1));
        input.update();

        if(input.GetKeyDown(SDLK_SPACE)){
            Sphere *new_sphere = new Sphere(world,
                                            200.0f,
                                            glm::vec4(1, 0, 0, 1),
                                            camera.get_position(),
                                            glm::vec3(0.0f, 0.0f, 0.0f),
                                            20.0f);
            new_sphere->set_linear_velocity(camera.get_forward()*100.0f);
            entities.push_back(new_sphere);
        }

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

        shader.bind();
        shader.loadViewMatrix(camera.get_view_matrix());


        for(Entity *e : entities){
            e->draw(&shader);
        }

        display.update();
    }

    cleanBullet();
    return 0;
}
