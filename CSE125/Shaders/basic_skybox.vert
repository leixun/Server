#version 400
layout (location=0) in vec3 VertexPosition;
layout (location=1) in vec3 VertexNormal;

uniform mat4 ProjectionMatrix;
uniform mat4 ViewMatrix;

out vec3 position;//position in world
out vec3 normal;//normal in world
flat out vec3 cam;//camera in world

void main()
{
	gl_Position = ProjectionMatrix*ViewMatrix*vec4(VertexPosition,1.0);

	normal = VertexNormal;
	position = VertexPosition;
	cam = vec3(inverse(ViewMatrix)*vec4(0,0,0,1));
}