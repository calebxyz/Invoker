#ifndef INVOKER_HEADER
#define INVOKER_HEADER

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
#if __cplusplus > 201703L
    template <typename I>
    requires std::convertible_to<I, Invocable_T>
#else // __cplusplus < 202002L
    template <typename I, typename = typename std::enable_if<std::is_convertible<I, Invocable_T>::value>::type>
#endif // __cplusplus > 201703L
    Invoker(I invocable) : invocable_(std::move(invocable)) {}
    
#if __cplusplus > 201703L
    template <typename... Args>
    requires std::invocable<Invocable&, Args...>
#else
    template <typename... Args, typename = typename std::enable_if<std::is_invocable<Invocable_T, Args...>::value>::type>
#endif // __cplusplus > 201703L
    decltype(auto) operator()(Args&&... args){
        return std::invoke(invocable_, std::forward<Args>(args)...);
    }
    
private:
    Invocable_T invocable_;
};

template <typename I>
Invoker(I) -> Invoker<I>;
#endif // __cplusplus > 201402L

#endif // INVOKER_HEADER
