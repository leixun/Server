#pragma once
#define _USE_MATH_DEFINES
#include "VAO.h"
#include "glslprogram.h"
#include "Structures.h"
#include <array>
#include <vector>
#include <cmath>

class Sphere
{
public:
	Sphere(void){
		generate(0.5,12,24);
		vao.generate();
		vao.addAttrib(GL_ARRAY_BUFFER, vertex_positions.size()*sizeof(float), &vertex_positions[0], GL_STATIC_DRAW, 0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);
		vao.addAttrib(GL_ARRAY_BUFFER, vertex_normals.size()*sizeof(float), &vertex_normals[0], GL_STATIC_DRAW, 1, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);
		vao.setDrawMode(GL_QUADS, quad_indices.size(), GL_UNSIGNED_INT, &quad_indices[0]);
	}
	Sphere(float radius, unsigned int rings, unsigned int sectors){
		generate(radius, rings, sectors);
		vao.generate();
		vao.addAttrib(GL_ARRAY_BUFFER, vertex_positions.size()*sizeof(float), &vertex_positions[0], GL_STATIC_DRAW, 0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);
		vao.addAttrib(GL_ARRAY_BUFFER, vertex_normals.size()*sizeof(float), &vertex_normals[0], GL_STATIC_DRAW, 1, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);
		vao.setDrawMode(GL_QUADS, quad_indices.size(), GL_UNSIGNED_INT, &quad_indices[0]);
	}
	~Sphere(void){}
	void draw(){
		shader->setUniform("material.Kd",material.Kd);
		shader->setUniform("material.Ka",material.Ka);
		shader->setUniform("material.Ks",material.Ks);
		shader->setUniform("material.Shininess",material.Shininess);
		shader->setUniform("material.ReflectFactor",material.ReflectFactor);
		shader->setUniform("material.Eta",material.Eta);
		shader->setUniform("material.DiffuseFactor",material.DiffuseFactor);
		shader->setUniform("material.SpecFactor",material.SpecFactor);
		//shader->setUniform("CubeMapTex",CubeMapUnit);
		shader->setUniform("DiffuseMap",DiffuseUnit);
		shader->setUniform("SpecMap",SpecUnit);
		shader->use();
		vao.draw();
	}
	void setShader(GLSLProgram* shader){ this->shader=shader;}
	void setKd(vec3 v){ material.Kd=v; }
	void setKa(vec3 v){ material.Ka=v; }
	void setKs(vec3 v){ material.Ks=v; }
	void setShininess(float f){ material.Shininess=f; }
	void setReflectFactor(vec2 v){ material.ReflectFactor=v; }
	void setEta(float f){ material.Eta=f; }
	void setCubeMapUnit(int u){ CubeMapUnit=u;}
	void setDiffuseUnit(int u){ DiffuseUnit=u;}
	void setSpecUnit(int u){ SpecUnit=u;}
	void setDiffuseFactor(float f){ material.DiffuseFactor=f; }
	void setSpecFactor(float f){ material.SpecFactor=f; }
private:
	void generate(float radius, unsigned int rings, unsigned int sectors)
	{
		float const R = 1./(float)(rings-1);
		float const S = 1./(float)(sectors-1);
		int r, s;

		vertex_positions.resize(rings * sectors * 3);
		vertex_normals.resize(rings * sectors * 3);
		texcoords.resize(rings * sectors * 2);
		std::vector<GLfloat>::iterator v = vertex_positions.begin();
		std::vector<GLfloat>::iterator n = vertex_normals.begin();
		std::vector<GLfloat>::iterator t = texcoords.begin();
		for(r = 0; r < rings; r++) for(s = 0; s < sectors; s++) {
			float const y = sin( -M_PI_2 + M_PI * r * R );
			float const x = cos(2*M_PI * s * S) * sin( M_PI * r * R );
			float const z = sin(2*M_PI * s * S) * sin( M_PI * r * R );

			*t++ = s*S;
			*t++ = r*R;

			*v++ = x * radius;
			*v++ = y * radius;
			*v++ = z * radius;

			*n++ = x;
			*n++ = y;
			*n++ = z;
		}

		quad_indices.resize(rings * sectors * 4);
		std::vector<GLuint>::iterator i = quad_indices.begin();
		for(r = 0; r < rings-1; r++) for(s = 0; s < sectors-1; s++) {
			*i++ = (r+1) * sectors + s;
			*i++ = (r+1) * sectors + (s+1);
			*i++ = r * sectors + (s+1);
			*i++ = r * sectors + s;
		}
	}
	VAO vao;
	GLSLProgram * shader;
	Material material;
	int CubeMapUnit;
	int DiffuseUnit;
	int SpecUnit;
	std::vector<GLfloat> vertex_positions;
	std::vector<GLfloat> vertex_normals;
	std::vector<GLfloat> texcoords;
	std::vector<GLuint> quad_indices;
};