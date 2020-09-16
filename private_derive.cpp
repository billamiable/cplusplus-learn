#include <iostream>
#include <vector>

using namespace std;

class Vector2Stack
{
public:
    void push(int x)
    {
        array.push_back(x);
    }
    int pop(){
        if(array.size() == 0)
        {
            cout << "Empty stack!" << endl;
            return INT_MIN;
        }
        else
        {
            int len = array.size();
            int res = array[len-1];
            array.pop_back();
            return res;
        }
    }

private:
    vector<int> array;
};


int main() {
    Vector2Stack vs;
    vs.push(1);
    vs.push(2);
    vs.push(3);
    cout << vs.pop() << endl;
    cout << vs.pop() << endl;
    cout << vs.pop() << endl;
    cout << vs.pop() << endl;
    return 0;
}