#pragma once
#ifdef _MSC_VER
#undef __cplusplus
#define __cplusplus _MSVC_LANG
#endif
#if __cplusplus < 201402L
#error "Invoker requires C++14 or higher!"
#endif
#if __cplusplus > 201103L
#include <type_traits> 
#include <functional>
#if __cplusplus > 201703L
#include <concepts>
#endif // __cplusplus > 201703L
template <typename Invocable>
struct Invoker{
#if __cplusplus > 201402L
    using Invocable_T = std::conditional_t<std::is_function_v<Invocable>, std::add_pointer_t<Invocable>, Invocable>;
#else
    using Invocable_T = typename std::conditional<std::is_function<Invocable>::value,
    typename std::add_pointer<Invocable>::type, Invocable>::type;
#endif
    template <typename I>
#if __cplusplus > 201703L
    requires std::convertible_to<I, Invocable_T>
#endif // __cplusplus > 201703L
    explicit Invoker(I invocable) : invocable_(std::move(invocable))
    {
#if __cplusplus == 201703L
        static_assert(std::is_convertible_v<I, Invocable_T>, "Type 'Invocable' is not a function!");
#else
        static_assert(std::is_convertible<I, Invocable_T>::value, "Type 'Invocable' is not a function!");
#endif // __cplusplus == 201703L
    }
    template <typename... Args>
#if __cplusplus > 201703L
    requires std::invocable<Invocable&, Args...>
#endif // __cplusplus > 201703L
    decltype(auto) operator()(Args&&... args){
#if __cplusplus == 201703L
        static_assert(std::is_invocable_v<Invocable_T, Args...>, "Type 'Invocable' is not a function or cannot invoke with these arguments!");
#endif // __cplusplus == 201703L
#if __cplusplus > 201402L
        return std::invoke(invocable_, std::forward<Args>(args)...);
#else
        return invocable_(std::forward<Args>(args)...);
#endif // __cplusplus > 201402L
    }
private:
    Invocable_T invocable_;
};
#if __cplusplus > 201402L
template <typename I>
Invoker(I) -> Invoker<I>;
#endif // __cplusplus > 201402L
#endif // __cplusplus > 201103L
