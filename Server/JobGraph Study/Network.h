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
	Network(short port, int size);
	~Network();

	void Start();
	void Stop();

	void Send(int id, const void* data);
	void QueuePacket(int id, const void* data);
	void FlushQueued();

private:
	void AddSession(int id, const std::shared_ptr<Session>& session);
	void RemoveSession(int id);

	std::atomic<int> _nextId;
	asio::io_context _ioCtx;
	asio::executor_work_guard<asio::io_context::executor_type> _workGuard;
	Listener _listener;

	std::mutex _mtx;
	std::unordered_map<int, std::shared_ptr<Session>> _sessions;
	// boost의 concurrent_flat_map or tbb의 concurrnet_hash_map으로 변경

	std::vector<std::thread> _workers;
};

// Send 함수 호출 정책
//
// 1. 즉시 전송
//  - 반응성이 좋아짐
//  - 구현도 쉬움
//  - 성능 부하가 심함
// 
// 2. 프레임 전송
//  - 반응성 좀 떨어짐 (0.5 ~ 1 Tick Delay)
//  - 성능 부하가 적음 (Send함수 호출이 적어지니까)

// 개선점
//
// 1. boost::asio를 사용하면 shared_ptr의 사용이 강제된다
//  - custom atomic_shared_ptr의 구현
//  - 흠...