#define _USE_MATH_DEFINES
#include <iostream>
#include <string>
#include <time.h>
#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>
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

class udp_server
{
public:
	udp_server(boost::asio::io_service& io_service)
		: socket_(io_service, udp::endpoint(udp::v4(), 13))
	{
		start_receive();
	}

	void send(){
		boost::array<mat4, 1> m;
		m[0] = scene->getPlayerMats()[0];
		boost::shared_ptr<std::string> message(
			new std::string("this is a string"));
		socket_.async_send_to(
			boost::asio::buffer(m), remote_endpoint_,
			boost::bind(&udp_server::handle_send, this, message,
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred));
	}

private:
	udp::socket socket_;
	udp::endpoint remote_endpoint_;
	boost::array<int, 1> recv_buf_;

	void start_receive()
	{
		socket_.async_receive_from(
			boost::asio::buffer(recv_buf_), remote_endpoint_,
			boost::bind(&udp_server::handle_receive, this,
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred));
	}

	void handle_receive(const boost::system::error_code& error,
		std::size_t /*bytes_transferred*/)
	{
		int retState = recv_buf_[0];

		cout << retState << endl;

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

		/*if (recv_buffer_[0] == 0){
			scene->setHMove(0, -1);
		}
		else if (recv_buffer_[0] == 1){
			scene->cancelHMove(0, -1);
		}
		else if (recv_buffer_[0] == 2){
			scene->setHMove(0, 1);
		}
		else if (recv_buffer_[0] == 3){
			scene->cancelHMove(0, 1);
		}
		else if (recv_buffer_[0] == 4){
			scene->setVMove(0, 1);
		}
		else if (recv_buffer_[0] == 5){
			scene->cancelVMove(0, 1);
		}
		else if (recv_buffer_[0] == 6){
			scene->setVMove(0, -1);
		}
		else if (recv_buffer_[0] == 7){
			scene->cancelVMove(0, -1);
		}*/
		boost::array<mat4, 1> m;
		m[0] = scene->getPlayerMats()[0];
		boost::shared_ptr<std::string> message(
			new std::string("this is a string"));
		socket_.async_send_to(
			boost::asio::buffer(m), remote_endpoint_,
			boost::bind(&udp_server::handle_send, this, message,
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred));

		start_receive();
	}
	void handle_send(boost::shared_ptr<std::string> /*message*/,
		const boost::system::error_code& /*error*/,
		std::size_t /*bytes_transferred*/)
	{
	}
};
udp_server* server;
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
	  server = new udp_server(io_service);
  }
  catch (std::exception& e)
  {
	  std::cerr << e.what() << std::endl;
  }

  while (true){
	  static time_t tick = clock();
	  float diff = (float)(clock() - tick) / CLOCKS_PER_SEC;
	  tick = clock();
	  scene->simulate(diff, 1.0 / 100);
	  //server->send();
	  io_service.poll();
  }

  return 0;
}
