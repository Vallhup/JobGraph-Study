#pragma once

#include <concurrent_queue.h>
#include "Event.h"

using namespace concurrency;

struct EventQueue {
	concurrent_queue<ConnectEvent> connectQ;
	concurrent_queue<DisconnectEvent> disconnectQ;
	concurrent_queue<MoveEvent> moveQ;

	void push(const ConnectEvent& ev) { connectQ.push(ev); }
	void push(const DisconnectEvent& ev) { disconnectQ.push(ev); }
	void push(const MoveEvent& ev) { moveQ.push(ev); }
};