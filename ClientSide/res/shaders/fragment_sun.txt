#version 430

layout(location = 0) out vec3 outColor;

uniform vec4 colorVector;

void main(){
    outColor = colorVector.xyz;
}
