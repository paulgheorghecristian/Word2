#version 430

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexture;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform float dt;

//out vec3 outNormal;

void main(){
    float A = 0.1f, f = 10.0f, w = 30.0f;

    vec2 new_direction = normalize(inPosition.xz);
    float height = 2 * A * pow( ((sin (dot(new_direction, inPosition.xz) * w + -dt * f) + 1.0) / 2.0), 2.0);
    //float normalTerm = 2 * w * A * ( (sin (dot(new_direction, inPosition.xz) * w + -dt * f) + 1) / 2) * cos( (dot(new_direction, inPosition.xz)) * w + -dt * f);
    //outNormal = mat3(viewMatrix) * mat3(modelMatrix) * vec3(normalTerm * new_direction.x, 0.0, normalTerm * new_direction.y);

    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(vec3(inPosition.x, height, inPosition.z), 1.0);
}
