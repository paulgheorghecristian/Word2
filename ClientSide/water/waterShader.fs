#version 430

layout(location = 0) out vec3 outColor;
layout(location = 1) out vec3 outEyeSpaceNormal;

uniform int width;
uniform int height;
uniform sampler2D refractionSampler;
uniform sampler2D reflectionSampler;
uniform sampler2D DuDvTextureSampler;
uniform sampler2D normalMapTextureSampler;
uniform vec4 colorVector;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform float dudvOffset;

in vec2 textureCoords;

const float waveStrength = 0.02;

void main(){
    vec2 distortedTexCoords = texture(DuDvTextureSampler, vec2(textureCoords.x + dudvOffset, textureCoords.y)).rg*0.2;
	distortedTexCoords = textureCoords + vec2(distortedTexCoords.x, distortedTexCoords.y+dudvOffset);
	vec2 totalDistortion = (texture(DuDvTextureSampler, distortedTexCoords).rg * 2.0 - 1.0) * waveStrength;

    vec2 projectedTextureCoords = vec2(gl_FragCoord.x / (width), 1.0 - gl_FragCoord.y / (height));
    projectedTextureCoords += totalDistortion;
    outColor = texture(reflectionSampler, projectedTextureCoords).rgb;
    outColor = mix(outColor, vec3(0, 0.3, 0.5), 0.2);

    vec3 normal = texture(normalMapTextureSampler, distortedTexCoords).rgb;
    vec3 transformedNormal = vec3(normal.r * 2.0 - 1.0, normal.b, normal.g * 2.0 - 1.0);
	outEyeSpaceNormal = (normalize(mat3(viewMatrix) * transformedNormal) + vec3(1)) * 0.5;
}
