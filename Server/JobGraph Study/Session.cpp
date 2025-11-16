#include "Session.h"

#include <iostream>
#include <concurrent_queue.h>

#include "Listener.h"
#include "Framework.h"
#include "SendBuffer.h"
#include "ObjectPoolManager.h"

Session::Session(tcp::socket s, int id) 
	: _socket(std::move(s)), _strand(_socket.get_executor()), _id(id)
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

	SendBuffer* buffer = ObjectPoolManager::Get().sendBuffers.Acquire();
	buffer->Write(data, size);

	auto self = shared_from_this();
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
				if (ec != asio::error::eof and
					ec != asio::error::operation_aborted)
				{
					std::cout << "Receive Error on Session[" << _id << "] EC["
						<< ec.message() << "]\n";
				}

				asio::dispatch(_strand, [this, self]() { Close(); });
				return;
			}

			_recvBuffer.Write(nullptr, static_cast<int>(bytes));
			ProcessPacket();
			Recv();
		});
}

void Session::InternalSend()
{
	if (_sendQueue.empty()) return;

	_gatherBufs.clear();
	_gatherBufs.reserve(MAX_BUFFERS);

	size_t totalBytes{ 0 };
	size_t batchCount{ 0 };

	for (auto& data : _sendQueue)
	{
		if (batchCount >= MAX_BUFFERS) break;
		if (totalBytes + data->GetSize() > MAX_BYTES) break;

		_gatherBufs.emplace_back(asio::buffer(data->GetBuffer(), data->GetSize()));
		totalBytes += data->GetSize();
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
				{
					SendBuffer* front = _sendQueue.front();
					ObjectPoolManager::Get().sendBuffers.Release(front);

					_sendQueue.pop_front();
				}

				if (not _sendQueue.empty())
					InternalSend();
			})
	);
}

void Session::ProcessPacket()
{
	// TODO : 패킷 구조 재설계 및 패킷 -> GameEvent 처리 Class 구현 필요
	char* ptr = _recvBuffer.GetReadPos();
	int size = _recvBuffer.GetContiguousUsedSize();

	while (true)
	{
		if (size < 0/* 패킷 헤더 사이즈 */) return;

		// 패킷 헤더 파싱
		if (size < 0/* 패킷 사이즈 (헤더에 포함) */) return;

		GameEvent event;
		if (/* 패킷 -> GameEvent 처리 Class */0)
			Framework::Get().eventQueue.push(event);

		_recvBuffer.Read(nullptr, 0/* 패킷 사이즈 */);

		ptr = _recvBuffer.GetReadPos();
		size = _recvBuffer.GetContiguousUsedSize();
	}
}
