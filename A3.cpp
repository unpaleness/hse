#include <iostream>

int main() {
    uint64_t n{0};
    std::cin >> n;
    uint64_t res{0};
    uint64_t k{1};
    uint64_t k2{1};
    while (k2 <= n) {
        res += n / k2;
        ++k;
        k2 = k * k;
    }
    std::cout << res << '\n';
    return 0;
}
