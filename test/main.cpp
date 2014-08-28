#include "algorithm-utils-cpp/algorithm-utils.h"

#include <boost/foreach.hpp>
#define foreach BOOST_FOREACH

#include <iostream>

bool is_even(const int& number)
{
    return number % 2 == 0;
}

bool is_greater_than(int base, const int& number)
{
    return number > base;
}

void test_item_exists()
{
    std::vector<int> numbers;

    numbers.push_back(1);
    assert(!item_exists<int>(numbers, is_even));

    numbers.push_back(2);
    assert(item_exists<int>(numbers, is_even));

    std::cout << "test_item_exists: OK" << std::endl;
}

void test_remove_items_if()
{
    std::vector<int> numbers;
    for (int i = 1; i < 10; ++i)
        numbers.push_back(i);

    remove_items_if<int>(numbers, is_even);

    assert(numbers.size() == 5);
    assert(numbers[1] == 3);

    std::cout << "test_remove_items_if: OK" << std::endl;
}

void test_copy_items_if()
{
    std::vector<int> numbers;
    for (int i = 1; i < 10; ++i)
        numbers.push_back(i);

    std::vector<int> even = copy_items_if<int>(numbers, is_even);

    assert(even.size() == 4);
    assert(even[1] == 4);

    // Example of passing a curried function that binds only the first
    // parameter, but leaves the second open
    std::vector<int> greater_than_6 =
        copy_items_if<int>(numbers, boost::bind(is_greater_than, 6, _1));

    assert(greater_than_6.size() == 3);
    assert(greater_than_6[0] == 7);

    std::cout << "test_copy_items_if: OK" << std::endl;
}

void test_find_item()
{
    std::vector<int> numbers;
    for (int i = 1; i < 10; ++i)
        numbers.push_back(i);

    std::vector<int>::iterator found = find_item<int>(numbers, is_even);

    assert(*found == 2);
    assert(numbers[1] == *found);

    std::cout << "test_find_item: OK" << std::endl;
}

int main()
{
    test_item_exists();
    test_remove_items_if();
    test_copy_items_if();
    test_find_item();

    return 0;
}
