#include <iostream>
#include <vector>
#include <string>
#include <any>
#include <cassert>
#include <typeinfo>
#include <list>
using namespace std;

//----------------------------------------------------
// Draw using Polymorphism
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

void test01_draw_using_polymorphism()
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
// Draw not using Polymorphism
//----------------------------------------------------
namespace yj02
{
class Shape
{
public:
    void draw() { cout << "Shape" << endl; }
};

class Rect
{
public:
    void draw() { cout << "Rect" << endl; };
private:
    int length;
    int height;
};

class Square
{
public:
    void draw() { cout << "Square" << endl; };
private:
    int length;
    int height;
};

class Ellipse
{
public:
    void draw() { cout << "Ellipse" << endl; };
private:
    int x,y;
    int r1,r2;
};

class Circle
{
public:
    void draw() { cout << "Circle" << endl; };
private:
    int x,y;
    int r1,r2;
};

void drawAll(const vector<any>& v)
{
    for (auto& i : v) {
        if (i.type() == typeid(Rect))
            any_cast<Rect>(i).draw();
        else if (i.type() == typeid(Square))
            any_cast<Square>(i).draw();
        else if (i.type() == typeid(Ellipse))
            any_cast<Ellipse>(i).draw();
        else if (i.type() == typeid(Circle))
            any_cast<Circle>(i).draw();
    }
}

void test02_draw_not_using_polymorphism()
{
    Rect r1;
    Square q1;

    r1.draw(); // Rect
    q1.draw(); // Square

    Rect* pr = new Rect;
    pr->draw(); // Rect
    delete pr;

    cout << sizeof(Shape) << endl;   // 1
    cout << sizeof(Rect) << endl;    // 8，多了两个数据
    cout << sizeof(Square) << endl;  // 8，同上
    cout << sizeof(Ellipse) << endl; // 16，多了四个数据
    cout << sizeof(Circle) << endl;  // 16，同上

    vector<any> v;
    // 注意下面不再传指针了，而是对象
    v.push_back(r1);
    v.push_back(q1);
    v.push_back(Circle());
    v.push_back(Ellipse());
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
    // 这里strcpy相当于保存了字符串名称
    Document(const char* fn) { strcpy(name, fn); }
    virtual void Open() = 0;
    virtual void Close() = 0;
    char* GetName() { return name; }
private:
    // 这里存的是"foo"和"bar"这两个name
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
    void NewDocument(const char* name)
    {
        cout << "App: NewDocument()" << endl;
        // Framework calls the "hole" reserved
        _docs[_index] = CreateDocument(name);
        _docs[_index++]->Open(); // 先执行_index的处理，再++
    }

    // 这里之所以可以调用到是因为父类存下来了MyDoc的对象
    void ReportDocs()
    {
        cout << "App: ReportDocs()" << endl;
        for (int i=0; i<_index; ++i)
            cout << "  " << _docs[i]->GetName() << endl;
    }

    // Framework declares a "hole" for the client
    virtual Document* CreateDocument(const char*) = 0;
private:
    int _index;
    // Franework uses Document's base class
    Document* _docs[20];
};

// Concrete derived class defined by client
class MyDoc : public Document
{
public:
    MyDoc(const char* fn): Document(fn) {}
    virtual void Open()
    {
        cout << "  MyDoc: Open()" << endl;
    }
    virtual void Close()
    {
        cout << "  MyDoc: Close()" << endl;
    }
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
    Document* CreateDocument(const char* fn)
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
    // 然后由于父类无法直接获得子类创建的class名称（在这里是MyDoc），其实这里对象都存了？
    // 要想获得则需要通过虚函数、指针、向上转型三要素
    // 即定义虚函数CreateDocument，用Document*指针，将MyDoc的子类指针向上转型成了Document*
    // 从而最后在父类中将子类创建的MyDoc对象存在_docs数组里
    myApp.NewDocument("foo");
    myApp.NewDocument("bar");
    // 针对存下来的东西进行后续的处理
    myApp.ReportDocs();
}


}

//----------------------------------------------------
// Any Reimplementation
//----------------------------------------------------
namespace jj08
{
class myAny
{
public:
    class placeholder
    {
    public:
        virtual ~placeholder()
        {
            cout << "placeholder dtor" << endl;
        }
        // virtual const type_info
        virtual placeholder* clone() const = 0;
    };
    template<typename T>
    class holder : public placeholder
    {
    public:
        holder(const T& value): held(value)
        {
            cout << "holder ctor" << endl;
        }

        // 看起来是一个深拷贝，但最后是父类指针，指向子类对象
        virtual placeholder* clone() const
        {
            cout << "holder clone" << endl;
            return new holder(held);
        }

        T held; // 真正的资料保存在这里
    };

public:
    myAny() : content(NULL)
    {
        cout << "myAny ctor" << endl;
    }

    // template ctor，参数可以为任意的type，真正资料保存在content里
    template<typename T>
    myAny(const T& value): content(new holder<T>(value))
    {
        cout << "myAny template ctor" << endl;
    }

    // copy ctor，0就代表了NULL指针
    myAny(const myAny& other): content(other.content ? other.content->clone() : 0)
    {
        cout << "myAny copy ctor" << endl;
    }

    ~myAny()
    {
        cout << "myAny dtor" << endl;
        if (content != NULL)
            delete content;
    }
private:
    placeholder* content;
};

typedef list<myAny> list_any;

void fill_list(list_any& la)
{
    // TODO: 单个的输出就很多，为啥呢？
    la.push_back(10);
    // la.push_back(string("I am a string"));
    // const char* p = "I am char arry";
    // la.push_back(p);
}

void test08_any_reimplementation()
{
    list_any la;
    fill_list(la);
}

}

int main(int argc, char** argv) 
{
    cout << "c++ version " << __cplusplus << endl;

    yj01::test01_draw_using_polymorphism();

    yj02::test02_draw_not_using_polymorphism();

    jj07::test07_factory_method();

    jj08::test08_any_reimplementation();
}