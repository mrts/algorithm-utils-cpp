#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/bind.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/foreach.hpp>
#include <boost/type_traits.hpp>

#define foreach BOOST_FOREACH

#include <vector>
#include <string>
#include <algorithm>
#include <iostream>

template <typename T>
bool item_exists(const std::vector<T>& items,
                 boost::function<bool(const T&)> criteria)
{
    return std::find_if(items.begin(), items.end(),
                        boost::bind(criteria, _1)) != items.end();
}

template <typename T>
void remove_if(std::vector<T>& items, boost::function<bool(const T&)> criteria)
{
    items.erase(
        std::remove_if(items.begin(), items.end(), boost::bind(criteria, _1)),
        items.end());
}

bool is_even(const int& number)
{
    return number % 2 == 0;
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

void test_remove_if()
{
    std::vector<int> numbers;
    for (int i = 0; i < 10; ++i)
        numbers.push_back(i);

    remove_if<int>(numbers, is_even);

    assert(numbers.size() == 5);
    assert(numbers[1] == 3);

    std::cout << "test_remove_if: OK" << std::endl;
}

int main()
{
    test_item_exists();
    test_remove_if();

    return 0;
}
