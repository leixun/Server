#define _USE_MATH_DEFINES
#pragma once
#include <iostream>
#include <string>
#include <time.h>
#include <boost/asio.hpp>
#include "udpServer.h"
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

Scene* scene;
boost::asio::io_service io_service;

std::string make_daytime_string()
{
	using namespace std; // For time_t, time and ctime;
	time_t now = time(0);
	return "efef";
}

udpServer* server;
void Window::idleCallback(void)
{
	static time_t tick = clock();
	float diff = (float)(clock() - tick)/CLOCKS_PER_SEC;
	tick = clock();
	scene->simulate(diff, 1.0 / 100);
}
void Window::reshapeCallback(int w, int h)
{
}
void Window::displayCallback(void)
{
}

void server_update(int value){
	glutTimerFunc(100, server_update, 0);
}

int main(int argc, char *argv[])
{
  scene = new Scene();

  try
  {
	  server = new udpServer(io_service);
  }
  catch (std::exception& e)
  {
	  std::cerr << e.what() << std::endl;
  }

  int retState = 0;

  LARGE_INTEGER freq, last, current;
  double diff;
  QueryPerformanceFrequency(&freq);
  QueryPerformanceCounter(&last);
  while (true){
	  //static time_t tick = clock();
	  //float diff = (float)(clock() - tick) / CLOCKS_PER_SEC;
	  //tick = clock();
	  QueryPerformanceCounter(&current);
	  diff = (double)(current.QuadPart - last.QuadPart) / (double)freq.QuadPart;
	  last = current;
	  scene->simulate(diff, 1.0 / 100);
	  retState = server->get_keyState();
	  io_service.poll();

	  if (retState & 1){
		  cout << "move left" << endl;
		  scene->setHMove(0, -1);
	  }
	  if (retState & 1 << 1){
		  cout << "move right" << endl;
		  scene->setHMove(0, 1);
	  }
	  if (retState & 1 << 2){
		  cout << "move up" << endl;
		  scene->setVMove(0, 1);
	  }
	  if (retState & 1 << 3){
		  cout << "move down" << endl;
		  scene->setVMove(0, -1);
	  }
	  if (!(retState | 0)){
		  scene->cancelHMove(0, -1);
	  }
	  if (!(retState | 0 << 1)){
		  scene->cancelHMove(0, 1);
	  }
	  if (!(retState | 0 << 2)){
		  scene->cancelVMove(0, 1);
	  }
	  if (!(retState | 0 << 3)){
		  scene->cancelVMove(0, -1);
	  }
	  boost::array<mat4, 1> m;
	  m[0] = scene->getPlayerMats()[0];

	  // Print out matrix contents
	  /*
	  cout << (m[0])[0][0] << (m[0])[0][1] << (m[0])[0][2] << (m[0])[0][3] << endl;
	  cout << (m[0])[1][0] << (m[0])[1][1] << (m[0])[1][2] << (m[0])[1][3] << endl;
	  cout << (m[0])[2][0] << (m[0])[2][1] << (m[0])[2][2] << (m[0])[2][3] << endl;
	  cout << (m[0])[3][0] << (m[0])[3][1] << (m[0])[3][2] << (m[0])[3][3] << endl;
	  */

	  server->send_mat4(m);
	  io_service.poll();
  }

  return 0;
}
