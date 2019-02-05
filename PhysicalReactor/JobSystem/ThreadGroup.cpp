#include "ThreadGroup.h"



ThreadGroup::ThreadGroup()
{}

ThreadGroup::~ThreadGroup()
{
	joinAll();
}

void ThreadGroup::joinAll()
{
	for (uint64_t i = 0; i < m_Threads.size(); ++i) {
		if (m_Threads[i].joinable())
			m_Threads[i].join();
	}
    m_Threads.clear();
}



