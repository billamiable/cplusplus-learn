#include <iostream>
#include <vector>
// 注意下面两个没关系，string是针对字符串，cstring/string.h是针对字符数组
#include <string>
#include <cstring>
#include <any>
#include <cassert>
#include <typeinfo>
#include <list>
#include <algorithm>
using namespace std;

//----------------------------------------------------
// Factory Method
//----------------------------------------------------
namespace jj07 {
// Abstract base class declared by framework
class Document {
public:
    // 通过strcpy把输入的字符串保存了下来
    Document(const char* fn) { strcpy(name, fn); }
    virtual void Open() = 0;
    virtual void Close() = 0;
    char* GetName() { return name; }

private:
    // 在后面的例子中，这里存了"foo"和"bar"这两个name
    char name[20];
};

// Framework declaration
class App {
public:
    App() : _index(0) { cout << "App: ctor" << endl; }

    // THe client will call this "entry point" of the framework
    void NewDocument(const char* name)
    {
        cout << "App: NewDocument()" << endl;
        // Framework calls the "hole" reserved
        _docs[_index] = CreateDocument(name);
        _docs[_index++]->Open();  // 先执行_index的处理，再++
    }

    // 这里之所以可以调用到是因为父类存下来了MyDoc的对象
    void ReportDocs()
    {
        cout << "App: ReportDocs()" << endl;
        for (int i = 0; i < _index; ++i) cout << "  " << _docs[i]->GetName() << endl;
    }

    // Framework declares a "hole" for the client
    virtual Document* CreateDocument(const char*) = 0;

private:
    int _index;
    // Franework uses Document's base class
    Document* _docs[20];
};

// Concrete derived class defined by client
class MyDoc : public Document {
public:
    MyDoc(const char* fn) : Document(fn) {}
    virtual void Open() { cout << "  MyDoc: Open()" << endl; }
    virtual void Close() { cout << "  MyDoc: Close()" << endl; }
};

// Customization of framework defined
class MyApp : public App {
public:
    MyApp() { cout << "  MyApp: ctor" << endl; }
    // CLient defines Framework's hole
    Document* CreateDocument(const char* fn)
    {
        cout << "  MyApp: CreateDocument()" << endl;
        return new MyDoc(fn);
    }
};

void test07_factory_method()
{
    cout << "\n----------------------------------------------------------\n";
    cout << "test07_factory_method()..........";
    cout << "\n----------------------------------------------------------\n";

    // Client's customization
    // 创建对象时，会先调用父类的构造函数，所谓从里到外的构造才坚实
    MyApp myApp;

    // 由于子类继承了所有父类函数的调用权，包括NewDocument，因此可以直接调用
    // 然后由于父类无法直接获得子类创建的class名称（在这里是MyDoc）
    // 要想获得则需要通过虚函数、指针、向上转型的三要素
    // 即定义虚函数CreateDocument，用Document*指针，将MyDoc的子类指针向上转型成了Document*类型
    // 从而最后在父类中将子类创建的MyDoc对象存在_docs数组里
    myApp.NewDocument("foo");
    myApp.NewDocument("bar");
    // 针对存下来的东西进行后续的处理
    myApp.ReportDocs();
}

}  // namespace jj07

//----------------------------------------------------
// Any Reimplementation
//----------------------------------------------------
namespace jj08 {
class myAny {
public:
    class placeholder {
    public:
        virtual ~placeholder() { cout << "placeholder dtor" << endl; }
        // virtual const type_info
        virtual placeholder* clone() const = 0;
    };
    template <typename T>
    class holder : public placeholder {
    public:
        holder(const T& value) : held(value) { cout << "holder ctor" << endl; }

        // 看起来是一个深拷贝，但最后是父类指针，指向子类对象
        virtual placeholder* clone() const
        {
            cout << "holder clone" << endl;
            return new holder(held);
        }

        T held;  // 真正的资料保存在这里
    };

public:
    myAny() : content(NULL) { cout << "myAny ctor" << endl; }

    // template ctor，参数可以为任意的type，真正资料保存在content里
    template <typename T>
    myAny(const T& value) : content(new holder<T>(value))
    {
        cout << "myAny template ctor" << endl;
    }

    // copy ctor，0就代表了NULL指针
    myAny(const myAny& other) : content(other.content ? other.content->clone() : 0)
    {
        cout << "myAny copy ctor" << endl;
    }

