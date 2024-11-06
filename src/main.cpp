#include <iostream>
#include <cmath>
#include <omp.h>

bool is_prime(int n) {
    if (n < 2) {
        return false;
    }

    // using brute force to check if n is prime
    for (int i = 2; i <= sqrt(n); i++) {
        if (n % i == 0) {
            return false;
        }
    }
    return true;
}

std::pair<int, int> factorize_n(int n) {
    int p = 0, q = 0;
    // using brute force to find the factors of n
    // based on the euclidean algorithm
    for (int i = 2; i <= sqrt(n); i++) {
        if (is_prime(i) && n % i == 0) {
            int possible_q = n / i;
            if (is_prime(possible_q)) {
                p = i;
                q = possible_q;
                break;
            }
        }
    }
    return std::make_pair(p, q);
}

int main() {
    int e, n;


    // reading the public key from the terminal
    std::cout << "Enter the public key (e, n): ";
    std::cin >> e >> n;

    auto [p, q] = factorize_n(n);

    std::cout << "p: " << p << std::endl;
    std::cout << "q: " << q << std::endl;

    int phi_n = (p - 1) * (q - 1);

    // finding the private key
    int d;


    return 0;
}