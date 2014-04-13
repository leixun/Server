#version 400
layout (location=0) in vec3 VertexPosition;
layout (location=1) in vec3 VertexNormal;

layout (std140) uniform MVPBlock {
	mat4 ModelMatrix;
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;
	mat4 MVP;	
};

uniform mat4 ProjectorMatrix;

out vec3 position;//position in world
out vec3 normal;//normal in world
flat out vec3 cam;//camera in world
out vec4 ProjTexCoord;//coordinate in projector space

void main()
{
	gl_Position = MVP*vec4(VertexPosition,1.0);

	normal = VertexNormal;
	position = VertexPosition;
	cam = vec3(inverse(ViewMatrix*ModelMatrix)*vec4(0,0,0,1));
	ProjTexCoord = ProjectorMatrix*(vec4(VertexPosition,1.0));
}