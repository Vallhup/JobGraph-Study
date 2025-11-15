#pragma once

#include <variant>

struct ConnectEvent {
	int sessionId;
};

struct DisconnectEvent {
	int sessionId;
};

using GameEvent = std::variant<
	ConnectEvent,
	DisconnectEvent
>;