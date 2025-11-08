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
	_listener.Start(*this);
}

void Network::Stop()
{
	_listener.Stop();
}

void Network::AddSession(int id, const std::shared_ptr<Session>& session)
{
	_sessions[id] = session;
}

void Network::RemoveSession(int id)
{
	_sessions.erase(id);
}
