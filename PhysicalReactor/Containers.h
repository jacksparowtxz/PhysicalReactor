#pragma once
#include"SpinLock.h"

namespace PRE
{
	template<typename T,size_t capacity>
	class ThreadSafeRingBuffer
	{
	public:

		inline bool push_back(const T& item)
		{
			bool result = false;
			lock.lock();
			size_t next = (head + 1) % capacity;
			if (next != tail)
			{
				data[head] = item;
				head = next;
				result = true;

			}
			lock.unlock();
			return result;
		}

		inline bool pop_front(T& item)
		{
			bool result = false;
			lock.lock();
			if (tail != head)
			{
				item = data[tail];
				tail = (tail + 1) % capacity;
				result = true;

			}
			lock.unlock();
			return result;
		
		}

	private:
		T data[capacity];
		size_t head = 0;
		size_t tail = 0;
		SpinLock lock;
		
	};
}