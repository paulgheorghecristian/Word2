#version 430

layout(location = 0) out vec3 outColor;

in vec2 textureCoords;
uniform sampler2D textureSampler;
uniform float pixelWidth;
uniform float pixelHeight;

in float pixelSize;

void main(){
    vec2 coords;
    vec3 pixel = vec3(0);
    coords = vec2(textureCoords.x, textureCoords.y + pixelSize*-5);
    pixel += 0.0093* texture(textureSampler, coords).xyz;
    coords = vec2(textureCoords.x, textureCoords.y + pixelSize*-4);
    pixel += 0.028002* texture(textureSampler, coords).xyz;
    coords = vec2(textureCoords.x, textureCoords.y + pixelSize*-3);
    pixel += 0.065984* texture(textureSampler, coords).xyz;
    coords = vec2(textureCoords.x, textureCoords.y + pixelSize*-2);
    pixel += 0.121703* texture(textureSampler, coords).xyz;
    coords = vec2(textureCoords.x, textureCoords.y + pixelSize*-1);
    pixel += 0.175713* texture(textureSampler, coords).xyz;
    coords = vec2(textureCoords.x, textureCoords.y + pixelSize*0);
    pixel += 0.198596* texture(textureSampler, coords).xyz;
    coords = vec2(textureCoords.x, textureCoords.y + pixelSize*1);
    pixel += 0.175713* texture(textureSampler, coords).xyz;
    coords = vec2(textureCoords.x, textureCoords.y + pixelSize*2);
    pixel += 0.121703* texture(textureSampler, coords).xyz;
    coords = vec2(textureCoords.x, textureCoords.y + pixelSize*3);
    pixel += 0.065984* texture(textureSampler, coords).xyz;
    coords = vec2(textureCoords.x, textureCoords.y + pixelSize*4);
    pixel += 0.028002* texture(textureSampler, coords).xyz;
    coords = vec2(textureCoords.x, textureCoords.y + pixelSize*5);
    pixel += 0.0093* texture(textureSampler, coords).xyz;

    outColor = pixel;
}
