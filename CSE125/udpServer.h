#pragma once
#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>
#include "glslprogram.h" //Mat4 data type
#include "Scene.h"
#include "Cube.h"

using boost::asio::ip::udp;

class udpServer
{
public:
	udpServer(boost::asio::io_service& io_service)
		: socket_(io_service, udp::endpoint(udp::v4(), 13))
	{
		start_receive();
	}

	~udpServer()
	{
		socket_.close();
	}

	void send_mat4(boost::array<mat4, 1> m){
		boost::shared_ptr<std::string> message(
			new std::string("this is a string"));
		socket_.async_send_to(
			boost::asio::buffer(m), remote_endpoint_,
			boost::bind(&udpServer::handle_send, this, message,
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred));
	}

	int get_keyState()
	{
		return retState;
	}

private:
	udp::socket socket_;
	udp::endpoint remote_endpoint_;
	boost::array<int, 1> recv_buf_;
	int retState;

	void start_receive()
	{
		socket_.async_receive_from(
			boost::asio::buffer(recv_buf_), remote_endpoint_,
			boost::bind(&udpServer::handle_receive, this,
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred));
	}

	void handle_receive(const boost::system::error_code& error,
		std::size_t /*bytes_transferred*/)
	{
		retState = recv_buf_[0];
		start_receive();
	}
	void handle_send(boost::shared_ptr<std::string> /*message*/,
		const boost::system::error_code& /*error*/,
		std::size_t /*bytes_transferred*/)
	{
	}
};