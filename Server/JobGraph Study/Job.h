#pragma once

#include <iostream>
#include <thread>
#include <functional>
#include <latch>

#include "Component.h"
#include "System.hpp"

struct Job {
	virtual ~Job() = default;
};

struct LogicSystemJob : public Job {
	LogicSystem* system;
	float* dTRef;

	LogicSystemJob(LogicSystem* s, float* ref) : system(s), dTRef(ref) {}
	static void Execute(void* ctx)
	{
		LogicSystemJob* job = static_cast<LogicSystemJob*>(ctx);
		job->system->Update(*job->dTRef);
	}
};

struct EventSystemJob : public Job {
	EventSystemBase* system;

	EventSystemJob(EventSystemBase* s) : system(s) {}
	static void Execute(void* ctx)
	{
		EventSystemJob* job = static_cast<EventSystemJob*>(ctx);
		job->system->ConsumeEvents();
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