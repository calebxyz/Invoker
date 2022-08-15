#include <type_traits> 
#include <concepts>
#include <functional>

template <typename Invocable>
struct Invoker{
    using Invocable_T = std::conditional_t<std::is_function_v<Invocable>, 
    std::add_pointer_t<Invocable>, Invocable>;
    template <typename I> 
    requires std::convertible_to<I, Invocable_T>
    Invoker(I invocable): 
    invocable_(std::move(invocable)) {} 
    
    template <typename... Args>
    requires std::invocable<Invocable&, Args...>
    decltype(auto) operator()(Args&&... args){
        return std::invoke(invocable_, std::forward<Args>(args)...);
    }
    
    private: 
    Invocable_T invocable_; 
};

template <typename I>
Invoker(I) -> Invoker<I>;
