#pragma once

#include <list>
#include <queue>
#include <mutex>
#include <thread>
#include <atomic>
#include <future>
#include <functional>
#include <shared_mutex>
#include <condition_variable>

class ThreadPool
{
public:
	ThreadPool() = default;
	~ThreadPool();

public:
	/// <summary>
	/// ��ʼ���߳�����
	/// </summary>
	/// <param name="count"></param>
	/// <returns>�Ƿ�ɹ�</returns>
	bool init(unsigned count);

	/// <summary>
	/// ��ʼ�����߳�
	/// </summary>
	void start();

	/// <summary>
	/// ֹͣ�����߳�
	/// </summary>
	void stop();

	/// <summary>
	/// �Ƿ���������
	/// </summary>
	/// <returns></returns>
	bool isRuning() const;

	/// <summary>
	/// �ȴ����������������
	/// </summary>
	/// <param name="ms">��ʱֵ</param>
	void wait(long long ms = -1);

	/// <summary>
	/// ��ȡ�Ѿ����е���������
	/// </summary>
	/// <returns></returns>
	unsigned alreadyRun() const;

	template<typename T, typename ...ARGS>
	auto exec(T&& taskFunc, ARGS&&... args);

private:
	/// <summary>
	/// �̺߳���
	/// </summary>
	void run();

	/// <summary>
	/// ��ȡ��һ������
	/// </summary>
	/// <returns></returns>
	std::function<void()> next();

private:
	bool m_Stop = true;
	std::list<std::thread*> m_Threads;

	std::mutex m_Mutex;
	std::condition_variable m_Condition;
	std::queue<std::function<void()>> m_Functions;

	std::atomic<unsigned> m_AlreadyRun{0};
};

template<typename T, typename ...ARGS>
auto ThreadPool::exec(T&& func, ARGS && ...args)
{
	auto task = std::make_shared<std::packaged_task<decltype(func(args...))()>>(std::bind(std::forward<T>(func), std::forward<ARGS>(args)...));

	std::unique_lock<std::mutex> lock(m_Mutex);
	m_Functions.push([task]() {(*task)(); });
	m_Condition.notify_one();
	return task->get_future();
}

