#pragma once

#include <iterator>
#include <tuple>

namespace fys
{

template<std::size_t N, class Iterator>
class tuple_iterator
{
private:
    Iterator m_it;

public:
    using iterator_type = Iterator;
    using iterator_category = typename std::iterator_traits<Iterator>::iterator_category;
    using value_type = typename std::tuple_element<N, typename std::iterator_traits<Iterator>::value_type>::type;
    using reference = value_type&;
    using pointer = value_type*;
    using difference_type = std::ptrdiff_t;

    tuple_iterator() = default;
    tuple_iterator(Iterator it):
        m_it(it)
    {}

    Iterator base() const
    { return m_it; }

    typename tuple_iterator::reference operator*()
    { return std::get<N>(*m_it); }

    tuple_iterator& operator++()
    {
        ++m_it;
        return *this;
    }

    tuple_iterator& operator--()
    {
        --m_it;
        return *this;
    }

    tuple_iterator& operator+=(std::size_t n)
    {
        m_it += n;
        return *this;
    }

    tuple_iterator& operator-=(std::size_t n)
    {
        m_it -= n;
        return *this;
    }

    friend bool operator==(tuple_iterator const& a, tuple_iterator const& b)
    { return a.m_it == b.m_it; }

    friend bool operator!=(tuple_iterator const& a, tuple_iterator const& b)
    { return !(a == b); }
};

template<std::size_t N, class Iterator>
tuple_iterator<N, typename std::remove_reference<Iterator>::type>
make_tuple_iterator(Iterator&& it)
{
    return {std::forward<Iterator>(it)};
}

/**
    Documents:
        class(tuple_iterator)
    Requires:
        N < std::tuple_size<std::iterator_traits<Iterator>::value_type>::value.
    Description:
        An iterator adaptator that permits to iterate over a sequence of tuples, while accessing only to the Nth type of the tuple.
    Members:
        Documents:
            function(operator*)
        Returns:
            A reference to the value of `std::get<N>` applied to the dereferenced underlying iterator.
    Helpers:
        Documents:
            function(make_tuple_iterator)
        Returns:
            A `tuple_iterator<N, std::remove_reference_t<Iterator>>` initialized with the value of `it`.
    Example:
```
int main()
{
    std::forward_list<std::tuple<int, std::string>> v{std::make_tuple(34, "coucou"),std::make_tuple(12, "wanna?")};
    std::unordered_map<int, std::string> m{std::make_pair(99, "hey!"),std::make_pair(22, "have a look!")};

    std::for_each(fys::make_tuple_iterator<0>(begin(v)), fys::make_tuple_iterator<0>(end(v)), [](int const& x){ std::cout << x << "\n"; });
    std::for_each(fys::make_tuple_iterator<1>(begin(m)), fys::make_tuple_iterator<1>(end(m)), [](std::string const& x){ std::cout << x << "\n"; });

    auto it = std::find(fys::make_tuple_iterator<0>(begin(v)), fys::make_tuple_iterator<0>(end(v)), 12).base();

    if(it != end(v))
    {
        std::cout << std::get<1>(*it) << "\n";
    }
}
```
    Output:
```
34
12
have a look!
hey!
wanna?
```
**/

}
