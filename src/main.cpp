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
typename std::vector<T>::const_iterator
find_item(const std::vector<T>& items, boost::function<bool(const T&)> criteria)
{
    return std::find_if(items.begin(), items.end(), boost::bind(criteria, _1));
}

template <typename T>
T& find_item(std::vector<T>& items, boost::function<bool(const T&)> criteria)
{
    return *std::find_if(items.begin(), items.end(), boost::bind(criteria, _1));
}

template <typename T>
bool item_exists(const std::vector<T>& items,
                 boost::function<bool(const T&)> criteria)
{
    return find_item(items, criteria) != items.end();
}

template <typename T>
void remove_if(std::vector<T>& items, boost::function<bool(const T&)> criteria)
{
    items.erase(
        std::remove_if(items.begin(), items.end(), boost::bind(criteria, _1)),
        items.end());
}

template <typename T>
std::vector<T> copy_if(const std::vector<T>& items,
                       boost::function<bool(const T&)> criteria)
{
    std::vector<T> result;
    std::remove_copy_if(items.begin(), items.end(), std::back_inserter(result),
                        !boost::bind(criteria, _1));
    return result;
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
    for (int i = 1; i < 10; ++i)
        numbers.push_back(i);

    remove_if<int>(numbers, is_even);

    assert(numbers.size() == 5);
    assert(numbers[1] == 3);

    std::cout << "test_remove_if: OK" << std::endl;
}

void test_copy_if()
{
    std::vector<int> numbers;
    for (int i = 1; i < 10; ++i)
        numbers.push_back(i);

    std::vector<int> even = copy_if<int>(numbers, is_even);

    assert(even.size() == 4);
    assert(even[1] == 4);

    std::cout << "test_copy_if: OK" << std::endl;
}

void test_find_item()
{
    std::vector<int> numbers;
    for (int i = 1; i < 10; ++i)
        numbers.push_back(i);

    int& found = find_item<int>(numbers, is_even);

    assert(found == 2);
    assert(numbers[1] == found);

    std::cout << "test_find_item: OK" << std::endl;
}

int main()
{
    test_item_exists();
    test_remove_if();
    test_copy_if();
    test_find_item();

    return 0;
}
