#pragma once
#include "VAO.h"
#include "glslprogram.h"
#include <array>

double ground_positions[] ={-10,0,10,
	-10,0,-10,
	10,0,-10,
	10,0,10};	 
double ground_normals[] =  {0,1,0,
	0,1,0,
	0,1,0,
	0,1,0};
int ground_indices[] = {0,2,1,
	0,3,2};

class Ground: public Object
{
public:
	Ground(void){
		vao.generate();
		vao.addAttrib(GL_ARRAY_BUFFER, 12*sizeof(double), ground_positions, GL_STATIC_DRAW, 0, 3, GL_DOUBLE, GL_FALSE, 0, (GLubyte*)NULL);
		vao.addAttrib(GL_ARRAY_BUFFER, 12*sizeof(double), ground_normals, GL_STATIC_DRAW, 1, 3, GL_DOUBLE, GL_FALSE, 0, (GLubyte*)NULL);
		vao.setDrawMode(GL_TRIANGLES, 6, GL_UNSIGNED_INT, ground_indices);
	}
	~Ground(void){}
	void setShader(GLSLProgram* shader){
		this->shader=shader;
	}
	void draw(){
		//shader->setUniform("material.Kd",Kd);
		//shader->setUniform("material.Ka",Ka);
		//shader->setUniform("material.Ks",Ks);
		//shader->setUniform("material.Shininess",Shininess);
		//shader->setUniform("material.ReflectFactor",(float)0.5);
		////shader->setUniform("TexMode",false);
		////shader->setUniform("SkyBox",true);
		////shader->setUniform("CubeMapTex",2);
		//shader->setUniform("ProjectorTex",ProjectorTex);
		//shader->setUniform("ProjectorMatrix",ProjectorMatrix);
		//shader->use();
		vao.draw();
	}
	void setKd(vec3 v){
		Kd=v;
	}
	void setKa(vec3 v){
		Ka=v;
	}
	void setKs(vec3 v){
		Ks=v;
	}
	void setShininess(float f){
		Shininess = f;
	}
	void setProjectorTex(int i){ProjectorTex=i;}
	void setProjectorMatrix(mat4 m){ProjectorMatrix=m;}
private:
	VAO vao;
	GLSLProgram * shader;
	vec3 Kd,Ka,Ks;
	float Shininess;
	int ProjectorTex;
	mat4 ProjectorMatrix;
};

