#version 430

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexcoord;
layout(location = 3) in mat4 modelMatrix;

uniform mat4 viewMatrix, projectionMatrix;
uniform vec3 sunLightDir;
uniform vec4 clipPlane;

out vec2 textureCoords;
out float lightIntensity;

void main(){
	vec3 worldNormal = normalize(mat3(modelMatrix) * inNormal);
    vec4 worldPosition = modelMatrix * vec4(inPosition, 1);

	lightIntensity = max(dot(worldNormal, sunLightDir), 0.2);
    textureCoords = inTexcoord;

    gl_ClipDistance[0] = dot(worldPosition, clipPlane);
	gl_Position = projectionMatrix * viewMatrix * worldPosition;
}
