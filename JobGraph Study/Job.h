#pragma once

#include <iostream>

struct Player {
	int x{ 0 };
	int y{ 0 };
	int inputX{ 0 };
	int inputY{ 0 };
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
		job->player->inputX = 1;
		job->player->inputY = 0;
		printf("[InputJob] Input set: (%d, %d)\n",
			job->player->inputX, job->player->inputY);
	}
};

struct MoveJob : public IJob {
	Player* player;

	MoveJob(Player* p) : player(p) {}
	static void Execute(void* ctx)
	{
		MoveJob* job = static_cast<MoveJob*>(ctx);
		job->player->x += job->player->inputX;
		job->player->y += job->player->inputY;
		printf("[MoveJob] Player moved to (%d, %d)\n",
			job->player->x, job->player->y);
	}
};