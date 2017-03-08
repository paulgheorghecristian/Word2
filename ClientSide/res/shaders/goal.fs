#version 430

//layout(location = 1) out vec3 outEyeSpaceNormal;
layout(location = 0) out vec3 outColor;

uniform vec4 colorVector;

//in vec3 outNormal;

void main(){
    //outEyeSpaceNormal = (normalize(outNormal) + vec3(1)) * 0.5;
    outColor = colorVector.rgb;
}
