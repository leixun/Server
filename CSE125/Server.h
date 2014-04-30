#pragma once
#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>
#include <set>
#include <deque>
#include <list>
#include "glslprogram.h" //Mat4 data type
#include "Scene.h"
#include "Cube.h"

#include <boost/enable_shared_from_this.hpp>

using boost::asio::ip::tcp;

class tcp_connection
	: public boost::enable_shared_from_this<tcp_connection>
{
public:
	typedef boost::shared_ptr<tcp_connection> tcp_connection_ptr;

	/*static pointer create(boost::asio::io_service& io_service)
	{
		return pointer(new tcp_connection(io_service));
	}*/

	tcp_connection(boost::asio::io_service& io_service, std::set<tcp_connection_ptr>& clients)
		: socket_(io_service), clients_(clients)
	{
		ret_.push_back(std::make_pair("emptyinit_connection_s_should not see", mat4(0.0f)));
		ret_.push_back(std::make_pair("emptyinit_connection_s_should not see", mat4(0.0f)));
	}

	~tcp_connection()
	{
		socket_.close();
	}

	tcp::socket& socket()
	{
		return socket_;
	}

	void start(int pID)
	{
		ret_.front() = std::make_pair(std::to_string(pID), mat4(0.0f));
		boost::asio::async_write(socket_,
			boost::asio::buffer(ret_, 2048),
			boost::bind(&tcp_connection::handle_read, this,
			boost::asio::placeholders::error));
	}

	void deliver(std::vector <pair<string, mat4>>& obj)
	{
		std::cout << "Server send data to connection" << std::endl;
		boost::asio::async_write(socket_,
			boost::asio::buffer(obj, 2048),
			boost::bind(&tcp_connection::handle_write, this,
			boost::asio::placeholders::error));
	}

	void handle_read(const boost::system::error_code& error)
	{
		if (!error)
		{
			std::cout << "Handling read" << std::endl;
			boost::asio::async_read(socket_,
				boost::asio::buffer(ret_, 2048),
				boost::bind(&tcp_connection::handle_read, shared_from_this(),
					boost::asio::placeholders::error));
		}
		// Else remove player from server
		else
		{
			clients_.erase(shared_from_this());
		}
	}

	void handle_write(const boost::system::error_code& error)
	{
		if (!error)
		{
			std::cout << "connection sent data to client" << std::endl;
			// all these pops might not be useful at all
		}
		// Else remove player from server
		else
		{
			std::cout << "Error sending from connection to client:" << error << std::endl;
			system("pause");
			clients_.erase(shared_from_this());
			socket_.close();
		}
	}
	std::vector <pair<string, mat4>> * getState()
	{
		return &ret_;
	}


private:
	std::set<tcp_connection_ptr>& clients_;
	tcp::socket socket_;
	std::vector <pair<string, mat4>> ret_;

};

typedef boost::shared_ptr<tcp_connection> tcp_connection_ptr;

class tcp_server
{
public:
	tcp_server(boost::asio::io_service& io_service,
		const tcp::endpoint& endpoint)
		: io_service_(io_service),
		acceptor_(io_service, endpoint)
	{
		emptyRet.push_back(std::make_pair("initKey_s", mat4(0.0f)));
		emptyRet.push_back(std::make_pair("initCam_s", mat4(0.0f)));
		pID = 0;
		start_accept();
	}

	void start_accept()
	{
		tcp_connection_ptr new_connection(new tcp_connection(io_service_, clients_));

		acceptor_.async_accept(new_connection->socket(),
			boost::bind(&tcp_server::handle_accept, this, new_connection,
			boost::asio::placeholders::error));
	}

	void handle_accept(tcp_connection_ptr connection,
		const boost::system::error_code& error)
	{
		if (!error)
		{
			std::cout << "Client connected!" << std::endl;
			join(connection);
			connection->start(pID);
			pID++;
		}
		start_accept();
	}

	void join(tcp_connection_ptr player)
	{
		std::cout << "added client to players" << std::endl;
		clients_.insert(player);
	}

	// If a player leaves? Probably will never be used.
	void leave(tcp_connection_ptr player)
	{
		clients_.erase(player);
	}

	void send(std::vector <pair<string, mat4>>& obj)
	{
		if (clients_.empty())
		{
			std::cout << "no clients to send to" << std::endl;
		}
		else
		{
			for each(tcp_connection_ptr client in clients_)
			{
				/*std::cout << "send called" << std::endl;
				std::cout << "sendVec string:" << obj.front().first << std::endl;
				std::cout << "sendVec dat:" << obj.front().second[0][0] <<
					obj.front().second[0][1] <<
					obj.front().second[0][2] <<
					obj.front().second[0][3] <<
					obj.front().second[1][0] <<
					obj.front().second[1][1] <<
					obj.front().second[1][2] <<
					obj.front().second[1][3] <<
					obj.front().second[2][0] <<
					obj.front().second[2][1] <<
					obj.front().second[2][2] <<
					obj.front().second[2][3] <<
					obj.front().second[3][0] <<
					obj.front().second[3][1] <<
					obj.front().second[3][2] <<
					obj.front().second[3][3] << std::endl;*/
				client->deliver(obj);
			}
		}
	}

	std::vector <pair<string, mat4>> * getState()
	{
		if (clients_.empty())
		{
			std::cout << "getting state but no clients" << std::endl;
			return &emptyRet;
		}

		for each(tcp_connection_ptr client in clients_)
		{
			std::cout << "getting from a client" << std::endl;
			return client->getState();
		}
	}

private:
	int pID;
	tcp::acceptor acceptor_;
	boost::asio::io_service& io_service_;
	std::set<tcp_connection_ptr> clients_;
	std::vector <pair<string, mat4>> emptyRet;
};