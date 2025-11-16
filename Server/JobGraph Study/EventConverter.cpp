#include "EventConverter.h"

bool EventConverter::HandleConnect(const PacketHeader& header, const char* data, GameEvent* out)
{
	Protocol::CS_LOGIN_PACKET login;
	if (not PacketFactory::Deserialize(header, data, &login))
		return false;

	*out = ConnectEvent{ login.sessionid() };
	return true;
}
