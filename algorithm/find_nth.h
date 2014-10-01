#pragma once

#include <algorithm>

namespace fys
{

template<class InputIt, class T>
InputIt find_nth(InputIt first, InputIt last, size_t n, T const& value)
{
    for(; (first = std::find(first, last, value)) != last && n != 0; ++first, --n);

    return first;
}

template<class InputIt, class UnaryPredicate>
InputIt find_nth_if(InputIt first, InputIt last, size_t n, UnaryPredicate p)
{
    for(; (first = std::find_if(first, last, p)) != last && n != 0; ++first, --n);

    return first;
}

template<class InputIt, class UnaryPredicate>
InputIt find_nth_if_not(InputIt first, InputIt last, size_t n, UnaryPredicate q)
{
    for(; (first = std::find_if_not(first, last, q)) != last && n != 0; ++first, --n);

    return first;
}

/**
    Documents:
        (1) function(find_nth),
        (2) function(find_nth_if),
        (3) function(find_nth_if_not)
    Returns:
        The `n`-th iterator `i` counting from `0` in the range `[first,last)` for which the following corresponding conditions hold:
            (1) `*i == value`,
            (2) `p(*i) != false`,
            (3) `q(*i) == false`.
        Returns `last` if no such iterator is found.
    Complexity:
        At most `last - first` applications of the corresponding predicate.
    Example:
```
int main()
{
    std::vector<int> v = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19};
    std::string s("hello world");

    std::cout << std::distance(begin(s), fys::find_nth(begin(s), end(s), 1, 'l')) << std::endl;
    std::cout << std::distance(begin(v), fys::find_nth_if(begin(v), end(v), 1, [](int i){ return (i > 6) && (i%4 == 0); })) << std::endl;
    std::cout << std::distance(begin(v), fys::find_nth_if_not(begin(v), end(v), 1, [](int i){ return (i <= 6) || (i%4 == 0); })) << std::endl;
}
```
    Outputs:
```
3
12
9
```
**/

}
