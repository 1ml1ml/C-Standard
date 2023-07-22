#include "ThreadPool.h"

ThreadPool::~ThreadPool()
{
	stop();
}

bool ThreadPool::init(unsigned count)
{
	if (true == m_Stop)
	{
		m_Threads.resize(count, nullptr);
		return true;
	}
	return false;
}

void ThreadPool::start()
{
	if (true == m_Stop)
	{
		m_Stop = false;
		for (auto& thread : m_Threads)
		{
			if (nullptr == thread)
				thread = new std::thread(&ThreadPool::run, this);
		}
	}
}

void ThreadPool::stop()
{
	m_Stop = true;
	for (auto& thread : m_Threads)
	{
		thread->join();
		delete thread;
		thread = nullptr;
	}
	m_Threads.clear();

	std::queue<std::function<void()>>().swap(m_Functions);
}

bool ThreadPool::isRuning() const
{
	return !m_Stop;
}

void ThreadPool::wait(long long ms)
{ 
	std::unique_lock<std::mutex> lock(m_Mutex);
	if (false == m_Functions.empty())
	{
		if (ms > 0)
			m_Condition.wait_for(lock, std::chrono::milliseconds(ms));
		else m_Condition.wait(lock);
	}
}

unsigned ThreadPool::alreadyRun() const
{
	return m_AlreadyRun;
}

void ThreadPool::run()
{
	while (isRuning())
	{
		auto function = next();
		if (nullptr != function)
			function();

		std::unique_lock<std::mutex> lock(m_Mutex);
		if (true == m_Functions.empty())
			m_Condition.notify_all();
		m_Condition.wait(lock);
	}
}

std::function<void()> ThreadPool::next()
{
	std::unique_lock<std::mutex> lock(m_Mutex);
	if (false == m_Functions.empty())
	{
		auto ret = std::move(m_Functions.front());
		m_Functions.pop();
		return ret;
	}
	return nullptr;
}

