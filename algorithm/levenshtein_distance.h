#pragma once

#include <algorithm>
#include <vector>

template<class iterator_S, class iterator_T>
int levenshtein_distance(iterator_S it_S_begin, iterator_S it_S_end, iterator_T it_T_begin, iterator_T it_T_end)
{
    #if __cplusplus >= 201402L
    std::tie(it_S_begin, it_T_begin) = std::mismatch(it_S_begin, it_S_end, it_T_begin, it_T_end);
    #else
    if(std::distance(it_S_begin, it_S_end) <= std::distance(it_T_begin, it_T_end))
        std::tie(it_S_begin, it_T_begin) = std::mismatch(it_S_begin, it_S_end, it_T_begin);
    else
        std::tie(it_T_begin, it_S_begin) = std::mismatch(it_T_begin, it_T_end, it_S_begin);
    #endif // __cplusplus

    size_t size_S = std::distance(it_S_begin, it_S_end);
    size_t size_T = std::distance(it_T_begin, it_T_end);

    if(size_S == 0)
        return size_T;
    if(size_T == 0)
        return size_S;

    std::vector<int> costs(size_T + 1);

    for(size_t i = 0; i < costs.size(); ++i)
        costs[i] = i;

    for(size_t i = 0; i < size_S; ++i)
    {
        costs[0] = i + 1;
        int corner = i;

        for(size_t j = 0; j < size_T; ++j)
        {
            int upper = costs[j + 1];

            if(*std::next(it_S_begin, i) == *std::next(it_T_begin, j))
                costs[j + 1] = corner;
            else
                costs[j + 1] = std::min(costs[j], std::min(upper, corner)) + 1;

            corner = upper;
        }
    }

    return costs.back();
}
