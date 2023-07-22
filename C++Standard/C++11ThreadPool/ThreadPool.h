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
	/// 初始化线程数量
	/// </summary>
	/// <param name="count"></param>
	/// <returns>是否成功</returns>
	bool init(unsigned count);

	/// <summary>
	/// 开始运行线程
	/// </summary>
	void start();

	/// <summary>
	/// 停止所有线程
	/// </summary>
	void stop();

	/// <summary>
	/// 是否正在运行
	/// </summary>
	/// <returns></returns>
	bool isRuning() const;

	/// <summary>
	/// 等待所有任务运行完成
	/// </summary>
	/// <param name="ms">超时值</param>
	void wait(long long ms = -1);

	/// <summary>
	/// 获取已经运行的任务数量
	/// </summary>
	/// <returns></returns>
	unsigned alreadyRun() const;

	template<typename T, typename ...ARGS>
	auto exec(T&& taskFunc, ARGS&&... args);

private:
	/// <summary>
	/// 线程函数
	/// </summary>
	void run();

	/// <summary>
	/// 获取下一条任务
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

