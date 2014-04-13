#version 400

struct Material{
	vec3 Kd;
	vec3 Ka;
	vec3 Ks;
	float Shininess;
	vec2 ReflectFactor;
	float Eta;//refraction: 0 to 1. 1 for no banding. 0 for band to nagtive normal.
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

uniform samplerCube CubeMapTex;//sky box texture // for reflection 
uniform Material material;
uniform Light light[1];//one light for basic shader. To add light, increment light array size, then update ads()

in vec3 position;//position in world
in vec3 normal;//normal in world
flat in vec3 cam;//camera in world

layout (location=0) out vec4 FragColor;

vec3 norm;//norm based on facing, decided in main()

vec4 myads(){
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
		            + vec4(light[0].specular*material.Ks*pow(max(dot(r,v),0.0), material.Shininess),1.0));
	}else if( light[0].type==2 && dot(normalize(vec3(-light[0].dir)),s) >= light[0].spotCutOff){//spot light
		ads += (dot(normalize(vec3(-light[0].dir)),s)-light[0].spotCutOff)/(1-light[0].spotCutOff)//to make soft edge
			   * max( vec4(light[0].ambient*material.Ka,1.0),  
			          vec4(light[0].diffuse*material.Kd*max(dot(s,n),0.0),1.0) 
			          + vec4(light[0].specular*material.Ks*pow(max(dot(r,v),0.0), material.Shininess),1.0));
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

	//apply skybox reflection
	vec3 reflectDir = reflect(position-cam,normalize(norm));
	vec4 reflectColor = texture(CubeMapTex,reflectDir);

	//apply skybox refraction
	vec3 refractDir = refract(position-cam,normalize(norm),material.Eta);
	vec4 refractColor = texture(CubeMapTex,refractDir);

	//ads += mix(refractColor, reflectColor, material.ReflectFactor[0]); //simper. ignores material color
	vec4 reColor = mix(refractColor, reflectColor, material.ReflectFactor[1]-(material.ReflectFactor[1]-material.ReflectFactor[0])*dot(normalize(reflectDir),normalize(norm)));
	ads += mix(myads(),reflectColor,material.ReflectFactor[1]-(material.ReflectFactor[1]-material.ReflectFactor[0])*dot(normalize(reflectDir),normalize(norm)));
	//ads += mix(ads(),reColor,material.ReflectFactor[1]-(material.ReflectFactor[1]-material.ReflectFactor[0])*dot(normalize(reflectDir),normalize(norm)));

	FragColor = ads;
}