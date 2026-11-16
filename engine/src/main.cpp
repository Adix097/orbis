#include <iostream>

#include <orbis/version.hpp>

int main() {
    std::cout << "ORBIS engine " << orbis::version() << '\n';
    return 0;
}