// author : Hou Jie (侯捷)
// date : 2015/11/05
// compiler : DevC++ 5.11 (MinGW with GNU 4.9.9.2)
//
// 說明：這是侯捷 E-learning video "C++2.0新特性" 的實例程式.
// 該課程的所有測試都出現在此.
// 每一個小測試單元都被放進一個 namespace 中,
// 如此保持各單元之間的最大獨立性.
//
// 本文件用到許多 C++ 2.0 New Features，所以你必須在你的集成環境 (IDE) 上設定 "C++11 on".
// 如果你的編譯器報錯，應是因為我用到 (測試) "GNU標準庫" 的獨特 (非標準) 組件，
// 將報錯語句改為註解 (comments) 應可通過編譯.

#include <iostream>
#include <vector>
#include <string>
#include <complex>
#include <tuple>
#include <ratio>
#include <array>
#include <forward_list>
#include <unordered_set>
#include <functional>
#include <initializer_list>
#include <ctime>
#include <bitset>
#include <deque>
#include <set>
#include <cstring>  //strlen()
#include <type_traits>
#include <list>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <stack>
#include <queue>
#include <memory>
#include <algorithm>  //sort, reverse_copy
#include <iterator>   //ostream_iterator
using namespace std;

//----------------------------------------------------
namespace jj00 {

void f(int) { cout << "f(int) be called" << endl; }
void f(void*) { cout << "f(void*) be called" << endl; }

void test00_nullptr()
{
    cout << "\ntest00_nullptr()..................\n";

    // nullptr and std::nullptr_t
    f(0);  // calls f(int)
    //!	f(NULL);	//calls f(int) if NULL is 0, ambiguous otherwise   	//[Warning] passing NULL to non-pointer
    //!argument 1 of 'void f(int)' [-Wconversion-null]
    f(nullptr);  // calls f(void*)
}

}  // namespace jj00
//----------------------------------------------------
namespace jj01 {
double func(){};

void test01_auto()
{
    cout << "\ntest01_auto()..................\n";

    // auto declaration
    auto i = 42;      // before C++11 => [Error] 'i' does not name a type
    auto d = func();  // d has type double

    vector<string> vs;
    auto pos = vs.begin();  // pos has type vector<string>::iterator

    auto l = [](int x) -> bool {  // l has the type of a lambda which
                                  // taking an int and returning a bool
    };
}
}  // namespace jj01
//----------------------------------------------------
namespace jj02 {
void test02_uniform_initializer()
{
    cout << "\ntest02_uniform_initializer()..................\n";

    // uniform initializer
    int values[]{1, 2, 3};
    // before C++11 => [Warning] extended initializer lists only available with -std=c++11 or -std=gnu++11

    vector<int> v{2, 3, 5, 7, 11, 13, 17};
    vector<string> cities{"Berlin", "New York", "London", "Braunschweig", "Cairo", "Cologne"};
    std::complex<double> c{4.0, 3.0};  // equivalent to c(4.0,3.0)
}
}  // namespace jj02
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
    for (auto p = vals.begin(); p != vals.end(); ++p) {  // process a list of values
        cout << *p << ' ';
    }
    cout << endl;
}

void test03_initializer_list()
{
    cout << "\ntest03_initializer_list()..................\n";

    print({12, 3, 5, 7, 11, 13, 17});  // pass a list of values to print()

    // initializer lists
    int i;                                                  // i has undefined value
    int j{};                                                // j is initialized by 0
    int* p;                                                 // p has undefined value
    int* q{};                                               // q is initialized by nullptr
    cout << i << " " << j << ' ' << p << ' ' << q << endl;  // 1  0  0x405cc4  0

    int x1(5.3);   // OK,but OUCH: x1 becomes 5
    int x2 = 5.3;  // OK,but OUCH: x2 becomes 5
    //!	int x3{5.0};	//ERROR: narrowing
    //[Warning] narrowing conversion of '5.0e+0' from 'double' to 'int' inside { } [-Wnarrowing]
    //!	int x4={5.3};	//ERROR: narrowing
    //[Warning] narrowing conversion of '5.2999999999999998e+0' from 'double' to 'int' inside { } [-Wnarrowing]
    char c1{7};  // OK: even though 7 is an int,this is not narrowing
    //!	char c2{99999};	//ERROR: narrowing (if 99999 doesn't into a char)
    //[Warning] narrowing conversion of '99999' from 'int' to 'char' inside { } [-Wnarrowing]
    //[Warning] overflow in implicit constant conversion [-Woverflow]
    //	cout << x1 << " " << x2 << ' ' << x3 << ' ' << x4 << ' ' << c1 << ' ' << c2 << endl;  	//5 5 5 5

    vector<int> v1{1, 2, 4, 5};      // OK
    vector<int> v2{1, 2.3, 4, 5.6};  // ERROR: narrowing doubles to ints
                                     //[Warning] narrowing conversion of '2.2999999999999998e+0' from 'double' to 'int'
                                     // inside { } [-Wnarrowing] [Warning] narrowing conversion of
                                     //'5.5999999999999996e+0' from 'double' to 'int' inside { } [-Wnarrowing]
    for (auto& elem : v2) {
        cout << elem << ' ';  // 1 2 4 5
    }
    cout << endl;

    {
        P p(77, 5);      // P(int, int), a=77, b=5
        P q{77, 5};      // P(initializer_list<int>), values= 77 5
        P r{77, 5, 42};  // P(initializer_list<int>), values= 77 5 42
        P s = {77, 5};   // P(initializer_list<int>), values= 77 5
    }

    {
        vector<int> v1{2, 5, 7, 13, 69, 83, 50};
        vector<int> v2({2, 5, 7, 13, 69, 83, 50});
        vector<int> v3;
        v3 = {2, 5, 7, 13, 69, 83, 50};
        v3.insert(v3.begin() + 2, {0, 1, 2, 3, 4});

        for (auto i : v3) cout << i << ' ';
        cout << endl;  // 2 5 0 1 2 3 4 7 13 69 83 50

        cout << max({string("Ace"), string("Stacy"), string("Sabrina"), string("Barkley")});  // Stacy
        cout << min({string("Ace"), string("Stacy"), string("Sabrina"), string("Barkley")});  // Ace
        cout << max({54, 16, 48, 5});                                                         // 54
        cout << min({54, 16, 48, 5});                                                         // 5
    }
}
}  // namespace jj03
//----------------------------------------------------
namespace jj04 {
struct Complex1 {
    int real, imag;

    Complex1(int re, int im = 0) : real(re), imag(im) {}

    Complex1 operator+(const Complex1& x) { return Complex1((real + x.real), (imag + x.imag)); }
};

struct Complex2 {
    int real, imag;

    explicit Complex2(int re, int im = 0) : real(re), imag(im) {}

    Complex2 operator+(const Complex2& x) { return Complex2((real + x.real), (imag + x.imag)); }
};

class P {
public:
    P(int a, int b) { cout << "P(int a, int b) \n"; }

    P(initializer_list<int>) { cout << "P(initializer_list<int>) \n"; }

