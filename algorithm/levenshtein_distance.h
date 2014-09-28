#pragma once

#include <algorithm>
#include <vector>

namespace fys
{

template<class ForwardIt1, class ForwardIt2>
int levenshtein_distance(ForwardIt1 first1, ForwardIt1 last1, ForwardIt2 first2, ForwardIt2 last2)
{
    #if __cplusplus >= 201402L
    std::tie(first1, first2) = std::mismatch(first1, last1, first2, last2);
    #else
    if(std::distance(first1, last1) <= std::distance(first2, last2))
        std::tie(first1, first2) = std::mismatch(first1, last1, first2);
    else
        std::tie(first2, first1) = std::mismatch(first2, last2, first1);
    #endif // __cplusplus

    size_t size1 = std::distance(first1, last1);
    size_t size2 = std::distance(first2, last2);

    if(size1 == 0)
        return size2;
    if(size2 == 0)
        return size1;

    std::vector<int> costs(size2 + 1);

    for(size_t i = 0; i < costs.size(); ++i)
        costs[i] = i;

    for(size_t i = 0; i < size1; ++i)
    {
        costs[0] = i + 1;
        int corner = i;

        for(size_t j = 0; j < size2; ++j)
        {
            int upper = costs[j + 1];

            if(*std::next(first1, i) == *std::next(first2, j))
                costs[j + 1] = corner;
            else
                costs[j + 1] = std::min(costs[j], std::min(upper, corner)) + 1;

            corner = upper;
        }
    }

    return costs.back();
}

}
