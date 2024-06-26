// author: Jie Hou & Yujie Wang
// 注意：以下内容整理自Hou Jie老师的代码示例，在本repo中为modern-feat-reference.cpp
//      本文件只包含了以上文件的部分代码，后面会陆续补充
//      在此基础上，添加了一些注释以帮助理解，并新增了一些测试用例（以yj的namespace区分）

#include <iostream>
#include <vector>
#include <list>
#include <deque>
// 注意下面两个没关系，string是针对字符串，cstring/string.h是针对字符数组
#include <string>
#include <cstring>
#include <bitset>
#include <memory>
#include <complex>
#include <ctime>
#include <algorithm>
#include <initializer_list>
#include <chrono>
using namespace std;

//----------------------------------------------------
// Initializer List
//----------------------------------------------------
namespace jj03 {
class P {
public:
    P(int a, int b) { cout << "P(int, int), a=" << a << ", b=" << b << endl; }

    P(initializer_list<int> initlist)
    {
        cout << "P(initializer_list<int>), values= ";
        for (auto i : initlist) cout << i << ' ';
        cout << endl;
    }
};

void print(initializer_list<int> vals)
{
    for (auto p = vals.begin(); p != vals.end(); ++p) {
        cout << *p << " ";
    }
    cout << endl;
}

void test03_initializer_list()
{
    cout << "\n----------------------------------------------------------\n";
    cout << "test03_initializer_list()..................";
    cout << "\n----------------------------------------------------------\n";

    print({12, 3, 6, 8});

    // initializer lists
    int i;                                                  // i has undefined value, default is 0
    int j{};                                                // j is initialized by 0
    int* p;                                                 // p has undefined value, default is nullptr
    int* q{};                                               // q is initialized by nullptr
    cout << i << " " << j << " " << p << " " << q << endl;  // 0  0  0  0

    int x1(5.3);
    int x2 = 5.3;

    //! int x3{5.0};     // ERROR: narrowing
    //! int x4 = {5.3};  // ERROR: narrowing

    char c1{7};  // OK: even though 7 is an int,this is not narrowing
    //[Warning] overflow in implicit constant conversion [-Woverflow]
    //!	char c2{99999};	// ERROR: narrowing

    // cout << x1 << " " << x2 << ' ' << x3 << ' ' << x4 << ' ' << c1 << ' ' << c2 << endl;  	//5 5 5 5

    vector<int> v1{1, 2, 3, 4};
    //! vector<int> v2{1, 2.1, 3.2, 4.4};  // ERROR: narrowing doubles to ints
    // for (auto& elem : v2) {
    //     cout << elem << " ";  // 1 2 3 4
    // }
    // cout << endl;

    {
        P p(77, 5);      // P(int, int), a=77, b=5
        P q{77, 5};      // P(initializer_list<int>), values= 77 5
        P r{77, 5, 42};  // P(initializer_list<int>), values= 77 5 42
        P s = {77, 5};   // P(initializer_list<int>), values= 77 5
    }

    {
        vector<int> v1{2, 4, 5, 6};
        vector<int> v2({2, 6, 7, 8});
        vector<int> v3;
        v3 = {4, 5, 6, 7};
        v3.insert(v3.begin() + 2, {0, 2, 3});

        for (auto i : v3) cout << i << " ";  // 4 5 0 2 3 6 7
        cout << endl;

        cout << max({string("A"), string("Stacy"), string("Sabrina")}) << endl;  // Stacy
        cout << min({string("A"), string("Stacy"), string("Sabrina")}) << endl;  // A
        cout << max({54, 23, 2}) << endl;                                        // 54
        cout << min({54, 23, 2}) << endl;                                        // 2
    }
}
}  // namespace jj03

//----------------------------------------------------
// Lambda
//----------------------------------------------------
namespace jj06 {
// TODO: function是什么？ lambda的retun type？
function<int(int, int)> returnLambda()
{
    return [](int x, int y) { return x * y; };
}

void test06_lambda()
{
    cout << "\n----------------------------------------------------------\n";
    cout << "test06_lambda()..................";
    cout << "\n----------------------------------------------------------\n";

    auto lf = returnLambda();
    // lf()创建了lambda的对象
    cout << lf(9, 8) << endl;  // 72

    // TODO: 没看懂下面要测试的东西
    // //[Error] no matching function for call to 'std::function<int(int, int)>::function(int, int)'
    // ! decltype(lf)(3,5);

    // no-op. 喚起的是 std::function 的 default ctor.
    // 這不是我要測的. 所以稍後又測一個 -- 那果然就編譯報錯了
    decltype(lf)();

    // lambda可以创建静态/非静态对象，也可以返回值（需要按照语法规则，编译器具有一定识别能力）
    [] {
        cout << "hello lambda" << endl;
        static int x = 5;
        int y = 6;
        return x;
    }();  // 这里在lambda后使用()会直接创建对象，并调用内部的print（一般很少这样写）

    // 下面是一组对比实验
    {
        int id = 0;
        // id是外部变量，取值
        // 若無 mutable ==> [Error] increment of read-only variable 'id'
        // id 變化不影響 outer scope (除非傳入的是 reference)
        auto f = [id]() mutable {
            cout << "id: " << id << endl;
            ++id;
        };

        id = 42;
        f();                 // id: 0
        f();                 // id: 1
        f();                 // id: 2
        cout << id << endl;  // 42，没改合理

        // TODO: 我是想創建一個 lambda 的 temp obj (then invoke it's default ctor,
        // but you know that lambda have no ctor or assignment operator.
        // [Error] no matching function for call to 'jj06::test06_lambda()::<lambda()>::__lambda9(int, int)'
        //! decltype(f)(3,5);

        // [Error] use of deleted function 'jj06::test06_lambda()::<lambda()>::<lambda>()'
        //! decltype(f)();
    }

    {
        int id = 0;
        // id是外部变量，取reference
        auto f = [&id](int param) {
            cout << "id: " << id << endl;
            ++id;     // 沒有 mutable 也能 ++
            ++param;  // 和 mutable 無關
        };
        id = 42;
        f(7);  // id: 42，由于使用引用不会创建新的对象，且f()在id=42后面，所以id的值变了
        f(7);  // id: 43
        f(7);  // id: 44
        cout << id << endl;  // 45，改了合理
    }

    // 给一个条件，大于30小于100拿掉
    vector<int> vi{5, 28, 50, 83, 70, 590, 245, 59, 24};
    int x = 30;
    int y = 100;
    vi.erase(remove_if(vi.begin(), vi.end(), [x, y](int n) { return x < n && n < y; }), vi.end());
    for (auto i : vi) cout << i << ' ';  // 5 28 590 245 24
    cout << endl;
}
}  // namespace jj06

