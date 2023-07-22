#include <iostream>

#include "ThreadPool.h"

void test(int a)
{
	std::cout << a;
}

int main()
{
	ThreadPool pool;
	pool.init(1);
	pool.start();
	pool.exec(&test, 10);
	pool.wait();
	return 0;
}