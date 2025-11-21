#include <iostream>
#include <vector>
#include <thread>

void hello() {
    std::cout << "Hello from thread!" << std::endl;
}

int main() {
    std::cout << "Hello, C++23 Environment!" << std::endl;
    std::vector<int> v = {1, 2, 3, 4, 5};
    
    std::jthread t(hello);
    return 0;
}
