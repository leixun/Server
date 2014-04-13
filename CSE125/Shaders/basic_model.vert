#version 400


uniform	mat4 projMatrix;
uniform	mat4 viewMatrix;
uniform	mat4 modelMatrix;

layout (location=0) in vec3 position;
layout (location=1) in vec2 texCoord;
layout (location=2) in vec3 normal;

//out vec4 vertexPos;
out vec2 TexCoord;
out vec3 Normal;

void main()
{
	Normal = normalize(vec3(viewMatrix * modelMatrix * vec4(normal,0.0)));	
	TexCoord = vec2(texCoord);
	gl_Position = projMatrix * viewMatrix * modelMatrix * vec4(position,1.0);
}