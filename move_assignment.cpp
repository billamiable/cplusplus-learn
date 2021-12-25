#include <iostream>
using namespace std;
class A {
public:
    A() : num(new int(0)) {}
    // copy construct
    A(const A &a) : num(new int(*a.num)) {}
    // move construct
    A(A &&a) : num(a.num)
    {
        a.num = NULL;
        cout << "move construct!" << endl;
    }
    ~A() {}

private:
    int *num;
};
A get_A() { return A(); }
int main()
{
    A a = get_A();
    return 0;
}