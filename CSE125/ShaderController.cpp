#include "ShaderController.h"


ShaderController::ShaderController(void)
{
	binding_point_counter=1;
}


ShaderController::~ShaderController(void)
{
}

void ShaderController::createVFShader(const string& shader_name, const char * vert_file, const char* frag_file )
{
	shaders[shader_name] = GLSLProgram();
	shaders[shader_name].compileShaderFromFile(vert_file,GLSLShader::VERTEX);
	shaders[shader_name].compileShaderFromFile(frag_file,GLSLShader::FRAGMENT);
	shaders[shader_name].link();
}

GLSLProgram* ShaderController::getShader( const string& shader_name )
{
	return &shaders[shader_name];
}

void ShaderController::use( const string& shader_name )
{
	shaders[shader_name].use();
}

void ShaderController::setUniform( const string& shader_name, const char *name, float x, float y, float z )
{
	shaders[shader_name].setUniform(name,x,y,z);
}

void ShaderController::setUniform( const string& shader_name, const char *name, const vec3 & v )
{
	shaders[shader_name].setUniform(name,v);
}

void ShaderController::setUniform( const string& shader_name, const char *name, const vec4 & v )
{
	shaders[shader_name].setUniform(name,v);
}

void ShaderController::setUniform( const string& shader_name, const char *name, const mat4 & m )
{
	shaders[shader_name].setUniform(name,m);
}

void ShaderController::setUniform( const string& shader_name, const char *name, const mat3 & m )
{
	shaders[shader_name].setUniform(name,m);
}

void ShaderController::setUniform( const string& shader_name, const char *name, float val )
{
	shaders[shader_name].setUniform(name,val);
}

void ShaderController::setUniform( const string& shader_name, const char *name, int val )
{
	shaders[shader_name].setUniform(name,val);
}

void ShaderController::setUniform( const string& shader_name, const char *name, bool val )
{
	shaders[shader_name].setUniform(name,val);
}

void ShaderController::createBlockBuffer( const string& buffer_name, GLsizeiptr size, GLenum usage /*= GL_DYNAMIC_DRAW*/ )
{
	GLuint buffer_id;
	glGenBuffers(1,&buffer_id);
	block_buffer[buffer_name]=buffer_id;
	glBindBuffer(GL_UNIFORM_BUFFER, block_buffer[buffer_name]);
	glBufferData(GL_UNIFORM_BUFFER, size, NULL, usage);
	block_buffer_binding_point[buffer_name] = binding_point_counter++;
	glBindBufferBase(GL_UNIFORM_BUFFER, block_buffer_binding_point[buffer_name], block_buffer[buffer_name]);
}

void ShaderController::bindUniformBlock( const string& shader_name, const GLchar * uniformBlockName, const string& buffer_name )
{
	GLuint blockIndex = glGetUniformBlockIndex(shaders[shader_name].getHandle(), uniformBlockName);
	glUniformBlockBinding(shaders[shader_name].getHandle(), blockIndex, block_buffer_binding_point[buffer_name]);
}

void ShaderController::setBufferSubData( const string& buffer_name, GLintptr offset, GLsizeiptr size, const GLvoid * data )
{
	glBindBuffer(GL_UNIFORM_BUFFER, block_buffer[buffer_name]);
	glBufferSubData(GL_UNIFORM_BUFFER, offset , size, data);
}
