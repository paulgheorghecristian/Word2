#version 430

layout(location = 0) out vec3 outColor;

in vec2 textureCoords;
in float pixelSize;
uniform sampler2D textureSampler;
uniform float pixelWidth;
uniform float pixelHeight;

void main(){
    vec2 coords;
    vec3 pixel = vec3(0);
    coords = vec2(textureCoords.x + pixelSize*-5, textureCoords.y);
    pixel += 0.0093* texture(textureSampler, coords).xyz;
    coords = vec2(textureCoords.x + pixelSize*-4, textureCoords.y);
    pixel += 0.028002* texture(textureSampler, coords).xyz;
    coords = vec2(textureCoords.x + pixelSize*-3, textureCoords.y);
    pixel += 0.065984* texture(textureSampler, coords).xyz;
    coords = vec2(textureCoords.x + pixelSize*-2, textureCoords.y);
    pixel += 0.121703* texture(textureSampler, coords).xyz;
    coords = vec2(textureCoords.x - pixelSize, textureCoords.y);
    pixel += 0.175713* texture(textureSampler, coords).xyz;
    coords = vec2(textureCoords.x, textureCoords.y);
    pixel += 0.198596* texture(textureSampler, coords).xyz;
    coords = vec2(textureCoords.x + pixelSize, textureCoords.y);
    pixel += 0.175713* texture(textureSampler, coords).xyz;
    coords = vec2(textureCoords.x + pixelSize*2, textureCoords.y);
    pixel += 0.121703* texture(textureSampler, coords).xyz;
    coords = vec2(textureCoords.x + pixelSize*3, textureCoords.y);
    pixel += 0.065984* texture(textureSampler, coords).xyz;
    coords = vec2(textureCoords.x + pixelSize*4, textureCoords.y);
    pixel += 0.028002* texture(textureSampler, coords).xyz;
    coords = vec2(textureCoords.x + pixelSize*5, textureCoords.y);
    pixel += 0.0093* texture(textureSampler, coords).xyz;
    outColor = pixel;
}
