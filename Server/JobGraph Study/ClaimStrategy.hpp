#pragma once

#include <atomic>

namespace disruptor {
	class ClaimStrategy {
	public:
		virtual int64_t Claim() = 0;
		virtual int64_t ClaimBatch(int64_t n, int64_t& startSeq) = 0;
	};

	class SingleProducerClaimStrategy final : public ClaimStrategy {
	public:
		virtual int64_t Claim() override
		{
			return _sequence++;
		}

		virtual int64_t ClaimBatch(int64_t, int64_t&) override
		{
			return _sequence++;
		}

	private:
		alignas(64) int64_t _sequence{ 0 };
	};

	class MultiProducerClaimStrategy final : public ClaimStrategy {
	public:
		MultiProducerClaimStrategy(RingBuffer& buffer) : _buffer(buffer) {}

		virtual int64_t Claim() override
		{
			// Common2 RMW연산의 합의 수 = 2라고 배웠는데 과연 이렇게 하는게 thread-safe한가?
			// 요즘 fetch_add 연산은 내부적으로 Common2가 아닌 방식으로 구현된다고 한다 흠...
			return _sequence.value.fetch_add(1);
		}

		virtual int64_t ClaimBatch(int64_t n, int64_t& startSeq) override
		{
			int64_t bufferSize = _buffer.GetSize();
			if (n < 1 || n > bufferSize)
				throw std::invalid_argument("n must be > 0 and < bufferSize");

			int64_t current = _sequence.value.fetch_add(n);

			int64_t nextSeq = current + n;
			int64_t wrapPoint = nextSeq - bufferSize;
			int64_t cachedGating = _cachedGating.value;

			if (wrapPoint > cachedGating || cachedGating > current)
			{
				int64_t gating;
				while (wrapPoint > (gating = _buffer.GetMinGatingSequence()))
					std::this_thread::sleep_for(std::chrono::nanoseconds(1));

				_cachedGating.value = gating;
			}

			startSeq = current + 1;
			return nextSeq;
		}

	private:
		AtomicCounter _sequence;
		AtomicCounter _cachedGating;
		RingBuffer& _buffer;
	};
}