#pragma once

#include "Number.h"
#include <intrin.h>


template<typename Work, uint64_t NumEntries>
class WorkStealingQueue {
	typedef Work value_type;
private:
	static const uint64_t Size = NumEntries;
	static const uint64_t MASK = Size - 1;
	static_assert(Number::IsPowerOf2(NumEntries), "The Work-Stealing Queue size must be a power-of-2");
public:
	WorkStealingQueue() : m_Bottom(0), m_Top(0) {}
	
	void push(const Work& work) {
		uint64_t b = m_Bottom;
		m_Values[b & MASK] = work;

		_ReadWriteBarrier();

		m_Bottom = b + 1;
	}

	value_type steal() {
		uint64_t t = m_Top;

		_ReadWriteBarrier();

		uint64_t b = m_Bottom;
		if (t < b) {
			value_type v = m_Values[t & MASK];
			
			if (!m_Top.compare_exchange_strong(t, t + 1)) {
				return value_type();
			}
			return v;
		}
		return value_type();
	}

	value_type pop(void)
	{
		uint64_t b = m_Bottom - 1;
		m_Bottom = b;

		//MEMORY_BARRIER;
		_mm_mfence();
		uint64_t t = m_Top;
		if (t <= b)
		{
			value_type v = m_Values[b & MASK];
			if (t != b)
			{
				return v;
			}

			if (!m_Top.compare_exchange_strong(t, t + 1))
			{
				v = value_type();
			}

			m_Bottom = t + 1;
			return v;
		}
		m_Bottom = t;
		return value_type();
	}

	uint64_t capacity() const { return Size; }

private:
	value_type m_Values[Size];
	uint64_t m_Bottom;
	std::atomic<uint64_t> m_Top;
};