    explicit P(int a, int b, int c) { cout << "explicit P(int a, int b, int c) \n"; }
};

void fp(const P&){};

void test04_explicit_multiple_argument_ctor()
{
    cout << "\ntest04_explicit_multiple_argument_ctor()..................\n";

    // explicit ctor with multi-arguments
    P p1(77, 5);      // P(int a, int b)
    P p2{77, 5};      // P(initializer_list<int>)
    P p3{77, 5, 42};  // P(initializer_list<int>)
    P p4 = {77, 5};   // P(initializer_list<int>)
    //!	P p5 = {77, 5, 42}; 	//[Error] converting to 'P' from initializer list would use explicit constructor
    //!'P::P(int, int, int)'
    P p6(77, 5, 42);  // explicit P(int a, int b, int c)

    fp({47, 11});  // P(initializer_list<int>)
    //!	fp( {47,11,3} );		//[Error] converting to 'const P' from initializer list would use explicit
    //! constructor 'P::P(int, int, int)'
    fp(P{47, 11});     // P(initializer_list<int>)
    fp(P{47, 11, 3});  // P(initializer_list<int>)

    P p11{77, 5, 42, 500};     // P(initializer_list<int>)
    P p12 = {77, 5, 42, 500};  // P(initializer_list<int>)
    P p13{10};                 // P(initializer_list<int>)

    {
        complex<int> c1(3, 2);
        cout << c1 << endl;  //(3,2)
        complex<int> c2{3, 2};  // <== 這是否表示 complex 的 ctor 有個版本接受 initializer_list ? 搜尋結果沒發現.
                                // 可見是編譯器對它做了 implicitly convert.
        cout << c2 << endl;  //(3,2)
    }

    {
        Complex1 c1(12, 5);
        Complex1 c2 = c1 + 5;
        cout << "[" << c2.real << "," << c2.imag << "i]" << endl;  //[17,5i]
    }

    {
        Complex2 c1(3, 8);
        // Complex2 c2 = c1 + 3;	  //[Error] no match for 'operator+' (operand types are 'jj04::Complex2' and
        // 'int')
        Complex2 c2{4, 0};
        c2 = c1 + c2;
        cout << "[" << c2.real << "," << c2.imag << "i]" << endl;  //[7,8i]

        // Complex2 c3 = c1 + {2,6};	//Error] expected primary-expression before '{' token.不懂
        //我以為它會呼叫 c1.operator+(&c1, {2,6}) ==> c1.operator+(&c1, Complex2(2,6))

        // anyway，我想知道 explicit for ctors taking more than one argument 有何用途,
        //但想不出來.
    }
}
}  // namespace jj04
//----------------------------------------------------
namespace jj05 {
void test05_range_based_for()
{
    cout << "\ntest05_range_based_for()..................\n";

    // range based for loop
    for (int i : {2, 3, 5, 7, 9, 13, 17, 19}) {
        cout << i << ' ';
    }
    cout << endl;

    vector<double> vec{2, 3, 5, 7, 11, 13, 17};
    for (auto& elem : vec) {
        cout << (elem *= 3) << ' ';
    }
    cout << endl;

    int array[] = {1, 2, 3, 4, 5};
    long sum = 0;  // process sum of all elements
    for (int x : array) {
        sum += x;
    }
    for (auto elem : {sum, sum * 2, sum * 4}) {  // print some multiples of sum
        cout << elem << ' ';
    }
    cout << endl;
}
}  // namespace jj05
//----------------------------------------------------
namespace jj06 {
function<int(int, int)> returnLambda()
{
    return [](int x, int y) { return x * y; };
}

void test06_lambda()
{
    cout << "\ntest06_lambda()..................\n";

    auto lf = returnLambda();
    cout << lf(9, 8) << endl;  // 72
    //! decltype(lf)(3,5);	//[Error] no matching function for call to 'std::function<int(int, int)>::function(int,
    //! int)'
    decltype(lf)();  // no-op. 喚起的是 std::function 的 default ctor.
                     //這不是我要測的. 所以稍後又測一個 -- 那果然就編譯報錯了

    [] {
        cout << "hello lambda" << endl;
        static int x = 5;
        int y = 6;
        return x;
    }();  // prints "hello lambda"

    {
        int id = 0;
        auto f = [id]() mutable {
            cout << "id: " << id << endl;
            ++id;  //若無 mutable ==> [Error] increment of read-only variable 'id'
                   // id 變化不影響 outer scope (除非傳入的是 reference)
        };
        id = 42;
        f();                 // id: 0
        f();                 // id: 1
        f();                 // id: 2
        cout << id << endl;  // 42

        //! decltype(f)(3,5);	//[Error] no matching function for call to
        //! 'jj06::test06_lambda()::<lambda()>::__lambda9(int, int)' decltype(f)();		//[Error] use of deleted
        //! function 'jj06::test06_lambda()::<lambda()>::<lambda>()'
        //我是想創建一個 lambda 的 temp obj (then invoke it's default ctor,
        //  but you know that lambda have no ctor or assignment operator.
    }

    {
        int id = 0;
        auto f = [&id](int param) {  //[]傳入 reference, ()傳入 value
            cout << "id: " << id << endl;
            ++id;     //沒有 mutable 也能 ++
            ++param;  //和 mutable 無關
        };
        id = 42;
        f(7);                // id: 42
        f(7);                // id: 43
        f(7);                // id: 44
        cout << id << endl;  // 45
    }

    vector<int> vi{5, 28, 50, 83, 70, 590, 245, 59, 24};
    int x = 30;
    int y = 100;
    vi.erase(remove_if(vi.begin(), vi.end(), [x, y](int n) { return x < n && n < y; }), vi.end());
    for (auto i : vi) cout << i << ' ';  // 5 28 590 245 24
    cout << endl;
}
}  // namespace jj06
//----------------------------------------------------
namespace jj07 {
// boost:  util/printtuple.hpp
// helper: print element with index IDX of tuple with MAX elements
template <int IDX, int MAX, typename... Args>
struct PRINT_TUPLE {
    static void print(ostream& os, const tuple<Args...>& t)
    {
        os << get<IDX>(t) << (IDX + 1 == MAX ? "" : ",");
        PRINT_TUPLE<IDX + 1, MAX, Args...>::print(os, t);
    }
};

// partial specialization to end the recursion
template <int MAX, typename... Args>
struct PRINT_TUPLE<MAX, MAX, Args...> {
    static void print(std::ostream& os, const tuple<Args...>& t) {}
};

// output operator for tuples
template <typename... Args>
ostream& operator<<(ostream& os, const tuple<Args...>& t)
{
    os << "[";
    PRINT_TUPLE<0, sizeof...(Args), Args...>::print(os, t);
    return os << "]";
}

void test07_tuples()
{
    cout << "\ntest07_tuples().......... \n";

    cout << "string, sizeof = " << sizeof(string) << endl;                    // 4
    cout << "double, sizeof = " << sizeof(double) << endl;                    // 8
    cout << "float, sizeof = " << sizeof(float) << endl;                      // 4
    cout << "int, sizeof = " << sizeof(int) << endl;                          // 4
    cout << "complex<double>, sizeof = " << sizeof(complex<double>) << endl;  // 16

    // tuples
    //  create a four-element tuple
    //  - elements are initialized with default value (0 for fundamental types)
    tuple<string, int, int, complex<double>> t;
    cout << "tuple<string, int, int, complex<double>>, sizeof = " << sizeof(t) << endl;  // 32, why not 28?

    // create and initialize a tuple explicitly

    tuple<int, float, string> t1(41, 6.3, "nico");
    cout << "tuple<int, float, string>, sizeof = " << sizeof(t1) << endl;  // 12
    // iterate over elements:
    cout << "t1: " << get<0>(t1) << ' ' << get<1>(t1) << ' ' << get<2>(t1) << ' ' << endl;

    //!	cout << sizeof(_Tuple_impl<3> << endl;
    //!	cout << sizeof(_Tuple_impl<2, string> << endl;
    //!	cout << sizeof(_Tuple_impl<1, float, string> << endl;
    //!	cout << sizeof(_Tuple_impl<0, int, float, string> << endl;

    // create tuple with make_tuple()
    // - auto declares t2 with type of right-hand side
    // - thus, type of t2 is tuple
    auto t2 = make_tuple(22, 44, "stacy");

    // assign second value in t2 to t1
    get<1>(t1) = get<1>(t2);
    cout << get<1>(t1) << endl;

    // comparison and assignment
    // - including type conversion from tuple<int,int,const  char*>
    //   to tuple<int,float,string>
    if (t1 < t2) {  // compares value for value
        cout << "t1 < t2" << endl;
    } else {
        cout << "t1 >= t2" << endl;
    }
    t1 = t2;                       // OK, assigns value for value
    cout << "t1: " << t1 << endl;  // provided there is a overloaded operator<< for tuple.

    //!	typename tuple<int, float, string> TupleType;			//[Error] expected nested-name-specifier [Error]
    //! expected
    //!'(' 	cout << tuple_size<TupleType>::value << endl;	  		//yields 3 	tuple_element<1,
    //! TupleType>::type  PI { 3.141592653 }; 	//yields float

    tuple<int, float, string> t3(77, 1.1, "more light");
    int i1;
    float f1;
    string s1;
    tie(i1, f1, s1) = t3;  // assigns values of t to i,f,and s
    cout << i1 << ' ' << f1 << ' ' << s1 << endl;

    int n = 100;
    auto t4 = tuple_cat(make_tuple(52, 7.7, "hello"), tie(n));
    //! for (int i=0; i<4; ++i)
    //!      cout << get<i>(t4);   //[Error] the value of 'i' is not usable in a constant expression
    // 注意，這和 template struct PRINT_TUPLE 中的 print() 內的 get<IDX>(t) 不一樣.
    //   i 是變數而 IDX 是模板參數，後者其實是個 constant.
    cout << t4;  // provided there is a overloaded operator<< for tuple.
    cout << endl;

    typedef tuple<int, float, string> TupleType;
    cout << tuple_size<TupleType>::value << endl;  // yields 3
    tuple_element<1, TupleType>::type fl = 1.0;    // yields float
    typedef tuple_element<1, TupleType>::type T;
    cout << "Type Traits...\n";
    cout << is_void<TupleType>::value << endl;    // 0
    cout << is_const<TupleType>::value << endl;   // 0
    cout << is_floating_point<T>::value << endl;  // 1

    cout << make_tuple(7.5, string("hello"), bitset<16>(377),
                       42);  // provided there is a overloaded operator<< for bitset.
}
}  // namespace jj07

