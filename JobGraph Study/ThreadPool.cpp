#include "ThreadPool.h"
#include "JobGraph.h"

ThreadPool::ThreadPool(size_t size) : _running(true)
{
	for (size_t i = 0; i < size; ++i)
		_workers.emplace_back(&ThreadPool::WorkerLoop, this);
}

void ThreadPool::Push(const JobData& job)
{
	_jobQueue.push(job);
}

void ThreadPool::Stop()
{
	_running = false;
	for (auto& w : _workers) {
		if(w.joinable())
			w.join();
	}
}

void ThreadPool::WorkerLoop()
{
	JobData job;
	while (_running) {
		if (_jobQueue.try_pop(job))
			job.func(job.context);

		else
			std::this_thread::yield();
	}
}
