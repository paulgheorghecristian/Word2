#include "player.h"

Mesh* Player::mesh;

Player::Player(btDynamicsWorld* world, float mass, glm::vec4 color, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) : Entity(world, "player", Player::mesh, color, position, rotation, scale)
{
    btTransform t;
    btVector3 inertia(0, 0, 0);

    t.setIdentity();
    t.setOrigin(btVector3(position.x, position.y, position.z));

    btCapsuleShape* capsuleShape = new btCapsuleShape(scale.x* 3.0f, scale.y*2.5f);
    if(mass != 0.0){
        capsuleShape->calculateLocalInertia(btScalar(mass), inertia);
    }
    btMotionState* motion = new btDefaultMotionState(t);
    btRigidBody::btRigidBodyConstructionInfo info(mass, motion, capsuleShape, inertia);
    info.m_restitution = 0.2f;
    info.m_friction = 1.0f;

    m_body = new btRigidBody(info);

    m_body->setDamping(btScalar(0), btScalar(1.0));
    m_body->setSleepingThresholds(0.0, 0.0);
    m_body->setAngularFactor(0.0);

    world->addRigidBody(m_body);
}

void Player::draw(Shader *shader){
    btTransform t;

    m_body->getMotionState()->getWorldTransform(t);
    float mat[16];
    t.getOpenGLMatrix(mat);

    position_for_camera = glm::vec3(mat[12], mat[13], mat[14]);

    this->set_model_matrix(glm::scale(glm::make_mat4(mat), scale));
    Entity::draw(shader);
}

glm::vec3 Player::get_position_for_camera(){
    return position_for_camera;
}

void Player::set_mesh(Mesh *mesh){
    Player::mesh = mesh;
}

Player::~Player()
{
    delete m_body;
}
