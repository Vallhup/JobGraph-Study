#pragma once

#include <unordered_map>

#include "Protocol.hpp"
#include "Event.h"

class EventConverter {
	using HandlerFunc = bool(*)(const PacketHeader&, const char*, GameEvent*);

public:
	static EventConverter& Get()
	{
		static EventConverter converter;
		return converter;
	}

	bool Convert(const PacketHeader& header, const char* data, GameEvent* out)
	{
		auto it = _handlerTable.find(header.type);
		if (it != _handlerTable.end())
			return it->second(header, data, out);

		return false;
	}

private:
	EventConverter()
	{
		_handlerTable[(uint16_t)PacketType::CS_LOGIN] = &EventConverter::HandleConnect;
	}

	// TODO : Handler 함수 추가
	static bool HandleConnect(const PacketHeader& header, const char* data, GameEvent* out);

	std::unordered_map<uint16_t, HandlerFunc> _handlerTable;
};

