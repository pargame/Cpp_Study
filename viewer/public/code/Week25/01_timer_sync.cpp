#include <iostream>
#include "asio.hpp" 

int main() {
    asio::io_context io;

    std::cout << "Sync wait 3 seconds...\n";
    asio::steady_timer t(io, asio::chrono::seconds(3));
    
    t.wait(); // Blocking

    std::cout << "Done!\n";
    return 0;
}
