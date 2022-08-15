# Invoker
Generic Invocation wrapper , lightweight function written in C++20 

As you may know std::functions are heavy because they use Type Erasure and their invocation might be much slower then 
a regular lambdas or a free functions but on the other hand we cannot use lambdas in virtual functions (yet) 
because we need to make the function templated.

So this small wrapper does the trick for you, 
you may use it very similar to std::function, 
passing a certain type of function pointer or just use it as a auto deduced template type where you may do that.

As you can see the performance is the same as using a lambda and it is much better then with std::function, The benchmark was compiled with Clang 13.

Usage example:

```C++
void foo(int i){
};

struct functor{
    bool operator ()(){
        return true;
    }
};

int doit(Invoker<std::add_pointer_t<int(const int&)>> f){
    return f(1);
}

int main(){
    Invoker<std::add_pointer_t<int(const int&)>> 
    f1([](const int& i) {return i;});
    auto f2 = Invoker([](const int& i) {return i+10;});
    auto f3 = Invoker(foo);
    auto f4 = Invoker(functor());
    auto f5 = Invoker([f1](int i) mutable {return f1(i);});
    return doit(f1);
}```






