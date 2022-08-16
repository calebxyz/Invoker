#pragma once

#ifdef _MSC_VER
#undef __cplusplus
#define __cplusplus _MSVC_LANG
#endif

#if __cplusplus > 201402L
#include <type_traits> 
#include <functional>
#if __cplusplus > 201703L
#include <concepts>
#endif // __cplusplus > 201703L

template <typename Invocable>
struct Invoker{
    using Invocable_T = std::conditional_t<std::is_function_v<Invocable>, std::add_pointer_t<Invocable>, Invocable>;
    template <typename I>
#if __cplusplus > 201703L
    requires std::convertible_to<I, Invocable_T>
#endif // __cplusplus > 201703L
    Invoker(I invocable) : invocable_(std::move(invocable))
    {
#if __cplusplus < 202002L
        static_assert(std::is_convertible_v<I, Invocable_T>, "Type 'Invocable' is not a function!");
#endif // __cplusplus < 202002L
    }
    
    template <typename... Args>
#if __cplusplus > 201703L
    requires std::invocable<Invocable&, Args...>
#endif // __cplusplus > 201703L
    decltype(auto) operator()(Args&&... args){
#if __cplusplus < 202002L
        static_assert(std::is_invocable_v<Invocable_T, Args...>, "Type 'Invocable' is not a function or cannot invoke with these arguments!");
#endif // __cplusplus < 202002L
        return std::invoke(invocable_, std::forward<Args>(args)...);
    }
    
private:
    Invocable_T invocable_;
};

template <typename I>
Invoker(I) -> Invoker<I>;
#endif // __cplusplus > 201402L
