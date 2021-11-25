// author: Yujie Wang & Jie Hou
// Rearrage the code according to the course order
// Incorporate my own implementation and thoughts

#include <iostream>
#include <vector>
#include <string>
#include <bitset>
#include <memory>
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
namespace jj48
{
//-----------
// http://en.cppreference.com/w/cpp/language/type_alias
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

//----------------------------------------------------
// Move Semantics with Noexcept
//----------------------------------------------------
namespace jj301
{
// http://www.ibm.com/developerworks/cn/aix/library/1307_lisl_c11/
// http://stackoverflow.com/questions/8001823/how-to-enforce-move-semantics-when-a-vector-grows

// 可以理解成给别人打保票，所以别人才会用
// You need to inform C++ (specifically std::vector) that your move constructor and destructor does not throw.
// Then the move constructor will be called when the vector grows.
// If the constructor is not noexcept, std::vector can't use it,
// since then it can't ensure the exception guarantees demanded by the standard.

// 注意，growable conatiners (會發生 memory reallocation) 只有 vector 和 deque.

// 以下的 MyString 和 jj30 中的唯一差異是 noexcept for move functions. 結果十分良好.

class MyString {
private:
    char* _data;
    size_t _len;
    void _init_data(const char *s) {
        _data = new char[_len+1];
        memcpy(_data, s, _len);
        _data[_len] = '\0';
    }
public:
    // default ctor
    MyString() : _data(NULL), _len(0) { }

    // ctor
    MyString(const char* p) : _len(strlen(p)) {
        _init_data(p);
    }

    // copy ctor
    MyString(const MyString& str) : _len(str._len) {
        // TODO: 这个print的是啥？
        cout << "Copy Constructor is called! source: " << str._data << " [" << (void*)(str._data) << ']' << endl;
        _init_data(str._data); 	//COPY
    }

    // move ctor, with "noexcept"
    MyString(MyString&& str) noexcept : _data(str._data), _len(str._len) {
        cout << "Move Constructor is called! source: " << str._data << " [" << (void*)(str._data) << ']' << endl;
        // TODO: 这里数据都没赋值啊？
        str._len = 0;
        str._data = NULL; //避免 delete (in dtor)
    }

    // copy assignment
    MyString& operator=(const MyString& str) {
        cout << "Copy Assignment is called! source: " << str._data << " [" << (void*)(str._data) << ']' << endl;
        // 判断是否是给自己拷贝赋值
        // TODO: 具体写法再学习下
        if (this != &str) {
            // 如果原来有数据要删掉
            if (_data) delete _data;
            _len = str._len;
            _init_data(str._data); //COPY!
        }
        else {
            cout << "Self Assignment, Nothing to do." << endl;
        }
        return *this;
    }

    // move assignment
    // TODO: 理解具体的用法
    MyString& operator=(MyString&& str) noexcept { //注意 noexcept
        cout << "Move Assignment is called! source: " << str._data << " [" << (void*)(str._data) << ']' << endl;
        if (this != &str) {
            if (_data) delete _data;
            _len = str._len;
            _data = str._data; //MOVE!
            str._len = 0;
            str._data = NULL; //避免 delete (in dtor)
        }
        return *this;
    }

    // dtor
    // 文檔說需 noexcept 但本處無. destructor is noexcept by default.
    virtual ~MyString() {
        cout << "Destructor is called! " << "source: ";
        if (_data) cout << _data;
        cout << " [" << (void*)(_data) << ']' << endl;

        if (_data) {
            delete _data;
        }
    }
};

void test301_move_semantics_with_noexcept()
{
    cout << "\ntest301_move_semantics_with_noexcept_and_swap().......\n";

    vector<MyString> vec;
    // without reserve(N); // ?

    // TODO: 这里的逻辑还有待研究
    // Move Constructor is called! source: jjhou
    vec.push_back(MyString("jjhou"));
    // Destructor is called! [0]

    //Move Constructor is called! source: sabrina
    vec.push_back(MyString("sabrina"));

    vec.push_back(MyString("stacy"));

    // 以上十分好：
    // TODO: 第1条没咋看懂
	//  1, 以 temp obj.放入容器，編譯器知道那是個 Rvalue, 於是呼叫 move ctor 而非 copy ctor.
	//  2, 當 push_back() 引發 vector 擴展，擴展過程中使用 move 而非 copy.
    cout << "vec.clear() ------- \n";
    vec.clear();

    // exit 前會 delete all existing objects.
}


}

//---------------------------------------------------
int main(int argc, char** argv) 
{
    cout << "c++ version " << __cplusplus << endl;

    jj15::test15_variadic_template();

    jj48::test48_type_alias();

    jj301::test301_move_semantics_with_noexcept();

}