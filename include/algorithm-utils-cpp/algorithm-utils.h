#ifndef ALGORITHM_UTILS_CPP_H__
#define ALGORITHM_UTILS_CPP_H__

#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <boost/lambda/lambda.hpp>

#include <vector>
#include <algorithm>

template <typename T>
typename std::vector<T>::const_iterator
find_item(const std::vector<T>& items, boost::function<bool(const T&)> criteria)
{
    return std::find_if(items.begin(), items.end(), boost::bind(criteria, _1));
}

template <typename T>
typename std::vector<T>::iterator
find_item(std::vector<T>& items, boost::function<bool(const T&)> criteria)
{
    return std::find_if(items.begin(), items.end(), boost::bind(criteria, _1));
}

template <typename T>
bool item_exists(const std::vector<T>& items,
                 boost::function<bool(const T&)> criteria)
{
    return find_item(items, criteria) != items.end();
}

template <typename T>
void remove_items_if(std::vector<T>& items,
                     boost::function<bool(const T&)> criteria)
{
    items.erase(
        std::remove_if(items.begin(), items.end(), boost::bind(criteria, _1)),
        items.end());
}

template <typename T>
std::vector<T> copy_items_if(const std::vector<T>& items,
                             boost::function<bool(const T&)> criteria)
{
    std::vector<T> result;
    std::remove_copy_if(items.begin(), items.end(), std::back_inserter(result),
                        !boost::bind(criteria, _1));
    return result;
}

#endif /* ALGORITHM_UTILS_CPP_H__ */