    ~myAny()
    {
        cout << "myAny dtor" << endl;
        if (content != NULL) delete content;
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
    cout << "\n----------------------------------------------------------\n";
    cout << "test08_any_reimplementation()..........";
    cout << "\n----------------------------------------------------------\n";

    list_any la;
    fill_list(la);
}

}  // namespace jj08

//----------------------------------------------------
// Draw using Polymorphism
//----------------------------------------------------
namespace yj01 {
class Shape {
public:
    virtual void draw() = 0;  // pure virtual
    // sizeof可以针对
    virtual int Sizeof() { return sizeof(Shape); }  // 可以改成sizeof(*this)
};

class Rect : public Shape {
public:
    virtual void draw() { cout << "Rect" << endl; };
    virtual int Sizeof() { return sizeof(Rect); }  // 可以改成sizeof(*this)
private:
    int length;
    int height;
};

class Square : public Rect {
public:
    virtual void draw() { cout << "Square" << endl; };
    virtual int Sizeof() { return sizeof(Square); }  // 可以改成sizeof(*this)
};

class Ellipse : public Shape {
public:
    virtual void draw() { cout << "Ellipse" << endl; };
    virtual int Sizeof() { return sizeof(Ellipse); }  // 可以改成sizeof(*this)
private:
    int x, y;
    int r1, r2;
};

class Circle : public Ellipse {
public:
    virtual void draw() { cout << "Circle" << endl; };
    virtual int Sizeof() { return sizeof(Circle); }  // 可以改成sizeof(*this)
};

void drawAll(const vector<Shape*>& v)
{
    for (int i = 0; i < v.size(); ++i) {
        // 注意：sizeof看的是static type，即最初定义的
        // 而自己写的虚函数Sizeof是看dynamic type，即upcast后的类型
        // 这个还挺牛逼的，感觉很容易弄错！
        // 看起来dynamic type和static type主要就是用来实现多态的
        // TODO: 与dynamic_cast, static_cast有关？后面自己实现下
        cout << sizeof(*v[i]) << " ";   // 这个就是Shape类，静态绑定，都是8
        cout << v[i]->Sizeof() << " ";  // 这个就是各个派生类的实际size，动态绑定
        v[i]->draw();                   // 打印派生类的draw内容
    }
}

void test01_draw_using_polymorphism()
{
    cout << "\n----------------------------------------------------------\n";
    cout << "test01_draw_using_polymorphism()..........";
    cout << "\n----------------------------------------------------------\n";

    Rect r1;
    Square q1;

    r1.draw();  // Rect
    q1.draw();  // Square

    // TODO: 换成智能指针
    Rect* pr = new Rect;
    pr->draw();  // Rect
    delete pr;

    // 大小为何是8，有2个虚指针？
    cout << sizeof(Shape) << endl;    // 8
    cout << sizeof(Rect) << endl;     // 16，多了两个数据
    cout << sizeof(Square) << endl;   // 16，同上
    cout << sizeof(Ellipse) << endl;  // 24，多了四个数据
    cout << sizeof(Circle) << endl;   // 24，同上

    vector<Shape*> v;
    v.push_back(&r1);
    v.push_back(&q1);
    v.push_back(new Circle);
    v.push_back(new Ellipse);
    drawAll(v);
}

}  // namespace yj01

//----------------------------------------------------
// Draw not using Polymorphism
//----------------------------------------------------
namespace yj02 {
class Shape {
public:
    void draw() { cout << "Shape" << endl; }
};

class Rect {
public:
    void draw() { cout << "Rect" << endl; };

private:
    int length;
    int height;
};

class Square {
public:
    void draw() { cout << "Square" << endl; };

private:
    int length;
    int height;
};

class Ellipse {
public:
    void draw() { cout << "Ellipse" << endl; };

private:
    int x, y;
    int r1, r2;
};

class Circle {
public:
    void draw() { cout << "Circle" << endl; };

private:
    int x, y;
    int r1, r2;
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
    cout << "\n----------------------------------------------------------\n";
    cout << "test02_draw_not_using_polymorphism()..........";
    cout << "\n----------------------------------------------------------\n";

    Rect r1;
    Square q1;

    r1.draw();  // Rect
    q1.draw();  // Square

    Rect* pr = new Rect;
    pr->draw();  // Rect
    delete pr;

    cout << sizeof(Shape) << endl;    // 1
    cout << sizeof(Rect) << endl;     // 8，多了两个数据
    cout << sizeof(Square) << endl;   // 8，同上
    cout << sizeof(Ellipse) << endl;  // 16，多了四个数据
    cout << sizeof(Circle) << endl;   // 16，同上

