#version 430

out vec3 outColor;

in vec2 textureCoords;;
in float lightIntensity;

uniform vec3 sunLightColor;
uniform vec4 colorVector;
uniform int hasTexture;
uniform int hasAlpha;
uniform sampler2D textureSampler;

void main(){
    vec3 albedoColor;
    if(hasTexture == 1){
        albedoColor = texture(textureSampler, vec2(textureCoords.x,1.0-textureCoords.y)).rgb;
        if(hasAlpha == 1){
            if(length(albedoColor) < 0.4){
                discard;
            }
        }
    }else{
        albedoColor = colorVector.rgb;
	}

	outColor = (albedoColor) *
                (sunLightColor * lightIntensity + vec3(0.2, 0.2, 0.2));
}
