#pragma once

#include <unordered_map>

#include "Protocol.hpp"
#include "Event.h"

class EventConverter {
	using HandlerFunc = bool(*)(const PacketHeader&, const char*);

public:
	static EventConverter& Get()
	{
		static EventConverter converter;
		return converter;
	}

	bool Convert(const PacketHeader& header, const char* data)
	{
		auto it = _handlerTable.find(header.type);
		if (it != _handlerTable.end())
			return it->second(header, data);

		return false;
	}

private:
	EventConverter()
	{
		_handlerTable[(uint16_t)PacketType::CS_LOGIN] = &EventConverter::HandleConnect;
		_handlerTable[(uint16_t)PacketType::CS_MOVE] = &EventConverter::HandleMove;
	}

	// TODO : Handler 함수 추가
	static bool HandleConnect(const PacketHeader& header, const char* data);
	static bool HandleMove(const PacketHeader& header, const char* data);

	std::unordered_map<uint16_t, HandlerFunc> _handlerTable;
};

