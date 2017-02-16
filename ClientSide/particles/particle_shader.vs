#version 430

layout(location = 0) in mat4 viewModelMatrix;

uniform mat4 viewMatrix;

out vec3 eyeSpaceNormal;
out mat4 viewModelMatrixOut;

void main(){
    eyeSpaceNormal = mat3(viewMatrix) * vec3(0, 0, -1);
    viewModelMatrixOut = viewModelMatrix;
    gl_Position = vec4(vec3(0), 1.0);
}
