#include "Job.h"
#include "JobGraph.h"
#include "ThreadPool.h"

#include "ECS.h"
#include "System.h"
#include "GameFramework.h"

#include <array>

const int playerCount = 20'000;

int main()
{
	GameFramework game{ 8 };

	for(int i = 0; i < 500; ++i)
		game.AddInstance();

	game.UpdateParallel(0.016f);

	for (int frame = 0; frame < 5; ++frame)
	{
		printf("=== Frame %d ===\n", frame);

		auto start = std::chrono::steady_clock::now();

		game.UpdateParallel(0.016f);

		auto end = std::chrono::steady_clock::now();

		std::cout << "Frame time: "
			<< std::chrono::duration<float, std::milli>(end - start).count()
			<< " ms\n";
	}
}