#include <cmath>
#include <iostream>
#include <omp.h>

/*
  Dado um texto cifrado com o RSA e sabendo que p e q são números primos
  menores que 1024 (10 bits), faça um programa que encontre a chave privada
  d sendo que a chave pública {e , n} é conhecida.
*/

std::pair<int, int> factorize_n(int n) {
    int p = 0, q = 0;

    for (int i = 2; i * i <= n; i++) {
        if (n % i == 0) {
            p = i;
            q = n / p;
            break;
        }
    }

    if (!p || !q) {
        std::cout << "Error: could not find p or q!\n";
        exit(1);
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

    int phi_n = (p - 1) * (q - 1); // totient

    // finding the private key
    int d;

    return 0;
}
