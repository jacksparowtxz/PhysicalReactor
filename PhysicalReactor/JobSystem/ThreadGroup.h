#pragma once

#include <thread>
#include <vector>



class ThreadGroup {

public:
	ThreadGroup();
	~ThreadGroup();
	void joinAll();

	template<typename F>
	void attach(F func) {
		m_Threads.emplace_back(std::thread(func));
	}

private:
	typedef std::vector<std::thread> Threads;
	Threads m_Threads;
};

