#ifndef MESH_H
#define	MESH_H

#pragma once
#include <map>
#include <vector>
#include <GL/glew.h>
// assimp include files. These three are usually needed.
#include "assimp/Importer.hpp"	//OO version Header!
#include "assimp/PostProcess.h"
#include "assimp/Scene.h"

#include "ogldev_util.h"
#include "Texture.h"

#include "VAO.h"

#include <glm/glm.hpp>
using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::mat4;
using glm::mat3;

#include "VAO.h"
#include "glslprogram.h"
#include "Structures.h"
#include <array>
#include "Object.h"

struct Vertex
{
	vec3 m_pos;
	vec2 m_tex;
	vec3 m_normal;

	Vertex() {}

	Vertex(const vec3& pos, const vec2& tex, const vec3& normal)
	{
		m_pos = pos;
		m_tex = tex;
		m_normal = normal;
	}
};


class Mesh: public Object
{
public:
	Mesh();

	~Mesh();

	bool LoadMesh(const std::string& Filename);

	void Render();

private:
	bool InitFromScene(const aiScene* pScene, const std::string& Filename);
	void InitMesh(unsigned int Index, const aiMesh* paiMesh);
	bool InitMaterials(const aiScene* pScene, const std::string& Filename);
	void Clear();

#define INVALID_MATERIAL 0xFFFFFFFF

	struct MeshEntry {
		MeshEntry();

		~MeshEntry();

		void Init(const std::vector<float>& pos, const std::vector<float>& tex, const std::vector<float>& norm, std::vector<unsigned int>& ind);
		void draw(){ vao.draw(); }

		GLuint VB;
		GLuint IB;
		unsigned int NumIndices;
		unsigned int MaterialIndex;
		unsigned int NumVertices;
		VAO vao;
		vector<unsigned int> inds;
	};

	std::vector<MeshEntry> m_Entries;
	std::vector<Texture*> m_Textures;
};


#endif	/* MESH_H */