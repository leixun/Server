#pragma once
#include <assert.h>
#include <iostream>

#include "mesh.h"

Mesh::MeshEntry::MeshEntry()
{
	VB = INVALID_OGL_VALUE;
	IB = INVALID_OGL_VALUE;
	NumIndices = 0;
	MaterialIndex = INVALID_MATERIAL;
};

Mesh::MeshEntry::~MeshEntry()
{
	if (VB != INVALID_OGL_VALUE)
	{
		glDeleteBuffers(1, &VB);
	}

	if (IB != INVALID_OGL_VALUE)
	{
		glDeleteBuffers(1, &IB);
	}
}

void Mesh::MeshEntry::Init(const std::vector<float>& pos, const std::vector<float>& tex, const std::vector<float>& norm, std::vector<unsigned int>& ind)
{
	NumIndices = ind.size();
	NumVertices = pos.size();
	inds = ind;

	vao.generate();
	vao.addAttrib(GL_ARRAY_BUFFER, pos.size() * sizeof(float), &pos[0], GL_STATIC_DRAW, 0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);
	vao.addAttrib(GL_ARRAY_BUFFER, tex.size() * sizeof(float), &tex[0], GL_STATIC_DRAW, 1, 2, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);
	vao.addAttrib(GL_ARRAY_BUFFER, norm.size() * sizeof(float), &norm[0], GL_STATIC_DRAW, 2, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);
	vao.setDrawMode(GL_TRIANGLES, ind.size(), GL_UNSIGNED_INT, &inds[0]);
}

Mesh::Mesh()
{
}


Mesh::~Mesh()
{
	Clear();
}


void Mesh::Clear()
{
	for (unsigned int i = 0; i < m_Textures.size(); i++) {
		SAFE_DELETE(m_Textures[i]);
	}
}


bool Mesh::LoadMesh(const std::string& Filename)
{
	// Release the previously loaded mesh (if it exists)
	Clear();

	bool Ret = false;
	Assimp::Importer Importer;

	const aiScene* pScene = Importer.ReadFile(Filename.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals);

	if (pScene) {
		Ret = InitFromScene(pScene, Filename);
	}
	else {
		printf("Error parsing '%s': '%s'\n", Filename.c_str(), Importer.GetErrorString());
	}

	return Ret;
}

bool Mesh::InitFromScene(const aiScene* pScene, const std::string& Filename)
{
	m_Entries.resize(pScene->mNumMeshes);
	m_Textures.resize(pScene->mNumMaterials);

	// Initialize the meshes in the scene one by one
	for (unsigned int i = 0; i < m_Entries.size(); i++) {
		const aiMesh* paiMesh = pScene->mMeshes[i];
		InitMesh(i, paiMesh);
	}

	return InitMaterials(pScene, Filename);
}

void Mesh::InitMesh(unsigned int Index, const aiMesh* paiMesh)
{
	m_Entries[Index].MaterialIndex = paiMesh->mMaterialIndex;

	std::vector<Vertex> Vertices;
	std::vector<unsigned int> Indices;
	std::vector<float> pos;
	std::vector<float> tex;
	std::vector<float> norm;
	std::vector<unsigned int> ind;

	const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

	for (unsigned int i = 0; i < paiMesh->mNumVertices; i++) {
		const aiVector3D* pPos = &(paiMesh->mVertices[i]);
		const aiVector3D* pNormal = &(paiMesh->mNormals[i]);
		const aiVector3D* pTexCoord = paiMesh->HasTextureCoords(0) ? &(paiMesh->mTextureCoords[0][i]) : &Zero3D;

		pos.push_back(pPos->x);
		pos.push_back(pPos->y);
		pos.push_back(pPos->z);
		tex.push_back(pTexCoord->x);
		tex.push_back(pTexCoord->y);
		norm.push_back(pNormal->x);
		norm.push_back(pNormal->y);
		norm.push_back(pNormal->z);
	}

	for (unsigned int i = 0; i < paiMesh->mNumFaces; i++) {
		const aiFace& Face = paiMesh->mFaces[i];
		assert(Face.mNumIndices == 3);
		ind.push_back(Face.mIndices[0]);
		ind.push_back(Face.mIndices[1]);
		ind.push_back(Face.mIndices[2]);
	}
	m_Entries[Index].Init(pos, tex, norm, ind);
}

bool Mesh::InitMaterials(const aiScene* pScene, const std::string& Filename)
{
	// Extract the directory part from the file name
	std::string::size_type SlashIndex = Filename.find_last_of("/");
	std::string Dir;

	if (SlashIndex == std::string::npos) {
		Dir = ".";
	}
	else if (SlashIndex == 0) {
		Dir = "/";
	}
	else {
		Dir = Filename.substr(0, SlashIndex);
	}

	bool Ret = true;

	// Initialize the materials
	for (unsigned int i = 0; i < pScene->mNumMaterials; i++) {
		const aiMaterial* pMaterial = pScene->mMaterials[i];

		m_Textures[i] = NULL;

		if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
			aiString Path;

			if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &Path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) {
				std::string FullPath = Dir + "/" + Path.data;
				m_Textures[i] = new Texture(GL_TEXTURE_2D, FullPath.c_str(), "PNG");
				//std::cout << FullPath << endl;

				if (!m_Textures[i]->Load()) {
					printf("Error loading texture '%s'\n", FullPath.c_str());
					delete m_Textures[i];
					m_Textures[i] = NULL;
					Ret = false;
				}
				else {
					printf("Loaded texture '%s'\n", FullPath.c_str());
				}
			}
		}

		// Load a white texture in case the model does not include its own texture
		if (!m_Textures[i]) {
			m_Textures[i] = new Texture(GL_TEXTURE_2D, "img/white.png", "PNG");

			Ret = m_Textures[i]->Load();
		}
	}

	return Ret;
}

void Mesh::Render()
{
	for (unsigned int i = 0; i < m_Entries.size(); i++) {
		const unsigned int MaterialIndex = m_Entries[i].MaterialIndex;

		if (MaterialIndex < m_Textures.size() && m_Textures[MaterialIndex]) {
			m_Textures[MaterialIndex]->Bind(GL_TEXTURE0);
		}

		m_Entries[i].draw();
	}
}