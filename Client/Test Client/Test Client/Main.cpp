#define ASIO_STANDALONE
#include <asio.hpp>
#include <iostream>
#include <thread>

using asio::ip::tcp;

void RunClient(int id)
{
    try {
        asio::io_context io;
        tcp::socket sock(io);
        tcp::resolver resolver(io);
        asio::connect(sock, resolver.resolve("127.0.0.1", "9000"));

        std::string msg = "Client" + std::to_string(id);
        for (int i = 0; i < 100; ++i)
        {
            asio::write(sock, asio::buffer(msg));
            char reply[128];
            size_t len = sock.read_some(asio::buffer(reply));
            reply[len] = '\0';

            if (std::string(reply) != msg)
                std::cerr << "Client[" << id << "] mismatch!\n";

            std::this_thread::sleep_for(std::chrono::milliseconds(2));
        }

        std::cout << "Client[" << id << "] finished.\n";
    }
    catch (std::exception& e)
    {
        std::cerr << "Client[" << id << "] exception: " << e.what() << "\n";
    }
}

int main() {
    constexpr int CLIENT_COUNT = 10;
    constexpr int ITER = 100;

    for (int iter = 0; iter < ITER; ++iter)
    {
        std::vector<std::thread> threads;
        for (int i = 0; i < CLIENT_COUNT; ++i)
            threads.emplace_back(RunClient, i);

        for (auto& t : threads)
            t.join();

        std::cout << "[Cycle " << iter << "] completed.\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}