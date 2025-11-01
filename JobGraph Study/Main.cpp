#include "Job.h"
#include "JobGraph.h"
#include "ThreadPool.h"

#include <array>

const int playerCount = 1'000;
std::array<Player, playerCount> players;
std::array<JobNode*, playerCount> inputs;
std::array<JobNode*, playerCount> moves;

int main()
{
	ThreadPool pool(1);
	JobGraph graph(pool);

	for (int i = 0; i < playerCount; ++i)
	{
		players[i].id = i;
		
		inputs[i] = graph.CreateNode<InputJob>(&players[i]);
		moves[i] = graph.CreateNode<MoveJob>(&players[i]);

		moves[i]->AddDependency(inputs[i]);
	}

	auto start = std::chrono::steady_clock::now();

	graph.Build();

	auto end = std::chrono::steady_clock::now();

	std::cout << "Elapsed: "
		<< std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
		<< " ms\n";

	pool.Stop();
}

// 1. hardware_concurrency()
//  - 1904ms
// 
// 2. 8 threads
//  - 3849ms
// 
// 3. 4 threads
//  - 7672ms
// 
// 4. 2 threads
//  - 15423ms
// 
// 5. 1 thread
//  - 30754ms