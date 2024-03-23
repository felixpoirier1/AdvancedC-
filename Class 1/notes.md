# Reflections

*Feature to be introduced in C++26*

Reflections allow C++ to get information about types at runtime.

```cpp
constexpr auto r = ^int; // r has type std::meta::info, which is a reflection object

typename
```

# Fundamental Types

* Integral types
* Floating-point types
* Character types

*All of the above types are built up from fundamental types (like atoms are to matter)* 

Fundamental built-in types are not fully specified by the standard, in order to adapt to the hardware. There are 3 kinds of behaviors of code that is not fully specified:
* Undefined behavior (least specified)
    - Example: Reasing a random memory address (program is likely to crash)
* Unspecified behavior (implementation has several choices)
    - Example:  `0.1 + 0.2 == 0.1 + 0.2` may or may not be true
* Implementation-defined behavior (implementation must document the behavior)
    - Example: Wether or not `char` is signed or unsigned

In general the better-defined, the better (i.e. prefer C++ variants over unions)

## *A deep dive into integers*
In C++ we have access to the following integer types that give us access to hardware capabilities:
* basic types (size is **implementation-defined**)
    - `short`: At least 16 bits
    - `int`: At least 16 bits
    - `long`: At least 32 bits
    - `long long`: At least 64 bits
* fixed-width types (size is **fixed**)
    - `int8_t`: Exactly 8 bits
    - `int16_t`: Exactly 16 bits
    - `int32_t`: Exactly 32 bits
    - `int64_t`: Exactly 64 bits
    - `uint8_t`: Exactly 8 bits and unsigned
    - etc.
* fastest types (size is **implementation-defined**)
    - `int_fast8_t`: At least 8 bits
    - `int_fast16_t`: At least 16 bits
    - `int_fast32_t`: At least 32 bits
    - `int_fast64_t`: At least 64 bits
    - `uint_fast8_t`: At least 8 bits and unsigned
* specialized integer types
    - `intptr_t`: Integer type that can hold a pointer
    - `intmax_t`: Largest integer type
    - `size_t`: Unsigned integer type that can hold the size of any object
    - `ptrdiff_t`: Signed integer type that can hold the difference between two pointers
    - `wchar_t`: Integer type that can hold a wide character
    - `char16_t`: Integer type that can hold a UTF-16 character
    - `char32_t`: Integer type that can hold a UTF-32 character

*To know what can fin into a type, use `std::numeric_limits<int>` => this will return the largest integer*

## Floating-point types
In C++ we have access to the following floating-point types that give us access to hardware capabilities:
* basic types (size is **implementation-defined**)
    - `float`: At least 32 bits (rarely used from IEEE 754)
    - `double`: At least 64 bits (from IEEE 754)
    - `long double`: At least 80 bits 

*Neural networks have introduced the need for 16-bit floating-point numbers (half-float)*

## Character types
The `char` type is the smallest type (i.e. it's C++'s byte) `sizeof(char) == 1`

More generally `wchar_t` is used for wide characters (e.g. emojis) but is not recommended. Instead `char16_t` and `char32_t` should be used for UTF-16 and UTF-32 characters respectively.

## Additional fundamental types
* `void`: Represents the absence of type
* `nullptr_t`: Represents the null pointer
* `bool`: Represents a boolean value

# Advanced Notifications
Last quarter they learned the `condition_variable` class... (from my understanding it's a way to notify a thread that a condition has been met, a lock is released, and the thread can continue)

*However locks are not always what we want*

## Condition variables

```
condition_variable cv;
mutex m;
boolean test();
unique_lock<mutex> lock(m);
cv.wait(lock, test);
```

RAII : Resource Acquisition Is Initialization

A solution is to use scope `{}` which will automatically release the lock when the scope is exited.

```cpp
{
    unique_lock<mutex> lock(m);
    cv.wait(lock, test);
}
```

however, a solution like this may disproportianately advantage a single thread, and make the program slower.

Instead, we can use `shared_mutex` which allows multiple threads to read a shared resource, but only one thread to write to it.

```cpp
shared_mutex m;
shared_lock<shared_mutex> lock(m); 
```

## Atomics
`std::atomic<T>`: A type that can be modified atomically (i.e. no other thread can modify it at the same time)

In C++20, we can do the program shown in slides fully atomically. 

Atomic variables have `wait` and `notify` functions that can be used to notify other threads that a condition has been met.

There is also `atomic_flag` which is a boolean atomic variable that can be used to implement spinlocks. (a spinlock is a lock that keeps checking if a condition is met, and if it is, it will lock)
* Prior to C++20, `atomic_flag` could have been initialized with garbage, `ATOMIC_FLAG_INIT` was used to initialize it to `false`

**LOOKUP**
* The `condition_variable` class
* `std::atomic<T>` class
