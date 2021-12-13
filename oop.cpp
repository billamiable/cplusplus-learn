#include <iostream>
#include <vector>
#include <cstring>
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

//----------------------------------------------------
// Factory Method
//----------------------------------------------------
namespace jj07
{
// Abstract base class declared by framework
class Document
{
public:
    Document(char* fn) { strcpy(name, fn); }
private:
    char name[20];
};

// Framework declaration
class App
{
public:
    App(): _index(0)
    {
        cout << "App: ctor" << endl;
    }

    // THe client will call this "entry point" of the framework
    void NewDocument(char* name)
    {
        cout << "App: NewDocument()" << endl;
        // Framework calls the "hole" reserved
        _docs[_index] = CreateDocument(name);
        // _docs[_index++]->Open();
    }

    // Framework declares a "hole" for the client
    virtual Document* CreateDocument(char*) = 0;
private:
    int _index;
    // Franework uses Document's base class
    Document* _docs[20];
};

// Concrete derived class defined by client
class MyDoc : public Document
{
public:
    MyDoc(char* fn): Document(fn) {}
};

// Customization of framework defined
class MyApp : public App
{
public:
    MyApp()
    {
        cout << "  MyApp: ctor" << endl;
    }
    // CLient defines Framework's hole
    Document* CreateDocument(char* fn)
    {
        cout << "  MyApp: CreateDocument()" << endl;
        return new MyDoc(fn);
    }
};

void test07_factory_method()
{
    // Client's customization
    // 创建对象时，会先调用父类的构造函数，所谓从里到外的构造才坚实
    MyApp myApp;

    // 由于子类继承了所有父类函数的调用权，包括NewDocument，因此可以直接调用
    // 然后由于父类无法直接获得子类创建的class名称（在这里是MyDoc）
    // 要想获得则需要通过虚函数、指针、向上转型三要素
    // 即定义虚函数CreateDocument，用Document*指针，将MyDoc的子类指针向上转型成了Document*
    // 从而最后在父类中将子类创建的MyDoc对象存在_docs数组里
    myApp.NewDocument("foo");
    // myApp.NewDocument("bar");
}


}

int main(int argc, char** argv) 
{
    cout << "c++ version " << __cplusplus << endl;

    yj01::test01_this_pointer_by_sizeof();

    jj07::test07_factory_method();
}