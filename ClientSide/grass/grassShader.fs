#version 430

layout(location = 0) out vec3 outEyeSpaceNormal;
layout(location = 1) out vec3 outColor;

uniform sampler2D textureSampler;
uniform vec4 colorVector;
uniform int hasTexture;
uniform int hasAlpha;

in vec3 eyeSpaceNormal;
in vec2 textureCoords;

void main(){
    vec3 color = texture(textureSampler, vec2(textureCoords.x,1.0-textureCoords.y)).rgb;
    if(length(color) > 0.9){
        discard;
    }
    outColor = color;
	outEyeSpaceNormal = (normalize(eyeSpaceNormal) + vec3(1)) * 0.5;
}
