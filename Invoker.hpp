#pragma once
#ifdef _MSC_VER
#define INVOKER_CPLUSPLUS _MSVC_LANG
#else
#define INVOKER_CPLUSPLUS __cplusplus
#endif

#if INVOKER_CPLUSPLUS < 201103L
#error "Invoker requires C++11 or higher!"
#endif

#if INVOKER_CPLUSPLUS >= 201103L
#include <type_traits>
#include <utility>
#include <functional>
#if INVOKER_CPLUSPLUS > 201703L
#include <concepts>
#endif // INVOKER_CPLUSPLUS > 201703L

template <typename Invocable>
struct Invoker{
#if INVOKER_CPLUSPLUS > 201402L
    using Invocable_T = std::conditional_t<std::is_function_v<Invocable>, std::add_pointer_t<Invocable>, Invocable>;
#else
    using Invocable_T = typename std::conditional<std::is_function<Invocable>::value,
                        typename std::add_pointer<Invocable>::type, Invocable>::type;
#endif
    template <typename I>
#if INVOKER_CPLUSPLUS > 201703L
    requires std::convertible_to<I, Invocable_T>
#endif // INVOKER_CPLUSPLUS > 201703L
    Invoker(I invocable) : invocable_(std::move(invocable))
    {
#if INVOKER_CPLUSPLUS == 201703L
        static_assert(std::is_convertible_v<I, Invocable_T>, "Type 'Invocable' is not a function!");
#else
        static_assert(std::is_convertible<I, Invocable_T>::value, "Type 'Invocable' is not a function!");
#endif // INVOKER_CPLUSPLUS == 201703L
    }
    
    template <typename... Args>
#if INVOKER_CPLUSPLUS > 201703L
    requires std::invocable<Invocable&, Args...>
#endif // INVOKER_CPLUSPLUS > 201703L
#if INVOKER_CPLUSPLUS > 201103L
    inline decltype(auto) operator()(Args&&... args){
#else
    inline auto operator()(Args&&... args) -> typename std::result_of<Invocable_T(Args&&...)>::type { // 
#endif
#if INVOKER_CPLUSPLUS == 201703L
        static_assert(std::is_invocable_v<Invocable_T, Args...>, "Type 'Invocable' is not a function or cannot invoke with these arguments!");
#endif // INVOKER_CPLUSPLUS == 201703L
#if INVOKER_CPLUSPLUS > 201402L
        return std::invoke(invocable_, std::forward<Args>(args)...);
#else
        return invocable_(std::forward<Args>(args)...);
#endif // INVOKER_CPLUSPLUS > 201402L
    }
    
private:
    Invocable_T invocable_;
};

#if INVOKER_CPLUSPLUS > 201402L
template <typename I>
Invoker(I) -> Invoker<I>;
#endif // INVOKER_CPLUSPLUS > 201402L
#endif // INVOKER_CPLUSPLUS > 201103L