//----------------------------------------------------
namespace jj09 {
void test09_array()
{
    cout << "\ntest09_array().......... \n";

    //!	array<int,10> c2 = { 42 };   	//[Error] expected primary-expression before 'int'
    array<int, 10> c2 = {{42}};                      // one element with value 42 followed by 9 elements with value 0
    for (const auto& s : c2) std::cout << s << ' ';  // 42 0 0 0 0 0 0 0 0 0
    std::cout << '\n';

    // construction uses aggregate initialization
    array<int, 3> a1{{1, 2, 3}};   // double-braces required in C++11 (not in C++14)
    array<int, 3> a2 = {1, 2, 3};  // never required after =
    array<string, 2> a3 = {string("a"), "b"};

    // container operations are supported
    sort(a1.begin(), a1.end());
    reverse_copy(a2.begin(), a2.end(), ostream_iterator<int>(cout, " "));  // 3 2 1
    std::cout << '\n';
}
}  // namespace jj09
//----------------------------------------------------
namespace jj10 {
void test10_forward_list()
{
    cout << "\ntest10_forward_list().......... \n";

    forward_list<int> fl{3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5};
    fl.push_front(89);
    //!	fl.push_back(10); 	//[Error] 'class std::forward_list<int>' has no member named 'push_back'
    cout << "fl.size " << distance(fl.begin(), fl.end()) << endl;  // 12
    cout << "fl.max_size " << fl.max_size() << endl;               // 536870911
    fl.sort();
}
}  // namespace jj10
//----------------------------------------------------
namespace jj11 {
class Customer {
private:
public:
    Customer(const string& fn, const string& ln, const long& n) : fname(fn), lname(ln), no(n) {}

    bool operator==(const Customer& c) const
    {
        if ((fname == c.fname) && (lname == c.lname) && (no == c.no))
            return true;
        else
            return false;
    }

    string fname;
    string lname;
    long no;
};

// output operator for Customer
ostream& operator<<(ostream& os,
                    const Customer& c)  //[Error] expected initializer before '<<' token
{
    os << "Customer: " << c.fname << ' ' << c.lname << ' ' << c.no << endl;
    return os;
}

class CustomerHash1 {
public:
    size_t operator()(const Customer& c) const
    {
        return hash<string>()(c.fname) + hash<std::string>()(c.lname) + hash<long>()(c.no);
    }
};

size_t CustomerHash2(const Customer& c)
{
    return hash<string>()(c.fname) + hash<std::string>()(c.lname) + hash<long>()(c.no);
};

//----------------------------------------------------
// from boost (functional/hash):
template <typename T>
inline void hash_combine(size_t& seed, const T& val)
{
    seed ^= hash<T>()(val) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

// auxiliary generic functions to create a hash value using a seed
template <typename T>
inline void hash_val(size_t& seed, const T& val)
{
    hash_combine(seed, val);
}

template <typename T, typename... Types>
inline void hash_val(size_t& seed, const T& val, const Types&... args)
{
    hash_combine(seed, val);
    hash_val(seed, args...);
}

// auxiliary generic function
template <typename... Types>
inline size_t hash_val(const Types&... args)
{
    size_t seed = 0;
    hash_val(seed, args...);
    return seed;
}

class CustomerHash3 {
public:
    size_t operator()(const Customer& c) const { return hash_val(c.fname, c.lname, c.no); }
};

void test11_unordered_set()
{
    cout << "\ntest11_unordered_set().......... \n";

    unordered_set<string> coll;
    coll.max_load_factor(0.7);
    cout << coll.max_load_factor() << endl;  // 0.7

    unordered_set<Customer, CustomerHash1> set1;
    set1.insert(Customer("Ace", "Hou", 1L));
    set1.insert(Customer("Sabri", "Hou", 2L));
    set1.insert(Customer("Stacy", "Chen", 3L));
    set1.insert(Customer("Mike", "Tseng", 4L));
    set1.insert(Customer("Paili", "Chen", 5L));
    set1.insert(Customer("Light", "Shiau", 6L));
    set1.insert(Customer("Shally", "Hwung", 7L));
    cout << "current size: " << set1.size() << endl;                          // 7
    cout << "current max_size: " << set1.max_size() << endl;                  // 214748364
    cout << "current load_factor: " << set1.load_factor() << endl;            // 0.636364
    cout << "current max_load_factor: " << set1.max_load_factor() << endl;    // 1
    cout << "set1 current bucket_count: " << set1.bucket_count() << endl;     // 11
    cout << "current max_bucket_count: " << set1.max_bucket_count() << endl;  // 214748364
    for (unsigned i = 0; i < set1.bucket_count(); ++i) {
        cout << "bucket #" << i << " has " << set1.bucket_size(i) << " elements.\n";
    }

    for (auto& elem : set1) {
        cout << elem;
    }
    cout << endl;

    unordered_set<Customer, size_t (*)(const Customer&)> set2(20, CustomerHash2);
    set2.insert(Customer("Ace", "Hou", 1L));
    set2.insert(Customer("Sabri", "Hou", 2L));
    set2.insert(Customer("Stacy", "Chen", 3L));
    set2.insert(Customer("Mike", "Tseng", 4L));
    set2.insert(Customer("Paili", "Chen", 5L));
    set2.insert(Customer("Light", "Shiau", 6L));
    set2.insert(Customer("Shally", "Hwung", 7L));
    cout << "set2 current bucket_count: " << set2.bucket_count() << endl;  // 23
    for (auto& elem : set2) {
        cout << elem;
    }
    cout << endl;

    unordered_set<Customer, CustomerHash3> set3;
    set3.insert(Customer("Ace", "Hou", 1L));
    set3.insert(Customer("Sabri", "Hou", 2L));
    set3.insert(Customer("Stacy", "Chen", 3L));
    set3.insert(Customer("Mike", "Tseng", 4L));
    set3.insert(Customer("Paili", "Chen", 5L));
    set3.insert(Customer("Light", "Shiau", 6L));
    set3.insert(Customer("Shally", "Hwung", 7L));
    cout << "set3 current bucket_count: " << set3.bucket_count() << endl;  // 11
    for (auto& elem : set3) {
        cout << elem;
    }
    cout << endl;

    CustomerHash3 hh;
    cout << "hash code of Ace = " << hh(Customer("Ace", "Hou", 1L)) << endl;
    cout << "hash code of Sabri = " << hh(Customer("Sabri", "Hou", 2L)) << endl;
    cout << "hash code of Stacy = " << hh(Customer("Stacy", "Chen", 3L)) << endl;
    cout << "hash code of Mike = " << hh(Customer("Mike", "Tseng", 4L)) << endl;
    cout << "hash code of Paili = " << hh(Customer("Paili", "Chen", 5L)) << endl;
    cout << "hash code of Light = " << hh(Customer("Light", "Shiau", 6L)) << endl;
    cout << "hash code of Shally = " << hh(Customer("Shally", "Hwung", 7L)) << endl;

    cout << "bucket position of Ace = " << hh(Customer("Ace", "Hou", 1L)) % 11 << endl;          // 2
    cout << "bucket position of Sabri = " << hh(Customer("Sabri", "Hou", 2L)) % 11 << endl;      // 4
    cout << "bucket position of Stacy = " << hh(Customer("Stacy", "Chen", 3L)) % 11 << endl;     // 10
    cout << "bucket position of Mike = " << hh(Customer("Mike", "Tseng", 4L)) % 11 << endl;      // 2
    cout << "bucket position of Paili = " << hh(Customer("Paili", "Chen", 5L)) % 11 << endl;     // 9
    cout << "bucket position of Light = " << hh(Customer("Light", "Shiau", 6L)) % 11 << endl;    // 6
    cout << "bucket position of Shally = " << hh(Customer("Shally", "Hwung", 7L)) % 11 << endl;  // 2

    for (unsigned i = 0; i < set3.bucket_count(); ++i) {
        cout << "bucket #" << i << " has " << set3.bucket_size(i) << " elements.\n";
    }
    // bucket #0 has 0 elements.
    // bucket #1 has 0 elements.
    // bucket #2 has 3 elements.
    // bucket #3 has 0 elements.
    // bucket #4 has 1 elements.
    // bucket #5 has 0 elements.
    // bucket #6 has 1 elements.
    // bucket #7 has 0 elements.
    // bucket #8 has 0 elements.
    // bucket #9 has 1 elements.
    // bucket #10 has 1 elements.
}
}  // namespace jj11

//----------------------------------------------------
namespace jj12 {
void test12_Rvalue_Move()
{
    cout << "\ntest12_Rvalue_Move().......... \n";

    string s1("international");
    string s2("ization");
    string s = s1 + s2;

    cout << "s: " << s << endl;    // internationalization
    cout << "s1: " << s1 << endl;  // international
    cout << "s2: " << s2 << endl;  // ization

    s = move(s1) + move(s2);
    cout << "s: " << s << endl;    // internationalization
    cout << "s1: " << s1 << endl;  //
    cout << "s2: " << s2 << endl;  // ization

    //----------------

    s1 = "Hello ";
    s1 + s2 = s2;                  //竟然可以通過編譯
    cout << "s1: " << s1 << endl;  // s1: Hello
    cout << "s2: " << s2 << endl;  // s2: ization
    string() = "World";            //對 temp obj 賦值可以嗎？

    complex<int> c1(3, 8), c2(1, 0);
    c1 + c2 = complex<int>(4, 9);         // c1+c2 可以當做 Lvalue 嗎？
    cout << "c1: " << c1 << endl;         // c1: (3,8)
    cout << "c2: " << c2 << endl;         // c2: (1,0)
    complex<int>() = complex<int>(4, 9);  //對 temp obj 賦值可以嗎？

    int x = 4;
    int y = 8;
    //! x + y = 10;	//[Error] lvalue required as left operand of assignment
}
}  // namespace jj12
//----------------------------------------------------
namespace jj15 {
// recursive function calling.
// case1
//  ...\4.9.2\include\c++\bits\predefined_oops.h
struct _Iter_less_iter {
    template <typename _Iterator1, typename _Iterator2>
    bool operator()(_Iterator1 __it1, _Iterator2 __it2) const
    {
        return *__it1 < *__it2;
    }
};

inline _Iter_less_iter __iter_less_iter() { return _Iter_less_iter(); }

// ...\4.9.2\include\c++\bits\stl_algo.h
template <typename _ForwardIterator, typename _Compare>
_ForwardIterator __max_element(_ForwardIterator __first, _ForwardIterator __last, _Compare __comp)
{
    if (__first == __last) return __first;
    _ForwardIterator __result = __first;
    while (++__first != __last)
        if (__comp(__result, __first)) __result = __first;
    return __result;
}

template <typename _ForwardIterator>
inline _ForwardIterator max_element(_ForwardIterator __first, _ForwardIterator __last)
{
    return __max_element(__first, __last, __iter_less_iter());
}

template <typename _Tp>
inline _Tp max(initializer_list<_Tp> __l)
{
    return *max_element(__l.begin(), __l.end());
}
//~~~~~~~~~~~~~~~
// case2
// http://stackoverflow.com/questions/3634379/variadic-templates
int maximum(int n) { return n; }

template <typename... Args>
int maximum(int n, Args... args)
{
    return std::max(n, maximum(args...));
}
//~~~~~~~~~~~~~~~
// case3
// http://stackoverflow.com/questions/3634379/variadic-templates
void printf(const char* s)
{
    while (*s) {
        if (*s == '%' && *(++s) != '%') throw std::runtime_error("invalid format string: missing arguments");
        std::cout << *s++;
    }
}

template <typename T, typename... Args>
void printf(const char* s, T value, Args... args)
{
    while (*s) {
        if (*s == '%' && *(++s) != '%') {
            std::cout << value;
            printf(++s, args...);  // call even when *s == 0 to detect extra arguments
                                   // 20151101: 上一行原是 printf(s, args...);
                                   //  這會把 specifier 列印出來, 不好.
            return;
        }
        std::cout << *s++;
    }
    throw std::logic_error("extra arguments provided to printf");
}
//~~~~~~~~~~~~~~~
// case4
void printX() {}

template <typename T, typename... Types>
void printX(const T& firstArg, const Types&... args)
{
    cout << firstArg << endl;  // print first argument
    printX(args...);           // call printX() for remaining arguments
}

template <typename... Types>
void printX(const Types&... args)
{
    cout << "printX(const Types&... args)" << endl;
}
//~~~~~~~~~~~~~~~
// case5, refer to test07_tuples()

//~~~~~~~~~~~~~~~
// case6
// recursive inheritance
template <typename... Values>
class tuple;
template <>
class tuple<> {
};

template <typename Head, typename... Tail>
class tuple<Head, Tail...> : private tuple<Tail...> {
    typedef tuple<Tail...> inherited;

public:
    tuple() {}
    tuple(Head v, Tail... vtail) : m_head(v), inherited(vtail...) {}
    //(1):
    //! typename Head::type head() { return m_head; }
    //[Error] no type named 'type' in 'class std::basic_string<char>'
    //[Error] 'long int' is not a class, struct, or union type
    //...沒有提供 typedef type 者統統不通過
    //(2):
    //  auto head()->decltype(m_head) { return m_head; }  //OK also.
    // m_head 若放在此行之後則無法通過編譯
    //移到最前則 ok. 這太離譜了吧!!
    //(3):
    //最後我發現，其實 return type 就是 Head 嘛.
    //神經病, 把事情搞那麼複雜, sigh.
    //於是又把 m_head 往下移回.
    Head head() { return m_head; }

