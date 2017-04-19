#version 430

layout(location = 0) out vec3 outColor;
layout(location = 1) out vec3 outEyeSpaceNormal;

uniform int width;
uniform int height;
uniform sampler2D refractionSampler;
uniform sampler2D reflectionSampler;
uniform vec4 colorVector;
uniform mat4 viewMatrix;
in vec3 eyeSpaceNormal;

in vec2 textureCoords;

void main(){
    vec2 projectedTextureCoords = vec2(gl_FragCoord.x / (width*8), 1.0 - gl_FragCoord.y / (height*8));
    outColor = texture(reflectionSampler, projectedTextureCoords).rgb;//colorVector.rgb;
	outEyeSpaceNormal = (normalize(eyeSpaceNormal) + vec3(1)) * 0.5;
}
