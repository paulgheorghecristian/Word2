#version 430

layout(location = 0) in vec3 inPosition;
layout(location = 0) in vec3 inNormal;

uniform mat4 viewMatrix;

out vec3 eyeSpaceNormal;

void main(){
    eyeSpaceNormal = mat3(viewMatrix) * vec3(0, 0, -1);
    gl_Position = vec4(inPosition, 1.0);
}
