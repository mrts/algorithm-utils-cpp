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
                 boost::function<bool(const T&)> conditionPredicate)
{
    return std::find_if(items.begin(), items.end(),
                        boost::bind(conditionPredicate, _1)) != items.end();
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

int main()
{
    // test_remove_if();
    test_item_exists();

    return 0;
}
