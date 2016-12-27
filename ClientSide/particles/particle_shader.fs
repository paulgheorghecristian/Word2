#version 430

layout(location = 0) out vec3 outEyeSpaceNormal;
layout(location = 1) out vec3 outColor;

in vec2 textCoords;
in vec3 normal;

uniform sampler2D particleSampler;

void main(){
    vec3 textColor = texture(particleSampler, textCoords).rgb;
    if(length(textColor) < 0.3){
        discard;
    }
    outColor = textColor;
    outEyeSpaceNormal = (normalize(normal) + vec3(1)) * 0.5;
}
