#pragma once

#define ASIO_STANDALONE
#include <asio.hpp>
#include <memory>

#include "Session.h"

class Network;

class Listener {
public:
	Listener(asio::io_context& io, short port);

	void Start(Network& net);
	void Stop();

private:
	tcp::acceptor _acceptor;
	asio::io_context& _ioCtx;
};

