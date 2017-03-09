#ifndef TREERENDERER_H
#define TREERENDERER_H

#include <vector>
#include <glm/glm.hpp>

#include "tree.h"

class TreeRenderer
{
    public:
        TreeRenderer(const std::vector<glm::vec3>& positionsAndRotations, Tree *wantedTreeModel);
        GLuint createTrunkVbo(const std::vector<Vertex>& vertices,
                              const std::vector<unsigned int>& indices);
        GLuint createCrownVbo(const std::vector<Vertex>& vertices,
                              const std::vector<unsigned int>& indices);
        virtual ~TreeRenderer();
    protected:
    private:
        std::vector<glm::vec3> positionsAndRotations;
        Tree *tree;
        GLuint trunkVao, crownVao;
};

#endif // TREERENDERER_H
