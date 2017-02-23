#version 330 core

in vec2 textureCoords;
uniform sampler2D textureSampler;
uniform float pixelWidth;
uniform float pixelHeight;
out vec4 outColor;

uniform vec2 sunPosition;

void main(){
    const float density = 1.0;
    const float exposure = 1.0;
    const float weight = 0.015;
    const float decay = 1.00;

    vec2 coords = textureCoords;
    vec2 center = vec2(sunPosition.x, sunPosition.y);
    vec2 deltaTexCoord = textureCoords - center;
    int numSamples = 100;
    deltaTexCoord *= (1.0f / float(numSamples)) * density;

    vec3 color = texture(textureSampler, textureCoords).xyz;
    float illuminationDecay = 1.0;

    for(int i = 0; i < numSamples; i++){
        coords -= deltaTexCoord;
        vec3 sample = texture(textureSampler, coords).xyz;
        sample *= illuminationDecay * weight;
        color += sample;
        illuminationDecay *= decay;
    }
   outColor = vec4(color * exposure, 1);

}
