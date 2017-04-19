#version 430

layout(location = 0) out vec3 outColor;
layout(location = 1) out vec3 outEyeSpaceNormal;

in vec2 textureCoords;;
in vec3 eyeSpaceNormal;

uniform sampler2D blendmap;
uniform sampler2D dirt;
uniform sampler2D grass;
uniform sampler2D soil;
uniform sampler2D rock;

void main(){

	vec4 blendmap_color = texture(blendmap, vec2(textureCoords.x, 1.0-textureCoords.y));
	vec2 tilecoords = textureCoords * 10.0;

	float black_amt = 1.0 - (blendmap_color.r + blendmap_color.g + blendmap_color.b);

	vec4 dirt_color = texture(dirt, tilecoords) * blendmap_color.b;
	vec4 grass_color = texture(grass, tilecoords) * blendmap_color.r;
	vec4 soil_color = texture(soil, tilecoords) * black_amt;
	vec4 rock_color = texture(rock, tilecoords) * blendmap_color.g;

	outColor = grass_color.rgb + dirt_color.rgb + soil_color.rgb + rock_color.rgb;
	outEyeSpaceNormal = (normalize(eyeSpaceNormal) + vec3(1)) * 0.5;
}