    inherited& tail() { return *this; }

protected:
    Head m_head;
};

//~~~~~~~~~~~~~~~
// case7
// recursive composition
template <typename... Values>
class tup;
template <>
class tup<> {
};

template <typename Head, typename... Tail>
class tup<Head, Tail...> {
    typedef tup<Tail...> composited;

protected:
    composited m_tail;
    Head m_head;

public:
    tup() {}
    tup(Head v, Tail... vtail) : m_tail(vtail...), m_head(v) {}

    Head head() { return m_head; }
    composited& tail() { return m_tail; }
};

//-----------
void test15_variadic_template()
{
    cout << "\ntest15_variadic_template().......... \n";

    // case1
    cout << jj15::max({57, 48, 60, 100, 20, 18}) << endl;  // 100

    // case2
    cout << maximum(57, 48, 60, 100, 20, 18) << endl;  // 100

    // case3
    int* pi = new int;
    printf("%d %s %p %f \n", 15, "This is Ace.", pi, 3.141592653);  // 15 This is Ace. 0x3e3e60 3.14159

    // case4
    printX(7.5, "hello", bitset<16>(377), 42);
    // 7.5
    // hello
    // 0000000101111001
    // 42

    // case6
    tuple<int, float, string> t1(41, 6.3, "nico");
    cout << sizeof(t1) << endl;               // 12
    cout << t1.head() << endl;                // 41
    cout << t1.tail().head() << endl;         // 6.3
    cout << t1.tail().tail().head() << endl;  // nico

    cout << sizeof(bitset<16>) << endl;  // 4

    tuple<string, complex<int>, bitset<16>, double> t2("Ace", complex<int>(3, 8), bitset<16>(377), 3.141592653);
    cout << sizeof(t2) << endl;                      // 24
    cout << t2.head() << endl;                       // Ace
    cout << t2.tail().head() << endl;                //(3,8)
    cout << t2.tail().tail().head() << endl;         // 0000000101111001
    cout << t2.tail().tail().tail().head() << endl;  // 3.14159

    tuple<string, complex<int>, double> t3("Stacy", complex<int>(4, 9), 1.6180339);
    cout << sizeof(t3) << endl;               // 24, why not 20?
    cout << t3.head() << endl;                // Stacy
    cout << t3.tail().head() << endl;         //(4,9)
    cout << t3.tail().tail().head() << endl;  // 1.61803

    // case7
    tup<int, float, string> it1(41, 6.3, "nico");
    cout << sizeof(it1) << endl;               // 16
    cout << it1.head() << endl;                // 41
    cout << it1.tail().head() << endl;         // 6.3
    cout << it1.tail().tail().head() << endl;  // nico

    tup<string, complex<int>, bitset<16>, double> it2("Ace", complex<int>(3, 8), bitset<16>(377), 3.141592653);
    cout << sizeof(it2) << endl;                      // 40
    cout << it2.head() << endl;                       // Ace
    cout << it2.tail().head() << endl;                //(3,8)
    cout << it2.tail().tail().head() << endl;         // 0000000101111001
    cout << it2.tail().tail().tail().head() << endl;  // 3.14159
}
}  // namespace jj15

//----------------------------------------------------
#include <map>
#include <typeinfo>
namespace jj18 {

template <typename T>
void test18_decltype(T obj)
//以上 T& or const T& 可不行, 後面編譯會說它 is not a class, struct or union type
{
    cout << "\ntest18_decltype().......... \n";

    map<string, float>::value_type elem1;
    //當我們手上有 type，可以取其 inner typedef，沒問題.
    //但我們無法通過 object (例如 map<...>()) 取其 class type 內的 typedef -- 即使有 traits!
    //  例 iterator_traits 可取 iterators 的 5 associated types)
    //  例 type_traits 可取 type 的若干特徵.
    //因手上只有 object 而 traits 針對的是 types (上述 iterator 也是 type).
    // c++ 有 typeof() 嗎？ 沒有, 只有 typeid() 搭配 #include <type_info>.
    //  獲得的東西不能拿來在程式中當 type 使用.
    cout << typeid(elem1).name() << endl;  // St4pairIKSsfE

    map<string, float> coll;
    decltype(coll)::value_type elem2;      //現在我們可以對 object 取其 class type 的 inner typedef 了.
    cout << typeid(elem2).name() << endl;  // St4pairIKSsfE

    //--------

    //(瞧我故意設計本測試函數為 template function, 接受任意參數 T& obj)
    cout << typeid(obj).name() << endl;  // St3mapISsSt7complexIlESt4lessISsESaISt4pairIKSsS1_EEE
                                         // depend on caller
    //那我可以這樣：
    typedef typename decltype(obj)::iterator iType;
    typedef typename T::iterator iTypes;  // OK also.
                                          // error if "obj" is a complex<T>(), for example,
                                          //     there is no inner typedef iterator in complex<T>
                                          //[Error] no type named 'iterator' in 'struct std::complex<int>'
    cout << typeid(typename iterator_traits<iType>::value_type()).name() << endl;  // FSt4pairIKSsSt7complexIlEEvE
                                                                                   // depend on caller
    //以上取 obj 的 type 內的 typedef (當然必須真有才行)
    //若實際無該 typedef ==>(例) [Error] no type named 'iterator' in 'struct std::complex<int>'
    //也就是說這個 function template 是否編譯成功，取決於 caller.
    //不奇怪, template 不都是如此嗎 (smile)

    //可不可以拿 decltype(obj) 直接用? 可.
    decltype(obj) anotherObj(obj);
    cout << typeid(anotherObj).name() << endl;  // St4pairIKSsfE
}
}  // namespace jj18
//----------------------------------------------------
namespace jj19 {
struct NoCopy {
    NoCopy() = default;                         // use the synthesized default constructor
    NoCopy(const NoCopy&) = delete;             // no copy
    NoCopy& operator=(const NoCopy&) = delete;  // no copy assignment
    ~NoCopy() = default;                        // use the synthesized destructor
                                                // other members
};

struct NoDtor {
    NoDtor() = default;  // use the synthesized default constructor
    ~NoDtor() = delete;  // we can’t destroy objects of type NoDtor
};

void test19_default_delete();  // forward declaration, since of the friend in PrivateCopy below.

class PrivateCopy {
    friend void test19_default_delete();

private:
    // no access specifier; following members are private by default; see § 7.2 (p.268)
    // copy control is private and so is inaccessible to ordinary user code
    PrivateCopy(const PrivateCopy&) = default;
    PrivateCopy& operator=(const PrivateCopy&) = default;
    // other members
public:
    PrivateCopy() = default;   // use the synthesized default constructor
    ~PrivateCopy() = default;  // users can define objects of this type but not copy them
};

struct Type1 {
    Type1() = default;  // The default constructor is explicitly stated.
    Type1(string value) {}
};

struct NonCopyable {
    NonCopyable() = default;
    NonCopyable(const NonCopyable&) = delete;
    NonCopyable& operator=(const NonCopyable&) = delete;
};

struct NoInt {
    void f(double d) {}
    // void f(int) = delete;
    template <class T>
    void f(T) = delete;  //禁止 double 以外的一切參數型別  (因此不接受其他型別自動轉為 double)

