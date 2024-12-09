// Autores:
//   - David Gomes (GRR20211757)
//   - Vinícius Fontoura (GRR20206873)

#include <getopt.h>

#include <algorithm>
#include <cmath>
#include <iostream>

#define DEBUG

/*
  Dado um texto cifrado com o RSA e sabendo que p e q são números primos
  menores que 1024 (10 bits), faça um programa que encontre a chave privada
  d sendo que a chave pública {e , n} é conhecida.
*/

// Função para calcular exponenciação modular: (base^exp) % mod
int mod_exp(int base, int exp, int mod) {
    int result = 1;

    while (exp > 0) {
        if (exp % 2 == 1) { // Se o expoente é ímpar, multiplica pela base
            result = (result * base) % mod;
        }

        base = (base * base) % mod; // Eleva ao quadrado a base
        exp /= 2;
    }

    return result;
}

// Função para fatorar n em p e q
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

// Função para calcular a chave privada d
int find_private_key(int e, int n, int p, int q) {
    int phi_n = (p - 1) * (q - 1); // totient
    std::cout << "phi(n): " << phi_n << std::endl;

    int d = 0;

    for (int i = 1; i < phi_n; i++) {
        if ((i * e) % phi_n == 1) {
            d = i;
            break;
        }
    }

    return d;
}

// Função para verificar se um número é primo
bool is_prime(int num) {
    if (num < 2)
        return false;

    for (int i = 2; i * i <= num; i++)
        if (num % i == 0)
            return false;

    return true;
}

// Função para gerar números primos aleatórios de 10 bits
int generate_random_prime() {
    int prime;

    do {
        prime = rand() % 1024 + 1; // Gera números entre 1 e 1024
    } while (!is_prime(prime));

    return prime;
}

// Gerar chave privada (p, q), ambos com no máximo 10 bits
std::pair<int, int> generate_private_key_primes() {
    int p = generate_random_prime();
    int q;

    do {
        q = generate_random_prime();
    } while (p == q); // Garantir que p e q sejam diferentes

    return std::make_pair(p, q);
}

// Gerar chave pública (e, n) a partir de p e q
std::pair<int, int> generate_public_key(int p, int q) {
    int n = p * q;
    int r = (p - 1) * (q - 1);

#ifdef DEBUG
    std::cout << "n: " << n << std::endl;
    std::cout << "r: " << r << std::endl;
#endif

    // Escolher 'e' pequeno que seja coprimo com r
    int e;
    if (r % 2 == 0) {
        e = 3; // Se r é par, escolher 'e' ímpar
    } else {
        e = 2; // Se r é ímpar, escolher 'e' par
    }

    // Encontrar um 'e' que seja coprimo com r
    while (std::__gcd(e, r) != 1 && e < r) {
        e += 2; // Incrementar apenas números ímpares
    }

    if (e >= r) {
        std::cout << "Error: Could not generate a valid public key!\n";
        exit(1);
    }

#ifdef DEBUG
    std::cout << "e: " << e << std::endl;
#endif

    return std::make_pair(e, n);
}

int main(int argc, char *argv[]) {
    int opt;
    std::pair<int, int> private_key_primes, public_keys, factors;
    int d, n, e, p, q, phi_n;

    srand(time(NULL));

    while ((opt = getopt(argc, argv, "hgf")) != -1) {
        switch (opt) {
        case 'h':
            std::cout << "Usage: " << argv[0] << " [-h] [-g] [-f]\n";
            std::cout << "Options:\n";
            std::cout << "  -h  Show this help message and exit\n";
            std::cout << "  -g  Generate a public/private key pair\n";
            std::cout << "  -f  Find the private key given the public key\n";
            return 0;
        case 'f':
            std::cout << "Enter your public key (e, n): ";
            std::cin >> e >> n;
            factors = factorize_n(n);
            std::cout << "Finding the private key...\n";
            d = find_private_key(e, n, factors.first, factors.second);
            std::cout << "The private key (d, n) is: " << d << " " << n
                      << std::endl;
            return 0;
        case 'g':
            std::cout << "Generating a key pair...\n";

            private_key_primes = generate_private_key_primes();
            p = private_key_primes.first;
            q = private_key_primes.second;

#ifdef DEBUG
            std::cout << "Private key primes (p, q): " << p << " " << q
                      << std::endl;
#endif

            public_keys = generate_public_key(p, q);
            e = public_keys.first;
            n = public_keys.second;

            std::cout << "Public key (e, n): " << e << " " << n << std::endl;

            phi_n = (p - 1) * (q - 1);
            d = 0;
            for (int i = 1; i < phi_n; i++) {
                if ((i * e) % phi_n == 1) {
                    d = i;
                    break;
                }
            }

            std::cout << "Private key (d, n): " << d << " " << n << std::endl;
            return 0;
        case '?':
            std::cerr << "Usage: " << argv[0] << " [-h] [-g] [-f]\n";
            return 1;
        }
    }

    return 0;
}
