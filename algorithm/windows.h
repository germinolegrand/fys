#pragma once

#include <algorithm>
#include <iterator>

namespace fys
{

template<class ForwardIt, class Function>
Function for_each_incomplete_window(ForwardIt first, ForwardIt last, Function f, typename std::iterator_traits<ForwardIt>::difference_type windowSize, typename std::iterator_traits<ForwardIt>::difference_type windowOffset = 1)
{
    for(; first != last; std::advance(first, std::min(windowOffset, std::distance(first, last))))
        f(first, std::next(first, std::min(windowSize, std::distance(first, last))));

    return std::move(f);
}

template<class ForwardIt, class Function>
Function for_each_complete_window(ForwardIt first, ForwardIt last, Function f, typename std::iterator_traits<ForwardIt>::difference_type windowSize, typename std::iterator_traits<ForwardIt>::difference_type windowOffset = 1)
{
    for(; std::distance(first, last) >= windowSize; std::advance(first, std::min(windowOffset, std::distance(first, last))))
        f(first, std::next(first, std::min(windowSize, std::distance(first, last))));

    return std::move(f);
}

template<class ForwardIt, class OutputIt>
OutputIt incomplete_windows_copy(ForwardIt first, ForwardIt last, OutputIt d_first, typename std::iterator_traits<ForwardIt>::difference_type windowSize, typename std::iterator_traits<ForwardIt>::difference_type windowOffset = 1)
{
    for(; first != last; std::advance(first, std::min(windowOffset, std::distance(first, last))))
        d_first = std::copy(first, std::next(first, std::min(windowSize, std::distance(first, last))), d_first);

    return d_first;
}

template<class ForwardIt, class OutputIt>
OutputIt complete_windows_copy(ForwardIt first, ForwardIt last, OutputIt d_first, typename std::iterator_traits<ForwardIt>::difference_type windowSize, typename std::iterator_traits<ForwardIt>::difference_type windowOffset = 1)
{
    for(; std::distance(first, last) >= windowSize; std::advance(first, std::min(windowOffset, std::distance(first, last))))
        d_first = std::copy(first, std::next(first, std::min(windowSize, std::distance(first, last))), d_first);

    return d_first;
}

namespace priv
{

template<bool Complete>
struct Windows
{
    template<class ForwardIt, class Function>
    static inline
    Function for_each_window(ForwardIt first, ForwardIt last, Function f, typename std::iterator_traits<ForwardIt>::difference_type windowSize, typename std::iterator_traits<ForwardIt>::difference_type windowOffset = 1)
    { return for_each_complete_window(first, last, f, windowSize, windowOffset); }

    template<class ForwardIt, class OutputIt>
    static inline
    OutputIt windows_copy(ForwardIt first, ForwardIt last, OutputIt d_first, typename std::iterator_traits<ForwardIt>::difference_type windowSize, typename std::iterator_traits<ForwardIt>::difference_type windowOffset = 1)
    { return complete_windows_copy(first, last, d_first, windowSize, windowOffset); }
};

template<>
struct Windows<false>
{
    template<class ForwardIt, class Function>
    static inline
    Function for_each_window(ForwardIt first, ForwardIt last, Function f, typename std::iterator_traits<ForwardIt>::difference_type windowSize, typename std::iterator_traits<ForwardIt>::difference_type windowOffset = 1)
    { return for_each_incomplete_window(first, last, f, windowSize, windowOffset); }