//----------------------------------------------------
// Rvalue Move
//----------------------------------------------------
namespace jj12 {
void test12_Rvalue_Move()
{
    cout << "\n----------------------------------------------------------\n";
    cout << "test12_Rvalue_Move()..........";
    cout << "\n----------------------------------------------------------\n";

    string s1("aaaa");
    string s2("bbbb");
    string s3("cccc");
    string s4("dddd");
    string s = s1 + s2;

    cout << "s: " << s << endl;    // aaaabbbb
    cout << "s1: " << s1 << endl;  // aaaa
    cout << "s2: " << s2 << endl;  // bbbb

    s = move(s1);
    cout << "s: " << s << endl;    // aaaa
    cout << "s1: " << s1 << endl;  // empty

    s = move(s2);
    cout << "s: " << s << endl;    // bbbb
    cout << "s2: " << s2 << endl;  // empty

    // TODO: 普通情况下使用Move后面值为空，但是在相加时的第二项却不符合？
    s = move(s3) + move(s4);
    cout << "s: " << s << endl;    // ccccdddd
    cout << "s3: " << s3 << endl;  // empty
    cout << "s4: " << s4 << endl;  // dddd

    //----------------

    // 对于没有移动构造函数的数据类型，移动就等同于复制！！！
    // 例如，对于int，move后值不为空！
    int a = 6;
    int b = 4;
    int c = a + b;

    c = move(a);

    cout << "a is " << a << ", b is " << b << ", c is " << c << endl;

    // 再比如，对bool类型也没用！
    bool aa = true;
    bool bb = false;
    bool cc = aa || bb;

    cc = move(aa);

    cout << "aa is " << aa << ", bb is " << bb << ", cc is " << cc << endl;

    int x = 4;
    int y = 8;
    //! x + y = 10; // [Error] lvalue required as left operand of assignment

    //----------------

    s1 = "Hello ";
    s1 + s2 = s2;                  // TODO: 竟然可以通過編譯，作者自己违反了规则
    cout << "s1: " << s1 << endl;  // s1: Hello
    cout << "s2: " << s2 << endl;  // s2:
    string() = "World";            // 對 temp obj 賦值可以，作者自己违反了规则

    complex<int> c1(2, 3), c2(4, 5);
    c1 + c2 = complex<int>(6, 9);         // TODO: 不理解什么意思
    cout << "c1: " << c1 << endl;         // c1: (2,3)
    cout << "c2: " << c2 << endl;         // c2: (4,5)
    complex<int>() = complex<int>(6, 9);  // 對 temp obj 賦值可以，作者自己违反了规则
}

}  // namespace jj12

