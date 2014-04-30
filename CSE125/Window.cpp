#define _USE_MATH_DEFINES
#pragma once
#include <iostream>
#include <string>
#include <time.h>
#include <boost/asio.hpp>
#include "Server.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Window.h"
#include <time.h>
#include "Object.h"
#include "VAO.h"
#include "glslprogram.h"
#include "Cube.h"
#include "ShaderController.h"
#include "Ground.h"
#include <Qt/QtGui/QImage> 
#include <Qt/QtOpenGL/QGLWidget>
#include "SkyBox.h"
#include "Structures.h"
#include "Sphere.h"
#include "TextureScreen.h"
#include "Camera.h"
#include "Scene.h"
#include "Mesh.h"
#include "Texture.h"

#include "ConfigSettings.h"

#include <AL/al.h>
#include <AL/alc.h>

#include <sys/types.h>
#include <sys/stat.h>


using boost::asio::ip::udp;

using glm::vec3;
using glm::vec4;
using glm::mat4;
using glm::mat3;
using glm::quat;
using namespace std;
int counter = 0;

float cam_sp = 0.1;

std::vector <pair<string, mat4>>* stateVec = new vector<pair<string, mat4>>;
std::vector <pair<string, mat4>>* recvVec = new vector<pair<string, mat4>>;

Scene* scene;
boost::asio::io_service io_service;

std::string make_daytime_string()
{
	using namespace std; // For time_t, time and ctime;
	time_t now = time(0);
	return "efef";
}

tcp_server* server;

void handle_key_state(){
	recvVec = server->getState();
	io_service.poll();
	std::cout << "recvVec key string:" << recvVec->front().first << std::endl;
	int retState = (int)recvVec->front().second[0][0];
	int pID = atoi(recvVec->front().first.c_str());
	std::cout << "keyState:" << retState << std::endl;

	if (retState & 1){ //'a'
		//cout << "move left" << endl;
		scene->setHMove(pID, -1);
	}else{
		scene->cancelHMove(pID, -1);
	}
	if (retState & 1 << 1){ //'d'
		//cout << "move right" << endl;
		scene->setHMove(pID, 1);
	}else{
		scene->cancelHMove(pID, 1);
	}
	if (retState & 1 << 2){ //'w'
		//cout << "move up" << endl;
		scene->setVMove(pID, 1);
	}else{
		scene->cancelVMove(pID, 1);
	}
	if (retState & 1 << 3){ //'s'
		//cout << "move down" << endl;
		scene->setVMove(pID, -1);
	}else{
		scene->cancelVMove(pID, -1);
	}
	if (retState & 1 << 4){ //' '
		//cout << "jump" << endl;
		scene->jump(pID);
	}
}

void handle_cam_rot(){

	recvVec = server->getState();
	io_service.poll();
	std::cout << "recvVec cam string:" << recvVec->back().first << std::endl;
	int rot = (int)recvVec->back().second[0][0];
	int pID = atoi(recvVec->front().first.c_str());
	std::cout << "camState:" << rot << std::endl;
	//server->reset_camRot(); // ret = 0 in server...need to do
	scene->pushRot(pID, -cam_sp*rot);
}

int main(int argc, char *argv[])
{
  scene = new Scene();
  scene->setGravity(vec3(0,-9.8,0));

  //init state vector
  stateVec->push_back(std::make_pair("initState_s", mat4(0.0f)));
  stateVec->push_back(std::make_pair("initState_s", mat4(0.0f)));
  stateVec->push_back(std::make_pair("initState_s", mat4(0.0f)));
  stateVec->push_back(std::make_pair("initState_s", mat4(0.0f)));

  try
  {
	  tcp::resolver resolver(io_service);
	  tcp::resolver::query query(tcp::v4(), "localhost", "13");
	  tcp::resolver::iterator itr = resolver.resolve(query);

	  tcp::endpoint endpoint = *itr;
	  server = new tcp_server(io_service, endpoint);
  }
  catch (std::exception& e)
  {
	  std::cerr << e.what() << std::endl;
  }

  LARGE_INTEGER freq, last, current;
  double diff;
  QueryPerformanceFrequency(&freq);
  QueryPerformanceCounter(&last);
  while (true){
	  QueryPerformanceCounter(&current);
	  diff = (double)(current.QuadPart - last.QuadPart) / (double)freq.QuadPart;
	  last = current;

	  handle_key_state();
	  handle_cam_rot();
	  scene->simulate(diff, 1.0 / 100);

	  boost::array<mat4, 4> m;
	  m[0] = scene->getPlayerMats()[0];
	  m[1] = scene->getPlayerMats()[1];
	  m[2] = scene->getPlayerMats()[2];
	  m[3] = scene->getPlayerMats()[3];

	  // Print out matrix contents
	  /*
	  cout << (m[0])[0][0] << (m[0])[0][1] << (m[0])[0][2] << (m[0])[0][3] << endl;
	  cout << (m[0])[1][0] << (m[0])[1][1] << (m[0])[1][2] << (m[0])[1][3] << endl;
	  cout << (m[0])[2][0] << (m[0])[2][1] << (m[0])[2][2] << (m[0])[2][3] << endl;
	  cout << (m[0])[3][0] << (m[0])[3][1] << (m[0])[3][2] << (m[0])[3][3] << endl;
	  */
	  (*stateVec)[0] = std::make_pair("0", m[0]);
	  (*stateVec)[1] = std::make_pair("1", m[1]);
	  (*stateVec)[2] = std::make_pair("2", m[2]);
	  (*stateVec)[3] = std::make_pair("3", m[3]);

	  server->send(*stateVec);
	  io_service.poll();
  }

  return 0;
}