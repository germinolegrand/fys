#ifndef EXTRACT_H_INCLUDED
#define EXTRACT_H_INCLUDED

#include <algorithm>

template<class iterator, class A, class B>
std::pair<iterator, iterator> extract(iterator itBegin, iterator itEnd, A const& a, B const& b)
{
    itBegin = std::find(itBegin, itEnd, a);

    iterator sub_itEnd = std::find(itBegin, itEnd, b);

    if(sub_itEnd != itEnd)
        ++sub_itEnd;

    return {itBegin, sub_itEnd};
}

template<class iterator, class A, class B>
std::pair<iterator, iterator> extract_if(iterator itBegin, iterator itEnd, A a, B b)
{
    itBegin = std::find_if(itBegin, itEnd, a);

    iterator sub_itEnd = std::find_if(itBegin, itEnd, b);

    if(sub_itEnd != itEnd)
        ++sub_itEnd;

    return {itBegin, sub_itEnd};
}

template<class iterator, class A, class B>
std::pair<iterator, iterator> extract_between(iterator itBegin, iterator itEnd, A const& a, B const& b)
{
    itBegin = std::find(itBegin, itEnd, a);

    if(itBegin != itEnd)
        ++itBegin;

    return {itBegin, std::find(itBegin, itEnd, b)};
}

template<class iterator, class A, class B>
std::pair<iterator, iterator> extract_between_if(iterator itBegin, iterator itEnd, A a, B b)
{
    itBegin = std::find_if(itBegin, itEnd, a);

    if(itBegin != itEnd)
        ++itBegin;

    return {itBegin, std::find_if(itBegin, itEnd, b)};
}

#endif // EXTRACT_H_INCLUDED
