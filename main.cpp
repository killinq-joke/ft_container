#include <iostream>
#include <vector>

int main()
{
    std::vector<int> a(1, 1);

    a.resize(a.max_size());
    return (0);
}