    void f2(const char* s) { cout << "void f2(const char*)  " << s << endl; }
    void f2(string& s) { cout << "void f2(string&)  " << s << endl; }
};

void test19_default_delete()
{
    cout << "\ntest19_default_delete().......... \n";

    //!	NoDtor nd;                 	// error: NoDtor destructor is deleted
    //[Error] use of deleted function 'NoDtor::~NoDtor()'
    NoDtor* p =
        new NoDtor();  // ok: but we can’t delete p
                       //!	delete p;                   //[Error] use of deleted function 'NoDtor::~NoDtor()'

    PrivateCopy pc;
    PrivateCopy pc2(pc);  //雖然 PrivateCopy 不能 copy，卻可由 friend or member 完成 copy
    pc2 = pc;             //雖然 PrivateCopy 不能 copy，卻可由 friend or member 完成 copy
                          //本函式就是 PrivateCopy 的 friend.

    NonCopyable nc1, nc2;
    //!	nc2 = nc1;					//[Error] use of deleted function 'NonCopyable&
    //! NonCopyable::operator=(const NonCopyable&)'

    NoInt ni;
    ni.f(5.6);
    //!	ni.f(7);		//Error] use of deleted function 'void NoInt::f(int)'
    //!	ni.f('a');		//Error] use of deleted function 'void NoInt::f(int)'
    char s[20]{"world"};
    ni.f2(s);  // void f2(const char*)  world
    string str("hello");
    ni.f2(str);  // void f2(string&)  hello
    //! ni.f2(string("hello"));		//[Error] no matching function for call to 'NoInt::f2(std::string)'
    //以上的 [Error] 為什麼？
    //是因為傳入 Rvalue 而不被 (parameter which passed by reference) 接受嗎？
}
}  // namespace jj19
//----------------------------------------------------
#include <typeinfo>
// global function template for test23-test26
template <typename T>
void type_traits_output(const T& x)
{
    cout << "\ntype traits for type : " << typeid(T).name() << endl;

    cout << "is_void\t" << is_void<T>::value << endl;
    cout << "is_integral\t" << is_integral<T>::value << endl;
    cout << "is_floating_point\t" << is_floating_point<T>::value << endl;
    cout << "is_arithmetic\t" << is_arithmetic<T>::value << endl;
    cout << "is_signed\t" << is_signed<T>::value << endl;
    cout << "is_unsigned\t" << is_unsigned<T>::value << endl;
    cout << "is_const\t" << is_const<T>::value << endl;
    cout << "is_volatile\t" << is_volatile<T>::value << endl;
    cout << "is_class\t" << is_class<T>::value << endl;
    cout << "is_function\t" << is_function<T>::value << endl;
    cout << "is_reference\t" << is_reference<T>::value << endl;
    cout << "is_lvalue_reference\t" << is_lvalue_reference<T>::value << endl;
    cout << "is_rvalue_reference\t" << is_rvalue_reference<T>::value << endl;
    cout << "is_pointer\t" << is_pointer<T>::value << endl;
    cout << "is_member_pointer\t" << is_member_pointer<T>::value << endl;
    cout << "is_member_object_pointer\t" << is_member_object_pointer<T>::value << endl;
    cout << "is_member_function_pointer\t" << is_member_function_pointer<T>::value << endl;
    cout << "is_fundamental\t" << is_fundamental<T>::value << endl;
    cout << "is_scalar\t" << is_scalar<T>::value << endl;
    cout << "is_object\t" << is_object<T>::value << endl;
    cout << "is_compound\t" << is_compound<T>::value << endl;
    cout << "is_standard_layout\t" << is_standard_layout<T>::value << endl;
    cout << "is_pod\t" << is_pod<T>::value << endl;
    cout << "is_literal_type\t" << is_literal_type<T>::value << endl;
    cout << "is_empty\t" << is_empty<T>::value << endl;
    cout << "is_polymorphic\t" << is_polymorphic<T>::value << endl;
    cout << "is_abstract\t" << is_abstract<T>::value << endl;
    cout << "has_virtual_destructor\t" << has_virtual_destructor<T>::value << endl;
    cout << "is_default_constructible\t" << is_default_constructible<T>::value << endl;
    cout << "is_copy_constructible\t" << is_copy_constructible<T>::value << endl;
    cout << "is_move_constructible\t" << is_move_constructible<T>::value << endl;
    cout << "is_copy_assignable\t" << is_copy_assignable<T>::value << endl;
    cout << "is_move_assignable\t" << is_move_assignable<T>::value << endl;
    cout << "is_destructible\t" << is_destructible<T>::value << endl;
    cout << "is_trivial\t" << is_trivial<T>::value << endl;
    //!	cout << "is_trivially_constructible\t" << is_trivially_constructible<T>::value << endl; 	//[Error] was
    //! not declared in this scope 	cout << "is_trivially_copyable\t" << is_trivially_copyable<T>::value << endl;
    //!//[Error] was not declared in this scope 	cout << "is_trivially_default_constructible\t" <<
    //! is_trivially_default_constructible<T>::value << endl; 	cout << "is_trivially_copy_constructible\t" <<
    //! is_trivially_copy_constructible<T>::value << endl; 	cout << "is_trivially_move_constructible\t" <<
    //! is_trivially_move_constructible<T>::value << endl; 	cout << "is_trivially_copy_assignable\t" <<
    //! is_trivially_copy_assignable<T,int>::value << endl; 	//注意, 需要兩個 template type param. 	cout <<
    //!"is_trivially_move_assignable\t" << is_trivially_move_assignable<T,string>::value << endl; 	//注意, 需要兩個
    //! template type param.

    // The C++ front end implements syntactic extensions that allow compile-time determination of
    // various characteristics of a type (or of a pair of types).
    cout << "__has_trivial_assign\t" << __has_trivial_assign(T) << endl;
    cout << "__has_trivial_copy\t" << __has_trivial_copy(T) << endl;
    cout << "__has_trivial_constructor\t" << __has_trivial_constructor(T) << endl;
    cout << "__has_trivial_destructor\t" << __has_trivial_destructor(T) << endl;

    cout << "is_trivially_destructible\t" << is_trivially_destructible<T>::value << endl;
    cout << "is_nothrow_default_constructible\t" << is_nothrow_default_constructible<T>::value << endl;
    cout << "is_nothrow_copy_constructible\t" << is_nothrow_copy_constructible<T>::value << endl;
    cout << "is_nothrow_move_constructible\t" << is_nothrow_move_constructible<T>::value << endl;
    cout << "is_nothrow_copy_assignable\t" << is_nothrow_copy_assignable<T>::value << endl;
    cout << "is_nothrow_move_assignable\t" << is_nothrow_move_assignable<T>::value << endl;
    cout << "is_nothrow_destructible\t" << is_nothrow_destructible<T>::value << endl;
}
//----------------------------------------------------
#include <typeinfo>
#include <complex>
#include <list>
#include <string>
namespace jj23 {
void test23_traits_for_string()
{
    cout << "\ntest23_traits_for_string().......... \n";

    typedef iterator_traits<set<string>::iterator>::value_type T;  // T 應該就是 string
    cout << "iterator_traits<set<string>::iterator>::value_type ==> " << typeid(T).name()
         << endl;    // Ss (奇怪, 只有縮寫)
    T obj("hello");  // test ctor, try to assign a literal string
    cout << "iterator_traits<set<string>::iterator>::value_type ==> " << typeid(obj).name() << endl;  // Ss

    ::type_traits_output(T());  // std::string id => Ss

    ::type_traits_output(complex<float>());  // id =>
    ::type_traits_output(list<int>());       // id =>
}
}  // namespace jj23
//----------------------------------------------------
namespace jj24 {
class Foo {
public:
private:
    int d1, d2;
};
// Foo is : trivial, standard_layout, POD, literal_type,

void test24_traits_for_Foo()
{
    cout << "\ntest24_traits_for_Foo().......... \n";

    ::type_traits_output(Foo());  // Foo id => N4jj243FooE
}
}  // namespace jj24
//----------------------------------------------------
namespace jj25 {
class Goo {
public:
    virtual ~Goo() {}

private:
    int d1, d2;
};
// Goo is : polymophic, has_virtual_dtor,

void test25_traits_for_Goo()
{
    cout << "\ntest25_traits_for_Goo().......... \n";

    ::type_traits_output(Goo());  // Goo id => N4jj253GooE
}
}  // namespace jj25
//----------------------------------------------------
namespace jj26 {
class Zoo {
public:
    Zoo(int i1, int i2) : d1(i1), d2(i2) {}

