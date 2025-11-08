#include "Session.h"

#include <iostream>
#include "Listener.h"

Session::Session(tcp::socket s, int id) 
	: _socket(std::move(s)), _id(id), _buffer(1024)
{
}

void Session::Start()
{
	std::cout << "[+] Client connected from "
		<< _socket.remote_endpoint().address().to_string()
		<< std::endl;

	Recv();
}

void Session::Close()
{
	std::error_code ec;
	_socket.close(ec);

	if (ec)
		std::cerr << "Socket close error: " << ec.message() << std::endl;
}

void Session::Send(void* packet)
{
	//_socket.async_write_some(/*SendBuffer*/,
	//	[this](std::error_code, size_t) {});
}

void Session::Recv()
{
	auto self = shared_from_this();
	_socket.async_read_some(asio::buffer(_buffer),
		[this, self](std::error_code ec, size_t bytes)
		{
			if (ec)
			{
				if (ec.value() != asio::error::operation_aborted)
				{
					std::cout << "Receive Erro on Session[" << _id << "] EC["
						<< ec.value() << "]\n";
					Close();
				}
				return;
			}
			// Process Packet : Game 모듈로 데이터 전송(double buffering)

			asio::async_write(_socket, asio::buffer(_buffer, bytes),
				[this, self](std::error_code, size_t) {});

			Recv();
		});
}
