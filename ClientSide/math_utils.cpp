#include "math_utils.h"

MathUtils::MathUtils()
{

}

glm::vec4 MathUtils::getPlaneFromPoints(const glm::vec3& point1, const glm::vec3& point2, const glm::vec3& point3){
    glm::vec3 u = point2 - point1;
    glm::vec3 v = point3 - point1;

    glm::vec3 normal = glm::normalize(glm::cross(u, v));
    float D = -glm::dot(normal, point1);

    return glm::vec4(normal.x, normal.y, normal.z, D);
}

float MathUtils::getDistanceFromPointToPlane(const glm::vec3& point, const glm::vec4& plane){
    glm::vec3 normal = glm::vec3(plane.x, plane.y, plane.z);

    return glm::dot(normal, point) + plane.w;
}

bool MathUtils::isSphereInsideThePlane(const glm::vec3& sphereCenter, const glm::vec4& plane, float radius){
    glm::vec3 normal(plane.x, plane.y, plane.z);
    return (glm::dot(sphereCenter, normal) + plane.w - radius) < 0;
}

bool MathUtils::isSphereInsideFrustum(const Frustum *frustum, const glm::vec3& sphereCenter, float radius){
    glm::vec4 plane1 = getPlaneFromPoints(frustum->nbl, frustum->ntl, frustum->ntr);
    glm::vec4 plane2 = getPlaneFromPoints(frustum->nbr, frustum->ntr, frustum->ftr);
    glm::vec4 plane3 = getPlaneFromPoints(frustum->fbl, frustum->ftl, frustum->ntl);
    glm::vec4 plane4 = getPlaneFromPoints(frustum->ntl, frustum->ftl, frustum->ftr);
    glm::vec4 plane5 = getPlaneFromPoints(frustum->nbr, frustum->fbr, frustum->fbl);
    glm::vec4 plane6 = getPlaneFromPoints(frustum->ftr, frustum->ftl, frustum->fbl);

    return isSphereInsideThePlane(sphereCenter, plane1, radius) &&
            isSphereInsideThePlane(sphereCenter, plane2, radius) &&
            isSphereInsideThePlane(sphereCenter, plane3, radius) &&
            isSphereInsideThePlane(sphereCenter, plane4, radius) &&
            isSphereInsideThePlane(sphereCenter, plane5, radius) &&
            isSphereInsideThePlane(sphereCenter, plane6, radius);
}

Frustum* MathUtils::calculateFrustum(Camera *camera, float near, float far, float fov, float aspect){
    Frustum *f = new Frustum();
    glm::vec3 cameraForward = camera->getForward();
    glm::vec3 cameraPosition = camera->getPosition();
    glm::vec3 cameraUp = camera->getUp();
    glm::vec3 cameraRight = camera->getRight();
    glm::vec3 farCenter = cameraForward*far + cameraPosition;
    glm::vec3 nearCenter = cameraForward*near + cameraPosition;
    glm::mat4 viewMatrixInverse = glm::inverse(camera->getViewMatrix());

    float nearPlaneHeight = 2 * glm::tan(glm::radians(fov/2.0)) * near;
    float nearPlaneWidth = nearPlaneHeight * aspect;

    float farPlaneHeight = 2 * glm::tan(glm::radians(fov/2.0)) * far;
    float farPlaneWidth = farPlaneHeight * aspect;

    f->fbl = glm::vec3(farCenter - cameraUp*(farPlaneHeight/2.0f) - cameraRight*(farPlaneWidth/2.0f));
    f->fbr = glm::vec3(farCenter - cameraUp*(farPlaneHeight/2.0f) + cameraRight*(farPlaneWidth/2.0f));
    f->ftl = glm::vec3(farCenter + cameraUp*(farPlaneHeight/2.0f) - cameraRight*(farPlaneWidth/2.0f));
    f->ftr = glm::vec3(farCenter + cameraUp*(farPlaneHeight/2.0f) + cameraRight*(farPlaneWidth/2.0f));

    f->nbl = glm::vec3(nearCenter - cameraUp*(nearPlaneHeight/2.0f) - cameraRight*(nearPlaneWidth/2.0f));
    f->nbr = glm::vec3(nearCenter - cameraUp*(nearPlaneHeight/2.0f) + cameraRight*(nearPlaneWidth/2.0f));
    f->ntl = glm::vec3(nearCenter + cameraUp*(nearPlaneHeight/2.0f) - cameraRight*(nearPlaneWidth/2.0f));
    f->ntr = glm::vec3(nearCenter + cameraUp*(nearPlaneHeight/2.0f) + cameraRight*(nearPlaneWidth/2.0f));

    return f;
}

bool MathUtils::isPointInsideRectangle(glm::vec3 A, glm::vec3 B, glm::vec3 D, glm::vec3 M){
    //r1->r2 r1->r3
    glm::vec3 AB = B-A;
    glm::vec3 AD = D-A;
    glm::vec3 AM = M-A;

    float dotAMAB = glm::dot(AM, AB);
    float dotABAB = glm::dot(AB, AB);
    float dotAMAD = glm::dot(AM, AD);
    float dotADAD = glm::dot(AD, AD);

    return (0 < dotAMAB && dotAMAB < dotABAB && 0 < dotAMAD && dotAMAD < dotADAD);
}

void MathUtils::test(){
    Camera *camera = new Camera(glm::vec3(0, 0, 0), 0, 0, 0);

    Frustum *f = calculateFrustum(camera, 1.0f, 500.0f, 75.0f, 4.0f/3.0f);

    glm::vec3 spherePos = glm::vec3(0, 0, -62.0);
    float radius = 50.0f;

    std::cout << isSphereInsideThePlane(spherePos, getPlaneFromPoints(f->nbl, f->ntl, f->ntr), radius) << std::endl;

    std::cout << isSphereInsideFrustum(f, spherePos, radius) << std::endl;
}
