#pragma once

#include <type_traits>

namespace fys
{

template<class T>
auto underlying_cast(T const& t) -> typename std::underlying_type<T>::type
{
    return static_cast<typename std::underlying_type<T>::type>(t);
}

template<class T>
auto underlying_reference_cast(T& t) -> typename std::underlying_type<T>::type&
{
    return reinterpret_cast<typename std::underlying_type<T>::type&>(t);
}

/**
    Documents:
        (1) function(underlying_cast)
        (2) function(underlying_reference_cast)
    Returns:
        (1) The underlying value of `t`. It is of type `std::underlying_type<decltype(t)>::type`
        (2) A reference to the underlying value of `t`. It is of type `std::underlying_type<decltype(t)>::type&`
    Example:
```
int main()
{
    enum class E {A,B,C};

    E e = E::B;
    std::cout << fys::underlying_cast(e) << std::endl;

    fys::underlying_reference_cast(e) = 2;
    std::cout << fys::underlying_cast(e) << std::endl;
}
```
    Output:
```
1
2
```
**/

}
