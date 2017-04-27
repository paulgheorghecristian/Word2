#version 430

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexcoord;

uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform mat4 projectionMatrix;

void main(){
    mat4 newModelMatrix = modelMatrix;
    newModelMatrix[0][0] = viewMatrix[0][0];
    newModelMatrix[0][1] = viewMatrix[1][0];
    newModelMatrix[0][2] = viewMatrix[2][0];
    newModelMatrix[1][0] = viewMatrix[0][1];
    newModelMatrix[1][1] = viewMatrix[1][1];
    newModelMatrix[1][2] = viewMatrix[2][1];
    newModelMatrix[2][0] = viewMatrix[0][2];
    newModelMatrix[2][1] = viewMatrix[1][2];
    newModelMatrix[2][2] = viewMatrix[2][2];
    gl_Position = projectionMatrix * viewMatrix * newModelMatrix * vec4(inPosition, 1.0);
}
