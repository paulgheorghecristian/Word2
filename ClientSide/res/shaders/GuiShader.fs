#version 430

out vec3 outColor;

uniform vec4 colorVector;

void main(){
    outColor = colorVector.rgb;
}
