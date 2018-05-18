#pragma once

#include <type_traits>

namespace fys
{

template<class T, class U>
constexpr auto is(U const&) -> typename std::is_same<T, U>::type
{
    return {};
}

}

/**
    Documents:
        (1) function(is)
    Returns:
        (1) A constexpr boolean resulting of the comparaison of T and the type of the argument, as if `std::is_same_v<std::decay_t<decltype(arg1)>, T>`. It is of type `std::is_same<T, U>::type`
    Example:
```
int main()
{
    std::vector<std::variant<int, bool>> v = {3, true, 40};
    for(auto const& var : v){
        std::visit([](auto& x){
            if constexpr(fys::is<int>(x)){
                std::cout << "integer " << x << std::endl;
            } else if constexpr(fys::is<bool>(x)){
                std::cout << "boolean " << x << std::endl;
            }
        }, var);
    }

    //Without fys::is, this code would require in the general case :
    // if constexpr(std::is_same_v<std::decay_t<decltype(x)>, int>)
}
```
    Output:
```
integer 3
boolean true
integer 40
```
**/
