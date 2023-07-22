#include <iostream>
#include <shared_mutex>
#include <type_traits>
#include <vector>

//3.函数返回类型推导, 不需要再使用decltype, 
//在递归调用中,递归前必须有一个return语句来推导返回值类型
auto test(int i)
{
#if 1	//合法
	if (1 == i)
		return 1;
	else return test(++i);
#else	//非法
	if (i != 1)
		return test(++i);
	else return 1;
#endif
}


//4.decltype(auto), 用来声明变量以及指示函数返回值
//C++11中,因为auto总是推导出非引用类型,而auto&&总是推导出引用类型.
//decltype却可以根据表达式的值类别和性质推导出引用或非引用.
//因此C++14中新增decltype(auto)
//当decltype(auto)被用于声明变量时，该变量必须立即初始化
//在推导变量类型时，先用初始化表达式替换decltype(auto)当中的auto，
//然后再根据decltype的语法规则来确定变量的类型
decltype(auto) test1()
{
	return 1;
}

decltype(auto) I = 3;

//5.放松的constexpr函数限制	constexpr修饰的方法既能在编译期调用也能在运行期调用
//声明为constexpr的函数可以含有以下内容
//任何声明，除了static或thread_local变量, 没有初始化的变量声明
//条件分支语句if和switch
//所有的循环语句，包括基于范围的for循环
//表达式可以改变一个对象的值, 只需该对象的生命期在声明为constexpr的函数内部开始, 包括对有constexpr声明的任何非const非静态成员函数的调用

//6.变量模板
template<typename T>
T pi = T(3.1415926);


//7.聚合类成员初始化
struct MyStruct
{
	int x;
	int y = 42;
};


//10.deprecated属性
[[deprecated("f 是即将被弃用的方法!!")]] void f()
{
	std::cout << "方法f" << std::endl;
}

//11.共享的互斥体和锁
struct mutexTest
{
	std::shared_timed_mutex stmutex;
	int val;

	//可以多个线程同时读
	int read()
	{
		std::shared_lock<std::shared_timed_mutex>(stmutex);
		return val;
	}

	//同时只能有一个写线程且没有读线程
	void write(int val)
	{
		std::unique_lock<std::shared_timed_mutex>(stmutex);
		this->val += val;
	}
};

//12.元函数别名
//如果标准库的某个类模板(假设为std::some_class)只含有唯一的成员, 
//即成员类型type, 那么标准库提供std::some_class_t作为typename std::some_class::type的别名.

//13.关联容器中的异构查找
//允许通过其他类型进行查找, 只需要这个类型和实际的键类型之间可以进行比较操作.
//列:允许std::setstd::string使用const char*, 或任何可以通过operator < 与std::string比较的类型作为查找的参数.

//14.标准自定义字面量
//s, 用于创建各种std::basic_string类型。
//h min s ms us ns, 用于创建相应的std::chrono::duration时间间隔。

class CFinal final { };

int main(void)
{
	//1.泛型的lambad表达式, 允许lambad的形参列表中使用auto
	{
		[](auto x, auto y) {};
	}

	//2.lambad捕获参数中使用表达式, value被自动推导为int, 推导方式同auto
	{
		auto  a = [value = 1]{ return value; };
		std::cout << a() << std::endl;
	}

	//3.函数返回类型推导
	{
		test(0);
	}

	//6.变量模板
	{
		std::cout << pi<int> << std::endl;
		std::cout << pi<float> << std::endl;
		//给pi<int>重新赋值
		pi<int> = 6;
		std::cout << pi<int> << std::endl;
		std::cout << pi<float> << std::endl;
	}

	//7.聚合类成员初始化
	{
		MyStruct b = { 1 };	//a.x = 1, a.y = 42
	}

	//8.二进制字面量 0b,0B
	{
		int c = 0b1001000001;
	}

	//9.数字分位符 '
	{
		long long d = 4'1546'8937'6022;
	}

	//10.deprecated属性
	{
		//f();
	}

	//15.通过地址寻址多元组
	{
		std::tuple<std::string, std::string, int> tuple("123", "456", 7);
		int a = std::get<int>(tuple);	//a = 7;
		int b = std::get<2>(tuple);	//b = 7;
		std::string c = std::get<0>(tuple);	//c = "123";
		std::string d = std::get<1>(tuple);	//c = "456";
		//std::string e = std::get<std::string>(tuple);	//error tuple中存在多个std::string
	}

	//16.较小的标准库特性
	{
		//新增std::make_unique用于生成std::unique_ptr
		std::unique_ptr<int> up = std::make_unique<int>(5);
		//新增元函数std::is_final用于识别一个class是否被禁止继承
		std::cout << std::is_final<CFinal>::value << std::endl;
		//std::integral_constant增加了一个返回常量值的operator()
		std::integral_constant<bool, false>();	//等同于std::integral_constant<bool, false>::value;
		//新增全局的std::cbegin, std::cend;	返回常量迭代器
		std::vector<int> vec;
		auto cBegin = std::cbegin(vec);
		auto cEnd = std::cend(vec);
	}

	return 0;
}