#include <iostream>


template <typename T>
void print(T t) {
	std::cout << t;
}

template <typename T, typename... Args>
void print(T t, Args... args) {
	std::cout << t;
	print(args...);
}

template<typename... Args>
auto add(Args... args) {
	return (... + args); // fold expression
}

int main() {
	print(1, 2, 3, 4, 5, "\n");
	std:: cout << add(1, 2, 3, 4, 5) << std::endl;
	return 0;
}
