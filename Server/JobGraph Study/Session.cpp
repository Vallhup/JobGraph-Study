#include "Session.h"

#include <iostream>
#include "Listener.h"

Session::Session(tcp::socket s, int id) 
	: _socket(std::move(s)), _id(id), _buffer(1024)
{
}

void Session::Start()
{
#ifdef _DEBUG
	std::cout << "[+] Client connected from "
		<< _socket.remote_endpoint().address().to_string()
		<< std::endl;
#endif

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
	_socket.async_read_some(
		asio::buffer(_recvBuffer.GetWritePos(), _recvBuffer.GetContiguousFreeSize()),
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

			_recvBuffer.Write(nullptr, static_cast<int>(bytes));
			ProcessPacket();
			Recv();
		});
}

void Session::ProcessPacket()
{
	// TODO : 패킷 처리
	//
	// 1. 패킷 재조립 (RecvBuffer 활용)
	// 2. Game모듈로 데이터 전송(double buffering / Lock-Free Queue)
}
