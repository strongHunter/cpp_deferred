#include <iostream>

#include "deferred.h"

// class without default constructor
struct MyObject {
    int v;

    MyObject() = delete;

    MyObject(int i)
            : v(i)
    {}

    ~MyObject()
    {
        std::cout << "~MyObject" << std::endl;
    }

    void foo() const
    {
        std::cout << "value: " << v << std::endl;
    }
};

// Demonstration of using deferred as class field
class Resource {
    deferred<MyObject> obj;

public:
    Resource() noexcept = default;
    ~Resource() noexcept = default;

    void set(int n)
    {
        obj = MyObject(n);
    }

    void useObject()
    {
        obj->foo();
    }
};

int main()
{
    static_assert(sizeof(deferred<MyObject>) == sizeof(MyObject));

    std::cout << "Resource:" << std::endl;
    {
        Resource resource;
        // resource.useObject(); // Undefined behaviour!

        std::cout << "initialized" << std::endl;
        resource.set(42);

        resource.useObject();
    }

    std::cout << std::endl
              << "deferred usage:" << std::endl;
    {
        // MyObject obj; // Not compiled
        deferred<MyObject> obj;
        // obj->foo(); // Undefined behaviour!

        std::cout << "initialized" << std::endl;
        obj = MyObject(10);

        obj->foo();
        (*obj).foo();
        obj.get().foo();

        MyObject& m = obj;
        const MyObject& cm = obj;
        m.v = -1;

        m.foo();
        cm.foo();
    }

    return 0;
}
