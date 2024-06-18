#include <iostream>
#include <string>

int main() {
    std::string line = "TODO: revive hitler";
    if (line.substr(6) == "revive hitler") {
        std::cout << "TRUE";
    } else {
        std::cout << line.substr(7);
    }
}