//----------------------------------------------------
// Variadic Template
//----------------------------------------------------
namespace jj15 {
// 以下代码包含了variadic template部分的所有内容

//~~~~~~~~~~~~~~~
// case4
// recursive function calling.
namespace case4 {
// 这个在recursive call的最后一轮被调用到，非常重要！
void printX() {}

template <typename T, typename... Types>
void printX(const T& firstArg, const Types&... args)
{
    // 实际使用时需要保证该数据类型支持了cout
    cout << firstArg << endl;  // print first argument
    printX(args...);           // recursive call for remaining arguments
}

}  // namespace case4

//~~~~~~~~~~~~~~~
// case3
// 用variadic template重写printf
namespace case3 {
// http://stackoverflow.com/questions/3634379/variadic-templates
// 这个在recursive call的最后一轮被调用到，只剩最开始的字符串
void printf(const char* s)
{
    // 模拟c的print方法，发现数量不匹配，输出错误
    while (*s) {
        if (*s == '%' && *(++s) != '%') throw std::runtime_error("invalid format string: missing arguments");
        std::cout << *s++ << "~end~";  // 打印空格和换行符
    }
    std::cout << std::endl;
}

template <typename T, typename... Args>
void printf(const char* s, T value, Args... args)
{
    while (*s) {
        if (*s == '%' && *(++s) != '%') {
            std::cout << value;
            // 用++s的原因是后面有个空格需要跳过
            printf(++s, args...);
            return;
        }
        // TODO: 递归的过程中打印空格，对于次序需要加深理解
        std::cout << *s++;
    }
    throw std::logic_error("extra arguments provided to printf");
}

}  // namespace case3

//~~~~~~~~~~~~~~~
// case1
// 检测数据里最大的（initializer_list实现）
namespace case1 {
// ...\4.9.2\include\c++\bits\predefined_oops.h
// 定义了仿函数，用于比大小
struct _Iter_less_iter {
    // 本质是一个仿函数，做一个比较
    template <typename _Iterator1, typename _Iterator2>
    bool operator()(_Iterator1 __it1, _Iterator2 __it2) const
    {
        return *__it1 < *__it2;
    }
};

// 生成仿函数的对象，用在下下个代码块里
inline _Iter_less_iter __iter_less_iter() { return _Iter_less_iter(); }

// 具体的实现，如何与所有的值做比较，存下来最大的那个
// ...\4.9.2\include\c++\bits\stl_algo.h
template <typename _ForwardIterator, typename _Compare>
_ForwardIterator __max_element(_ForwardIterator __first, _ForwardIterator __last, _Compare __comp)
{
    if (__first == __last) return __first;
    _ForwardIterator __result = __first;
    // 比大小并存下来其中较大的
    while (++__first != __last) {
        if (__comp(__result, __first)) __result = __first;
    }
    return __result;
}

// initializer_list的max函数的第一层调用实现
template <typename _ForwardIterator>
inline _ForwardIterator max_element(_ForwardIterator __first, _ForwardIterator __last)
{
    return __max_element(__first, __last, __iter_less_iter());
}

// 在初始化阶段使用{}会自动生成initializer_list，并调用里面的max函数
template <typename _Tp>
inline _Tp max(initializer_list<_Tp> __l)
{
    return *max_element(__l.begin(), __l.end());
}

}  // namespace case1

//~~~~~~~~~~~~~~~
// case2
// 检测数据里最大的（直接variadic template recursive实现）
namespace case2 {
// http://stackoverflow.com/questions/3634379/variadic-templates
int maximum(int n) { return n; }

template <typename... Args>
int maximum(int n, Args... args)
{
    return std::max(n, maximum(args...));
}
}  // namespace case2

//~~~~~~~~~~~~~~~
// case5
// 以非一般的方式处理第一个和最后一个元素
// refer to test07_tuples()
// not yet implemented, refer to modern-feat-reference.cpp

//~~~~~~~~~~~~~~~
// case6
// recursive inheritance
namespace case6 {
template <typename... Values>
class tuple;
// TODO: 这个是recursive call的最后一轮调用的吗？
template <>
class tuple<> {
};

// 分成head一个和tail一包
template <typename Head, typename... Tail>
// TODO: 为何继承tuple<Tail...> ？
class tuple<Head, Tail...> : private tuple<Tail...> {
    typedef tuple<Tail...> inherited;  // 继承的父类变成了inherited?

public:
    tuple() {}
    tuple(Head v, Tail... vtail) : m_head(v), inherited(vtail...) {}
    Head head() { return m_head; }
    // TODO: 如何判断这里的this是inherited类型？
    // 可以跟后面的recursive composition对比下，后面是直接return m_tail
    inherited& tail() { return *this; }

protected:
    Head m_head;
};

}  // namespace case6

//~~~~~~~~~~~~~~~
// case7
// recursive composition
// not yet implemented, refer to modern-feat-reference.cpp
namespace case7 {

}

//-----------
void test15_variadic_template()
{
    cout << "\n----------------------------------------------------------\n";
    cout << "test15_variadic_template().......... ";
    cout << "\n----------------------------------------------------------\n";

    // case4
    cout << "\n.....case4..........\n";
    case4::printX(7.5, "hello", bitset<16>(377), 42);  // 7.5 hello 0000000101111001 42

    // case3
    cout << "\n.....case3..........\n";
    int* pi = new int;  // 为了体现variadic template能支持许多不同的数据类型打印
                        // 15 This is Ace. 0x7fb5aa403430 3.14159 ~end~
                        // ~end~
    case3::printf("%d %s %p %f \n", 15, "This is Ace.", pi, 3.141592653);

    // case1
    cout << "\n.....case1..........\n";
    cout << case1::max({57, 48, 60, 100, 20, 18}) << endl;  // 100

    // case2
    cout << "\n.....case2..........\n";
    cout << case2::maximum(57, 48, 60, 100, 20, 18) << endl;  // 100

    // case 6
    cout << "\n.....case6..........\n";
    case6::tuple<int, float, string> t1(41, 6.3, "nico");
    cout << sizeof(t1) << endl;               // 32
    cout << t1.head() << endl;                // 41
    cout << t1.tail().head() << endl;         // 6.3
    cout << t1.tail().tail().head() << endl;  // nico

    cout << sizeof(bitset<16>) << endl;  // 8

    case6::tuple<string, complex<int>, bitset<16>, double> t2("Ac", complex<int>(3, 8), bitset<16>(37), 3.14);
    cout << sizeof(t2) << endl;                      // 48
    cout << t2.head() << endl;                       // Ac
    cout << t2.tail().head() << endl;                // (3,8)
    cout << t2.tail().tail().head() << endl;         // 0000000000100101
    cout << t2.tail().tail().tail().head() << endl;  // 3.14

    case6::tuple<string, complex<int>, double> t3("St", complex<int>(4, 9), 1.6);
    cout << sizeof(t3) << endl;               // 40
    cout << t3.head() << endl;                // St
    cout << t3.tail().head() << endl;         // (4,9)
    cout << t3.tail().tail().head() << endl;  // 1.6
}
}  // namespace jj15

//----------------------------------------------------
// Move Semantics with Noexcept
//----------------------------------------------------
namespace jj301 {
// 注意：以下内容基于Hou Jie老师的代码示例新增了一些测试

// 理解move的方法：针对move的来源端，在move后就变成了“废物”，后面不会用到了
// 举个例子，对于string来说，是一个指针指向一块内存
// 当执行move后，相当于老的指针没用了，但是分配的内存没必要换了
// 因此直接把新的指针指向这块内存，然后删除老的指针

// http://www.ibm.com/developerworks/cn/aix/library/1307_lisl_c11/
// http://stackoverflow.com/questions/8001823/how-to-enforce-move-semantics-when-a-vector-grows

// 对于nonexcept，可以理解成给编译器打保票不会有exception，这样编译器才愿意使用
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
    bool _print;
    void _init_data(const char* s)
    {
        _data = new char[_len + 1];
        memcpy(_data, s, _len);
        _data[_len] = '\0';  // 最后都以这个结尾
    }

public:
    // default ctor
    MyString(bool print = true) : _data(NULL), _len(0), _print(print) {}

    // ctor
    MyString(const char* p, bool print = true) : _len(strlen(p)), _print(print) { _init_data(p); }

    // copy ctor
    MyString(const MyString& str) : _len(str._len), _print(str._print)
    {
        // print内存位置，主要是为了看在析构时哪里的内存被释放了
        if (_print)
            cout << "Copy Constructor is called! source: " << str._data << " [" << (void*)(str._data) << ']' << endl;
        _init_data(str._data);  // COPY!
    }

    // move ctor, with "noexcept"
    // 注意参数类型，是两个&，代表右值引用符号
    // 加了exception有什么好处？只有加了后才会调用move ctor，否则默认调用copy ctor！
    // 这里的str是右值，但是由于它有名字，所以根据编译器的规则是看作为左值，后面如果转调用是调用左值的函数
    // 假设这里要做正确的转调用，那么必须要用std::forward，而且得做成template的方式！
    MyString(MyString&& str) noexcept : _data(str._data), _len(str._len), _print(str._print)
    {
        if (_print)
            cout << "Move Constructor is called! source: " << str._data << " [" << (void*)(str._data) << ']' << endl;
        // TODO: 为何还要改成default value？对结果没影响？可能只是规范？
        str._len = 0;
        str._print = false;
        // 下面这行特别重要，如果不这么做，会导致多个指针指向同一块内存
        // 在这里具体的体现是，输入的str是一个临时对象，在这个函数调用结束后会做析构，此时就会自动把数据删掉了！
        // 会报错：pointer being freed was not allocated，释放的c++指针没有分配（在程序最后释放另一个指针时报错）
        // 或者直接报错：double free
        str._data = NULL;  // 避免 delete (in dtor)
    }

