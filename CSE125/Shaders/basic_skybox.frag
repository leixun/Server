#version 400

/* UNCOMMENT FOR FOG (1/2)*/
/*struct FogInfo{
	float maxDist;
	float minDist;
	vec3 color;
};
uniform FogInfo fog;*/

uniform samplerCube CubeMapTex;//sky box texture unit

in vec3 position;//position in world
in vec3 normal;//normal in world
flat in vec3 cam;//camera in world

layout (location=0) out vec4 FragColor;

void main()
{
	vec4 ads=vec4(0.0,0.0,0.0,1.0);

	//apply cube map
	vec3 skyDir = position-cam;
	vec4 cubeMapColor = texture(CubeMapTex,skyDir);
	ads += cubeMapColor;

	/* UNCOMMENT FOR FOG (2/2)*/
	/*//apply fog
	float dist = distance(position,cam);
	float fog_factor = (fog.maxDist-dist)/(fog.maxDist-fog.minDist);
	fog_factor = clamp(fog_factor,0.0,1.0);
	ads = mix(vec4(fog.color,1.0),ads,fog_factor);*/

	FragColor = ads;
}