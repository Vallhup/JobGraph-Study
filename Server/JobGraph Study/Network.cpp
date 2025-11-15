#include "Network.h"

Network::Network(short port, asio::io_context& ioCtx)
	: _ioCtx(ioCtx), _listener(_ioCtx, port), _nextId(0)
{
}

Network::~Network()
{
}

void Network::Start()
{
	_listener.Start(this);

	for (int i = 0; i < 4; ++i)
	{
		_workers.emplace_back(
			[this]()
			{
				_ioCtx.run();
			});
	}
}

void Network::Stop()
{
	_listener.Stop();
	_ioCtx.stop();

	for (auto& worker : _workers)
	{
		if (worker.joinable())
			worker.join();
	}
}

void Network::Send(int id, const void* data)
{
	// TODO : id로 session 찾아서 Send 함수 호출
}

void Network::QueuePacket(int id, const void* data)
{
	// TODO : 별도의 Packet Queue 두고 push
}

void Network::FlushQueued()
{
	// TODO : Packet Queue 모두 꺼내서 Send
}

void Network::AddSession(int id, const std::shared_ptr<Session>& session)
{
	_sessions[id] = session;
}

void Network::RemoveSession(int id)
{
	_sessions.erase(id);
}
