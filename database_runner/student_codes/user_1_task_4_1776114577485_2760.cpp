
#include <iostream>
int main() {
    int a, b;
    if (!(std::cin >> a >> b)) {
        std::cout << "NO_INPUT";
        return 1;
    }
    std::cout << a + b;
    return 0;
}
