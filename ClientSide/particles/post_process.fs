#version 430

layout(location = 0) out vec3 outColor;

in vec2 textureCoords;
uniform sampler2D textureSampler;
uniform float pixelWidth;
uniform float pixelHeight;

void main(){
    outColor = texture(textureSampler, textureCoords).xyz;
}
