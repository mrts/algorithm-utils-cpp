algorithm-utils-cpp
===================

High-level utility wrappers around standard algorithms and Boost to make
functional programming in C++ easier.

Introduction
------------

Using standard algorithms like `std::find_if`, `std::remove_if` and
`std::remove_copy_if` manually is somewhat tedious, may be error-prone and not
[DRY](http://en.wikipedia.org/wiki/Don't_repeat_yourself). For example, we had
an issue in our project where the last argument, the end iterator, was
accidentally omitted from `vector::erase` calls and this "idiom" had propagated
into several places in the codebase, causing hard-to-find bugs.

While examining our codebase we found that the four simple wrappers below cover
80% of our standard algorithms usage - and so we mostly use the wrappers in our
code now, falling back to the former only for non-trivial cases.

Overview
--------

For a full overview, see [tests](test/main.cpp).

The following wrappers are available:

* to find the first item in a collection that matches given criteria, use
  `find_item`
* to check whether an item exists in a collection that matches given criteria,
  use `item_exists`
* to create a copy of items in a collection that match given criteria, use
  `copy_items_if`
* to remove items that match given criteria from a collection, use
  `remove_items_if`.

*Criteria* is a predicate, a function that returns `true` when the given item
matches the required conditions and `false` otherwise.

Note that the wrappers work with `std::vector`s as that's what we use. It is
relatively easy to generalize them as follows, but that would complicate
calling code and we don't want that:

```c++
template <typename Collection>
typename Collection::const_iterator
find_item(const Collection& items,
          boost::function<bool(const Collection::value_type&)> criteria)
```

Assume the following declarations in the examples below:

```c++
class Item
{
public:
    bool is_green() const; // criteria function, boost::bind is also able to bind fields
};

std::vector<Item> items;
```

### FINDING AN ITEM

Use

```c++
std::vector<Item>::iterator found = find_item<Item>(items, &Item::is_green);
if (found != items.end())
    show_message("Found a green item", *found);
```

instead of

```c++
std::vector<Item>::iterator found = std::find_if(items.begin(), items.end(),
    boost::bind(&Item::is_green, _1));
if (found != items.end())
    show_message("Found a green item", *found);
```

### FINDING IF AN ITEM EXISTS IN A COLLECTION

Use

```c++
if (item_exists<Item>(items, &Item::is_green))
    show_message("Found a green item");
```

instead of

```c++
if (std::find_if(items.begin(), items.end(), boost::bind(&Item::is_green, _1)) != items.end())
    show_message("Found a green item");
```

### COPYING ITEMS ACCORDING TO CRITERIA

Use

```c++
std::vector<Items> green_items = copy_items_if<Item>(items, &Item::is_green);
```

instead of

```c++
std::vector<Items> green_items;
std::remove_copy_if(items.begin(), items.end(), std::back_inserter(green_items),
                    !boost::bind(&Item::is_green, _1));
```

### REMOVING ITEMS ACCORDING TO CRITERIA

Use

```c++
remove_items_if<Item>(items, &Item::is_green);
```

instead of

```c++
items.erase(std::remove_if(items.begin(), items.end(), boost::bind(&Item::is_green, _1)),
            items.end());
```

Using functions with multiple arguments as criteria
---------------------------------------------------

You can utilize partial function application (currying) with `boost::bind` to
use functions with multiple arguments.

Suppose the following functions that take two or more parameters:

```c++
class Item
{
public:
    enum ItemColor { Red, Green, Blue };
    bool is_of_color(ItemColor item_color) const;
};

bool item_quantity_is_greater_than_a_plus_b(int a, Item item, int b);
```

You can bind the parameter `item_color` as follows so that the function takes
only one parameter, `this`, the item itself, in the `copy_items_if` call:

```c++
std::vector<Item> green_items = copy_items_if<Item>(items,
     boost::bind(&Item::is_of_color, _1, Item::Green)); // `this` comes first
```

Note that `this` is always the (otherwise hidden) first argument in instance
calls. Other than that, you can place the unbound argument placeholder `_1`
anywhere you like or need when binding multiple parameters:

```c++
std::vector<Item>::iterator quantity_greater_than_7 = find_item<Item>(items,
    boost::bind(item_quantity_is_greater_than_a_plus_b, 3, _1, 4));
```

The partially bound function can be saved as a function object for later usage:

```c++
boost::function<bool(int)> item_quantity_is_greater_than_7 =
    boost::bind(item_quantity_is_greater_than_a_plus_b, 3, _1, 4);
remove_items_if<Item>(items, item_quantity_is_greater_than_7);
```

Using comparison in the criteria
--------------------------------

`boost::bind` supports various operators, including the equality operator `==`.
This enables building the criteria function in-line. For example, given the
following class:

```c++
class Item
{
public:
    enum ItemColor { Red, Green, Blue };
    ItemColor color;
};
```

`==` can be used as follows to filter green items:

```c++
std::vector<Items> green_items = copy_items_if<Item>(items,
    boost::bind(&Item::color, _1) == Item::Green);
```


Negating the criteria
---------------------

`boost::bind` can also be used to negate the criteria function, for example, to
find non-green items:

```c++
std::vector<Items> non_green_items = copy_items_if<Item>(items,
    !boost::bind(&Item::is_green, _1));
```
