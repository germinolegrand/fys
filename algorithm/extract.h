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

}
