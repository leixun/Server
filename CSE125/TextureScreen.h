#pragma once
#include "VAO.h"
#include "glslprogram.h"
#include "Structures.h"
#include <array>

class TextureScreen
{
public:
	TextureScreen(void){
		generate();
		vao.generate();
		vao.addAttrib(GL_ARRAY_BUFFER, 12*sizeof(float), &vertex_positions, GL_STATIC_DRAW, 0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);
		vao.addAttrib(GL_ARRAY_BUFFER, 8*sizeof(float), &texture_coords, GL_STATIC_DRAW, 1, 2, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);
		vao.setDrawMode(GL_QUADS, 0, 4);
	}
	~TextureScreen(void){}
	void draw(){
		shader->setUniform("RenderTex",TextureUnit);
		shader->setUniform("EdgeThreshold",EdgeThreshold);
		shader->setUniform("Width",width);
		shader->setUniform("Height",height);
		shader->use();
		vao.draw();
	}
	void setShader(GLSLProgram* shader){ this->shader=shader;}
	void setTextureUnit(int u){TextureUnit = u;}
	void setWidth(int w){width = w;}
	void setHeight(int h){height = h;}
	void setEdgeThreshold(float f){EdgeThreshold = f;}
private:
	void generate(){
		std::array<float,12> positions = {
			-1,1,0, 1,1,0, 1,-1,0, -1,-1,0
		}; 

		std::array<float,8> texCoords = {
			0,1, 1,1, 1,0, 0,0
		};
		vertex_positions = positions;
		texture_coords = texCoords;
	}
	VAO vao;
	GLSLProgram * shader;
	std::array<float,12> vertex_positions;
	std::array<float,8> texture_coords;
	int TextureUnit;
	int width;
	int height;
	float EdgeThreshold;

};