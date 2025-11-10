#pragma once

#include <iostream>
#include <thread>
#include <chrono>

#include "Constants.h"

class NODE {
public:
	size_t value;
	NODE* volatile next;
	NODE(size_t v) : value(v), next(nullptr) {}
};

class BackOff {
public:
	BackOff(int min_d, int max_d)
		: minDelay(min_d), maxDelay(max_d), limit(min_d)
	{
		if (limit <= 0)
		{
			limit = 1;
			minDelay = 1;
		}
	}

	void backoff()
	{
		int delay = rand() % limit;
		limit = limit * 2;
		if (limit > maxDelay) limit = maxDelay;
		std::this_thread::sleep_for(std::chrono::microseconds(delay));
	}

private:
	int minDelay, maxDelay;
	int limit;
};

class LF_Stack {
public:
	LF_Stack() : top(nullptr) {}

	~LF_Stack()
	{
		clear();
	}

	void clear()
	{
		while (nullptr != top) pop();
	}

	bool CAS(NODE* volatile* addr, NODE* expected, NODE* desired)
	{
		return std::atomic_compare_exchange_strong(
			reinterpret_cast<std::atomic<NODE*> volatile*>(addr),
			&expected, desired);
	}

	void push(size_t v)
	{
		BackOff backoff(1, 16);
		NODE* new_node = new NODE(v);
		while (true)
		{
			NODE* temp = top;
			new_node->next = temp;

			if (CAS(&top, temp, new_node)) return;
			else backoff.backoff();
		}
	}

	size_t pop()
	{
		BackOff backoff(1, 16);
		while (true)
		{
			NODE* temp = top;
			if (nullptr == temp) return -2;

			NODE* next = temp->next;
			int result = temp->value;

			if (temp != top) continue;
			if (CAS(&top, temp, next)) return result;
			else backoff.backoff();
		}
	}

	bool empty()
	{
		return top == nullptr;
	}

	void print20()
	{
		NODE* curr = top;
		for (int i = 0; i < 20 && curr != nullptr; i++, curr = curr->next)
			std::cout << curr->value << ", ";

		std::cout << std::endl;
	}

private:
	NODE* top;
};