    Zoo(const Zoo&) = delete;
    //!	Zoo(const Zoo&&) = default;	//[Error] does not match expected signature 'constexpr Zoo::Zoo(Zoo&&)'
    Zoo(Zoo&&) = default;

    Zoo& operator=(const Zoo&) = default;
    Zoo& operator=(const Zoo&&) = delete;
    virtual ~Zoo() {}

private:
    int d1, d2;
};
// Zoo is :

void test26_traits_for_Zoo()
{
    cout << "\ntest26_traits_for_Zoo().......... \n";

    ::type_traits_output(Zoo(1, 2));  // Zoo id => N4jj263ZooE
}
}  // namespace jj26

//----------------------------------------------------
namespace jj27 {
void process(int& i) { cout << "process(int&): " << i << endl; }
void process(int&& i) { cout << "process(int&&): " << i << endl; }
void forward(int&& i)
{
    cout << "forward(int&&): " << i << ",  ";
    process(i);
}
void test27_Rvalue_reference()
{
    cout << "\ntest27_Rvalue_reference().......... \n";

    int a = 0;

    process(a);  // process(int&): 0
                 //變數被視為 lvalue 處理
    process(1);  // process(int&&): 1
                 // temp. object 被視為 Rvalue 處理

    process(move(a));  // process(int&&): 0
                       //強制將 a 由 Lvalue 改為 Rvalue

    forward(2);  // forward(int&&): 2,  process(int&): 2
                 // Rvalue 經由 forward() 傳給另一函數，變成 Lvalue，
                 //因為傳遞過程中它變成了一個 named object.

    forward(move(a));  // forward(int&&): 0,  process(int&): 0
                       // Rvalue 經由 forward() 傳給另一函數，變成 Lvalue

    //!	forward(a);			//[Error] cannot bind 'int' lvalue to 'int&&'

    const int& b = 1;
    //!	process(b);			//[Error] no matching function for call to 'process(const int&)'
    //!	process(move(b));	//[Error] no matching function for call to 'process(std::remove_reference<const
    //! int&>::type)'

    //! int& c(5);			//[Error] invalid initialization of non-const reference of type 'int&' from an
    //! rvalue of type 'int' process(move(c));	//上行失敗本行當然不必說, "c" was not declared in this scope
}
}  // namespace jj27
//----------------------------------------------------
namespace jj28 {
//加上 void process(const int& i); 如下

void process(const int& i) { cout << "process(const int&): " << i << endl; }
}  // namespace jj28
//----------------------------------------------------
namespace jj291 {
using jj27::process;
using jj28::process;

//完全學習 std::forward
template <typename T>
void forward(T& val)
{
    cout << "forward<T>(T&) " << val << ",  ";
    process(static_cast<T&&>(val));  //這是 move.h 中的 forward 的作法
}
template <typename T>
void forward(T&& val)
{
    cout << "forward<T>(T&&) " << val << ",  ";
    process(static_cast<T&&>(val));  //這是 move.h 中的 forward 的作法
}

void test291_perfect_forward()
{
    cout << "\ntest291_perfect_forward().......... \n";
    int a = 0;
    const int& b = 1;

    forward(a);  // forward<T>(T&) 0,   process(int&&): 0 -- good?! (lvalue 怎麼變成了 Rvalue?!)
    forward(b);  // forward<T>(T&) 1,   process(const int&): 1 -- good

    forward(2);        // forward<T>(T&&) 2,  process(int&&): 2 -- good
    forward(move(a));  // forward<T>(T&&) 0,  process(int&&): 0 -- good
    forward(move(b));  // forward<T>(T&&) 1,  process(const int&): 1 -- good?!
    forward(int(9));   // forward<T>(T&&) 9,  process(int&&): 9 -- good
}
}  // namespace jj291
//----------------------------------------------------
namespace jj301 {
// http://www.ibm.com/developerworks/cn/aix/library/1307_lisl_c11/
// http://stackoverflow.com/questions/8001823/how-to-enforce-move-semantics-when-a-vector-grows

// You need to inform C++ (specifically std::vector) that your move constructor and destructor does not throw.
// Then the move constructor will be called when the vector grows.
// If the constructor is not noexcept, std::vector can't use it,
// since then it can't ensure the exception guarantees demanded by the standard.

// 注意，growable conatiners (會發生 memory reallocation) 只有 vector 和 deque.

//以下的 MyString 和 jj30 中的唯一差異是 noexcept for move functions. 結果十分良好.

class MyString {
private:
    char* _data;
    size_t _len;
    void _init_data(const char* s)
    {
        _data = new char[_len + 1];
        memcpy(_data, s, _len);
        _data[_len] = '\0';
    }

public:
    // default ctor
    MyString() : _data(NULL), _len(0) {}

    // ctor
    MyString(const char* p) : _len(strlen(p)) { _init_data(p); }

    // copy ctor
    MyString(const MyString& str) : _len(str._len)
    {
        cout << "Copy Constructor is called! source: " << str._data << " [" << (void*)(str._data) << ']' << endl;
        _init_data(str._data);  // COPY
    }

    // move ctor, with "noexcept"
    MyString(MyString&& str) noexcept : _data(str._data), _len(str._len)
    {
        cout << "Move Constructor is called! source: " << str._data << " [" << (void*)(str._data) << ']' << endl;
        str._len = 0;
        str._data = NULL;  //避免 delete (in dtor)
    }

    // copy assignment
    MyString& operator=(const MyString& str)
    {
        cout << "Copy Assignment is called! source: " << str._data << " [" << (void*)(str._data) << ']' << endl;
        if (this != &str) {
            if (_data) delete _data;
            _len = str._len;
            _init_data(str._data);  // COPY!
        } else {
            cout << "Self Assignment, Nothing to do." << endl;
        }
        return *this;
    }

    // move assignment
    MyString& operator=(MyString&& str) noexcept
    {  //注意 noexcept
        cout << "Move Assignment is called! source: " << str._data << " [" << (void*)(str._data) << ']' << endl;
        if (this != &str) {
            if (_data) delete _data;
            _len = str._len;
            _data = str._data;  // MOVE!
            str._len = 0;
            str._data = NULL;  //避免 delete (in dtor)
        }
        return *this;
    }

