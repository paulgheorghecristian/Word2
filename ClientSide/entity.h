#ifndef ENTITY_H
#define ENTITY_H

#include "mesh.h"
#include <glm/glm.hpp>
#include "shader.h"
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <vector>
#include <glm/gtc/type_ptr.hpp>
#include <bullet/btBulletDynamicsCommon.h>

class Entity
{
    public:
        Entity(btDynamicsWorld*, std::string, Mesh*, glm::vec4, glm::vec3, glm::vec3, glm::vec3);
        virtual void draw(Shader*);
        void draw(Shader*, float*);
        void setRotation(float, float, float);
        void setScale(float, float, float);
        void setPosition(float, float, float);
        void setPosition(glm::vec3);
        void addRotation(float, float, float);
        void addScale(float, float, float);
        void addPosition(float, float, float);
        glm::mat4 getModelMatrix();
        void setModelMatrix(glm::mat4);
        void setColor(glm::vec4);
        std::string getName();
        glm::vec4 getColor();
        glm::vec3 getPosition();
        glm::vec3 getRotation();
        glm::vec3 getScale();
        virtual ~Entity();
    protected:
        void computeModelMatrix();
        std::string name;
        Mesh* mesh;
        glm::vec3 position, rotation;
        glm::vec3 scale;
        glm::vec4 color;
        glm::mat4 modelMatrix;
        btDynamicsWorld* world;
        btRigidBody *m_body;
    private:
        bool isModelMatrixModified;
};

#endif // ENTITY_H
