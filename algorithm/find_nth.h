#pragma once

#include <algorithm>

namespace fys
{

template<class iterator, class T>
iterator find_nth(iterator itBegin, iterator itEnd, size_t n, T const& value)
{
    for(; (itBegin = std::find(itBegin, itEnd, value)) != itEnd && n != 0; ++itBegin, --n);

    return itBegin;
}

template<class iterator, class F>
iterator find_nth_if(iterator itBegin, iterator itEnd, size_t n, F f)
{
    for(; (itBegin = std::find_if(itBegin, itEnd, f)) != itEnd && n != 0; ++itBegin, --n);

    return itBegin;
}

}