    // dtor
    virtual ~MyString()
    {  //文檔說需 noexcept 但本處無. destructor is noexcept by default. Johan Lundberg Mar 18 '13 at 12:12
        cout << "Destructor is called! "
             << "source: ";
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
    // without reserve(N);

    vec.push_back(MyString("jjhou"));    // Move Constructor is called! source: jjhou
                                         // Destructor is called! [0]
    vec.push_back(MyString("sabrina"));  // Move Constructor is called! source: sabrina
                                         // Move Constructor is called! source: jjhou
                                         // Destructor is called! [0]
                                         // Destructor is called!	[0]
    vec.push_back(MyString("stacy"));    // Move Constructor is called! source: stacy
                                         // Move Constructor is called! source: jjhou
                                         // Move Constructor is called! source: sabrina
                                         // Destructor is called! [0]
                                         // Destructor is called! [0]
                                         // Destructor is called! [0]
    //以上十分好：
    //  1, 以 temp obj.放入容器，編譯器知道那是個 Rvalue, 於是呼叫 move ctor 而非 copy ctor.
    //  2, 當 push_back() 引發 vector 擴展，擴展過程中使用 move 而非 copy.
    cout << "vec.clear() ------- \n";
    vec.clear();

    // exit 前會 delete all existing objects.
}
}  // namespace jj301
//----------------------------------------------------
namespace jj31 {
void test31_containers_size()
{
    cout << "\ntest31_containers_size().......\n";

    cout << "containers size : \n";
    cout << sizeof(array<int, 10>) << endl;                   // 40
    cout << sizeof(vector<int>) << endl;                      // 12
    cout << sizeof(list<int>) << endl;                        // 8 = sizeof(_List_node_base)+sizeof(allocator) = 8+0
    cout << sizeof(forward_list<int>) << endl;                // 4
    cout << sizeof(deque<int>) << endl;                       // 40
    cout << sizeof(stack<int>) << endl;                       // 40
    cout << sizeof(queue<int>) << endl;                       // 40
    cout << sizeof(set<int>) << endl;                         // 24
    cout << sizeof(map<int, string>) << endl;                 // 24
    cout << sizeof(multiset<int>) << endl;                    // 24
    cout << sizeof(multimap<int, string>) << endl;            // 24
    cout << sizeof(unordered_set<int>) << endl;               // 28
    cout << sizeof(unordered_map<int, string>) << endl;       // 28
    cout << sizeof(unordered_multiset<int>) << endl;          // 28
    cout << sizeof(unordered_multimap<int, string>) << endl;  // 28

    cout << "iterators size : \n";
    cout << sizeof(array<int, 10>::iterator) << endl;                   // 4
    cout << sizeof(vector<int>::iterator) << endl;                      // 4
    cout << sizeof(list<int>::iterator) << endl;                        // 4
    cout << sizeof(forward_list<int>::iterator) << endl;                // 4
    cout << sizeof(deque<int>::iterator) << endl;                       // 16
    cout << sizeof(set<int>::iterator) << endl;                         // 4
    cout << sizeof(map<int, string>::iterator) << endl;                 // 4
    cout << sizeof(multiset<int>::iterator) << endl;                    // 4
    cout << sizeof(multimap<int, string>::iterator) << endl;            // 4
    cout << sizeof(unordered_set<int>::iterator) << endl;               // 4
    cout << sizeof(unordered_map<int, string>::iterator) << endl;       // 4
    cout << sizeof(unordered_multiset<int>::iterator) << endl;          // 4
    cout << sizeof(unordered_multimap<int, string>::iterator) << endl;  // 4
}
}  // namespace jj31
//----------------------------------------------------
namespace jj34 {

template <typename T>
using Vec = list<T, allocator<T>>;  // template alias

template <typename First, typename Second, int Third>
class SomeType {
};
template <typename Second>
using TypedefName = SomeType<string, Second, 5>;

typedef void (*Func1)(double);   // Old style
using Func2 = void (*)(double);  // New introduced syntax

void test34_alias_template()
{
    cout << "\ntest34_alias_template().......\n";

    Vec<int> coll;
    coll.push_back(10);
    coll.push_back(11);
    coll.push_back(12);

    TypedefName<string> obj;
}
}  // namespace jj34
//----------------------------------------------------
namespace jj35 {

class Foo {
public:
    int _i;

    Foo(const int& i) : _i(i) { cout << "Foo(i) \n"; }
    Foo(const Foo&) { cout << "Foo Copy Ctor \n"; }
    Foo& operator=(const Foo&)
    {
        cout << "Foo Copy Assignment \n";
        return *this;
    }
    ~Foo() { cout << "~Foo()" << endl; }
};

ostream& operator<<(ostream& os, const Foo& c)
{
    os << c._i << endl;
    return os;
}

struct D {  // a verbose array deleter:
    void operator()(int* p)
    {
        std::cout << "[deleter called]\n";
        delete[] p;
    }
};

void test35_shared_ptr()
{
    cout << "\ntest35_shared_ptr().......\n";

    cout << "sizeof(shared_ptr<string>) = " << sizeof(shared_ptr<string>) << endl;  // 8

    shared_ptr<Foo> sp1(new Foo(1));  // Foo(i)
    shared_ptr<Foo> sp2(sp1);

    list<shared_ptr<Foo>> lst;
    lst.push_back(sp2);
    lst.push_back(sp2);
    lst.push_back(sp2);
    //現在，sp1, sp2 and 3 elements of lst 共享同一個 Foo(1).
    cout << "sp1.use_count()=" << sp1.use_count() << endl;  // 5
    cout << "sp1.unique()=" << sp1.unique() << endl;        // 0

    for (auto& elem : lst) cout << (*elem)._i << endl;  // 1 1 1

    list<shared_ptr<Foo>>::iterator itr = lst.begin();  //取頭元素
    (*(*itr))._i = 9;                                   //改為 9
    for (auto& elem : lst) {
        cout << (*elem)._i << endl;  // 9 9 9 (沒做 copy-on-write 的後果 -- 只改 begin 卻影響了所有元素)
        cout << elem.use_count() << endl;  // 5 5 5
    }

    // reset : Replaces the managed object with an object pointed to by ptr.
    //   	  If the object pointed to by ptr is already owned, the function results in undefined behavior.
    // reset(): Releases the ownership of the managed object, if any. After the call, *this manages no object.
    //          Equivalent to shared_ptr().swap(*this);
    // reset(Y* ptr): Uses the delete expression as the deleter. A valid delete expression must be available,
    //          i.e. delete ptr must be well formed, have well-defined behavior and not throw any exceptions.
    //          Effectively calls shared_ptr<T>(ptr).swap(*this);.
    (*itr).reset(new Foo(3));  // Foo(i) 	//目前 itr 指向頭元素；重置頭元素令其指向 Foo(3)
    for (auto& elem : lst) {
        cout << (*elem)._i << endl;        // 3 9 9
        cout << elem.use_count() << endl;  // 1 4 4
    }

    lst.clear();                 //~Foo() ==> 這是頭元素 (unique) 被清除後喚起 Foo Dtor.
    cout << lst.size() << endl;  // 0
    cout << "sp1.use_count()=" << sp1.use_count() << endl;  // 2
    cout << "done \n";

    //嘗試安裝 deleter (should be a callable entity)
    shared_ptr<int> sp3(new int[10], D());

    //離開 scope.
    //[deleter called] ==> 這是 sp3 被清除而喚起 deleter
    //~Foo() ==> 這是 sp1 (and sp2) 被清除而喚起 Foo Dtor.
}
}  // namespace jj35
//----------------------------------------------------
namespace jj36 {
class T {
public:
    T() : intArray{6, 5, 4, 3, 2, 1}
    {  //[Warning] list-initializer for non-class type must not be parenthesized
    }  //上述, 可拿掉小括弧 (已測試ok)
    print()
    {
        for (auto& elem : intArray) cout << elem << ' ';
        cout << endl;
    }

private:
    int intArray[6];
};

void test36_array_initialized_in_ctor()
{
    cout << "\ntest36_array_initialized_in_ctor().......\n";

    T t;
    t.print();  // 6 5 4 3 2 1
}
}  // namespace jj36
//----------------------------------------------------
namespace jj37 {
struct Base {
    virtual void vfunc(float) {}
};
struct Derived1 : Base {
    virtual void vfunc(int) {}
    // accidentally create a new virtual function, when one intended to override a base class function.
    // This is a common problem, particularly when a user goes to modify the base class.
};

struct Derived2 : Base {
    //! virtual void vfunc(int) override { }
    //[Error] 'virtual void Derived2::vfunc(int)' marked override, but does not override
    // override means that the compiler will check the base class(es) to see
    //   if there is a virtual function with this exact signature.
    //   And if there is not, the compiler will indicate an error.
    virtual void vfunc(float) override {}
};

struct BaseX final {
};

//! struct DerivedX : BaseX { };
//[Error] cannot derive from 'final' base 'BaseX' in derived type 'DerivedX'

struct BaseY {
    virtual void f() final;
};

struct DerivedY : BaseY {
    //! void f(); 	//[Error] overriding final function 'virtual void BaseY::f()'
};

void test37_override_final() { cout << "\ntest37_override_final().......\n"; }
}  // namespace jj37
//----------------------------------------------------
// http://www.cplusplus.com/reference/functional/bind/?kw=bind
#include <functional>  // std::bind
namespace jj41 {
// a function: (also works with function object ==> std::divides<double> my_divide;)
double my_divide(double x, double y) { return x / y; }

struct MyPair {
    double a, b;
    double multiply() { return a * b; }  // member function 其實有個 argument: this
};

void test41_bind()
{
    cout << "\ntest41_bind().......\n";

    using namespace std::placeholders;  // adds visibility of _1, _2, _3,...

    // binding functions:
    auto fn_five = bind(my_divide, 10, 2);  // returns 10/2
    cout << fn_five() << '\n';              // 5

    auto fn_half = bind(my_divide, _1, 2);  // returns x/2
    cout << fn_half(10) << '\n';            // 5

    auto fn_invert = bind(my_divide, _2, _1);  // returns y/x
    cout << fn_invert(10, 2) << '\n';          // 0.2

    auto fn_rounding = bind<int>(my_divide, _1, _2);  // returns int(x/y)
    cout << fn_rounding(10, 3) << '\n';               // 3

    // binding members:
    MyPair ten_two{10, 2};
    // member function 其實有個 argument: this
    auto bound_memfn = bind(&MyPair::multiply, _1);  // returns x.multiply()
    cout << bound_memfn(ten_two) << '\n';            // 20

    auto bound_memdata = bind(&MyPair::a, ten_two);  // returns ten_two.a
    cout << bound_memdata() << '\n';                 // 10

    auto bound_memdata2 = bind(&MyPair::b, _1);  // returns x.b
    cout << bound_memdata2(ten_two) << '\n';     // 2

    vector<int> v{15, 37, 94, 50, 73, 58, 28, 98};
    int n = count_if(v.cbegin(), v.cend(), not1(bind2nd(less<int>(), 50)));
    cout << "n= " << n << endl;  // n= 5

    auto fn_ = bind(less<int>(), _1, 50);
    cout << count_if(v.cbegin(), v.cend(), fn_) << endl;                      // 3
    cout << count_if(v.begin(), v.end(), bind(less<int>(), _1, 50)) << endl;  // 3
    //好像很難以 bind 替換 not and compose

    //! cout << count_if(v.cbegin(), v.cend(), not1(fn_)) << endl;  //[ERROR]
}
}  // namespace jj41
//---------------------------------------------------
// came from Boose.bind test.
#include <functional>  // std::bind
namespace jj42 {
class button {
public:
    std::function<void(int)> onClick;
};

class player {
public:
    void play(void* sender, int param)
    {
        printf("Play: %d => %d\n", (int)sender, param);
        cout << "Play: " << sender << " => " << param << endl;
    };
    void stop(void* sender, int param)
    {
        printf("Stop: %d => %d\n", (int)sender, param);
        cout << "Stop: " << sender << " => " << param << endl;
    };
};

button playButton, stopButton;
player thePlayer;

void connect()
{
    playButton.onClick = std::bind(&player::play, &thePlayer, &playButton, std::placeholders::_1);
    stopButton.onClick = std::bind(&player::stop, &thePlayer, &stopButton, std::placeholders::_1);
}
//----------
void test42_bind_and_function()
{
    cout << "\ntest42_bind_and_function().......\n";

    connect();
    playButton.onClick(0);  // Play: 4964384 => 0
                            // Play: 0x4bc020 => 0
    stopButton.onClick(7);  // Stop: 4964400 => 7
                            // Stop: 0x4bc030 => 7
}
}  // namespace jj42
//---------------------------------------------------
namespace jj45 {
class Foo {
public:
    Foo(int i) : _i(i) {}
    Foo() = default;  //於是和上一個並存 (ctor 可以多個並存)

