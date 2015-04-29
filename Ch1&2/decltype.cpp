#include <initializer_list>

template<typename T>
auto foo(const T& t) -> decltype(t)
{ return t; }

template<typename T>
auto bar(const T& t)
{ return t; }

foo({1, 2, 3});
bar({1, 2, 3});

