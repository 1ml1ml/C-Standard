#include <map>
#include <any>
#include <string>
#include <vector>
#include <variant>
#include <optional>
#include <charconv>

//16.����filesysem �ṩ�����ļ��Ĺ���
#include <filesystem>

//8.__has_includeԤ�������� �ж�ͷ�ļ��Ƿ񱻰���
#if __has_include(<iostream>)
#include <iostream>
#endif

class A
{
public:
	A() = default;
	A(int first, int second) {};

	//4.�������� ������ͷ�ļ��г�ʼ��
	inline static int val = 10;

	void func()
	{
		//9.lambda���ʽ��������*this����
		auto lamb = [*this]() {};
	}

	static void apply(int first, int second) {};
};

//5.�۵����ʽ
template<typename ...ARGS>
auto p(ARGS... args)
{
	return (args + ...);
}

//7.namespaceǶ��
namespace B::C {}

//9.��������Ҫ����һ������ʱ����ʹ��optional���ж��Ƿ񷵻صĿ�ֵ
std::optional<A> returnA(bool b)
{
	if (true == b)
		return A();
	else return std::nullopt;
}

int main()
{
	//1.��ģ�幹���Զ��Ƶ�
	std::pair(1, 2);

	//2.�ṹ����
	std::map<int, std::string> map{ {0, "0" } };
	for (const auto& [k, v] : map)
		std::cout << k << ":" << v << '\n';

	//3.if(init; condition) �� switch(init; conditon)
	if (auto a = 1; a == 1) { }

	//6.constexpr lambda �����ڼ����е���������
	constexpr auto lamb = []() {return 0; };

	//10.�����Ƶ��ַ�������ֵ���ת��
	std::string str = "123";
	auto val = 0;
	auto from_res = std::from_chars(str.data(), str.data() + 3, val);
	auto to_res = std::to_chars(str.data(), str.data() + str.size(), val);

	//11.��union��֧��
	std::variant<int, double> var(3.0);

	//12.���Ա����κ�ֵ����
	std::any any = 10;
	std::cout << any.type().name() << '\n';
	any = "10";
	std::cout << any.type().name() << '\n';
	any = 10.0;
	std::cout << any.type().name() << '\n';
	any = A();
	std::cout << any.type().name() << '\n';

	//13.std::applyչ��tuple��Ϊ�������뺯��
	std::apply(&A::apply, std::pair(1, 2));

	//14.std::make_from_tupleչ��tuple��Ϊ���캯������
	auto make_res = std::make_from_tuple<A>(std::make_tuple(1, 2));

	//15.as_const����ֵת����const����
	const std::string& const_str = std::as_const(str);

	return 0;
}