    Foo(const Foo& x) : _i(x._i) {}
    //! Foo(const Foo&) = default;	//[Error] 'Foo::Foo(const Foo&)' cannot be overloaded
    //! Foo(const Foo& x) = delete;

    Foo& operator=(const Foo& x)
    {
        _i = x._i;
        return *this;
    }
    //! Foo& operator=(const Foo& x) = default;	//[Error] 'Foo& Foo::operator=(const Foo&)' cannot be overloaded
    //! Foo& operator=(const Foo& x) = delete;	//[Error] 'Foo& Foo::operator=(const Foo&)' cannot be overloaded

    //! void func1() = default;	//[Error] 'void jj45::Foo::func1()' cannot be defaulted
    void func2() = delete;

    //! ~Foo() = delete; //這會造成使用 Foo object 時出錯 => [Error] use of deleted function 'jj45::Foo::~Foo()'
    ~Foo() = default;

private:
    int _i;
};

void test45_default_and_delete()
{
    cout << "\ntest45_default_and_delete().......\n";

    Foo f1(5);
    Foo f2;      //如果沒有寫出 =default 版本 ==> [Error] no matching function for call to 'Foo::Foo()'
    Foo f3(f1);  // copy ctor   (如果 copy ctor = delete; [Error] use of deleted function 'Foo::Foo(const Foo&)')
    f3 = f2;     // copy assign (如果 copy ctor = delete; [Error] use of deleted function 'Foo& Foo::operator=(const
                 // Foo&)')
}
}  // namespace jj45
//---------------------------------------------------
class MyString {
};
class MyStrNoMove {
};

namespace jj47 {

template <typename T>
void output_static_data(const T& myStr)
{
    // output...
}

template <typename Container>
void test_moveable(Container c)
{
    typedef typename iterator_traits<typename Container::iterator>::value_type Valtype;
    //下面這樣其實也可以：(建立在對標準庫容器的了解--每個容器都有 value_type)
    // typedef typename Container::value_type  Valtype;

    for (long i = 0; i < 100; ++i) c.insert(c.end(), Valtype());

    output_static_data(*(c.begin()));

    Container c1(c);
    Container c2(std::move(c));
    c1.swap(c2);
}
//--------------
template <typename T,
          template <class>  //==> template <class T> 可. template <typename T> 亦可
          class Container   //==> typename Container 不可
          >
class XCls {
private:
    Container<T> c;

public:
    XCls()
    {
        for (long i = 0; i < 100; ++i) c.insert(c.end(), T());

        output_static_data(T());

        Container<T> c1(c);
        Container<T> c2(std::move(c));
        c1.swap(c2);
    }
};

//-----------------
template <typename T>
using Vec = vector<T, allocator<T>>;  //不可在 function 內宣告
template <typename T>
using Lst = list<T, allocator<T>>;
template <typename T>
using Deq = deque<T, allocator<T>>;

void test47_template_template_param_vs_iterator_traits()
{
    cout << "\ntest47_template_template_param_vs_iterator_traits().......\n";

    test_moveable(list<MyString>());
    test_moveable(list<MyStrNoMove>());

    test_moveable(vector<MyString>());
    test_moveable(vector<MyStrNoMove>());

    test_moveable(deque<MyString>());
    test_moveable(deque<MyStrNoMove>());

    //! test_moveable(multiset<MyString>(),multiset<MyStrNoMove>(), value);
    //  暫時不測 set, 因為它需要 operator<

    //! test_moveable(unordered_multiset<MyString>(),unordered_multiset<MyStrNoMove>(), value);
    //  暫時不測 unordered_multiset, 因為它需要 operator<, operator==, hash function
    //  (完整 MyString 見 ...\handout\...\Test-STL\TestSTL.cpp

    XCls<MyString, Vec> c1;
    XCls<MyStrNoMove, Vec> c2;

    XCls<MyString, Lst> c3;
    XCls<MyStrNoMove, Lst> c4;

    XCls<MyString, Deq> c5;
    XCls<MyStrNoMove, Deq> c6;
}
}  // namespace jj47
//---------------------------------------------------
// http://en.cppreference.com/w/cpp/language/type_alias
namespace jj48 {

// type alias, identical to
// typedef void (*func)(int, int);
// 所以using是c++2.0的新特性啊！
using func = void (*)(int, int);

// the name 'func' now denotes a pointer to function:
void example(int, int) {}
func fn = example;

// type alias used to hide a template parameter
template <class CharT>
using mystring = std::basic_string<CharT, std::char_traits<CharT>>;
mystring<char> str;
//其實在 <string>, <string_fwd.h> 都有以下 typedef
// typedef basic_string<char>   string;

// type alias can introduce a member typedef name
template <typename T>
struct Container {
    using value_type = T;
};
// which can be used in generic programming
template <typename Container>
void fn2(const Container& c)
{
    typename Container::value_type n;
}

void test48_type_alias()
{
    cout << "\ntest48_type_alias().......\n";

    Container<int> c;
    fn2(c);  // Container::value_type will be int in this function
}
}  // namespace jj48
//---------------------------------------------------
namespace jj50 {
void test50_hash()
{
    cout << "\ntest50_test_hash()......\n";

    void* pi = (void*)(new int(100));
    cout << hash<int>()(123) << endl;               // 123
    cout << hash<long>()(123L) << endl;             // 123
    cout << hash<string>()(string("Ace")) << endl;  // 1765813650
    cout << hash<const char*>()("Ace") << endl;     // 5075396
    cout << hash<char>()('A') << endl;              // 65
    cout << hash<float>()(3.141592653) << endl;     // 1294047319
    cout << hash<double>()(3.141592653) << endl;    // 3456236477
    cout << hash<void*>()(pi) << endl;              // 4079456
}
}  // namespace jj50

//---------------------------------------------------
int main(int argc, char** argv)
{
    cout << "c++ version " << __cplusplus << endl;

    // space in template expression
    vector<vector<int>> vv;  // before C++11 ==> [Error] '>>' should be '> >' within a nested template argument list

    jj00::test00_nullptr();

    jj01::test01_auto();

    jj02::test02_uniform_initializer();

    jj03::test03_initializer_list();

    jj04::test04_explicit_multiple_argument_ctor();

    jj05::test05_range_based_for();

    jj06::test06_lambda();

    jj07::test07_tuples();

    jj09::test09_array();

    jj10::test10_forward_list();

    jj11::test11_unordered_set();

    jj12::test12_Rvalue_Move();

    jj15::test15_variadic_template();

    jj18::test18_decltype(map<string, complex<long>>());

    jj18::test18_decltype(deque<double>());

    jj18::test18_decltype(string());

    //! jj18::test18_decltype(complex<int>());	//這會導致 function template 編譯失敗

    jj19::test19_default_delete();

    jj23::test23_traits_for_string();

    jj24::test24_traits_for_Foo();

    jj25::test25_traits_for_Goo();

    jj26::test26_traits_for_Zoo();

    jj27::test27_Rvalue_reference();

    jj291::test291_perfect_forward();

    jj301::test301_move_semantics_with_noexcept();

    jj31::test31_containers_size();

    jj34::test34_alias_template();

    jj35::test35_shared_ptr();

    jj36::test36_array_initialized_in_ctor();

    jj37::test37_override_final();

    jj41::test41_bind();

    jj42::test42_bind_and_function();

    jj45::test45_default_and_delete();

    jj47::test47_template_template_param_vs_iterator_traits();

    jj48::test48_type_alias();

    jj50::test50_hash();

    return 0;
}