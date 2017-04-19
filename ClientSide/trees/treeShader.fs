#version 430

layout(location = 0) out vec3 outColor;
layout(location = 1) out vec3 outEyeSpaceNormal;

uniform sampler2D textureSampler;
uniform vec4 colorVector;
uniform int hasTexture;
uniform int hasAlpha;

in vec3 eyeSpaceNormal;
in vec2 textureCoords;

void main(){
    if(hasTexture == 1){
        vec3 color = texture(textureSampler, vec2(textureCoords.x,1.0-textureCoords.y)).rgb;
        if(hasAlpha == 1){
            if(length(color) < 0.4){
                discard;
            }
        }
        outColor = color;
    }else{
        outColor = colorVector.rgb;
	}
    int frontCond = -(1 - int(gl_FrontFacing)*2);
	outEyeSpaceNormal = (normalize(eyeSpaceNormal * frontCond) + vec3(1)) * 0.5;
}
