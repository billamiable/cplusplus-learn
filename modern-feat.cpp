// author: Yujie Wang & Jie Hou
// Rearrage the code according to the course order
// Incorporate my own implementation and thoughts

#include <iostream>
#include <vector>
#include <string>
#include <bitset>
#include <memory>
#include <complex>
using namespace std;

//----------------------------------------------------
// Variadic Templates
//----------------------------------------------------
namespace jj15
{
// 这里的内容包含了最开始和专门针对variadic template来讲的两块内容

//~~~~~~~~~~~~~~~
// case4
// recursive function calling.
namespace case4
{
// 这个是作为stopping criterion，非常重要！
// 用来在recursive call的最后一轮用的
void printX()
{
}

template <typename T, typename... Types>
void printX(const T& firstArg, const Types&... args)
{
    // 实际使用的时候需要保证可以cout
    cout << firstArg << endl; // print first argument
    printX(args...);          // recursive call for remaining arguments
}

}

//~~~~~~~~~~~~~~~
// case3
// 用variadic template重写printf
namespace case3
{
// http://stackoverflow.com/questions/3634379/variadic-templates
// 最后一个和一堆都没了，就只剩最开始的字符串
void printf(const char *s)
{
    // 这个是stopping criterion
    // 模拟c的print方法，发现数量不匹配，输出错误
    while (*s)
    {
        if (*s == '%' && *(++s) != '%')
            throw std::runtime_error("invalid format string: missing arguments");
        std::cout << *s++ << "~end~"; // 打印空格和换行符
    }
    std::cout << std::endl;
}

template<typename T, typename... Args>
void printf(const char* s, T value, Args... args)
{
    while (*s)
    {
        if (*s == '%' && *(++s) != '%')
        {
            std::cout << value;
            // 用++s的原因是后面有个空格需要跳过
            printf(++s, args...);
            return;
        }
        // TODO: 打印空格，这就是递归的后手，我常常不能理解的
        std::cout << *s++;
    }
    throw std::logic_error("extra arguments provided to printf");
}

}

//~~~~~~~~~~~~~~~
// case1
// 检测数据里最大的（initializer_list实现）
namespace case1
{
// ...\4.9.2\include\c++\bits\predefined_oops.h
// 最终比大小用到的仿函数
struct _Iter_less_iter
{
    // 本质是一个仿函数，做一个比较
    template<typename _Iterator1, typename _Iterator2>
        bool
        operator()(_Iterator1 __it1, _Iterator2 __it2) const
        { return *__it1 < *__it2; }
};

// 生成仿函数的对象，用在下下个代码块里
inline _Iter_less_iter
__iter_less_iter()
{ return _Iter_less_iter(); }

// 具体的实现，如何与所有的值做比较，存下来最大的那个
// ...\4.9.2\include\c++\bits\stl_algo.h
template<typename _ForwardIterator, typename _Compare>
    _ForwardIterator
    __max_element(_ForwardIterator __first, _ForwardIterator __last,
                  _Compare __comp)
    {
        if (__first == __last) return __first;
        _ForwardIterator __result = __first;
        // 很经典的比大小存下来最大的
        while (++__first != __last) {
            if (__comp(__result, __first))
                __result = __first;
        }
        return __result;
    }

// initializer_list的max函数的第一层调用实现
template<typename _ForwardIterator>
    inline _ForwardIterator
    max_element(_ForwardIterator __first, _ForwardIterator __last)
    {
        return __max_element(__first, __last,
                             __iter_less_iter());
    }

// 在输入数据阶段{}会自动生成initializer_list，然后调用里面的max函数
template<typename _Tp>
    inline _Tp
    max(initializer_list<_Tp> __l)
    { return *max_element(__l.begin(), __l.end()); }

}

//~~~~~~~~~~~~~~~
// case2
// 检测数据里最大的（直接variadic template recursive实现）
namespace case2
{
// http://stackoverflow.com/questions/3634379/variadic-templates
int maximum(int n)
{
    return n;
}

template<typename... Args>
int maximum(int n, Args... args)
{
    return std::max(n, maximum(args...));
}
}

//~~~~~~~~~~~~~~~
// case5
// 以非一般的方式处理第一个和最后一个元素
// refer to test07_tuples()


//~~~~~~~~~~~~~~~
// case6
// recursive inheritance
namespace case6
{

}

//~~~~~~~~~~~~~~~
// case7
// recursive composition
namespace case7
{

}

//-----------
void test15_variadic_template()
{
    cout << "\n----------------------------------------------------------\n";
    cout << "test15_variadic_template().......... ";
    cout << "\n----------------------------------------------------------\n";

    // case4
    cout << "\n.....case4..........\n";
    case4::printX(7.5, "hello", bitset<16>(377), 42);

    //case3
    cout << "\n.....case3..........\n";
	int* pi = new int; // 只是为了打印多样性
    // 15 This is Ace. 0x7fb5aa403430 3.14159
	case3::printf("%d %s %p %f \n", 15, "This is Ace.", pi, 3.141592653);

    // case1
    cout << "\n.....case1..........\n";
    cout << case1::max( { 57, 48, 60, 100, 20, 18} ) << endl; //100

    // case2
    cout << "\n.....case2..........\n";
	cout << case2::maximum(57, 48, 60, 100, 20, 18) << endl; // 100
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
    cout << "\n----------------------------------------------------------\n";
    cout << "test48_type_alias().......";
    cout << "\n----------------------------------------------------------\n";

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
        // print的内容是内存位置？
        cout << "Copy Constructor is called! source: " << str._data << " [" << (void*)(str._data) << ']' << endl;
        _init_data(str._data); 	//COPY!
    }

