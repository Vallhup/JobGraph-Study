#include "Network.h"
#include <iostream>

Network::Network(short port, int size)
	: _ioCtx(1), _workGuard(asio::make_work_guard(_ioCtx)),
	_listener(_ioCtx, port), _nextId(0)
{
}

Network::~Network()
{
}

void Network::Start()
{
	_listener.Start(this);

	for (int i = 0; i < 1; ++i)
	{
		_workers.emplace_back(
			[this]()
			{
				_ioCtx.run();
				std::cout << "[io_context] stopped? " << _ioCtx.stopped() << std::endl;
			});
	}
}

void Network::Stop()
{
	_workGuard.reset();
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