    // copy assignment
    MyString& operator=(const MyString& str)
    {
        if (str._print)
            cout << "Copy Assignment is called! source: " << str._data << " [" << (void*)(str._data) << ']' << endl;
        // 判断是否是给自己拷贝赋值
        // this定义：谁调用它，this就指向谁，因此这里this指向类的对象（被赋值的）
        // 因此&str同样是取类的对象的地址，可以用来判断是不是自己拷贝给自己
        // 必须有自己检测，不然可能把指向的东西杀掉
        if (this != &str) {
            // 如果有数据先要delete掉，不能直接覆盖，因为大小不一定一样
            if (_data) delete _data;
            _len = str._len;
            _print = str._print;
            _init_data(str._data);  // COPY!
        }
        // 自己赋值的话直接跳过就好
        else {
            cout << "Self Assignment, Nothing to do." << endl;
        }
        return *this;
    }

    // move assignment
    MyString& operator=(MyString&& str) noexcept
    {  // 注意 noexcept
        if (str._print)
            cout << "Move Assignment is called! source: " << str._data << " [" << (void*)(str._data) << ']' << endl;
        if (this != &str) {
            if (_data) delete _data;
            _len = str._len;
            _print = str._print;
            // 一个浅拷贝，没有new新的空间，相当于偷过来了
            _data = str._data;  // MOVE!
            str._len = 0;
            str._print = false;
            // 跟上面的逻辑一样，非常重要
            str._data = NULL;  // 避免 delete (in dtor)
        }
        return *this;
    }

    // dtor
    // 文檔說需 noexcept 但本處無. destructor is noexcept by default.
    virtual ~MyString()
    {
        if (_print) {
            cout << "Destructor is called! "
                 << "source: ";
            if (_data) cout << _data;
            cout << " [" << (void*)(_data) << ']' << endl;
        }

        // 只有当指针不是NULL的时候才delete，做一个doublecheck
        // 与上面的设计配套，保证没问题
        if (_data) {
            delete _data;
        }
    }

    void test() { _init_data(NULL); }

    void print() { cout << "data is " << _data << endl; }
};

// 这个函数是原版代码示例里的
void test301_move_semantics_with_noexcept()
{
    cout << "\n----------------------------------------------------------\n";
    cout << "test301_move_semantics_with_noexcept().......";
    cout << "\n----------------------------------------------------------\n";

    // 做了一个MyString的容器
    vector<MyString> vec;
    // VIP: 加上reserve后会预先分配好容器的容量，从而减少扩容的次数
    // 因此，如果在使用时能知道容器要放的东西总共的数量的话，reserve可以显著提升效率！
    // 容器扩容方法：每次不够时容量都乘以2，这个整体上还是比较合理的，因此在数据少时拷贝的效率也还好
    // vec.reserve(2);
    // without reserve(N);
    // 如果没有reserve，那么capcity初值为0
    cout << "vector capacity is " << vec.capacity() << endl;

    // Move Constructor is called! source: jjhou
    vec.push_back(MyString("jjhou"));
    cout << "after push 1 element, vector capacity is " << vec.capacity() << endl;

    // 这里会出现两次，是因为vector自动进行扩展，包含一次内部拷贝
    // Move Constructor is called! source: sabrina
    // Move Constructor is called! source: jjhou
    vec.push_back(MyString("sabrina"));
    cout << "after push 2 elements, vector capacity is " << vec.capacity() << endl;

    // TODO: 这里会再次进行vector扩展，注意后面的顺序，先jjhou再sabrina
    // Move Constructor is called! source: stacy
    // Move Constructor is called! source: jjhou
    // Move Constructor is called! source: sabrina
    vec.push_back(MyString("stacy"));
    cout << "after push 3 elements, vector capacity is " << vec.capacity() << endl;

    // 这里不需要再进行vector扩展，容量还够
    // Move Constructor is called! source: baby
    vec.push_back(MyString("baby"));
    cout << "after push 4 elements, vector capacity is " << vec.capacity() << endl;

    for (auto& v : vec) {
        v.print();
    }

    // 以上十分好：
    //  1, 以 temp obj.放入容器，編譯器知道那是個 Rvalue, 於是呼叫 move ctor 而非 copy ctor.
    //  2, 當 push_back() 引發 vector 擴展，擴展過程中使用 move 而非 copy.
    cout << "vec.clear() ------- \n";
    vec.clear();

    // exit 前會 delete all existing objects.
}

// 新增的测试：验证move ctor vs. copy ctor在不同容器上的效率
// 以下测试整理自于Hou Jie老师的课件
enum RV { Rvalue, Lvalue };

template <typename Container>
void test_moveable(Container& cntr, long times, RV option)
{
    // 下面三种方法获得的结果是一样的，在下面的get_type_using_decltype函数中测试了
    // decltype的缺点：必须先有一个元素来获得
    typedef typename iterator_traits<typename Container::iterator>::value_type ElemType;
    typedef typename Container::value_type ElemType2;
    // 可以用C++ 11的decltype功能，获得container的数据type
    // 这样就不需要事先知道函数定义，不过看起来不是很elegant？
    auto foo = *(cntr.begin());
    using ElemType3 = decltype(foo);

    char buf[10];

    // clock_t timeStart = clock();
    auto startTime = std::chrono::high_resolution_clock::now();
    for (long i = 0; i < times; ++i) {
        // 拼接随机数变成string，赋值到buf里，每个元素一个数字，可能不满
        snprintf(buf, 10, "%d", rand());  // 随机数
        // cout << buf[8] << endl;
        // 在container的最后insert数据
        auto itr = cntr.end();
        if (Rvalue == option) {
            cntr.insert(itr, ElemType3(buf, false));  // 临时对象
        } else {
            ElemType3 elem(buf, false);
            cntr.insert(itr, elem);
        }
    }
    // cout << "milli-seconds: " << (clock() - timeStart) << endl;
    auto endTime = std::chrono::high_resolution_clock::now();

    // 计算代码段运行的时间
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
    std::cout << "代码段运行了 " << duration.count() << " ms" << std::endl;
}

