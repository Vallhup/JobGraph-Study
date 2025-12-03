#pragma once

#include <array>
#include <variant>

enum class EventType {
	EV_CONNECT,
	EV_DISCONNECT,
	EV_MOVE
};

namespace std {
	template<>
	struct hash<EventType> {
		size_t operator()(const EventType& id) const noexcept
		{
			return std::hash<int>()(static_cast<int>(id));
		}
	};
}

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

using EventPayload = std::variant<
	ConnectEvent,
	DisconnectEvent,
	MoveEvent
>;

struct Event {
	EventType type;
	EventPayload payload;
};