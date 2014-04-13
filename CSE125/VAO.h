#pragma once
#include <GL/glew.h>
class VAO
{
public:
	VAO(void);
	~VAO(void);
	void generate();
	void addAttrib(GLenum buffer_target,GLsizeiptr buffer_size,const GLvoid * buffer_data,GLenum buffer_usage,
		GLuint attrib_index,GLint attrib_size,GLenum attrib_type,GLboolean attrib_normalized,GLsizei attrib_stride,const GLvoid * attrib_pointer);
	void setDrawMode(GLenum mode,GLint first,GLsizei count);
	void setDrawMode(GLenum mode,GLsizei count,GLenum type, GLvoid* indices);
	void use();
	void draw();

private:
	unsigned int vao;
	GLenum draw_mode;
	GLint draw_first;
	GLsizei draw_count;
	GLenum indice_type;
	GLvoid* indices;
	int mode;//0:draw array;  1:draw elements
};

