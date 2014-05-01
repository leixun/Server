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

float cam_sp = 0.01;

std::vector <pair<string, mat4>>* stateVec = new vector<pair<string, mat4>>;
std::vector <pair<string, mat4>>* recvVec = new vector<pair<string, mat4>>;

Scene* scene;
boost::asio::io_service io_service;

int player_ID = 0;
int key_state = 0;
int cam_rot = 0;

std::string make_daytime_string()
{
	using namespace std; // For time_t, time and ctime;
	time_t now = time(0);
	return "efef";
}

tcp_server* server;

void handle_key_state(int pid, int key_state){

	if (key_state & 1){ //'a'
		//cout << "move left" << endl;
		scene->setHMove(pid, -1);
	}else{
		scene->cancelHMove(pid, -1);
	}
	if (key_state & 1 << 1){ //'d'
		//cout << "move right" << endl;
		scene->setHMove(pid, 1);
	}else{
		scene->cancelHMove(pid, 1);
	}
	if (key_state & 1 << 2){ //'w'
		//cout << "move up" << endl;
		scene->setVMove(pid, 1);
	}else{
		scene->cancelVMove(pid, 1);
	}
	if (key_state & 1 << 3){ //'s'
		//cout << "move down" << endl;
		scene->setVMove(pid, -1);
	}else{
		scene->cancelVMove(pid, -1);
	}
	if (key_state & 1 << 4){ //' '
		//cout << "jump" << endl;
		scene->jump(pid);
	}
}

void handle_cam_rot(int pid, int cam_rot){
	scene->pushRot(pid, -cam_sp*cam_rot);
	cam_rot = 0; // possibly a problem
}

int main(int argc, char *argv[])
{
  scene = new Scene();
  scene->setGravity(vec3(0,-9.8,0));

  //init state vector
  stateVec->push_back(std::make_pair("", mat4(0.0f)));
  stateVec->push_back(std::make_pair("", mat4(0.0f)));
  stateVec->push_back(std::make_pair("", mat4(0.0f)));
  stateVec->push_back(std::make_pair("", mat4(0.0f)));

  recvVec->push_back(std::make_pair("", mat4(0.0f)));
  recvVec->push_back(std::make_pair("", mat4(0.0f)));
  recvVec->push_back(std::make_pair("", mat4(0.0f)));
  recvVec->push_back(std::make_pair("", mat4(0.0f)));
  recvVec->push_back(std::make_pair("", mat4(0.0f)));
  recvVec->push_back(std::make_pair("", mat4(0.0f)));
  recvVec->push_back(std::make_pair("", mat4(0.0f)));
  recvVec->push_back(std::make_pair("", mat4(0.0f)));

  try
  {
	  tcp::resolver resolver(io_service);
	  tcp::resolver::query query(tcp::v4(), "localhost", "13"); // does nothing for the moment
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

	  recvVec = server->getState();
	  io_service.poll();
	  //std::cout << "recvVec key string:" << recvVec->front().first << std::endl; 

	  if (strcmp((*recvVec)[0].first.c_str(), ""))
	  {
		  handle_key_state(atoi((*recvVec)[0].first.c_str()), (int)(*recvVec)[0].second[0][0]);
		  //std::cout << "id: " << atoi((*recvVec)[0].first.c_str()) << std::endl;
		  //std::cout << "val: " << (int)(*recvVec)[0].second[0][0] << std::endl;
		  handle_cam_rot(atoi((*recvVec)[1].first.c_str()), (int)(*recvVec)[1].second[0][0]);
		  //std::cout << "id: " << atoi((*recvVec)[1].first.c_str()) << std::endl;
		  //std::cout << "val: " << (int)(*recvVec)[1].second[0][0] << std::endl;
	  }

	  if (strcmp((*recvVec)[2].first.c_str(), ""))
	  {
		  handle_key_state(atoi((*recvVec)[2].first.c_str()), (int)(*recvVec)[2].second[0][0]);
		  handle_cam_rot(atoi((*recvVec)[3].first.c_str()), (int)(*recvVec)[3].second[0][0]);
	  }

	  if (strcmp((*recvVec)[4].first.c_str(), ""))
	  {
		  handle_key_state(atoi((*recvVec)[4].first.c_str()), (int)(*recvVec)[4].second[0][0]);
		  handle_cam_rot(atoi((*recvVec)[5].first.c_str()), (int)(*recvVec)[5].second[0][0]);
	  }

	  if (strcmp((*recvVec)[6].first.c_str(), ""))
	  {
		  handle_key_state(atoi((*recvVec)[6].first.c_str()), (int)(*recvVec)[6].second[0][0]);
		  handle_cam_rot(atoi((*recvVec)[7].first.c_str()), (int)(*recvVec)[7].second[0][0]);
	  }

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