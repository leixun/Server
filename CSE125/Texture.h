#ifndef TEXTURE_H
#define	TEXTURE_H

#pragma once
#include <string>

#include <GL/glew.h>
#include <Qt/QtGui/QImage> 
#include <Qt/QtOpenGL/QGLWidget>

class Texture
{
public:
	Texture(GLenum TextureTarget, const char* FileName, const char* FileType);

	bool Load();
	bool LoadCube(const char* FilePre, const char* FileSuf);

	void Bind(GLenum TextureUnit);

private:
	const char* m_fileName;
	const char* m_fileType;
	GLenum m_textureTarget;
	GLuint m_textureObj;
	QImage timg;
};

#endif	/* TEXTURE_H */