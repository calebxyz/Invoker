#include <type_traits> 
#include <concepts>
#include <functional>

template <typename Invocable>
struct Invoker{
    using Invocable_t = Invocable;
    template <typename I> 
    requires std::convertible_to<I, Invocable_t>
    Invoker(I invocable): 
    invocable_(std::move(invocable)) {}; 
    
    template <typename... Args>
    requires std::invocable<Invocable&, Args...>
    decltype(auto) operator()(Args&&... args){
        return std::invoke(invocable_, std::forward<Args>(args)...);
    }
    
    private: 
    Invocable invocable_; 
};

template <typename I>
Invoker(I) -> Invoker<I>;
