#pragma once

#define ASIO_STANDALONE
#include <asio.hpp>
#include <unordered_map>
#include <memory>

#include "Listener.h"
#include "Session.h"

class Network {
	friend class Listener;

public:
	Network(short port, asio::io_context& ioCtx);
	~Network();

	void Start();
	void Stop();

private:
	void AddSession(int id, const std::shared_ptr<Session>& session);
	void RemoveSession(int id);

	int _nextId;
	asio::io_context& _ioCtx;
	Listener _listener;
	std::unordered_map<int, std::shared_ptr<Session>> _sessions;
	// boost의 concurrent_flat_map or tbb의 concurrnet_hash_map으로 변경
};

