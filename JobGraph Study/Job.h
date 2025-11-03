#pragma once

#include <iostream>
#include <thread>
#include <functional>
#include <latch>

#include "Component.h"
#include "System.h"

struct Player {
	int id;
	int x{ 0 };
	int input{ 0 };
};

struct Job {
	virtual ~Job() = default;
};

struct InputJob : public Job {
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

struct MoveJob : public Job {
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

struct SystemJob : public Job {
	System* system;
	float* dTRef;

	SystemJob(System* s, float* ref) : system(s), dTRef(ref) {}
	static void Execute(void* ctx)
	{
		SystemJob* job = static_cast<SystemJob*>(ctx);
		job->system->Update(*job->dTRef);
	}
};

extern std::atomic<int> activeJobs;
extern std::atomic<int> peak;

struct ParallelForJob : public Job {
	int begin;
	int end;
	std::function<void(int, int)> func;
	std::latch* latch;
	std::atomic<bool> executed;

	ParallelForJob(int b, int e, const std::function<void(int, int)>& f, std::latch* l)
		: begin(b), end(e), func(f), latch(l), executed(false) {}
	static void Execute(void* ctx)
	{
		ParallelForJob* job = static_cast<ParallelForJob*>(ctx);

		bool expected{ false };
		if (job->executed.compare_exchange_strong(expected, true)) {
			job->func(job->begin, job->end);
			job->latch->count_down();
			delete job;
		}
	}
};