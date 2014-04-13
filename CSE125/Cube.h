#pragma once
#include "VAO.h"
#include "glslprogram.h"
#include "Structures.h"
#include <array>

class Cube: public Object
{
public:
	Cube(void){
		generate(-0.5,0.5,-0.5,0.5,-0.5,0.5);
		setModelM(mat4(1.0));
		jumpVelocity = 0;
		vao.generate();
		vao.addAttrib(GL_ARRAY_BUFFER, 72*sizeof(float), &vertex_positions, GL_STATIC_DRAW, 0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);
		vao.addAttrib(GL_ARRAY_BUFFER, 72*sizeof(float), &vertex_normals, GL_STATIC_DRAW, 1, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);
		vao.setDrawMode(GL_TRIANGLES, 36, GL_UNSIGNED_INT, &triangle_indices);
	}
	Cube(float negx,float posx,float negy,float posy,float negz,float posz){
		generate(negx,posx,negy,posy,negz,posz);
		setModelM(mat4(1.0));
		jumpVelocity = 0;
		vao.generate();
		vao.addAttrib(GL_ARRAY_BUFFER, 72*sizeof(float), &vertex_positions, GL_STATIC_DRAW, 0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);
		vao.addAttrib(GL_ARRAY_BUFFER, 72*sizeof(float), &vertex_normals, GL_STATIC_DRAW, 1, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);
		vao.setDrawMode(GL_TRIANGLES, 36, GL_UNSIGNED_INT, &triangle_indices);
	}
	~Cube(void){}
	void draw(){
		//shader->setUniform("material.Kd",material.Kd);
		//shader->setUniform("material.Ka",material.Ka);
		//shader->setUniform("material.Ks",material.Ks);
		//shader->setUniform("material.Shininess",material.Shininess);
		//shader->setUniform("material.ReflectFactor",material.ReflectFactor);
		//shader->setUniform("material.Eta",material.Eta);
		//shader->setUniform("CubeMapTex",CubeMapUnit);
		//shader->use();
		vao.draw();
	}
	void setShader(GLSLProgram* shader){ this->shader=shader;}
	void setKd(vec3 v){ material.Kd=v; }
	vec3 getKd(){ return material.Kd; }
	void setKa(vec3 v){ material.Ka=v; }
	vec3 getKa(){ return material.Ka; }
	void setKs(vec3 v){ material.Ks=v; }
	vec3 getKs(){ return material.Ks; }
	void setShininess(float f){ material.Shininess=f; }
	float getShininess(){ return material.Shininess; }
	void setReflectFactor(vec2 v){ material.ReflectFactor=v; }
	vec2 getReflectFactor(){ return material.ReflectFactor; }
	void setEta(float f){ material.Eta=f; }
	float getEta(){ return material.Eta; }
	void setCubeMapUnit(int u){ CubeMapUnit=u;}
	int getCubeMapUnit(){ return CubeMapUnit; }

	void physicsSimulation(float time, float substep){
		if ((modelM*vec4(0, -0.5, 0, 1))[1] == 0.0 && jumpVelocity==0)
			return;
		while (time > substep)
		{
			time -= substep;
			float dy = jumpVelocity*substep;
			jumpVelocity -= 9.8*substep;
			preTrans(glm::translate(vec3(0, dy, 0)));
			if ((modelM*vec4(0, -0.5, 0, 1))[1] < 0.0){
				preTrans(glm::translate(vec3(0, -(modelM*vec4(0, -0.5, 0, 1))[1], 0)));
				jumpVelocity = 0;
				return;
			}
		}
		float dy = jumpVelocity*time;
		jumpVelocity -= 9.8*time;
		preTrans(glm::translate(vec3(0, dy, 0)));
		if ((modelM*vec4(0, -0.5, 0, 1))[1] < 0.0){
			preTrans(glm::translate(vec3(0, -(modelM*vec4(0, -0.5, 0, 1))[1], 0)));
			jumpVelocity = 0;
			return;
		}
	}
private:
	void generate(float negx,float posx,float negy,float posy,float negz,float posz){
		std::array<float,72> positions = {
			negx,posy,posz,  negx,posy,posz,  negx,posy,posz,  //ftl
			negx,negy,posz,  negx,negy,posz,  negx,negy,posz,  //fbl
			posx,negy,posz,  posx,negy,posz,  posx,negy,posz,  //fbr
			posx,posy,posz,  posx,posy,posz,  posx,posy,posz,  //ftr
			negx,posy,negz,  negx,posy,negz,  negx,posy,negz,  //btl
			negx,negy,negz,  negx,negy,negz,  negx,negy,negz,  //bbl
			posx,negy,negz,  posx,negy,negz,  posx,negy,negz,  //bbr
			posx,posy,negz,  posx,posy,negz,  posx,posy,negz}; //btr

		std::array<float,72> normals = {
			0,0,1,   0,1,0,   -1,0,0,   //  0: 1A,  1: 1B,  2: 1D
			0,0,1,   0,-1,0,  -1,0,0,   //  3: 2A,  4: 2E,  5: 2D
			0,0,1,   0,-1,0,   1,0,0,   //  6: 3A,  7: 3E,  8: 3C
			0,0,1,   0,1,0,    1,0,0,	//  9: 4A, 10: 4B, 11: 4C
			0,0,-1,  0,1,0,   -1,0,0,   // 12: 5F, 13: 5B, 14: 5D
			0,0,-1,  0,-1,0,  -1,0,0,   // 15: 6F, 16: 6E, 17: 6D
			0,0,-1,  0,-1,0,   1,0,0,   // 18: 7F, 19: 7E, 20: 7C
			0,0,-1,  0,1,0,    1,0,0};  // 21: 8F, 22: 8B, 23: 8C

		std::array<int,36> indices = {
			0,3,6,     0,6,9,      //front
			12,18,15,  12,21,18,   //back
			2,14,5,    17,5,14,    //left
			8,23,11,   8,20,23,    //right
			1,22,13,   1,10,22,    //top
			4,16,19,   4,19,7};    //bottom
		vertex_positions = positions;
		vertex_normals = normals;
		triangle_indices = indices;
	}
	VAO vao;
	GLSLProgram * shader;
	Material material;
	std::array<float,72> vertex_positions;
	std::array<float,72> vertex_normals;
	std::array<int,36> triangle_indices;
	int CubeMapUnit;
};

