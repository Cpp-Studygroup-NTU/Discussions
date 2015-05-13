#include <iostream>
#include <typeinfo>

int main()
{
    float x;
    struct { int a; } y;
    std::cout << typeid(x).name() << '\n'
              << typeid(y).name() << std::endl;
    return 0;
}
