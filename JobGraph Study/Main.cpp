#include "Job.h"
#include "JobGraph.h"
#include "ThreadPool.h"

#include "ECS.h"
#include "System.h"
#include "GameFramework.h"

#include <array>

int max_threads{ 16 };

int main()
{
	for (size_t num_threads = 1; num_threads <= max_threads; num_threads *= 2)
	{
		GameFramework game(num_threads);

		double total{ 0.0f };
		for (int frame = 0; frame < 100; ++frame)
			game.Update(0.016f);

		for (int frame = 0; frame < 10000; ++frame)
		{
			auto start = std::chrono::steady_clock::now();

			game.Update(0.016f);

			auto end = std::chrono::steady_clock::now();

			total += std::chrono::duration<double, std::milli>(end - start).count();
		}

		std::cout << num_threads << "threads Frame time Avg: "
			<< total / 10000.0 << " ms\n";
	}
}