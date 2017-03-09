#include "tree.h"

Tree::Tree(std::string name,
           Mesh *trunkMesh,
           Mesh *crownMesh) : name(name), trunkMesh(trunkMesh), crownMesh(crownMesh)
{

}

Mesh* Tree::getTrunkMesh(){
    return trunkMesh;
}

Mesh* Tree::getCrownMesh(){
    return crownMesh;
}

Tree::~Tree()
{
    delete trunkMesh;
    delete crownMesh;
}
