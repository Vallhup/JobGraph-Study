#define ASIO_STANDALONE
#include <asio.hpp>
#include <iostream>
#include <thread>

using asio::ip::tcp;

int main() {
    try {
        asio::io_context io;
        tcp::socket socket(io);
        tcp::resolver resolver(io);

        auto endpoints = resolver.resolve("127.0.0.1", "7777");
        asio::connect(socket, endpoints);

        std::cout << "Connected to server.\n";

        std::thread reader([&]() {
            char data[1024];
            while (true) {
                std::error_code ec;
                size_t len = socket.read_some(asio::buffer(data), ec);
                if (ec) break;
                std::cout << "[Recv] " << std::string(data, len) << "\n";
            }
            });

        std::string line;
        while (std::getline(std::cin, line)) {
            if (line == "quit") break;
            line.push_back('\n');
            asio::write(socket, asio::buffer(line));
        }

        socket.close();
        reader.join();
    }
    catch (std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
    }
}