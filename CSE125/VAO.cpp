#include "VAO.h"
#include <iostream>
using namespace std;

VAO::VAO(void)
{
}


VAO::~VAO(void)
{
}

void VAO::generate()
{
	//glGenVertexArrays (1, &vao);
}

void VAO::addAttrib( GLenum buffer_target,GLsizeiptr buffer_size,const GLvoid * buffer_data,GLenum buffer_usage, 
					GLuint attrib_index,GLint attrib_size,GLenum attrib_type,GLboolean attrib_normalized,GLsizei attrib_stride,const GLvoid * attrib_pointer )
{
	//glBindVertexArray (vao);
	//glEnableVertexAttribArray (attrib_index);
	//unsigned int vbo = 0;
	//glGenBuffers (1, &vbo);
	//glBindBuffer (buffer_target, vbo);
	//glBufferData (buffer_target, buffer_size, buffer_data, buffer_usage);
	//glVertexAttribPointer (attrib_index, attrib_size, attrib_type, attrib_normalized, attrib_stride, attrib_pointer);
}

void VAO::setDrawMode( GLenum mode,GLint first,GLsizei count )
{
	draw_mode=mode;
	draw_first=first;
	draw_count=count;
	this->mode=0;
}

void VAO::setDrawMode( GLenum mode,GLsizei count,GLenum type, GLvoid* indices )
{
	draw_mode=mode;
	draw_count=count;
	indice_type=type;
	this->indices=indices;
	this->mode=1;
}

void VAO::use()
{
	glBindVertexArray (vao);
}

void VAO::draw()
{
	use();
	if(mode==0)
		glDrawArrays (draw_mode,draw_first,draw_count);
	else if(mode==1)
		glDrawElements(draw_mode,draw_count,indice_type,indices);
}