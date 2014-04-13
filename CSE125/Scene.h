#pragma once
#include <vector>
#include "Object.h"
#include "Octree.h"
using namespace std;
class Scene
{
public:
	Scene(){
		initialize();
	}
	~Scene(){
		for (int i = 0; i<stationary.size(); i++){
			delete stationary[i];
		}
		stationary.clear();
		for (int i = 0; i<player.size(); i++){
			delete player[i];
		}
		player.clear();
	}
	void simulate(float t, float sub){
		resolvePlayerRotation();
		while (t > sub){
			t -= sub;
			resolvePlayerTransition(sub);
			//octree here
			collisionDetection();
		}
		resolvePlayerTransition(t);
		//octree here
		collisionDetection();
	}
	void collisionDetection(Octree* octree);
	void collisionDetection(){
		//player-stationary detection
		for (int i = 0; i < player.size(); i++){
			bool touchGround1 = false;
			bool touchGround2 = false;
			for (int j = 0; j < stationary.size(); j++){
				AABB pBox = player[i]->getAABB();
				AABB sBox = stationary[j]->getAABB();
				bool collide = true;
				for (int v = 0; v < 3; v++){
					if (pBox.max[v]<=sBox.min[v]||sBox.max[v]<=pBox.min[v]){
						collide = false;
						break;
					}
				}
				if (collide){
					fixCollision(player[i], stationary[j], pBox, sBox, touchGround1, touchGround2);
				}
				stationary[j]->touchGround(touchGround2);
			}
			player[i]->touchGround(touchGround1);
		}
	}
	void fixCollision(Object* obj1, Object* obj2, AABB& box1, AABB& box2, bool& onGround1, bool& onGround2){
		float Rewind[3];
		float minRewind = 999;
		int minID = 0;
		vec3 v1 = vec3(obj1->getModelM()*vec4(obj1->getVelocity(), 0.0));
		vec3 v2 = vec3(obj2->getModelM()*vec4(obj2->getVelocity(), 0.0));
		vec3 vDiff = v1 - v2;

		for (int v = 0; v<3; v++){
			if (vDiff[v] > 0){
				Rewind[v] = (box1.max[v] - box2.min[v]) / vDiff[v];
			}
			else if (vDiff[v] < 0){
				Rewind[v] = (box1.min[v] - box2.max[v]) / vDiff[v];
			}
			else{//avoid division by 0
				Rewind[v] = 999;
			}
			if (Rewind[v] < minRewind){
				minRewind = Rewind[v];
				minID = v;
			}
		}
		vec3 rwVelocity1 = vec3(0.0);
		rwVelocity1[minID] = -v1[minID];
		obj1->preTrans(glm::translate(rwVelocity1*minRewind));
		vec3 rwVelocity2 = vec3(0.0);
		rwVelocity2[minID] = -v2[minID];
		obj2->preTrans(glm::translate(rwVelocity2*minRewind));
		if (minID == 1){
			obj1->clearYVelocity();
			onGround1 = true;
			obj2->clearYVelocity();
			onGround2 = true;
		}
		else{//stair effect
			AABB b1 = obj1->getAABB();
			AABB b2 = obj2->getAABB();
			if (b2.max[1] > b1.min[1] && b2.max[1] - b1.min[1] <= 0.11){
				obj1->preTrans(glm::translate(0.0f, b2.max[1]-b1.min[1]+0.01f, 0.0f));
			}
		}
	}
	void addPlayer(Object* p){ player.push_back(p); }
	void addStationary(Object* s){ stationary.push_back(s); }
	void setGravity(vec3& g){ gravity = g; }
	void resolvePlayerRotation(){
		for (int i = 0; i < player.size(); i++){
			player[i]->postRotate(glm::rotate(mat4(1.0), player[i]->getPendingRote(), vec3(0, 1, 0)));
			player[i]->setPendingRot(0);
		}
	}
	void resolvePlayerTransition(float t){
		for (int i = 0; i < player.size(); i++){
			player[i]->addVelocity(gravity*t);
			player[i]->postTrans(glm::translate(player[i]->getVelocity()*t));
		}
	}

	void setHMove(int playerID, int m){ player[playerID]->setHMove(m); }
	void cancelHMove(int playerID, int m){ player[playerID]->cancelHMove(m); }
	void setVMove(int playerID, int m){ player[playerID]->setVMove(m); }
	void cancelVMove(int playerID, int m){ player[playerID]->cancelVMove(m); }
	void setPendingRot(int playerID, float f){ player[playerID]->setPendingRot(f); }
	void pushRot(int playerID, float f){ player[playerID]->pushRot(f); }
	void jump(int playerID){ player[playerID]->jump(); }

	vector<mat4> getPlayerMats(){
		vector<mat4> m;
		for (int i = 0; i < player.size(); i++){
			m.push_back(player[i]->getModelM());
		}
		return m;
	}

	void initialize(){
		Cube* cube = new Cube();
		cube->setSpeed(5);
		cube->postTrans(glm::translate(vec3(0, 0.5, 0)));
		cube->setAABB(AABB(vec3(-0.5, -0.5, -0.5), vec3(0.5, 0.5, 0.5)));
		cube->setType("Cube");
		cube->setName("Player Cube");
		addPlayer(cube);

		//TEST
		Cube* cube2 = new Cube();
		cube2->setSpeed(5);
		cube2->postTrans(glm::translate(vec3(3, 0.5, 0)));
		cube2->setAABB(AABB(vec3(-0.5, -0.5, -0.5), vec3(0.5, 0.5, 0.5)));
		cube2->setType("Cube");
		cube2->setName("Test Cube");
		addStationary(cube2);

		Cube* cube3 = new Cube();
		cube3->setSpeed(5);
		cube3->postTrans(glm::translate(vec3(-3, 0.5, 0)));
		cube3->setAABB(AABB(vec3(-0.5, -0.5, -0.5), vec3(0.5, 0.5, 0.5)));
		cube3->setType("Cube");
		cube3->setName("Test Cube2");
		addStationary(cube3);

		Cube* cube4 = new Cube();
		cube4->setSpeed(5);
		cube4->postTrans(glm::translate(vec3(0, 0.5, -3)));
		cube4->setAABB(AABB(vec3(-0.5, -0.5, -0.5), vec3(0.5, 0.5, 0.5)));
		cube4->setType("Cube");
		cube4->setName("Test Cube3");
		addStationary(cube4);

		Cube* cube5 = new Cube();
		cube5->setSpeed(5);
		cube5->postTrans(glm::translate(vec3(0, 0.5, 3)));
		cube5->setAABB(AABB(vec3(-0.5, -0.5, -0.5), vec3(0.5, 0.5, 0.5)));
		cube5->setType("Cube");
		cube5->setName("Test Cube4");
		addStationary(cube5);

		Ground* ground = new Ground();
		ground->setAABB(AABB(vec3(-10.0, 0.0, -10.0), vec3(10.0, 0.0, 10.0)));
		ground->setType("Ground");
		ground->setName("Ground");
		addStationary(ground);
	}

protected:
	vector<Object*> stationary;
	vector<Object*> player;
	vector<Object*> tower;
	vector<Object*> skillShot;
	vector<Object*> virtualTower;
	vec3 gravity;
};

