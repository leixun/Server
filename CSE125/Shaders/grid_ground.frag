#version 400

/* UNCOMMENT FOR FOG EFFECT (1/2)*/
/*struct FogInfo{
	float maxDist;
	float minDist;
	vec3 color;
};
uniform FogInfo fog;*/

/* UNCOMMENT FOR SKY BOX REFLECTION (1/2)*/
/*uniform samplerCube CubeMapTex;//sky box texture // for reflection */

uniform vec3 color;
uniform float width;

in vec3 position;//position in world
in vec3 normal;//normal in world
flat in vec3 cam;//camera in world

layout (location=0) out vec4 FragColor;

vec3 norm;//norm based on facing, decided in main()

void main()
{
	if( ceil(position[0])-position[0]<width || position[0]-floor(position[0])<width
	 || ceil(position[2])-position[2]<width || position[2]-floor(position[2])<width
	 || abs(ceil(position[0])-0.5-position[0])<width/5.0
	 || abs(ceil(position[2])-0.5-position[2])<width/5.0 )
	{
		FragColor = vec4(color,1.0);
	}
	else
	{
		discard;
	}

	/* UNCOMMENT FOR SKYBOX REFLECTION (2/2) */
	/* //apply skybox reflection
	vec3 reflectDir = reflect(position-cam,normalize(norm));
	vec4 cubeMapColor = texture(CubeMapTex,reflectDir);
	ads = mix(ads,cubeMapColor,material.ReflectFactor);
	//ads = mix(ads,cubeMapColor,1-(1-material.ReflectFactor)*dot(normalize(reflectDir),normalize(norm))); for more complex reflect */

	/* UNCOMMENT FOR FOG EFFECT (2/2)*/
	/* //apply fog
	float dist = distance(position,cam);
	float fog_factor = (fog.maxDist-dist)/(fog.maxDist-fog.minDist);
	fog_factor = clamp(fog_factor,0.0,1.0);
	ads = mix(vec4(fog.color,1.0),ads,fog_factor); */
}