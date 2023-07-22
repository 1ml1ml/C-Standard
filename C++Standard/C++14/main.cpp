#include <iostream>
#include <shared_mutex>
#include <type_traits>
#include <vector>

//3.�������������Ƶ�, ����Ҫ��ʹ��decltype, 
//�ڵݹ������,�ݹ�ǰ������һ��return������Ƶ�����ֵ����
auto test(int i)
{
#if 1	//�Ϸ�
	if (1 == i)
		return 1;
	else return test(++i);
#else	//�Ƿ�
	if (i != 1)
		return test(++i);
	else return 1;
#endif
}


//4.decltype(auto), �������������Լ�ָʾ��������ֵ
//C++11��,��Ϊauto�����Ƶ�������������,��auto&&�����Ƶ�����������.
//decltypeȴ���Ը��ݱ��ʽ��ֵ���������Ƶ������û������.
//���C++14������decltype(auto)
//��decltype(auto)��������������ʱ���ñ�������������ʼ��
//���Ƶ���������ʱ�����ó�ʼ�����ʽ�滻decltype(auto)���е�auto��
//Ȼ���ٸ���decltype���﷨������ȷ������������
decltype(auto) test1()
{
	return 1;
}

decltype(auto) I = 3;

//5.���ɵ�constexpr��������	constexpr���εķ��������ڱ����ڵ���Ҳ���������ڵ���
//����Ϊconstexpr�ĺ������Ժ�����������
//�κ�����������static��thread_local����, û�г�ʼ���ı�������
//������֧���if��switch
//���е�ѭ����䣬�������ڷ�Χ��forѭ��
//���ʽ���Ըı�һ�������ֵ, ֻ��ö����������������Ϊconstexpr�ĺ����ڲ���ʼ, ��������constexpr�������κη�const�Ǿ�̬��Ա�����ĵ���

//6.����ģ��
template<typename T>
T pi = T(3.1415926);


//7.�ۺ����Ա��ʼ��
struct MyStruct
{
	int x;
	int y = 42;
};


//10.deprecated����
[[deprecated("f �Ǽ��������õķ���!!")]] void f()
{
	std::cout << "����f" << std::endl;
}

//11.����Ļ��������
struct mutexTest
{
	std::shared_timed_mutex stmutex;
	int val;

	//���Զ���߳�ͬʱ��
	int read()
	{
		std::shared_lock<std::shared_timed_mutex>(stmutex);
		return val;
	}

	//ͬʱֻ����һ��д�߳���û�ж��߳�
	void write(int val)
	{
		std::unique_lock<std::shared_timed_mutex>(stmutex);
		this->val += val;
	}
};

//12.Ԫ��������
//�����׼���ĳ����ģ��(����Ϊstd::some_class)ֻ����Ψһ�ĳ�Ա, 
//����Ա����type, ��ô��׼���ṩstd::some_class_t��Ϊtypename std::some_class::type�ı���.

//13.���������е��칹����
//����ͨ���������ͽ��в���, ֻ��Ҫ������ͺ�ʵ�ʵļ�����֮����Խ��бȽϲ���.
//��:����std::setstd::stringʹ��const char*, ���κο���ͨ��operator < ��std::string�Ƚϵ�������Ϊ���ҵĲ���.

//14.��׼�Զ���������
//s, ���ڴ�������std::basic_string���͡�
//h min s ms us ns, ���ڴ�����Ӧ��std::chrono::durationʱ������

class CFinal final { };

int main(void)
{
	//1.���͵�lambad���ʽ, ����lambad���β��б���ʹ��auto
	{
		[](auto x, auto y) {};
	}

	//2.lambad���������ʹ�ñ��ʽ, value���Զ��Ƶ�Ϊint, �Ƶ���ʽͬauto
	{
		auto  a = [value = 1]{ return value; };
		std::cout << a() << std::endl;
	}

	//3.�������������Ƶ�
	{
		test(0);
	}

	//6.����ģ��
	{
		std::cout << pi<int> << std::endl;
		std::cout << pi<float> << std::endl;
		//��pi<int>���¸�ֵ
		pi<int> = 6;
		std::cout << pi<int> << std::endl;
		std::cout << pi<float> << std::endl;
	}

	//7.�ۺ����Ա��ʼ��
	{
		MyStruct b = { 1 };	//a.x = 1, a.y = 42
	}

	//8.������������ 0b,0B
	{
		int c = 0b1001000001;
	}

	//9.���ַ�λ�� '
	{
		long long d = 4'1546'8937'6022;
	}

	//10.deprecated����
	{
		//f();
	}

	//15.ͨ����ַѰַ��Ԫ��
	{
		std::tuple<std::string, std::string, int> tuple("123", "456", 7);
		int a = std::get<int>(tuple);	//a = 7;
		int b = std::get<2>(tuple);	//b = 7;
		std::string c = std::get<0>(tuple);	//c = "123";
		std::string d = std::get<1>(tuple);	//c = "456";
		//std::string e = std::get<std::string>(tuple);	//error tuple�д��ڶ��std::string
	}

	//16.��С�ı�׼������
	{
		//����std::make_unique��������std::unique_ptr
		std::unique_ptr<int> up = std::make_unique<int>(5);
		//����Ԫ����std::is_final����ʶ��һ��class�Ƿ񱻽�ֹ�̳�
		std::cout << std::is_final<CFinal>::value << std::endl;
		//std::integral_constant������һ�����س���ֵ��operator()
		std::integral_constant<bool, false>();	//��ͬ��std::integral_constant<bool, false>::value;
		//����ȫ�ֵ�std::cbegin, std::cend;	���س���������
		std::vector<int> vec;
		auto cBegin = std::cbegin(vec);
		auto cEnd = std::cend(vec);
	}

	return 0;
}