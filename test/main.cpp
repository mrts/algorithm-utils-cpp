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

bool c_is_greater_than_a_plus_b(int a, int b, const int& c)
{
    return c > a + b;
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

    // Example of negating criteria
    std::vector<int> odd =
        copy_items_if<int>(numbers, !boost::bind(is_even, _1));

    assert(odd.size() == 5);
    assert(odd[1] == 3);

    // Example of bind more than one parameter in different order
    std::vector<int> greater_than_7 = copy_items_if<int>(
        numbers, boost::bind(c_is_greater_than_a_plus_b, 3, 4, _1));

    assert(greater_than_7.size() == 2);
    assert(greater_than_7[0] == 8);

    // Example of leaving first parameter open and binding latter
    std::vector<int> less_than_4 = copy_items_if<int>(
        numbers, boost::bind(c_is_greater_than_a_plus_b, _1, 1, 5));

    assert(less_than_4.size() == 3);
    assert(less_than_4[2] == 3);

    // Example of negating bind
    std::vector<int> not_less_than_5 = copy_items_if<int>(
        numbers, !boost::bind(is_greater_than, _1, 5));

    assert(not_less_than_5.size() == 5);
    assert(not_less_than_5[4] == 9);

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

class Item
{
public:
    Item(int number) : _number(number) {}
    bool is_even() const { return _number % 2 == 0; }
    bool is_greater(int other) const { return _number > other; }
    int number() const { return _number; }

private:
    int _number;
};

void test_objects()
{
    std::vector<Item> items;
    for (int i = 1; i < 10; ++i)
        items.push_back(Item(i));

    std::vector<Item> even = copy_items_if<Item>(items, &Item::is_even);

    assert(even.size() == 4);
    assert(even[1].number() == 4);

    std::vector<Item> greater_than_3 =
        copy_items_if<Item>(items, boost::bind(&Item::is_greater, _1, 3));

    assert(greater_than_3.size() == 6);
    assert(greater_than_3[1].number() == 5);

    std::cout << "test_objects: OK" << std::endl;
}

int main()
{
    test_item_exists();
    test_remove_items_if();
    test_copy_items_if();
    test_find_item();
    test_objects();

    return 0;
}
