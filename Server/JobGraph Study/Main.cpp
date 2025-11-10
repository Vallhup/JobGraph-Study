#define ASIO_STANDALONE
#include <asio.hpp>
#include <array>

#include "Network.h"

#include "Job.h"
#include "JobGraph.h"
#include "ThreadPool.h"

#include "ECS.h"
#include "System.h"
#include "Game.h"

#include "Constants.h"

int main()
{
    try {
        asio::io_context io;
        Network net(9000, io);

        net.Start();

        std::vector<std::thread> workers;

        for (size_t i = 0; i < max_threads; ++i)
        {
            workers.emplace_back(
                [&io]()
                {
                    io.run();
                });
        }

        std::cout << "[Server] Running on port 9000\n";
        std::cout << "Press Enter to stop...\n";
        std::cin.get();

        net.Stop();
        io.stop();

        for (std::thread& worker : workers)
            if (worker.joinable())
                worker.join();

    }
    catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
}