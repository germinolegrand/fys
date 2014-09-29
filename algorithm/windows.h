#pragma once

#include <algorithm>
#include <iterator>

namespace fys
{

template<class ForwardIt, class OutputIt>
OutputIt incomplete_windows(ForwardIt first, ForwardIt last, OutputIt d_first, typename std::iterator_traits<ForwardIt>::difference_type windowSize, typename std::iterator_traits<ForwardIt>::difference_type windowOffset = 1)
{
    for(; first != last; std::advance(first, std::min(windowOffset, std::distance(first, last))))
        d_first = std::copy(first, std::next(first, std::min(windowSize, std::distance(first, last))), d_first);

    return d_first;
}

template<class ForwardIt, class OutputIt>
OutputIt complete_windows(ForwardIt first, ForwardIt last, OutputIt d_first, typename std::iterator_traits<ForwardIt>::difference_type windowSize, typename std::iterator_traits<ForwardIt>::difference_type windowOffset = 1)
{
    for(; std::distance(first, last) >= windowSize; std::advance(first, std::min(windowOffset, std::distance(first, last))))
        d_first = std::copy(first, std::next(first, std::min(windowSize, std::distance(first, last))), d_first);

    return d_first;
}

/**
    Documents:
        (1) function(incomplete_windows)
        (2) function(complete_windows)
    Effects:
        Copies the elements of successive subsequences [i,j) of the range `[first,last)` into the range
            (1) `[d_first, d_first + ???)`,
            (2) `[d_first, d_first + ((last - first) < windowsSize ? 0 : (last - first + min(windowsOffset - windowsSize, 0))/windowsOffset*windowsSize))`
            starting from `first` and proceeding to `last`.
        For each subsequence, performs `copy(i, j, d_first)`.
        The first subsequence is `[first,first + windowsSize)`.
        Therefore, assuming a previous subsequence `[g,h)`,
            the next subsequence is `[g + windowsOffset,h + windowsOffset)`.
        If `j > last`,
            (1) then the subsequence becomes `[i, last)`,
            (2) then the subsequence becomes `[last, last)`. [Note: the elements of the subsequence are not copied]
    Returns:
        The end of the resulting range.
    Complexity:
        (1) Exactly `???` assignements,//TODO calculate complexity
        (2) Exactly `(last - first) < windowsSize ? 0 : (last - first + min(windowsOffset - windowsSize, 0))/windowsOffset*windowsSize` assignements.
**/

}