#define TIMES 100000L
void test301_move_with_nonmove()
{
    cout << "\n----------------------------------------------------------\n";
    cout << "test301_move_with_nonmove()..........";
    cout << "\n----------------------------------------------------------\n";

    MyString str("Hello world!", false);
    // cout << str << endl; // 没有重载cout
    int container_size = 50000;
    {
        cout << "-------------------------------------------" << endl;
        cout << "Container-based right value test" << endl;
        cout << "-------------------------------------------" << endl;

        cout << "\nvector test..." << endl;
        // 注意：这里必须给一个初始化对象，不然会调用默认ctor
        // 之后进行push_back/insert时调用move/copy ctor
        // 相当于给_init_data赋值NULL，从而导致segment fault
        // str.test(); // 可以通过这个小测试来验证以上想法
        std::vector<MyString> vec_MyS(container_size, str);
        test_moveable(vec_MyS, TIMES, Rvalue);
        cout << "container size = " << vec_MyS.size() << endl;

        cout << "\nlist test..." << endl;
        std::list<MyString> lst_MyS(container_size, str);
        test_moveable(lst_MyS, TIMES, Rvalue);
        cout << "container size = " << lst_MyS.size() << endl;

        cout << "\ndeque test..." << endl;
        std::vector<MyString> deq_MyS(container_size, str);
        test_moveable(deq_MyS, TIMES, Rvalue);
        cout << "container size = " << deq_MyS.size() << endl;
    }
    cout << endl;
    {
        cout << "-------------------------------------------" << endl;
        cout << "Container-based left value test" << endl;
        cout << "-------------------------------------------" << endl;

        cout << "\nvector test..." << endl;
        std::vector<MyString> vec_MyS(container_size, str);
        test_moveable(vec_MyS, TIMES, Lvalue);
        cout << "container size = " << vec_MyS.size() << endl;

        cout << "\nlist test..." << endl;
        std::list<MyString> lst_MyS(container_size, str);
        test_moveable(lst_MyS, TIMES, Lvalue);
        cout << "container size = " << lst_MyS.size() << endl;

        cout << "\ndeque test..." << endl;
        std::vector<MyString> deq_MyS(container_size, str);
        test_moveable(deq_MyS, TIMES, Lvalue);
        cout << "container size = " << deq_MyS.size() << endl;
    }
    // TODO: 给一个结果分析
    // 看起来move copy还更慢。。
    // -------------------------------------------
    // Container-based right value test
    // -------------------------------------------

    // vector test...
    // 代码段运行了 24 ms
    // container size = 150000

    // list test...
    // 代码段运行了 15 ms
    // container size = 150000

    // deque test...
    // 代码段运行了 17 ms
    // container size = 150000

    // -------------------------------------------
    // Container-based left value test
    // -------------------------------------------

    // vector test...
    // 代码段运行了 14 ms
    // container size = 150000

    // list test...
    // 代码段运行了 13 ms
    // container size = 150000

    // deque test...
    // 代码段运行了 17 ms
    // container size = 150000
}

// 新增的测试：验证是否可以通过decltype来获得元素类型
template <typename Container>
void get_type_using_decltype(Container& cntr)
{
    // 先获得Iterator，再调用*得到容器的元素
    // 这里会自动调用copy ctor
    // TODO: 但如果vector事先定了capacity，这里却会调用move ctor？
    auto foo = *(cntr.begin());
    cout << "type of container element is " << typeid(foo).name() << endl;

    // 注意：这里只是声明，没有定义！
    decltype(foo) aa;
    cout << "newly createdly element type is " << typeid(aa).name() << endl;

    typedef typename iterator_traits<typename Container::iterator>::value_type ElemType;
    typedef typename Container::value_type ElemType2;
    using new_type = decltype(foo);

    ElemType bb;
    cout << "ElemType type is " << typeid(bb).name() << endl;
    ElemType2 bb1;
    cout << "ElemType2 type is " << typeid(bb1).name() << endl;

    new_type cc;
    cout << "Using decltype type is " << typeid(cc).name() << endl;
}

void test301_moveable_decltype()
{
    cout << "\n----------------------------------------------------------\n";
    cout << "test301_moveable_decltype()..........";
    cout << "\n----------------------------------------------------------\n";

    MyString str("Hello world!", true);
    // 这里的原因同样必须给一个初始化对象，不然会报segment fault
    // 与test301_move_with_nonmove里的原因一致
    std::vector<MyString> vec_MyS(10, str);
    vec_MyS.push_back(str);

    get_type_using_decltype(vec_MyS);
}

}  // namespace jj301

//----------------------------------------------------
// Type Alias
//----------------------------------------------------
namespace jj48 {
//-----------
// http://en.cppreference.com/w/cpp/language/type_alias
// 功能上与typedef相同，下面是函数名的alias
// 在C++1.0时写作：typedef void (*func)(int, int);
// 但是以上写法不容易看清，下面这样写就比较清楚了
using func = void (*)(int, int);

// 下面创建了对象，本质是一个函数指针
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

//-----------
// 模板类里也可以用这个
// TODO: 有什么实际场景的用处？
// type alias can introduce a member typedef name
template <typename T>
struct Container {
    // 相当于类里有一个成员叫做value_type
    using value_type = T;  // same as typedef T value_type;
};
// which can be used in generic programming
template <typename Container>
void fn2(const Container& c)
{
    // 使用typename是为了告诉编译器后面的表示类型，而不是变量
    // typename Container::value_type n; // 原版

    // 下面是新加的测试，会用到隐式类型转换
    typename Container::value_type n = 1;  // n = 1.5
    cout << "value is " << n << endl;
}

//-----------
void test48_type_alias()
{
    cout << "\n----------------------------------------------------------\n";
    cout << "test48_type_alias().......";
    cout << "\n----------------------------------------------------------\n";

    Container<int> c;
    fn2(c);  // Container::value_type will be int in this function
}
}  // namespace jj48

