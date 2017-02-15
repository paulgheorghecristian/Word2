#ifndef PUZZLEOBJECT_H
#define PUZZLEOBJECT_H

#include "entity.h"
#include "general_shader.h"
#include <bullet/btBulletDynamicsCommon.h>
#include <string>
#include <vector>
#include <functional>

class PuzzleObject
{
    public:
        PuzzleObject(btDynamicsWorld*, const std::string&, const std::vector<Entity*>&, const std::function<void(PuzzleObject*)>&);
        PuzzleObject(btDynamicsWorld*, const std::string&, const std::vector<Entity*>&, const std::function<void(PuzzleObject*)>&, const std::function<void(PuzzleObject*, unsigned int)>&);
        void update();
        void draw(GeneralShader*);
        void executeAction(unsigned int);
        std::vector<Entity*>& getEntities();
        glm::vec3 boundingRectangle[4];
        glm::mat4& getModelMatrix();
        virtual ~PuzzleObject();
    protected:
    private:
        std::string name;
        std::vector<Entity*> entities;
        glm::vec3 scale;
        std::function<void(PuzzleObject*)> entitiesRelationshipFunction;
        std::function<void(PuzzleObject*, unsigned int)> actionFunction;
        btRigidBody* m_body;
        glm::mat4 modelMatrix;
};

#endif // PUZZLEOBJECT_H
