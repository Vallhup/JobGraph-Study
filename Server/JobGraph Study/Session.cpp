#include "Session.h"

#include <iostream>
#include "Listener.h"

Session::Session(tcp::socket s, int id) 
	: _socket(std::move(s)), _strand(_socket.get_executor()), _id(id), _buffer(1024)
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
	_socket.shutdown(tcp::socket::shutdown_both, ec);
	_socket.close(ec);

	if (ec)
		std::cerr << "Socket close error: " << ec.message() << std::endl;
}

void Session::Send(const void* data, size_t size)
{
	if (size == 0) return;

	auto self = shared_from_this();
	const char* data_ = static_cast<const char*>(data);
	std::vector<char> buffer(data_, data_ + size);

	asio::dispatch(_strand,
		[this, self, buffer = std::move(buffer)]()
		{
			bool idle = _sendQueue.empty();
			_sendQueue.emplace_back(std::move(buffer));
			if (idle)
				InternalSend();
		});
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
						<< ec.message() << "]\n";
					Close();
				}
				return;
			}

			_recvBuffer.Write(nullptr, static_cast<int>(bytes));
			ProcessPacket();
			Recv();
		});
}

void Session::InternalSend()
{
	// TODO : scatter/gather IO 및 SendBuffer Pooling
	if (_sendQueue.empty()) return;

	_gatherBufs.clear();
	_gatherBufs.reserve(MAX_BUFFERS);

	size_t totalBytes{ 0 };
	size_t batchCount{ 0 };

	for (const auto& data : _sendQueue)
	{
		if (batchCount >= MAX_BUFFERS) break;
		if (totalBytes + data.size() > MAX_BYTES) break;

		_gatherBufs.emplace_back(asio::buffer(data));
		totalBytes += data.size();
		++batchCount;
	}

	auto self = shared_from_this();
	asio::async_write(
		_socket, _gatherBufs,
		asio::bind_executor(_strand,
			[this, self, batchCount](std::error_code ec, size_t)
			{
				if (ec)
				{
					std::cerr << "Send error: " << ec.message() << std::endl;
					Close();
					return;
				}

				for (size_t i = 0; i < batchCount; ++i)
					_sendQueue.pop_front();

				if (not _sendQueue.empty())
					InternalSend();
			})
	);
}

void Session::ProcessPacket()
{
	// TODO : 패킷 처리
	//
	// 1. 패킷 재조립 (RecvBuffer 활용)
	// 2. Game모듈로 데이터 전송(double buffering / Lock-Free Queue)
}