//----------------------------------------------------
// Hash
//----------------------------------------------------
namespace jj50 {
void test50_hash()
{
    cout << "\n----------------------------------------------------------\n";
    cout << "test50_hash()..........";
    cout << "\n----------------------------------------------------------\n";

    void* pi = (void*)(new int(100));
    // 第一个()是创建对象，第二个()是输入函数参数
    cout << hash<void*>()(pi) << endl;
    cout << hash<int>()(123) << endl;    // 123
    cout << hash<long>()(123L) << endl;  // 123
    cout << hash<string>()(string("ssss")) << endl;
    cout << hash<const char*>()("sss") << endl;
    cout << hash<char>()('s') << endl;       // ASIC码, 115
    cout << hash<float>()(3.1415) << endl;   // 没有定义不知道怎么算得
    cout << hash<double>()(3.1415) << endl;  // 没有定义不知道怎么算得
}

}  // namespace jj50

//----------------------------------------------------
// Push_back vs Emplace_back
//----------------------------------------------------
namespace yj01 {
// TODO: emplace_back输入是universal reference，再调用move处理可变成rvalue
// push_back有两种重载函数，右值输入内部直接调用到emplace_back函数
class President {
public:
    President(string p_name, int p_year) : name(p_name), year(p_year) { cout << "ctor is called" << endl; }

    President(const President& other) : name(other.name), year(other.year) { cout << "copy ctor is called" << endl; }

    // 注意：只有在使用noexcept的时候，vector扩展才会调用move ctor
    // 但是与initializer list中针对输入参数是否使用std::move无关
    // 为何要用std::move呢？
    // 这个的答案很有意思，分成两个层面来回答（目前的想法）：
    // 1. other在内容上是右值，但是由于它有参数名，所以在编译器的规则下是左值，所以为了让编译器认识需要加上move
    // 2. 由于数据类型是string，必须要用move才会真正调用move操作以提升效率，不然还是copy（待验证）
    President(President&& other) noexcept : name(move(other.name)), year(other.year)
    // President(President&& other) noexcept : name(other.name), year(other.year)
    {
        cout << "move ctor is called" << endl;
    }

    President& operator=(const President& other) = default;

    void print() { cout << "name is " << name << ", year is " << year << endl; }

private:
    string name;
    int year;
};

void test01_emplace_back()
{
    cout << "\n----------------------------------------------------------\n";
    cout << "test01_emplace_back()..........";
    cout << "\n----------------------------------------------------------\n";

    vector<President> elections;
    elections.reserve(10);
    cout << "...push_back left value..." << endl;
    President p1("Franklin", 1936);  // ctor is called
    President p2("Nelson", 1994);    // ctor is called
    President p3("Billy", 2030);     // ctor is called
    elections.push_back(p1);         // copy ctor is called
    cout << ".." << endl;
    elections.push_back(p2);  // copy ctor is called
    cout << ".." << endl;
    elections.push_back(p3);  // copy ctor is called
    cout << "...push_back right value..." << endl;
    elections.push_back(President("Franklin", 1936));  // ctor is called & move ctor is called
    cout << ".." << endl;
    elections.push_back(President("Nelson", 1994));  // ctor is called & move ctor is called

    // print result
    // 注意：一定要用auto&，不然这里会调用拷贝构造函数！
    for (auto& p : elections) p.print();

    vector<President> elections1;
    cout << "...emplace_back left value..." << endl;
    elections1.reserve(10);
    President q1("Franklin", 1936);  // ctor is called
    elections1.emplace_back(q1);     // copy ctor is called
    q1.print();

    cout << "...emplace_back right value..." << endl;
    elections1.emplace_back(President("Nelson", 1994));  // ctor is called & move ctor is called
    // TODO: 只有按照下面的形式输入参数，可以让每次emplace back都少了一次move ctor，原因是什么？
    // 自动调用构造函数，且直接使用container里事先分配好的内存，而不是重新分配内存？
    elections1.emplace_back("Franklin", 1936);  // ctor is called
    cout << ".." << endl;
    elections1.emplace_back("Nelson", 1994);  // ctor is called

    for (auto& p : elections1) p.print();
}

}  // namespace yj01

//----------------------------------------------------
// Move Constructor
//----------------------------------------------------
namespace yj02 {
// 注意：这部分代码参考了Hou Jie老师的MyString部分的代码
// 旨在确定move ctor具体实现中对哪些类型的数据需要加move，哪些不需要
class SomeDataStructure {
public:
    SomeDataStructure() { cout << " Base::Base()" << endl; }
    ~SomeDataStructure() { cout << " Base::~Base()" << endl; }
};

class MyObject {
private:
    char* _data;
    size_t _len;
    string _str;
    // composition by reference
    shared_ptr<SomeDataStructure> _ptr;

    void _init_data(const char* s)
    {
        _data = new char[_len + 1];
        memcpy(_data, s, _len);
        _data[_len] = '\0';  // 最后都以这个结尾
    }

public:
    // default ctor
    MyObject() : _data(NULL), _len(0), _str("ctor") { _ptr = make_shared<SomeDataStructure>(); }

    // ctor
    MyObject(const char* p) : _len(strlen(p)), _str("ctor"), _ptr(nullptr)
    {
        _init_data(p);
        _ptr = make_shared<SomeDataStructure>();
    }

    // copy ctor
    MyObject(const MyObject& obj) : _len(obj._len), _str(obj._str), _ptr(obj._ptr)
    {
        cout << "Copy Constructor is called! source: " << obj._data << " [" << (void*)(obj._data) << ']' << endl;
        _init_data(obj._data);  // COPY!
    }

    // move ctor, with "noexcept"
    // 问题：string是怎么实现move的？为何不需要像char*那样将指针赋值为NULL？
    // 实现的方法参考jj301里定义的MyString，内部包含了针对指针的赋值
    // MyObject(MyObject&& obj) noexcept : _data(move(obj._data)), _len(move(obj._len)), _str(move(obj._str))
    // 给一个cpp reference上的高级写法，exchange是给obj._len赋值为0，然后return obj._len的old value给_len
    // TODO: 结论是数据类型中包含指针的（例如string, smart pointer）就需要用move，而其他的都直接用exchange？
    MyObject(MyObject&& obj) noexcept
        : _data(obj._data), _len(exchange(obj._len, 0)), _str(move(obj._str)), _ptr(move(obj._ptr))
    {
        cout << "Move Constructor is called! source: " << obj._data << " [" << (void*)(obj._data) << ']' << endl;
        cout << "obj._str is " << obj._str << ", _str is " << _str << endl;
        cout << "obj._ptr is " << obj._ptr << ", _ptr is " << _ptr << endl;
        // 由于用了exchange，所以这里不用再赋值了
        // TODO: 假设没用exchange，不对obj._len赋值也不会有问题？
        // obj._len = 0;
        obj._data = NULL;  // 避免 delete (in dtor)
    }

