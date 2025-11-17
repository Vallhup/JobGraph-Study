#pragma once

#include "Game.h"
#include "Network.h"
#include "EventQueue.h"

class Framework {
public:
	static Framework& Get()
	{
		static Framework framework;
		return framework;
	}

	Framework(size_t size = std::thread::hardware_concurrency());

	void Start();
	void Stop();

	static BOOL WINAPI ConsoleHandler(DWORD ctrlType);

	//concurrent_queue<GameEvent> eventQueue;
	EventQueue eventQueue;

	std::unordered_map<int, Entity> sessionToEntity;
	std::unordered_map<Entity, int> entityToSession;

private:
	Network _network;
	Game _game;

	bool _running;
};