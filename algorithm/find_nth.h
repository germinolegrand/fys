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

}