    template<class ForwardIt, class OutputIt>
    static inline
    OutputIt windows_copy(ForwardIt first, ForwardIt last, OutputIt d_first, typename std::iterator_traits<ForwardIt>::difference_type windowSize, typename std::iterator_traits<ForwardIt>::difference_type windowOffset = 1)
    { return incomplete_windows_copy(first, last, d_first, windowSize, windowOffset); }
};

}

template<bool Complete = true, class ForwardIt, class Function>
Function for_each_window(ForwardIt first, ForwardIt last, Function f, typename std::iterator_traits<ForwardIt>::difference_type windowSize, typename std::iterator_traits<ForwardIt>::difference_type windowOffset = 1)
{
    return priv::Windows<Complete>::for_each_window(first, last, f, windowSize, windowOffset);
}

template<bool Complete = true, class ForwardIt, class OutputIt>
OutputIt windows_copy(ForwardIt first, ForwardIt last, OutputIt d_first, typename std::iterator_traits<ForwardIt>::difference_type windowSize, typename std::iterator_traits<ForwardIt>::difference_type windowOffset = 1)
{
    return priv::Windows<Complete>::windows_copy(first, last, d_first, windowSize, windowOffset);
}

/**
    Documents:
        (1) function(incomplete_windows_copy)
        (2) function(complete_windows_copy)
        (3) function(for_each_incomplete_window)
        (4) function(for_each_complete_window)
        (5) function(windows_copy)
        (6) function(for_each_window)
    Effects:
        Takes successive subsequences [i,j) from the range `[first,last)` and
        (1)(2) copies the elements of `[i,j)` into the range
            (1) `[d_first, d_first + ???)`,
            (2) `[d_first, d_first + ((last - first) < windowsSize ? 0 : (last - first + min(windowsOffset - windowsSize, 0))/windowsOffset*windowsSize))`
            starting from `first` and proceeding to `last`,
        (3)(4) applies f to `[i,j)`.
        For each subsequence, performs
            (1)(2) `copy(i, j, d_first)`,
            (3)(4) `f(i, j)`.
        The first subsequence is `[first,first + windowsSize)`.
        Therefore, assuming a previous subsequence `[g,h)`,
            the next subsequence is `[g + windowsOffset,h + windowsOffset)`.
        If `j > last`,
            (1)(3) then the subsequence becomes `[i, last)`,
            (2)(4) then the subsequence becomes `[last, last)`.
        (5) is an alias of (1) if Complete is false, an alias of (2) by default otherwise.
        (6) is an alias of (3) if Complete is false, an alias of (4) by default otherwise.
    Returns:
        The end of the resulting range.
    Requires:
        `windowsOffset` shall not be zero,
        `d_first` shall not be in the range `[first,last)`,
        `Function` shall meet the requirements of MoveConstructible.
    Complexity:
        (1) Exactly `???` assignements,//TODO calculate complexity
        (2) Exactly `(last - first) < windowsSize ? 0 : (last - first + min(windowsOffset - windowsSize, 0))/windowsOffset*windowsSize` assignements,
        (3) Exactly `(last - first)/windowsOffset + min(1, last - first)` applications of `f`,
        (4) Exactly `(last - first) < windowsSize ? 0 : (last - first + min(windowsOffset - windowsSize, 0))/windowsOffset` applications of `f`.
    Example:
```
int main()
{
    std::vector<int> v = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19};
    std::string s("hello world");

    fys::windows_copy(begin(s), end(s), std::ostream_iterator<char>(std::cout, ""), 3, 2);

    std::cout << std::endl;

    fys::windows_copy<false>(begin(s), end(s), std::ostream_iterator<char>(std::cout, ""), 3, 2);

    std::cout << std::endl;

    fys::for_each_window(begin(v), end(v), [](decltype(v)::iterator first, decltype(v)::iterator last){
        std::cout << "[";
        std::for_each(first, last, [](decltype(v)::value_type& x){ std::cout << " " << x; });
        std::cout << " ]";
    }, 3, 2);

    std::cout << std::endl;

    fys::for_each_window<false>(begin(v), end(v), [](decltype(v)::iterator first, decltype(v)::iterator last){
        std::cout << "[";
        std::for_each(first, last, [](decltype(v)::value_type& x){ std::cout << " " << x; });
        std::cout << " ]";
    }, 3, 2);
}
```
    Output:
```
hellloo wworrld
hellloo wworrldd
[ 0 1 2 ][ 2 3 4 ][ 4 5 6 ][ 6 7 8 ][ 8 9 10 ][ 10 11 12 ][ 12 13 14 ][ 14 15 16 ][ 16 17 18 ]
[ 0 1 2 ][ 2 3 4 ][ 4 5 6 ][ 6 7 8 ][ 8 9 10 ][ 10 11 12 ][ 12 13 14 ][ 14 15 16 ][ 16 17 18 ][ 18 19 ]
```
**/

}
