#pragma once

#include "Game.h"
#include "Network.h"
#include "EventQueue.h"

struct alignas(64) PlayerState {
	std::atomic<int> inputX{ 0 };
	std::atomic<int> inputZ{ 0 };
	std::atomic<float>  yaw{ 0 };
	std::atomic<uint64_t> lastSeq{ 0 };
};

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

	EventQueue eventQueue;

	std::unordered_map<int, std::unique_ptr<PlayerState>> playerStates;
	std::unordered_map<int, Entity> sessionToEntity;
	std::unordered_map<Entity, int> entityToSession;

	Network network;
	Game game;

private:
	bool _running;
};