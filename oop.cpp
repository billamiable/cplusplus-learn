#include <iostream>
#include <vector>
using namespace std;

//----------------------------------------------------
// This Pointer by Sizeof
//----------------------------------------------------
namespace yj01
{
class Shape
{
public:
    virtual void draw() = 0; // pure virtual
    // sizeof可以针对
    virtual int Sizeof() { return sizeof(Shape); } // 可以改成sizeof(*this)
};

class Rect: public Shape
{
public:
    virtual void draw() { cout << "Rect" << endl; };
    virtual int Sizeof() { return sizeof(Rect); } // 可以改成sizeof(*this)
private:
    int length;
    int height;
};

class Square: public Rect
{
public:
    virtual void draw() { cout << "Square" << endl; };
    virtual int Sizeof() { return sizeof(Square); } // 可以改成sizeof(*this)
};

class Ellipse: public Shape
{
public:
    virtual void draw() { cout << "Ellipse" << endl; };
    virtual int Sizeof() { return sizeof(Ellipse); } // 可以改成sizeof(*this)
private:
    int x,y;
    int r1,r2;
};

class Circle: public Ellipse
{
public:
    virtual void draw() { cout << "Circle" << endl; };
    virtual int Sizeof() { return sizeof(Circle); } // 可以改成sizeof(*this)
};

void drawAll(const vector<Shape*>& v)
{
    for(int i=0; i<v.size(); ++i) {
        // 注意：sizeof看的是static type，即最初定义的
        // 而自己写的虚函数Sizeof是看dynamic type，即upcast后的类型
        // 这个还挺牛逼的，感觉很容易弄错！
        // 看起来dynamic type和static type主要就是用来实现多态的
        // TODO: 与dynamic_cast, static_cast有关？后面自己实现下
        cout << sizeof( *v[i] ) << " "; // 这个就是Shape类，静态绑定，都是8
        cout << v[i]->Sizeof() << " ";  // 这个就是各个派生类的实际size，动态绑定
        v[i]->draw();                   // 打印派生类的draw内容
    }
}

void test01_this_pointer_by_sizeof()
{
    Rect r1;
    Square q1;

    r1.draw(); // Rect
    q1.draw(); // Square

    Rect* pr = new Rect;
    pr->draw(); // Rect
    delete pr;

    // 大小为何是8，有2个虚指针？
    cout << sizeof(Shape) << endl;   // 8
    cout << sizeof(Rect) << endl;    // 16，多了两个数据
    cout << sizeof(Square) << endl;  // 16，同上
    cout << sizeof(Ellipse) << endl; // 24，多了四个数据
    cout << sizeof(Circle) << endl;  // 24，同上

    vector<Shape*> v;
    v.push_back(&r1);
    v.push_back(&q1);
    v.push_back(new Circle);
    v.push_back(new Ellipse);
    drawAll(v);
}

}

int main(int argc, char** argv) 
{
    cout << "c++ version " << __cplusplus << endl;

    yj01::test01_this_pointer_by_sizeof();
}