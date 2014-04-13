#pragma once
#include "glslprogram.h"
#include <map>

using std::map;

/*
1. createVFShader: add a shader program to controller
//for single uniform data:
		setUniform: set the uniform of the shader program
//for uniform block:
		createBlockBuffer: create the buffer
		bindUniformBlock: bind block-bindingPoint-buffer //bind only once. No need to call every time after setBufferSubData call.
		setBufferSubData: set the data of buffer
*/


class ShaderController
{
public:
	ShaderController(void);
	~ShaderController(void);
	void createVFShader(const string& shader_name, const char * vert_file, const char* frag_file);
	GLSLProgram* getShader(const string& shader_name);
	void use(const string& shader_name);
	void setUniform( const string& shader_name, const char *name, float x, float y, float z);
	void setUniform( const string& shader_name, const char *name, const vec3 & v);
	void setUniform( const string& shader_name, const char *name, const vec4 & v);
	void setUniform( const string& shader_name, const char *name, const mat4 & m);
	void setUniform( const string& shader_name, const char *name, const mat3 & m);
	void setUniform( const string& shader_name, const char *name, float val );
	void setUniform( const string& shader_name, const char *name, int val );
	void setUniform( const string& shader_name, const char *name, bool val );
	void createBlockBuffer(const string& buffer_name, GLsizeiptr size, GLenum usage = GL_DYNAMIC_DRAW);
	void setBufferSubData(const string& buffer_name, GLintptr offset, GLsizeiptr size, const GLvoid * data);
	void bindUniformBlock(const string& shader_name, const GLchar * uniformBlockName, const string& buffer_name);
private:
	map<string,GLSLProgram> shaders;
	map<string,GLuint> block_buffer;
	map<string,GLuint> block_buffer_binding_point;
	GLuint binding_point_counter;
};

