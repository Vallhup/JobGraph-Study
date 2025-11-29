#pragma once

#include <iostream>
#include "ECS.h"
#include "System.h"

class LoginEventSystem : public EventSystemBase {
public:
	LoginEventSystem(ECS& e, int p = 0) : EventSystemBase(e, p) {}

	virtual void ConsumeEvents() override;

	void ProcessEvent(const ConnectEvent& event);
};