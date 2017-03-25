#version 330 core

in vec2 textureCoords;
uniform sampler2D textureSampler;
uniform sampler2D lensFlareColorSampler;
uniform float pixelWidth;
uniform float pixelHeight;
out vec4 outColor;

uniform vec3 sunPosition;

void main(){
    const float density = 1.0;
    const float exposure = 1.00;
    const float weight = 0.05;
    const float decay = 1.00;

    vec2 coords = textureCoords;
    vec2 center = vec2(sunPosition.x, sunPosition.y);
    vec2 deltaTexCoord = textureCoords - center;
    int numSamples = 50;
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
    vec4 outColor1 = vec4(color * exposure, 1);

    vec2 texcoord = -textureCoords + vec2(1.0);
    vec2 texelSize = 1.0 / vec2(pixelWidth, pixelHeight);
    float uGhostDispersal = 0.2f;
    int uGhosts = 4;
    float uHaloWidth = 0.45;

    vec2 ghostVec = (vec2(0.5) - texcoord) * uGhostDispersal;

    vec4 result = vec4(0.0);
    for (int i = 0; i < uGhosts; i++) {
        vec2 offset = fract(texcoord + ghostVec * float(i));
        result += texture(textureSampler, offset);
    }

    vec2 haloVec = normalize(ghostVec) * uHaloWidth;
    float weight2 = length(vec2(0.5) - fract(texcoord + haloVec)) / length(vec2(0.5));
    weight2 = pow(1.0 - weight2, 5.0);
    vec4 outColor2 = texture(textureSampler, texcoord + haloVec) * weight2;

    vec2 texcoord2 = vec2(length(vec2(0.5) - texcoord) / length(vec2(0.5)), 0.0);
    outColor = outColor1 + (result+outColor2) * texture(lensFlareColorSampler, texcoord2);
}
