#include "NetworkHandler.h"
#include "Framework.h"

bool NetworkHandler::HandleConnect(int id, const PacketHeader& header, const char* data)
{
	Protocol::CS_LOGIN_PACKET login;
	if (not PacketFactory::Deserialize(header, data, &login))
		return false;

	ConnectEvent ev{ id };
	Framework::Get().eventQueue.connectQ.push(ev);
	Framework::Get().playerStates.try_emplace(id, std::make_unique<PlayerState>());

	return true;
}

bool NetworkHandler::HandleMove(int id, const PacketHeader& header, const char* data)
{
	Protocol::CS_MOVE_PACKET move;
	if (not PacketFactory::Deserialize(header, data, &move))
		return false;

	auto& ps = Framework::Get().playerStates[id];

	uint64_t newSeq = move.sequence();
	uint64_t oldSeq = ps->lastSeq.load();

	while (true)
	{
		if (newSeq <= oldSeq) return true;
		if (ps->lastSeq.compare_exchange_strong(oldSeq, newSeq)) break;
	}

	ps->inputX.store(move.inputx());
	ps->inputZ.store(move.inputz());
	ps->yaw.store(move.yaw());

	return true;
}