    // copy assignment
    MyObject& operator=(const MyObject& obj)
    {
        cout << "Copy Assignment is called! source: " << obj._data << " [" << (void*)(obj._data) << ']' << endl;
        if (this != &obj) {
            if (_data) delete _data;
            _len = obj._len;
            _str = obj._str;
            _ptr = obj._ptr;
            _init_data(obj._data);  // COPY!
        } else {
            cout << "Self Assignment, Nothing to do." << endl;
        }
        return *this;
    }

    // move assignment
    MyObject& operator=(MyObject&& obj) noexcept
    {
        cout << "Move Assignment is called! source: " << obj._data << " [" << (void*)(obj._data) << ']' << endl;
        if (this != &obj) {
            if (_data) delete _data;
            // 同理可以用上新特性
            _len = exchange(obj._len, 0);
            // _len = obj._len;
            // 大概率是这样实现move assignment
            _str = move(obj._str);
            _ptr = move(obj._ptr);
            // 一个浅拷贝，没有new新的空间，相当于偷过来了
            _data = obj._data;  // MOVE!
            // obj._len = 0;
            // 跟上面的逻辑一样，非常重要
            obj._data = NULL;  // 避免 delete (in dtor)
        }
        return *this;
    }

    // dtor
    // 文檔說需 noexcept 但本處無. destructor is noexcept by default.
    virtual ~MyObject()
    {
        cout << "Destructor is called! "
             << "source: ";
        if (_data) cout << _data;
        cout << " [" << (void*)(_data) << ']' << endl;

        // 只要当指针不是NULL的时候才delete，做一个doublecheck
        // 与上面的设计配套，保证没问题
        if (_data) {
            delete _data;
        }
    }

    void print() { cout << "data is " << _data << ", str is " << _str << endl; }
};

void test02_move_constructor()
{
    cout << "\n----------------------------------------------------------\n";
    cout << "test02_move_constructor().......";
    cout << "\n----------------------------------------------------------\n";

    // 做了一个MyObject的容器
    vector<MyObject> vec;
    // 如果没有reserve，那么capcity初值为0
    cout << "vector capacity is " << vec.capacity() << endl;

    // Move Constructor is called! source: jjhou
    // Destructor is called! [0]
    vec.push_back(MyObject("jjhou"));
    cout << "after push 1 element, vector capacity is " << vec.capacity() << endl;

    for (auto& v : vec) {
        v.print();
    }

    // 以上十分好：
    //  1, 以 temp obj.放入容器，編譯器知道那是個 Rvalue, 於是呼叫 move ctor 而非 copy ctor.
    //  2, 當 push_back() 引發 vector 擴展，擴展過程中使用 move 而非 copy.
    cout << "vec.clear() ------- \n";
    vec.clear();
}

}  // namespace yj02

//----------------------------------------------------
// Universal Reference
//----------------------------------------------------
namespace yj03 {
// 注：此段代码整理自Hou Jie老师的讲义
// 整理了支持copy+move，只支持copy和只支持move三种情况
namespace case1 {
class CopyMove {
public:
    CopyMove() { cout << "CopyMove ctor" << endl; }
    ~CopyMove() { cout << "CopyMove dtor" << endl; }

    // TODO: copy ctor的写法与普通的不一样？
    CopyMove(CopyMove const&) { cout << "CopyMove copy ctor" << endl; }
    CopyMove& operator=(CopyMove const&)
    {
        cout << "CopyMove copy assignment" << endl;
        return *this;
    }
    // TODO: 为何不用加noexcept?
    CopyMove(CopyMove&&) { cout << "CopyMove move ctor" << endl; }
    CopyMove& operator=(CopyMove&&)
    {
        cout << "CopyMove move assignment" << endl;
        return *this;
    }
};

// 调用函数列表
void f1(CopyMove) { cout << "f1(), pass by value" << endl; }
void f2(CopyMove&) { cout << "f2(), pass by reference" << endl; }
void f3(const CopyMove&) { cout << "f3(), pass by reference to const" << endl; }
void f4(CopyMove&&) { cout << "f4(), pass by R-value reference" << endl; }
void f5(const CopyMove&&) { cout << "f5(), pass by R-value reference to const" << endl; }
template <typename T>
void f6(T&&)
{
    cout << "f6(), pass by universal reference" << endl;
}
}  // namespace case1

namespace case2 {
class CopyOnly {
public:
    CopyOnly() { cout << "CopyOnly ctor" << endl; };
    ~CopyOnly() { cout << "CopyOnly dtor" << endl; };

    CopyOnly(CopyOnly const&) { cout << "CopyOnly copy ctor" << endl; };
    CopyOnly& operator=(CopyOnly const&)
    {
        cout << "CopyOnly copy assignment" << endl;
        return *this;
    }
    // 通过使用delete禁用move相关的函数
    CopyOnly(CopyOnly&&) = delete;
    CopyOnly& operator=(CopyOnly&&) = delete;
};

// 调用函数列表
void f1(CopyOnly) { cout << "f1(), pass by value" << endl; }
void f2(CopyOnly&) { cout << "f2(), pass by reference" << endl; }
void f3(const CopyOnly&) { cout << "f3(), pass by reference to const" << endl; }
void f4(CopyOnly&&) { cout << "f4(), pass by R-value reference" << endl; }
void f5(const CopyOnly&&) { cout << "f5(), pass by R-value reference to const" << endl; }
template <typename T>
void f6(T&&)
{
    cout << "f6(), pass by universal reference" << endl;
}

}  // namespace case2

namespace case3 {
class MoveOnly {
public:
    MoveOnly() { cout << "MoveOnly ctor" << endl; };
    ~MoveOnly() { cout << "MoveOnly dtor" << endl; };

