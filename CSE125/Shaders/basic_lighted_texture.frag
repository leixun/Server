#version 400

struct Material{
	vec3 Kd;
	vec3 Ka;
	vec3 Ks;
	float Shininess;
	vec2 ReflectFactor;
	float Eta;//refraction: 0 to 1. 1 for no banding. 0 for band to nagtive normal.
	float DiffuseFactor;//global diffuse light: 0 to 1. 0 for no global diffuse. 1 for full global diffuse
	float SpecFactor;//global specular light: 0 to 1. 0 for no global specular. 1 for full global specular
};

uniform samplerCube DiffuseMap; // The diffuse env. map
uniform samplerCube SpecMap; // The specular env. map
uniform Material material;

in vec3 position;//position in world
in vec3 normal;//normal in world
flat in vec3 cam;//camera in world

layout (location=0) out vec4 FragColor;

vec3 norm;//norm based on facing, decided in main()

void main()
{
	if(gl_FrontFacing){
		norm = normal;
	}else{
		norm = -normal;
	}

	vec4 diffuseIrr = texture(DiffuseMap, norm);
	vec3 specDir = reflect(position-cam,normalize(norm));
	vec4 specIrr = texture(SpecMap, specDir);

	vec3 color = mix( material.Kd,
					  material.Kd * vec3(diffuseIrr),
					  material.DiffuseFactor );
	// The specular comp. is added to the diffuse color
	color = color + vec3(specIrr) * material.SpecFactor;

	FragColor = vec4(color,1.0);
}