#pragma once

#include <iostream>
#include <thread>
#include <functional>

#include "Component.h"

struct Player {
	int id;
	int x{ 0 };
	int input{ 0 };
};

struct IJob {
	virtual ~IJob() = default;
};

struct InputJob : public IJob {
	Player* player;
	
	InputJob(Player* p) : player(p) {}
	static void Execute(void* ctx)
	{
		InputJob* job = static_cast<InputJob*>(ctx);
		job->player->input = 1;
		std::this_thread::sleep_for(std::chrono::milliseconds(1));

#ifdef _DEBUG
		printf("[T%zu] InputJob %d done\n",
			std::hash<std::thread::id>{}(std::this_thread::get_id()) % 100,
			job->player->id);
#endif
	}
};

struct MoveJob : public IJob {
	Player* player;

	MoveJob(Player* p) : player(p) {}
	static void Execute(void* ctx)
	{
		MoveJob* job = static_cast<MoveJob*>(ctx);
		job->player->x += job->player->input;
		std::this_thread::sleep_for(std::chrono::milliseconds(1));

#ifdef _DEBUG
		printf("[T%zu] MoveJob %d done\n",
			std::hash<std::thread::id>{}(std::this_thread::get_id()) % 100,
			job->player->id);
#endif
	}
};

struct ComponentUpdateJob : public IJob {
	Component* comp;
	float deltaTime;

	explicit ComponentUpdateJob(Component* c, float dT)
		: comp(c), deltaTime(dT) {}
	static void Execute(void* ctx)
	{
		ComponentUpdateJob* job = static_cast<ComponentUpdateJob*>(ctx);
		job->comp->Update(job->deltaTime);
	}
};