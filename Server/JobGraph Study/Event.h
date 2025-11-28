#pragma once

#include <array>

struct ConnectEvent {
	int sessionId;
};

struct DisconnectEvent {
	int sessionId;
};

struct MoveEvent {
	int sessionId;
	int inputX;
	int inputZ;
	float yaw;
};

//using GameEvent = std::variant<
//	ConnectEvent,
//	DisconnectEvent,
//	MoveEvent
//>;