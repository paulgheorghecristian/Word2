#include "tree.h"

Tree::Tree(std::string name,
           Mesh *trunkMesh,
           Mesh *crownMesh,
           glm::vec3 scale) : name(name), trunkMesh(trunkMesh), crownMesh(crownMesh), scale(scale)
{

}

Mesh* Tree::getTrunkMesh(){
    return trunkMesh;
}

Mesh* Tree::getCrownMesh(){
    return crownMesh;
}

glm::vec3 Tree::getScale(){
    return scale;
}

Tree::~Tree()
{
    delete trunkMesh;
    delete crownMesh;
}
