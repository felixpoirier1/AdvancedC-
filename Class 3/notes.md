# Class 3 notes

### Advanced Variadics
A variadic is a template that can take any number of arguments. 

```cpp
temlate <typename T>
T adder(T v) {
    return v;
}

template <typename T, typename... Args>
auto adder(T first, Args... args) {
    return first + adder(args...);
}
```

*In the above code snippet, the first function is the base case, and the second function is the recursive case.*

An example of functionality which utilizes variadics is `std::format`.
```cpp
std::string s = std::format("The answer is {}.", 42);
```/
