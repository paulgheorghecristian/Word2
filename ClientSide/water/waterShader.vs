#version 430

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexcoord;

uniform mat4 viewMatrix, projectionMatrix, modelMatrix;
out vec2 textureCoords;
out vec3 eyeSpaceNormal;


void main(){
	textureCoords = inTexcoord;
	eyeSpaceNormal = (mat3(viewMatrix) * mat3(modelMatrix) * inNormal).xyz;
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(inPosition,1);
}
