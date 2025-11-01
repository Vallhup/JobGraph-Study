#include "Job.h"
#include "JobGraph.h"
#include "ThreadPool.h"

int main()
{
	ThreadPool pool(2);
	JobGraph graph(pool);

	Player player;

	JobNode* inputNode = graph.CreateNode<InputJob>(&player);
	JobNode* moveNode = graph.CreateNode<MoveJob>(&player);

	moveNode->AddDependency(inputNode);
	graph.Build();

	std::this_thread::sleep_for(std::chrono::milliseconds(200));

	pool.Stop();
}