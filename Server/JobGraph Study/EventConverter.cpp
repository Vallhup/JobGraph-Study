#include "EventConverter.h"
#include "Framework.h"

bool EventConverter::HandleConnect(const PacketHeader& header, const char* data)
{
	Protocol::CS_LOGIN_PACKET login;
	if (not PacketFactory::Deserialize(header, data, &login))
		return false;

	ConnectEvent ev{ login.sessionid() };
	Framework::Get().eventQueue.connectQ.push(ev);

	return true;
}

bool EventConverter::HandleMove(const PacketHeader& header, const char* data)
{
	Protocol::CS_MOVE_PACKET move;
	if (not PacketFactory::Deserialize(header, data, &move))
		return false;

	MoveEvent ev{ move.sessionid(), move.dx(), move.dy() };
	Framework::Get().eventQueue.moveQ.push(ev);

	return true;
}
