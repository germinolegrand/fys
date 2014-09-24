#pragma once

#include <algorithm>
#include <iterator>

namespace fys
{

template<class ForwardIt, class OutputIt>
OutputIt incomplete_windows(ForwardIt itBegin, ForwardIt itEnd, OutputIt itOut, typename std::iterator_traits<ForwardIt>::difference_type windowSize, typename std::iterator_traits<ForwardIt>::difference_type windowOffset = 1)
{
    for(; itBegin != itEnd; std::advance(itBegin, std::min(windowOffset, std::distance(itBegin, itEnd))))
        itOut = std::copy(itBegin, std::next(itBegin, std::min(windowSize, std::distance(itBegin, itEnd))), itOut);

    return itOut;
}

template<class ForwardIt, class OutputIt>
OutputIt complete_windows(ForwardIt itBegin, ForwardIt itEnd, OutputIt itOut, typename std::iterator_traits<ForwardIt>::difference_type windowSize, typename std::iterator_traits<ForwardIt>::difference_type windowOffset = 1)
{
    for(; std::distance(itBegin, itEnd) >= windowSize; std::advance(itBegin, std::min(windowOffset, std::distance(itBegin, itEnd))))
        itOut = std::copy(itBegin, std::next(itBegin, std::min(windowSize, std::distance(itBegin, itEnd))), itOut);

    return itOut;
}

}
