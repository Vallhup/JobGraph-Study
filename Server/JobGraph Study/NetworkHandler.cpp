#include "NetworkHandler.h"
#include "Framework.h"

bool NetworkHandler::HandleConnect(int id, const PacketHeader& header, const char* data)
{
	Protocol::CS_LOGIN_PACKET login;
	if (not PacketFactory::Deserialize(header, data, &login))
		return false;

	ConnectEvent ev{ id };
	Framework::Get().eventQueue.push(ev);

	return true;
}

bool NetworkHandler::HandleMove(int id, const PacketHeader& header, const char* data)
{
	Protocol::CS_MOVE_PACKET move;
	if (not PacketFactory::Deserialize(header, data, &move))
		return false;

	MoveEvent ev{ id, move.inputx(), move.inputz(), move.yaw() };
	Framework::Get().eventQueue.push(ev);

	return true;
}
