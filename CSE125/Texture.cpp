#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "Texture.h"

using namespace std;

Texture::Texture(GLenum TextureTarget, const char* FileName, const char* FileType)
{
	m_textureTarget = TextureTarget;
	m_fileName = FileName;
	m_fileType = FileType;

	//timg = QGLWidget::convertToGLFormat(QImage(m_fileName, m_fileType));
	//glGenTextures(1, &m_textureObj);
}

bool Texture::Load()
{
	timg = QGLWidget::convertToGLFormat(QImage(m_fileName, m_fileType));

	glGenTextures(1, &m_textureObj);
	glBindTexture(m_textureTarget, m_textureObj);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, timg.width(), timg.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, timg.bits());
	glTexParameterf(m_textureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(m_textureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(m_textureTarget, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(m_textureTarget, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	return true;
}

bool Texture::LoadCube(const char* FilePre, const char* FileSuf)
{
	const char * suffixes[] = { "posx", "negx", "posy",
		"negy", "posz", "negz" };
	GLuint targets[] = {
		GL_TEXTURE_CUBE_MAP_POSITIVE_X,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
	};

	glGenTextures(1, &m_textureObj);
	glBindTexture(m_textureTarget, m_textureObj);

	for (int i = 0; i < 6; i++) {
		string texName = string(FilePre) +
			"_" + suffixes[i] + FileSuf;
		timg = QGLWidget::convertToGLFormat(QImage(texName.c_str(), m_fileType));
		glTexImage2D(targets[i], 0, GL_RGBA, timg.width(), timg.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, timg.bits());
	}

	// Typical cube map settings
	glTexParameterf(m_textureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(m_textureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(m_textureTarget, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(m_textureTarget, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(m_textureTarget, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return true;
}

void Texture::Bind(GLenum TextureUnit)
{
	glActiveTexture(TextureUnit);
	glBindTexture(m_textureTarget, m_textureObj);
}