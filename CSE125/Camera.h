#pragma once
#include "Structures.h"
#include "Cube.h"

class Camera: public Object
{
public:
	Camera(){ pendingRot = 0; prerot = mat4(1.0); postrot = mat4(1.0); }
	~Camera(){}
	void attach(Cube* cube){ this->cube = cube; }
	void setCamM(mat4 m){ CamM = m; }
	mat4 getCamM(){ return prerot*CamM*postrot; }
	void preRotate(mat4 rot){ prerot=rot*prerot; }
	void postRotate(mat4 rot){ postrot = postrot*rot; }
	void preTrans(mat4 trans){ CamM=trans*CamM; }
	void postTrans(mat4 trans){ CamM=CamM*trans; }
	void setPreRot(mat4 m){ prerot = m; }
	void setPostRot(mat4 m){ postrot = m; }

	void setPendingRot(float f){ pendingRot = f; }
	void pushRot(float f){ pendingRot += f; }
	float getPendingRote(){ return pendingRot; }

	mat4 getViewM(){ return glm::inverse(prerot*CamM*postrot)*glm::inverse(cube->getModelM()); }

private:
	mat4 prerot,postrot;
	mat4 CamM;
	Cube* cube;
	float pendingRot;
};

