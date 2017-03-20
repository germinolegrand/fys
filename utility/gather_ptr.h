#pragma once

#include <memory>

namespace fys
{

template<class T, class F>
std::shared_ptr<T> make_gather_ptr(F&& callback)
{
    return std::shared_ptr<T>(new T, [callback](T* t){
        callback(*t);
        delete t;
    });
}

template<class T, class F>
std::shared_ptr<T> make_gather_ptr(T* pointer, F&& callback)
{
    return {pointer, [callback](T* t){
        callback(*t);
        delete t;
    }};
}

}
