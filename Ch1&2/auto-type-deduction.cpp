// Compiled with: clang++ -std=c++14 -c auto-type-deduction.cpp
#include<initializer_list>

auto foo()
{
    return { 1, 2, 3 };
}

auto variable = { 1, 2, 3 };

auto bar = [](auto& v) { return v; };
bar({1, 2, 3});
