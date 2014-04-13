#version 400

struct Material{
	vec3 Kd;
	vec3 Ka;
	vec3 Ks;
	float Shininess;
	float ReflectFactor;
};

struct Light{
	int type;//0:directional  1:point  2:spot
	vec4 pos;
	vec4 dir;
	float spotCutOff;//cos(a)
	vec3 specular;
	vec3 diffuse;
	vec3 ambient;
};

/* UNCOMMENT FOR FOG EFFECT (1/2)*/
/*struct FogInfo{
	float maxDist;
	float minDist;
	vec3 color;
};
uniform FogInfo fog;*/

/* UNCOMMENT FOR SKY BOX REFLECTION (1/2)*/
/*uniform samplerCube CubeMapTex;//sky box texture // for reflection */

uniform Material material;
uniform Light light[1];//one light for basic shader. To add light, increment light array size, then update ads()
uniform sampler2D ProjectorTex;

in vec3 position;//position in world
in vec3 normal;//normal in world
flat in vec3 cam;//camera in world
in vec4 ProjTexCoord;//coordinate in projector space

layout (location=0) out vec4 FragColor;

vec3 norm;//norm based on facing, decided in main()

vec4 ads(){
	vec4 ads=vec4(0.0,0.0,0.0,1.0);
	vec3 n = normalize(norm);
	vec3 s,v,r;

	//light[0]
	if(light[0].type==0){
		s = normalize( vec3(light[0].pos));
	}else{
		s = normalize( vec3(light[0].pos)-position);
	}
	v = normalize(vec3(cam-position));
	r = reflect(-s,n);
	if( light[0].type!=2 ){//direction light and point light
		ads += max( vec4(light[0].ambient*material.Ka,1.0), 
		            vec4(light[0].diffuse*material.Kd*max(dot(s,n),0.0),1.0) 
		            + light[0].specular*material.Ks*pow(max(dot(r,v),0.0), material.Shininess));
	}else if( light[0].type==2 && dot(normalize(vec3(-light[0].dir)),s) >= light[0].spotCutOff){//spot light
		ads += (dot(normalize(vec3(-light[0].dir)),s)-light[0].spotCutOff)/(1-light[0].spotCutOff)//to make soft edge
			   * max( vec4(light[0].ambient*material.Ka,1.0),  
			          vec4(light[0].diffuse*material.Kd*max(dot(s,n),0.0),1.0) 
			          + light[0].specular*material.Ks*pow(max(dot(r,v),0.0), material.Shininess));
	}

	return ads;
}

void main()
{
	vec4 ads=vec4(0.0,0.0,0.0,1.0);

	if(gl_FrontFacing){
		norm = normal;
	}else{
		norm = -normal;
	}

	//apply light
	ads+=ads();

	//apply projector texture
	vec4 projTexColor = vec4(0.0);
	if(ProjTexCoord.z>0){
		projTexColor = textureProj(ProjectorTex,ProjTexCoord);
	}
	ads+=0.5*projTexColor;

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

	FragColor = ads;
}