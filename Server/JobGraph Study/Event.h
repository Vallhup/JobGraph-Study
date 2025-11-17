#pragma once

#include <variant>

struct ConnectEvent {
	int sessionId;
};

struct DisconnectEvent {
	int sessionId;
};

struct MoveEvent {
	int sessionId;
	int dirX;
	int dirZ;
};

//using GameEvent = std::variant<
//	ConnectEvent,
//	DisconnectEvent,
//	MoveEvent
//>;