#version 430

out vec3 outColor;

in vec2 textureCoords;;
in float lightIntensity;

uniform vec3 sunLightColor;
uniform vec4 colorVector;
uniform int hasTexture;
uniform sampler2D textureSampler;

void main(){
    vec3 albedoColor;
    if(hasTexture == 1){
        vec3 color = texture(textureSampler, vec2(textureCoords.x,1.0-textureCoords.y)).rgb;
        albedoColor = color * colorVector.rgb;
    }else{
        albedoColor = colorVector.rgb;
	}

	outColor = (albedoColor) *
                (sunLightColor * lightIntensity + vec3(0.2, 0.2, 0.2));
}