    vector<any> v;
    // 注意下面不再传指针了，而是对象
    v.push_back(r1);
    v.push_back(q1);
    v.push_back(Circle());
    v.push_back(Ellipse());
    drawAll(v);
}

}  // namespace yj02


//----------------------------------------------------
// Design Pattern: Observer
//----------------------------------------------------
namespace dp02 {
// 这里感觉啥也没有，只有一个update的纯虚函数，实现都在子类
class Observer {
public:
    // 一般来说，虚析构函数是用于继承体系里，指向子类的父类指针在被delete时
    // 如果父类没有虚析构函数，就只会调用父类的析构函数，而不会调用子类的析构函数
    // 从而如果子类用了指针数据，就会造成内存泄露
    // 因此，常用的做法就是在父类里都定义一个虚析构函数，从而释放子类的内存
    // 这里其实是可以不用加的，因为没有指针数据
    virtual ~Observer() { cout << "observer dctor" << endl; };
    virtual void update(int value) = 0;
};

// 这个的顺序比较重要！不然就会找不到，把父类都放在最前面
// 这个在实际使用时还可以派生一些子类来使用
class Subject {
public:
    // 这里讲道理都是要加虚析构函数，因为里面有指针对象
    virtual ~Subject() { cout << "subject dctor" << endl; };
    void attach(Observer* obs)
    {
        cout << "attach..." << endl;
        views_.push_back(obs);
    }
    void detach(Observer* obs)
    {
        cout << "detach..." << endl;
        remove(views_.begin(), views_.end(), obs);
    }
    void set_value(int value)
    {
        cout << "set value" << endl;
        // 这里set value相当于改的是全局的
        // 因为value_会在notify函数中用于update observer里的数据
        value_ = value;
        // 下面这个函数其实是可以直接写在这里的
        notify();
    }
    void notify()
    {
        cout << "notify..." << endl;
        for (int i = 0; i < views_.size(); ++i) views_[i]->update(value_);
    }

private:
    int value_;
    // 符合strategy设计模式
    // 本质都是类中有指针指向另一个父类
    // 该父类可以有许多派生的子类
    // 由于这里存了父类指针指向子类对象，即可以很方便使用多态特性
    // 从而对子类进行改动
    vector<Observer*> views_;
};

class Observer1 : public Observer {
public:
    // 这样的写法比较清晰，在Observer的对象里不再传subject指针或者对象
    Observer1(int div) : div_(div) { cout << "observer1 ctor" << endl; }
    ~Observer1() { cout << "observer1 dtor" << endl; }
    virtual void update(int v)
    {
        cout << "update in observer1" << endl;
        div_ = v;
    }
    int get_value() { return div_; }

private:
    int div_;
};

void test02_observer()
{
    cout << "\n----------------------------------------------------------\n";
    cout << "test02_observer()..........";
    cout << "\n----------------------------------------------------------\n";

    Subject subj;
    Observer1 o1(4);
    Observer1 o2(3);

    subj.attach(&o1);
    subj.attach(&o2);

    cout << "Observer 1 value " << o1.get_value() << endl;
    cout << "Observer 2 value " << o2.get_value() << endl;

    subj.set_value(1);

    // set后两个observer的value都会变成1
    cout << "Observer 1 value " << o1.get_value() << endl;
    cout << "Observer 2 value " << o2.get_value() << endl;

    subj.detach(&o1);

    cout << "Detached o1" << endl;
}

}  // namespace dp02

//----------------------------------------------------
// Design Pattern: Composite
//----------------------------------------------------
namespace dp03 {
class Component  // lowest common
{
public:
    // 父类定义一个子类都可以用的纯虚函数
    virtual void traverse() = 0;
};

class Leaf : public Component {
private:
    int value;

public:
    Leaf(int val) : value(val) {}
    // 针对不同的结构输出不同的结果
    void traverse() { cout << value << " "; }
};

class Composite : public Component {
private:
    // 注意使用的是父类的指针，指向子类的对象
    // 因此这里既可以是Leaf，又可以是Composite
    vector<Component*> children;

public:
    void add(Component* ele) { children.push_back(ele); }
    void traverse()
    {
        int size = children.size();
        for (int i = 0; i < size; ++i) {
            children[i]->traverse();
        }
    }
};
void test03_composite()
{
    cout << "\n----------------------------------------------------------\n";
    cout << "test03_composite()..........";
    cout << "\n----------------------------------------------------------\n";

    Composite containers[4];
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 3; ++j) {
            containers[i].add(new Leaf(i * 3 + j));
        }
    }

    // 这里相当于把第2-4个container的内容拷贝一份在第1个container下面
    // 体现出一种composite特性，即leaf下面可以再有一个
    for (int i = 1; i < 4; ++i) {
        containers[0].add(&(containers[i]));
    }

    // traverse并print结果
    for (int i = 0; i < 4; ++i) {
        containers[i].traverse();
        cout << endl;
    }
}
}  // namespace dp03

int main(int argc, char** argv)
{
    cout << "c++ version " << __cplusplus << endl;

    jj07::test07_factory_method();

    jj08::test08_any_reimplementation();

    yj01::test01_draw_using_polymorphism();

    yj02::test02_draw_not_using_polymorphism();

    dp02::test02_observer();

    dp03::test03_composite();
}