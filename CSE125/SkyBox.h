#pragma once
#include "VAO.h"
#include "glslprogram.h"
#include <array>

class SkyBox: public Object
{
public:
	SkyBox(float negx,float posx,float negy,float posy,float negz,float posz){
		generate(negx,posx,negy,posy,negz,posz);
		vao.generate();
		vao.addAttrib(GL_ARRAY_BUFFER, 24*sizeof(float), &skybox_positions, GL_STATIC_DRAW, 0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);
		vao.setDrawMode(GL_TRIANGLES, 36, GL_UNSIGNED_INT, &skybox_indices);
	}
	~SkyBox(void){}
	void setShader(GLSLProgram* shader){
		this->shader=shader;
	}
	void setTexUnit(int u){texUnit = u;}
	int getTexUnit(){ return texUnit; }
	void draw(){
		//shader->setUniform("CubeMapTex",texUnit);
		//shader->use();
		vao.draw();
	}

private:
	void generate(float negx,float posx,float negy,float posy,float negz,float posz){
		std::array<float,24> positions = {
			negx,posy,posz,
			negx,negy,posz,
			posx,negy,posz,
			posx,posy,posz,
			negx,posy,negz,
			negx,negy,negz,
			posx,negy,negz,
			posx,posy,negz};
		std::array<int,36> indices = {
			1,0,3,
			1,3,2,
			4,5,7,
			7,5,6,
			3,7,6,
			3,6,2,
			0,1,5,
			0,5,4,
			0,7,3,
			0,4,7,
			1,2,6,
			1,6,5
		};
		skybox_positions = positions;
		skybox_indices = indices;
	}
	VAO vao;
	GLSLProgram * shader;
	std::array<float,24> skybox_positions;
	std::array<int,36> skybox_indices;
	int texUnit;
};

