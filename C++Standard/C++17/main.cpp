#include <map>
#include <any>
#include <string>
#include <vector>
#include <variant>
#include <optional>
#include <charconv>

//16.引入filesysem 提供关于文件的功能
#include <filesystem>

//8.__has_include预编译命令 判断头文件是否被包含
#if __has_include(<iostream>)
#include <iostream>
#endif

class A
{
public:
	A() = default;
	A(int first, int second) {};

	//4.内联变量 可以在头文件中初始化
	inline static int val = 10;

	void func()
	{
		//9.lambda表达式中允许捕获*this副本
		auto lamb = [*this]() {};
	}

	static void apply(int first, int second) {};
};

//5.折叠表达式
template<typename ...ARGS>
auto p(ARGS... args)
{
	return (args + ...);
}

//7.namespace嵌套
namespace B::C {}

//9.当函数需要返回一个对象时可以使用optional来判断是否返回的空值
std::optional<A> returnA(bool b)
{
	if (true == b)
		return A();
	else return std::nullopt;
}

int main()
{
	//1.类模板构造自动推导
	std::pair(1, 2);

	//2.结构化绑定
	std::map<int, std::string> map{ {0, "0" } };
	for (const auto& [k, v] : map)
		std::cout << k << ":" << v << '\n';

	//3.if(init; condition) 和 switch(init; conditon)
	if (auto a = 1; a == 1) { }

	//6.constexpr lambda 编译期间运行的匿名函数
	constexpr auto lamb = []() {return 0; };

	//10.更完善的字符串和数值间的转换
	std::string str = "123";
	auto val = 0;
	auto from_res = std::from_chars(str.data(), str.data() + 3, val);
	auto to_res = std::to_chars(str.data(), str.data() + str.size(), val);

	//11.对union的支持
	std::variant<int, double> var(3.0);

	//12.可以保存任何值类型
	std::any any = 10;
	std::cout << any.type().name() << '\n';
	any = "10";
	std::cout << any.type().name() << '\n';
	any = 10.0;
	std::cout << any.type().name() << '\n';
	any = A();
	std::cout << any.type().name() << '\n';

	//13.std::apply展开tuple作为参数传入函数
	std::apply(&A::apply, std::pair(1, 2));

	//14.std::make_from_tuple展开tuple作为构造函数参数
	auto make_res = std::make_from_tuple<A>(std::make_tuple(1, 2));

	//15.as_const将左值转换成const类型
	const std::string& const_str = std::as_const(str);

	return 0;
}