#include <vector>

template<typename T>
auto foo2(T& t, int i)
{ return t[i]; }

template<typename T>
decltype(auto) foo3(T& t, int i)
{ return t[i]; }

std::vector<int> i = {3};
int c = (foo2(i, 0) = 4) = (foo3(i, 0) = 3);

