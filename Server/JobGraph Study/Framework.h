#pragma once

#include <concurrent_queue.h>

#include "Game.h"
#include "Network.h"
#include "Event.h"

using namespace concurrency;

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

	concurrent_queue<GameEvent> eventQueue;

private:
	asio::io_context _ioCtx;
	Network _network;
	Game _game;

	bool _running;
};