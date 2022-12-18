#ifndef INVOKER_HPP
#define INVOKER_HPP

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
    using Invocable_T = std::conditional_t<std::is_function_v<Invocable>, 
                        std::add_pointer_t<Invocable>, Invocable>;
    
    template <typename F, typename... Args>
    using is_invocable = std::is_invocable<F, Args...>;   
#else
    using Invocable_T = typename std::conditional<std::is_function<Invocable>::value,
                        typename std::add_pointer<Invocable>::type, Invocable>::type;

    template <typename F, typename... Args>
    struct is_invocable :
    std::is_constructible<std::function<void(Args ...)>, 
    std::reference_wrapper<typename std::remove_reference<F>::type>>
    {};
#endif
    template <typename I>
#if INVOKER_CPLUSPLUS > 201703L
    requires std::convertible_to<I, Invocable_T>
#endif // INVOKER_CPLUSPLUS > 201703L
    Invoker(I invocable) : invocable_(std::move(invocable))
    {
#if INVOKER_CPLUSPLUS <= 201703L
        static_assert(std::is_convertible<I, Invocable_T>::value && 
        std::is_constructible<std::function<Invocable>, 
        std::reference_wrapper<typename std::remove_reference<I>::type>>::value, 
        "'Invoker<Invocable>::invocable_' has incomplete type!");
#endif // INVOKER_CPLUSPLUS <= 201703L
    }
    
    template <typename... Args>
#if INVOKER_CPLUSPLUS > 201703L
    requires std::invocable<Invocable&, Args...>
#endif // INVOKER_CPLUSPLUS > 201703L
#if INVOKER_CPLUSPLUS > 201103L
    inline decltype(auto) operator()(Args&&... args)
#else
    inline auto operator()(Args&&... args) -> typename std::result_of<Invocable_T(Args&&...)>::type 
#endif // INVOKER_CPLUSPLUS > 201103L
    {
#if INVOKER_CPLUSPLUS <= 201703L
        static_assert(is_invocable<Invocable_T, Args...>::value, 
        "Argument constraints not satisfied!");
#endif // INVOKER_CPLUSPLUS <= 201703L
#if INVOKER_CPLUSPLUS > 201402L
        return std::invoke(invocable_, std::forward<Args>(args)...);
#else
        return invocable_(std::forward<Args>(args)...);
#endif // INVOKER_CPLUSPLUS > 201402L
    }
    
    
        template <typename... Args>
#if INVOKER_CPLUSPLUS > 201703L
    requires std::invocable<Invocable&, Args...>
#endif // INVOKER_CPLUSPLUS > 201703L
#if INVOKER_CPLUSPLUS > 201103L
    inline decltype(auto) operator()(Args&&... args) const
#else
    inline auto operator()(Args&&... args) const -> typename std::result_of<Invocable_T(Args&&...)>::type 
#endif // INVOKER_CPLUSPLUS > 201103L
    {
#if INVOKER_CPLUSPLUS <= 201703L
        static_assert(is_invocable<Invocable_T, Args...>::value, 
        "Argument constraints not satisfied!");
#endif // INVOKER_CPLUSPLUS <= 201703L
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

#endif // INVOKER_HPP