    // 通过使用delete禁用copy相关的函数
    MoveOnly(MoveOnly const&) = delete;
    MoveOnly& operator=(MoveOnly const&) = delete;
    MoveOnly(MoveOnly&&) { cout << "MoveOnly move ctor" << endl; };
    MoveOnly& operator=(MoveOnly&&)
    {
        cout << "MoveOnly move assignment" << endl;
        return *this;
    }
};

// 调用函数列表
void f1(MoveOnly) { cout << "f1(), pass by value" << endl; }
void f2(MoveOnly&) { cout << "f2(), pass by reference" << endl; }
void f3(const MoveOnly&) { cout << "f3(), pass by reference to const" << endl; }
void f4(MoveOnly&&) { cout << "f4(), pass by R-value reference" << endl; }
void f5(const MoveOnly&&) { cout << "f5(), pass by R-value reference to const" << endl; }
template <typename T>
void f6(T&&)
{
    cout << "f6(), pass by universal reference" << endl;
}
}  // namespace case3

// 具体的测试
void test03_universal_reference()
{
    cout << "\n----------------------------------------------------------\n";
    cout << "test03_universal_reference()..........";
    cout << "\n----------------------------------------------------------\n";

    {
        using namespace case1;
        cout << "-------------------------------------------" << endl;
        cout << "CopyMove test" << endl;
        cout << "-------------------------------------------" << endl;

        CopyMove myCopyMove;  // ctor
        f1(myCopyMove);       // copy ctor + f1 + dtor
        f2(myCopyMove);       // f2
        f3(myCopyMove);       // f3

        // 左值value可以接受右值的输入
        f1(move(myCopyMove));  // move ctor + f1 + dtor
        // 左值reference不可以接受右值的输入
        // 编译报错： cannot bind non-const lvalue reference to an rvalue
        //! f2(move(myCopyMove));
        // 左值const reference可以接受右值的输入
        f3(move(myCopyMove));  // f3

        // 右值reference和右值const reference都不接受左值的输入
        // 编译报错： cannot bind rvalue reference to lvalue
        //! f4(myCopyMove);
        //! f5(myCopyMove);
        // 正常使用时内部不涉及到数据拷贝和类型转换
        f4(move(myCopyMove));  // f4
        // TODO： 用不用const对结果没影响？一般的规范怎么要求？
        f5(move(myCopyMove));  // f5

        // 这里同理，只是会构造一个临时对象
        f4(CopyMove());  // ctor + f4 + dtor
        f5(CopyMove());  // ctor + f5 + dtor

        // universal reference什么都可以接收
        // 注意这里不会调用到任何函数，包括copy ctor和move ctor
        f6(myCopyMove);  // f6
        // 注意这里不会调用到任何函数，包括copy ctor和move ctor
        f6(move(myCopyMove));  // f6
        f6(CopyMove());        // ctor + f6 + dtor

        cout << endl;
    }

    {
        using namespace case2;
        cout << "-------------------------------------------" << endl;
        cout << "CopyOnly test" << endl;
        cout << "-------------------------------------------" << endl;

        CopyOnly myCopyOnly;  // ctor
        f1(myCopyOnly);       // copy ctor + f1 + dtor
        f2(myCopyOnly);       // f2
        f3(myCopyOnly);       // f3

        // 删除move ctor后左值value就接受不了右值了
        // 编译报错：use of deleted function (move ctor)
        //! f1(move(myCopyOnly));
        // 左值reference本来就不能接受右值，跟上面一样
        // 编译报错： cannot bind non-const lvalue reference to an rvalue
        //! f2(move(myCopyOnly));
        // 左值const reference仍然可以接受右值的输入
        // TODO: 说明里面包含一个隐式类型转换！！！
        f3(move(myCopyOnly));  // f3

        // 下面这一趴跟上面都一样
        //! f4(myCopyOnly);
        //! f5(myCopyOnly);
        f4(move(myCopyOnly));  // f4
        f5(move(myCopyOnly));  // f5

        f4(CopyOnly());  // ctor + f4 + dtor
        f5(CopyOnly());  // ctor + f5 + dtor

        // 都可以接收
        f6(myCopyOnly);        // f6
        f6(move(myCopyOnly));  // f6
        f6(CopyOnly());        // ctor + f6 + dtor

        cout << endl;
    }

    {
        using namespace case3;
        cout << "-------------------------------------------" << endl;
        cout << "MoveOnly test" << endl;
        cout << "-------------------------------------------" << endl;

        MoveOnly myMoveOnly;  // ctor
        // 由于copy ctor被人为禁用，所以这里就不能用了
        // 编译报错：use of deleted function (copy ctor)
        //! f1(myMoveOnly);
        f2(myMoveOnly);  // f2
        f3(myMoveOnly);  // f3

        // 这里相当于用给定的move ctor修改了数据类型
        f1(move(myMoveOnly));  // move ctor + f1 + dtor
        // 编译报错：cannot bind non-const lvalue reference to an rvalue
        //! f2(move(myMoveOnly));
        f3(move(myMoveOnly));  // f3

        // 这里依然和上面同理
        //! f4(myMoveOnly);
        //! f5(myMoveOnly);
        f4(move(myMoveOnly));  // f4
        f5(move(myMoveOnly));  // f5

        f4(MoveOnly());  // ctor + f4 + dtor
        f5(MoveOnly());  // ctor + f5 + dtor

        // 都可以接收
        f6(myMoveOnly);        // f6
        f6(move(myMoveOnly));  // f6
        f6(MoveOnly());        // ctor + f6 + dtor

        cout << endl;
    }
}

}  // namespace yj03

//---------------------------------------------------
int main(int argc, char** argv)
{
    cout << "c++ version " << __cplusplus << endl;

    // jj03::test03_initializer_list();

    // jj06::test06_lambda();

    // jj12::test12_Rvalue_Move();

    // jj15::test15_variadic_template();

    // jj301::test301_move_semantics_with_noexcept();

    jj301::test301_move_with_nonmove();

    // jj301::test301_moveable_decltype();  // 一个小测试学习decltype的，内含在test301_move_with_nonmove里了

    // jj48::test48_type_alias();

    // jj50::test50_hash();

    // yj01::test01_emplace_back();

    // yj02::test02_move_constructor();

    // yj03::test03_universal_reference();
}