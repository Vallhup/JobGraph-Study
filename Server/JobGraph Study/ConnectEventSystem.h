#pragma once

#include <iostream>
#include "System.h"



class ConnectEventSystem : public EventSystem<ConnectEvent> {
public:
	ConnectEventSystem(int p = 0) : EventSystem(p) {}

	virtual void ProcessEvent(const ConnectEvent& event) override
	{
		std::cout << "[ConnectEventSystem] Player[" << event.sessionId << "] Connected\n";
	}
};

