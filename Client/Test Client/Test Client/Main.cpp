#define ASIO_STANDALONE
#include <asio.hpp>
#include <iostream>
#include <thread>
#include <conio.h>

#include "../../../Server/JobGraph Study/SendBuffer.h"
#include "../../../Server/JobGraph Study/Protocol.hpp"
#include "../../../Server/JobGraph Study/Protocols/Protocol.pb.h"

using asio::ip::tcp;

class TestClient {
public:
    TestClient(asio::io_context& io, const std::string& host, uint16_t port)
        : _io(io), _socket(io), _strand(io.get_executor())
    {
        tcp::resolver resolver(io);
        auto endpoints = resolver.resolve(host, std::to_string(port));

        asio::async_connect(_socket, endpoints,
            [this](std::error_code ec, tcp::endpoint)
            {
                if (!ec)
                {
                    // 중요: 연결 성공 후 옵션 설정
                    _socket.set_option(tcp::no_delay(true));

                    std::cout << "[Client] Connected" << std::endl;
                    SendLogin();
                    StartRead();
                    StartInputThread();
                }
                else
                {
                    std::cout << "Connect failed: " << ec.message() << "\n";
                }
            });
    }

    void SendLogin()
    {
        Protocol::CS_LOGIN_PACKET login;
        login.set_sessionid(777);

        auto buf = PacketFactory::Serialize(PacketType::CS_LOGIN, login);

        // 단일 async_write 안전 전송
        asio::dispatch(_strand,
            [this, buf]()
            {
                asio::async_write(_socket, asio::buffer(buf.data(), buf.size()),
                    [this](std::error_code ec, std::size_t bytes)
                    {
                        if (ec)
                            std::cout << "SendLogin failed: " << ec.message() << "\n";
                        else
                            std::cout << "[Client] Sent Login (" << bytes << " bytes)\n";
                    });
            });
    }

    void SendMove(int dx, int dy)
    {
        Protocol::CS_MOVE_PACKET mp;
        mp.set_dx(dx);
        mp.set_dy(dy);
        mp.set_sessionid(777);

        auto buf = PacketFactory::Serialize(PacketType::CS_MOVE, mp);

        // async_write는 직렬화되지 않으면 위험하므로 strand 사용
        asio::dispatch(_strand,
            [this, buf]()
            {
                asio::async_write(_socket, asio::buffer(buf.data(), buf.size()),
                    [this](std::error_code ec, std::size_t bytes)
                    {
                        if (ec)
                            std::cout << "SendMove failed: " << ec.message() << "\n";
                        else
                            std::cout << "[Client] Sent Move (" << bytes << " bytes)\n";
                    });
            });
    }

    void StartRead()
    {
        _socket.async_read_some(asio::buffer(_recvBuf),
            [this](std::error_code ec, size_t bytes)
            {
                if (ec)
                {
                    std::cout << "[Client] Read error: " << ec.message() << "\n";
                    return;
                }

                std::cout << "[Client] Received " << bytes << " bytes\n";
                StartRead();
            });
    }

    void StartInputThread()
    {
        std::thread([this]() {
            while (true)
            {
                char c = getchar();
                if (c == 'w') SendMove(0, -1);
                if (c == 's') SendMove(0, 1);
                if (c == 'a') SendMove(-1, 0);
                if (c == 'd') SendMove(1, 0);
            }
            }).detach();
    }

private:
    asio::io_context& _io;
    tcp::socket _socket;
    asio::strand<asio::io_context::executor_type> _strand; // 직렬화 용도
    std::array<char, 4096> _recvBuf{};
};

int main()
{
    try
    {
        asio::io_context io;

        // 서버 주소/포트
        TestClient client(io, "127.0.0.1", 7777);

        // 네트워크 쓰레드 실행
        std::thread netThread([&]() {
            io.run();
            });

        netThread.join();
    }
    catch (const std::exception& e)
    {
        std::cout << "Exception: " << e.what() << "\n";
    }

    return 0;
}