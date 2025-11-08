#pragma once

#include "Game.h"
#include "Network.h"

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

private:
	Game _game;
	Network _network;
	asio::io_context _ioCtx;
};