    // move ctor, with "noexcept"
    MyString(MyString&& str) noexcept : _data(str._data), _len(str._len) {
        cout << "Move Constructor is called! source: " << str._data << " [" << (void*)(str._data) << ']' << endl;
        // 这里的数据复制在上面一行里了，下面只是防止两个指针指向同一个位置，需要删除
        // TODO: move的核心逻辑有待学习
        str._len = 0;
        str._data = NULL; // 避免 delete (in dtor)？
    }

    // copy assignment
    MyString& operator=(const MyString& str) {
        cout << "Copy Assignment is called! source: " << str._data << " [" << (void*)(str._data) << ']' << endl;
        // 判断是否是给自己拷贝赋值
        // TODO: 具体写法再学习下
        // this表示指向类的指针，那为啥输入数据的地址会相等呢？
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
    MyString& operator=(MyString&& str) noexcept { // 注意 noexcept
        cout << "Move Assignment is called! source: " << str._data << " [" << (void*)(str._data) << ']' << endl;
        if (this != &str) {
            if (_data) delete _data;
            _len = str._len;
            // 明显这里就不是调用init_data函数来创建的
            // 看起来是一个浅拷贝，没有new新的空间，那感觉还是不懂啊？
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
    cout << "\n----------------------------------------------------------\n";
    cout << "test301_move_semantics_with_noexcept_and_swap().......";
    cout << "\n----------------------------------------------------------\n";

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


//----------------------------------------------------
// Lambda
//----------------------------------------------------
namespace jj06
{
// TODO: function是啥？
// 其实可以用另一个写法来实现一样的功能
function<int(int, int)> returnLambda ()
{
    return [] (int x, int y) {
        return x*y;
    };
}

void test06_lambda()
{
    cout << "\n----------------------------------------------------------\n";
    cout << "test06_lambda()..................";
    cout << "\n----------------------------------------------------------\n";

    auto lf = returnLambda();
    // lf()创建了lambda的对象
    cout << lf(9,8) << endl;  //72

    // TODO: 这里没看明白在干啥？？
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
    } (); // 这里()直接创建对象，就print了，一般很少这样写

    {
        int id = 0;
        // id是外部变量，取值
        // 若無 mutable ==> [Error] increment of read-only variable 'id'
        // id 變化不影響 outer scope (除非傳入的是 reference)
        auto f = [id] () mutable {
            cout << "id: " << id << endl;
            ++id;
        };
        // 这个实验挺有趣
        id = 42;
        f(); //id: 0
        f(); //id: 1
        f(); //id: 2
        cout << id << endl; // 42，没改合理

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
        auto f = [&id] (int param) {
            cout << "id: " << id << endl;
            ++id; // 沒有 mutable 也能 ++
            ++param; // 和 mutable 無關
        };
        id = 42;
        f(7); //id: 42，因为reference不会创建新的，f()在id=42后面，所以已经变了
        f(7); //id: 43
        f(7); //id: 44
        cout << id << endl; // 45，改了合理
    }

    // 给一个条件，大于30小于100拿掉
    vector<int> vi {5,28,50,83,70,590,245,59,24};
    int x = 30;
    int y = 100;
    vi.erase(remove_if(vi.begin(),
                       vi.end(),
                       [x, y](int n) { return x<n && n<y; }
                       ),
             vi.end()
            );
    for (auto i : vi)
        cout << i << ' '; //5 28 590 245 24
    cout << endl;

}
}


//----------------------------------------------------
// Rvalue Move
//----------------------------------------------------
namespace jj12
{
void test12_Rvalue_Move()
{
    cout << "\n----------------------------------------------------------\n";
    cout << "test12_Rvalue_Move()..........";
    cout << "\n----------------------------------------------------------\n";

    string s1("aaaa");
    string s2("bbbb");
    string s = s1 + s2;

    cout << "s: "  << s  << endl; // aaaabbbb
    cout << "s1: " << s1 << endl; // aaaa
    cout << "s2: " << s2 << endl; // bbbb

    // TODO: 这是为啥？？而且只有s1没了？？
    s = move(s1) + move(s2);
    cout << "s: "  << s  << endl; // aaaabbbb
    cout << "s1: " << s1 << endl; //
    cout << "s2: " << s2 << endl; // bbbb

    //----------------

    int x = 4;
    int y = 8;
    //! x + y = 10; // [Error] lvalue required as left operand of assignment

    //----------------

    s1 = "Hello ";
    // 这里也没理解是啥逻辑？
    s1 + s2 = s2; // 竟然可以通過編譯，作者自己违反了规则
    cout << "s1: " << s1 << endl;	// s1: Hello
    cout << "s2: " << s2 << endl;	// s2: bbbb
    string() = "World"; // 對 temp obj 賦值可以，作者自己违反了规则

    complex<int> c1(2,3), c2(4,5);
    c1 + c2 = complex<int>(6,9); // 感觉没用？？
    cout << "c1: " << c1 << endl;	// c1: (2,3)
    cout << "c2: " << c2 << endl;	// c2: (4,5)
    complex<int>() = complex<int>(6,9);	// 對 temp obj 賦值可以，作者自己违反了规则
}

}

//---------------------------------------------------
int main(int argc, char** argv) 
{
    cout << "c++ version " << __cplusplus << endl;

    jj15::test15_variadic_template();

    jj48::test48_type_alias();

    jj301::test301_move_semantics_with_noexcept();

    jj06::test06_lambda();

    jj12::test12_Rvalue_Move();
}