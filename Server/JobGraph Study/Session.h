#pragma once

#define ASIO_STANDALONE
#include <asio.hpp>
#include <memory>

using asio::ip::tcp;

class Session : public std::enable_shared_from_this<Session> {
public:
	explicit Session(tcp::socket s, int id);

	void Start();
	void Close();

	void Send(void* packet);

private:
	void Recv();

	int _id;
	tcp::socket _socket;

	std::vector<char> _buffer;
};

