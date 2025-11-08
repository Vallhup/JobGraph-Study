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
}

void Network::Stop()
{
	_listener.Stop();
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
