# deferred

`deferred` — это легковесный шаблон-обертка для отложенного создания объектов в C++. Его основная цель — решить проблему работы с классами, у которых отсутствует конструктор по умолчанию.

## Мотивация

Некоторые классы требуют аргументов для создания объекта, например:
```cpp
struct NoDefaultCtor {
    int value;

    NoDefaultCtor() = delete;
    NoDefaultCtor(int v) : value(v) {}
};
```
В таких случаях нельзя объявить объект и отложить его конструирование до более позднего момента:
```cpp
NoDefaultCtor obj; // Compile error: no default constructor available
...
// `obj` should be constructed here
```
Обычно это приводит к необходимости использовать указатели или другие обертки:
```cpp
NoDefaultCtor* ptr;
...
ptr = new NoDefaultCtor(0);
```

## Особенности
### `deferred` предоставляет несколько ключевых преимуществ и ограничений, которые важно учитывать:

#### 1. Отложенное создание объекта
Позволяет объявить объект заранее и инициализировать позже:
```c++
deferred<NoDefaultCtor> obj;
...
obj = NoDefaultCtor(42);
auto value = obj->value; // ok
```
Также допускает инициализацию объекта вне списка инициализации класса:
```c++
class Resource {
    NoDefaultCtor obj;
    deferred<NoDefaultCtor> dobj;

public:
    Resource() noexcept
            : obj{ 0 } // `obj` must be constructed here and only here
    {
        dobj = NoDefaultCtor(0); // `dobj` can be constructed anywhere
    }

};
```

#### 2. Отсутствие обработки ошибок
Обращение к неинициализированному объекту через `deferred` никак не проверяется и приводит к неопределенному поведению (UB)
```c++
deferred<NoDefaultCtor> obj; // Not initialized
auto value = obj->value; // UB
```

#### 3. Удобный интерфейс
`deferred` предоставляет легкий доступ к объекту через:
- `get()` для явного получения ссылки на объект
- `operator->()`
- `operator*()`

#### 4. Отсутствие накладных расходов
`deferred<T>` не имеет накладных расходов, относительно использования объекта `T` напрямую.
Размер объекта также не меняется:
```c++
static_assert(sizeof(deferred<T>) == sizeof(T)); // For any type T
```


## Примеры использования

### Как обертка
```cpp
#include <iostream>
#include "deferred.h"

deferred<NoDefaultCtor> obj;

void foo()
{
    std::cout << obj->value << std::endl;
    std::cout << (*obj).value << std::endl;
    std::cout << obj.get().value << std::endl;
}

int main()
{    
    obj = NoDefaultCtor(42);
    foo();
    
    NoDefaultCtor& ref = obj;
    const NoDefaultCtor& cref = obj;
    ref.value = -1;

    std::cout << ref.value << std::endl;
    std::cout << cref.value << std::endl;

    return 0;
}
```

### Как поле класса
```c++
class Resource {
    deferred<NoDefaultCtor> obj;

public:
    Resource() noexcept = default;    
    ~Resource() noexcept = default;

    void set(int n)
    {
        obj = NoDefaultCtor(n);
    }

    void useObject()
    {
        std::cout << obj->value << std::endl;
    }
};

Resource res;
...
res.set(42);
res.useObject();
```
