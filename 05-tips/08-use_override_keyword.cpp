// 仮想関数をオーバーライドするときは`override`を付ける癖を持とう
#include <iostream>
#include <memory>

class Base
{
public:
    virtual void f()
    {
        std::cout << "Base class default behaviour\n";
    }
};

namespace Bad
{
    class Derived : public Base
    {
    public:
        virtual void f() const
        {
            std::cout << "Derived class overridden behaviour\n";
        }
    };
}

namespace Good
{
    class Derived : public Base
    {
    public:
        // virtual void f() const override
        virtual void f() override
        {
            std::cout << "Derived class overridden behaviour\n";
        }
    };
}

int main()
{
    auto ptr = std::make_unique<Bad::Derived>();

    ptr->f();

    return 0;
}