#include <iostream>
#include <vector>
#include "vector.hpp"

template <typename T>
void show_vector(std::vector<T>& x)
{
    std::cout << "capacity == " << x.capacity() << std::endl;
    std::cout << "size == " << x.size() << std::endl;
    for (auto it = x.begin(); it != x.end(); it++)
    {
        std::cout << "x["<<it - x.begin()<<"] == " << *it << std::endl;
    }
}

template <typename T>
void show_vector(ft::vector<T>& x)
{
    std::cout << "capacity == " << x.capacity() << std::endl;
    std::cout << "size == " << x.size() << std::endl;
    // std::cout << "begin == " << x.begin() << std::endl;
    // std::cout << "end == " << x.end() << std::endl;
    std::cout << "[";
    for (size_t i = 0; i < x.size(); i++)
    {
        // std::cout << "it == " << it << std::endl;
        std::cout <<  x[i] << ", ";
    }
    std::cout << "]" << std::endl;
}

int main()
{
    std::vector<int> v1(2, -4);
    std::vector<int> v2(v1);
    ft::vector<int> v0;

    v1.insert(v1.begin(), 3);
    show_vector(v1);
    for (int i = 0; i < 10; i++)
        v0.push_back(i);
    show_vector(v0);
    v0.erase(v0.begin() + 2, v0.begin() + 4);
    show_vector(v0);

    return (0);
}


