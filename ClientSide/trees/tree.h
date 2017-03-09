#ifndef TREE_H
#define TREE_H

#include <glm/glm.hpp>
#include <string>
#include "../mesh.h"

class Tree
{
    public:
        Tree(std::string name, Mesh *trunkMesh, Mesh *crownMesh, glm::vec3 scale);
        Mesh* getTrunkMesh();
        Mesh* getCrownMesh();
        glm::vec3 getScale();
        virtual ~Tree();
    protected:

    private:
        std::string name;
        Mesh *trunkMesh;
        Mesh *crownMesh;
        glm::vec3 scale;
};

#endif // TREE_H
