#pragma once

#include <algorithm>

namespace fys
{

template<class ForwardIt, class A, class B>
std::pair<ForwardIt, ForwardIt> extract(ForwardIt first, ForwardIt last, A const& delimiter1, B const& delimiter2)
{
    first = std::find(first, last, delimiter1);

    ForwardIt sub_last = std::find(first, last, delimiter2);

    if(sub_last != last)
        ++sub_last;

    return {first, sub_last};
}

template<class ForwardIt, class UnaryPredicate1, class UnaryPredicate2>
std::pair<ForwardIt, ForwardIt> extract_if(ForwardIt first, ForwardIt last, UnaryPredicate1 p_delimiter1, UnaryPredicate2 p_delimiter2)
{
    first = std::find_if(first, last, p_delimiter1);

    ForwardIt sub_last = std::find_if(first, last, p_delimiter2);

    if(sub_last != last)
        ++sub_last;

    return {first, sub_last};
}

template<class ForwardIt, class A, class B>
std::pair<ForwardIt, ForwardIt> extract_between(ForwardIt first, ForwardIt last, A const& delimiter1, B const& delimiter2)
{
    first = std::find(first, last, delimiter1);

    if(first != last)
        ++first;

    return {first, std::find(first, last, delimiter2)};
}

template<class ForwardIt, class UnaryPredicate1, class UnaryPredicate2>
std::pair<ForwardIt, ForwardIt> extract_between_if(ForwardIt first, ForwardIt last, UnaryPredicate1 p_delimiter1, UnaryPredicate2 p_delimiter2)
{
    first = std::find_if(first, last, p_delimiter1);

    if(first != last)
        ++first;

    return {first, std::find_if(first, last, p_delimiter2)};
}

/**
    Documents:
        (1) function(extract)
        (2) function(extract_if)
        (3) function(extract_between)
        (4) function(extract_between_if)
    Effect:
        In a sequence `[first,last)`, finds the first subsequence delimited by two endpoints `i` and `j` satisfying specific criteria:
            (1)(3) `*i == delimiter1` and `*j == delimiter2`,
            (2)(4) `p_delimiter1(*i) != false` and `p_delimiter2(*j) != false`.
        Those endpoints are:
            (1)(2) included in the subsequence,
            (3)(4) excluded from the subsequence.
    Returns:
        `make_pair(sub_first, sub_last)`, where
            `sub_first` is the first iterator in the resulting subsequence or `last` if no first endpoint was found,
            and `sub_end` is past-the-end iterator in the resulting subsequence or `last` if no second endpoint was found.
    Complexity:
        At most `last - first` applications of one of the delimiters predicates.
    Example:
```
int main()
{
    std::vector<int> v = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19};

    {
        auto extract_pair = fys::extract(begin(v), end(v), 2, 9);
        std::for_each(extract_pair.first, extract_pair.second, [](decltype(v)::value_type const& x){ std::cout << x; });
    }
    std::cout << std::endl;
    {
        auto extract_pair = fys::extract_between(begin(v), end(v), 2, 9);
        std::for_each(extract_pair.first, extract_pair.second, [](decltype(v)::value_type const& x){ std::cout << x; });
    }
    std::cout << std::endl;
    {
        auto extract_pair = fys::extract_between(begin(v), end(v), 2, 1);
        std::for_each(extract_pair.first, extract_pair.second, [](decltype(v)::value_type const& x){ std::cout << x; });
    }
    std::cout << std::endl;
    {
        auto extract_pair = fys::extract_between_if(begin(v), end(v), [](decltype(v)::value_type const& x){ return x > 4 && x%2 == 0 && x%3 == 0; }, [](decltype(v)::value_type const& x){ return x%5 == 0;});
        std::for_each(extract_pair.first, extract_pair.second, [](decltype(v)::value_type const& x){ std::cout << x; });
    }
}
```
    Output:
```
23456789
345678
345678910111213141516171819
789
```
**/

}
