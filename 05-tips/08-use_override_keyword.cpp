// 仮想関数をオーバーライドするときは`override`を付ける癖を持とう(C++11～)
#include <iostream>
#include <memory>

class Base
{
public:
    virtual void f()
    {
        std::cout << "Base class\n";
    }
};

namespace Bad
{
    class Derived : public Base
    {
    public:
        virtual void f() const // 実は別の関数に...
        {
            std::cout << "Derived class\n";
        }
    };
}

namespace Good
{
    class Derived : public Base
    {
    public:
        // virtual void f() const override と書くとコンパイルエラーにしてくれる
        virtual void f() override
        {
            std::cout << "Derived class (overridden)\n";
        }
    };
}

int main()
{
    {
        Bad::Derived obj{};
        obj.f(); // → Derived class

        std::unique_ptr<Base> ptr = std::make_unique<Bad::Derived>();
        // Base *ptr = new Bad::Derived(); でもOK
        ptr->f(); // → Base class (!?)
    }

    {
        Good::Derived obj{};
        obj.f(); // → Derived class (overridden)

        std::unique_ptr<Base> ptr = std::make_unique<Good::Derived>();
        // Base *ptr = new Good::Derived(); でもOK
        ptr->f(); // → Derived class (overridden) (yay!)
    }

    return 0;
}