#include <iostream>
#include <chrono>
#include <thread>
using namespace std;

int main()
{
    for (int i = 0; i < 1000000; ++i) {
        int* q = new int;
        *q = i;
        // no delete
        cout << "new element created " << *q << endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    return 0;
}