#ifndef MATHUTILS_H
#define MATHUTILS_H

#include "camera.h"
#include <glm/glm.hpp>

struct Frustum{
    glm::vec3 nbl, nbr, ntl, ntr, fbl, fbr, ftl, ftr;
};

class MathUtils
{
    public:
        static glm::vec4 getPlaneFromPoints(const glm::vec3&, const glm::vec3&, const glm::vec3&);
        static float getDistanceFromPointToPlane(const glm::vec3&, const glm::vec4&);
        static Frustum* calculateFrustum(Camera *, float, float, float, float);
        static bool isSphereInsideThePlane(const glm::vec3&, const glm::vec4&, float);
        static bool isSphereInsideFrustum(const Frustum*, const glm::vec3&, float);
        static void test();
    protected:
    private:
        MathUtils();
};

#endif // MATHUTILS_H
