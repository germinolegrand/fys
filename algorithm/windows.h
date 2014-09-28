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

}
