#pragma once

#define ASIO_STANDALONE
#include <asio.hpp>
#include <memory>
#include <deque>
#include <concurrent_vector.h>

#include "RecvBuffer.h"

using asio::ip::tcp;

class Session : public std::enable_shared_from_this<Session> {
public:
	explicit Session(tcp::socket s, int id);

	void Start();
	void Close();

	void Send(const void* data, size_t size);

private:
	void Recv();
	void InternalSend();
	void ProcessPacket();

	int _id;
	tcp::socket _socket;
	asio::strand<asio::any_io_executor> _strand;

	std::vector<char> _buffer;
	RecvBuffer _recvBuffer;
	std::deque<std::vector<char>> _sendQueue;
};