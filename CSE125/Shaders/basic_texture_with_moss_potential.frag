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

uniform Material material;
uniform Light light[1];//to add more light, increment array size, update ads()
uniform sampler2D Texture;//texture unit

/* UNCOMMENT FOR SKY BOX REFLECTION (1/2)*/
/*uniform samplerCube CubeMapTex;//sky box texture // for reflection */

/* UNCOMMENT FOR MOSS TEXTURE (1/2)*/
/*uniform sampler2D MossTex;//moss texture unit */

in vec3 position;//position in world
in vec3 normal;//normal in world
in vec2 texCoord;//texture coordinate
flat in vec3 cam;//camera in world

layout (location=0) out vec4 FragColor;

vec3 norm;//norm based on facing

vec4 ads(vec4 TexColor){
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
	
	if( light[0].type!=2 ){//directional and point light
		ads += max(TexColor*vec4(light[0].ambient*material.Ka,1.0),
		           TexColor*vec4(light[0].diffuse*material.Kd*max(dot(s,n),0.0),1.0) 
		           + light[0].specular*material.Ks*pow(max(dot(r,v),0.0), material.Shininess));
	}else if( light[0].type==2 && dot(normalize(vec3(-light[0].dir)),s) >= light[0].spotCutOff){//spot light
		ads += (dot(normalize(vec3(-light[0].dir)),s)-light[0].spotCutOff)/(1-light[0].spotCutOff)//for soft edge
				*max(TexColor*vec4(light[0].ambient*material.Ka,1.0), 
				     TexColor*vec4(light[0].diffuse*material.Kd*max(dot(s,n),0.0),1.0) 
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

	//apply texture
	vec4 texColor = texture( Texture, texCoord );
	/* UNCOMMENT FOR MOSS TEXTURE (2/2) */
	/* vec4 mossColor = texture( MossTex, texCoord);
	//texColor = mix(texColor,mossColor,mossColor.a); //overlay two texture
	//if(mossColor.a<0.5) discard; //or create hollow effect */

	//apply light
	texColor = ads(texColor);

	/* UNCOMMENT FOR SKYBOX REFLECTION (2/2) */
	/* //apply skybox reflection
	vec3 reflectDir = reflect(position-cam,normalize(norm));
	vec4 cubeMapColor = texture(CubeMapTex,reflectDir);
	texColor = mix(texColor,cubeMapColor,material.ReflectFactor); */

	ads+=texColor;

	FragColor = ads;
}