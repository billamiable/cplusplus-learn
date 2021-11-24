// author: Yujie Wang & Jie Hou
// Rearrage the code according to the course order
// Incorporate my own implementation and thoughts

#include <iostream>
#include <bitset>
using namespace std;

//----------------------------------------------------
// Variadic Templates
//----------------------------------------------------
namespace jj15
{

//~~~~~~~~~~~~~~~
//case4
// 这个是作为stopping criterion，非常重要！
// 用来在recursive call的最后一轮用的
void printX()
{
}

template <typename T, typename... Types>
void printX(const T& firstArg, const Types&... args)
{
    cout << firstArg << endl; // print first argument
    printX(args...);          // recursive call for remaining arguments
}

//-----------
void test15_variadic_template()
{
    cout << "\ntest15_variadic_template().......... \n";

    //case4
    printX(7.5, "hello", bitset<16>(377), 42);


}
}


//----------------------------------------------------
// Type Alias
//----------------------------------------------------

// http://en.cppreference.com/w/cpp/language/type_alias
namespace jj48
{
//-----------
// 本质上和typedef是一样的
// 这个看起来很怪，但其实是一个函数名的alias
// 在C++1.0时写作：
// typedef void (*func)(int, int);
// 但是上面的写法看起来不太明显，func是一个类型
// 下面这样写就比较清楚了
using func = void (*) (int,int);

// 使用时只要赋给一个函数名称即可，本质是一个函数指针
// 下面创建了对象
void example(int, int) {}
func fn = example;

//-----------
// 
// 下面两种写法是一样的
// template <class CharT> using mystring = 
//     std::basic_string<CharT, std::char_traits<CharT>>;
template <class CharT>
using mystring = std::basic_string<CharT, std::char_traits<CharT>>;
// 实际使用时：
mystring<char> str;
// 其實在 <string>, <string_fwd.h> 都有以下 typedef
// typedef basic_string<char>   string;
// typedef相当于是c++1.0的实现方法，using更高级了

//-----------
// 模板类里也可以用这个
// type alias can introduce a member typedef name
template<typename T>
// 把struct当做class来看
struct Container {
    // 相当于这个类里有一个成员叫做value_type
    using value_type = T; // same as typedef T value_type;
};
// which can be used in generic programming
template<typename Container>
void fn2(const Container& c)
{
    // 强行告诉C++下面这个是类型，不是变量
    // 这样不用等到实例化再来确定是啥类型
    // 可能是创建了一个n的对象，把typename去掉来看，单纯认为是表明后者是类型
    // typename Container::value_type n; // 原版

    // 下面是我自己写的，会用到隐式类型转换
    // TODO: 这里对于实际的用处不理解？猜测可以用创建的对象去执行后续操作
    typename Container::value_type n = 1; // n = 1.5
    cout << "value is " << n << endl;
}

//-----------
void test48_type_alias()
{
    cout << "\ntest48_type_alias().......\n";

    Container<int> c;
    fn2(c); // Container::value_type will be int in this function
}
}


//---------------------------------------------------
int main(int argc, char** argv) 
{
    cout << "c++ version " << __cplusplus << endl;

    jj15::test15_variadic_template();

    jj48::test48_type_alias();

}