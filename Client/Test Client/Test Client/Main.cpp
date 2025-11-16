#define ASIO_STANDALONE
#include <asio.hpp>
#include <iostream>
#include <thread>

#include "../../../Server/JobGraph Study/SendBuffer.h"
#include "../../../Server/JobGraph Study/Protocol.hpp"
#include "../../../Server/JobGraph Study/Protocols/Protocol.pb.h"

using asio::ip::tcp;

class TestClient {
public:
    TestClient(asio::io_context& io, const std::string& host, uint16_t port)
        : _io(io), _socket(io)
    {
        tcp::resolver resolver(io);
        auto endpoints = resolver.resolve(host, std::to_string(port));

        asio::async_connect(_socket, endpoints,
            [&](std::error_code ec, tcp::endpoint)
            {
                if (!ec)
                {
                    std::cout << "[Client] Connected to server\n";
                    SendLogin();
                    StartRead();
                }
                else
                {
                    std::cout << "[Client] Connect failed: " << ec.message() << "\n";
                }
            });
    }

    void SendLogin()
    {
        Protocol::CS_LOGIN_PACKET login;
        login.set_sessionid(777);  // test session id

        auto buf = PacketFactory::Serialize(PacketType::CS_LOGIN, login);

        asio::async_write(_socket, asio::buffer(buf.data(), buf.size()),
            [](std::error_code ec, std::size_t)
            {
                if (!ec)
                    std::cout << "[Client] Sent CS_LOGIN_PACKET\n";
                else
                    std::cout << "[Client] Send failed: " << ec.message() << "\n";
            });
    }

    void StartRead()
    {
        _socket.async_read_some(asio::buffer(_recvBuf),
            [&](std::error_code ec, size_t bytes)
            {
                if (ec)
                {
                    std::cout << "[Client] Read error: " << ec.message() << "\n";
                    return;
                }

                std::cout << "[Client] Received [" << bytes << "] bytes\n";
                std::cout << "Data: ";

                for (size_t i = 0; i < bytes; ++i)
                    std::cout << std::hex << (unsigned)(_recvBuf[i] & 0xFF) << " ";

                std::cout << std::dec << "\n";

                StartRead();
            });
    }

private:
    asio::io_context& _io;
    tcp::socket _socket;
    std::array<char, 4096> _recvBuf{};
};

int main()
{
    try
    {
        asio::io_context io;

        TestClient client(io, "127.0.0.1", 7777);

        std::thread t([&]() { io.run(); });
        t.join();
    }

    catch (std::exception& e)
    {
        std::cout << "Exception: " << e.what() << "\n";
    }
}