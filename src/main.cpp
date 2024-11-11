#include <cmath>
#include <iostream>
#include <omp.h>
#include <vector>

/*
  Dado um texto cifrado com o RSA e sabendo que p e q são números primos
  menores que 1024 (10 bits), faça um programa que encontre a chave privada
  d sendo que a chave pública {e , n} é conhecida.
*/

// Função para calcular exponenciação modular: (base^exp) % mod
int mod_exp(int base, int exp, int mod) {
    int result = 1;
    while (exp > 0) {
        if (exp % 2 == 1) { // Se o expoente é ímpar, multiplica pelo base
            result = (result * base) % mod;
        }
        base = (base * base) % mod; // Eleva ao quadrado o base
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

    int d = 0;

    for (int i = 1; i < phi_n; i++) {
        if ((i * e) % phi_n == 1) {
            d = i;
            break;
        }
    }

    return d;
}

// Função de criptografia usando exponenciação modular
std::vector<int> encrypt(const std::string &m, int e, int n) {
    std::vector<int> encrypted_message;
    for (char ch : m) {
        int encrypted_char = mod_exp(static_cast<int>(ch), e, n);
        encrypted_message.push_back(encrypted_char);
    }
    return encrypted_message;
}

// Função de descriptografia usando exponenciação modular
std::string decrypt(const std::vector<int> &encrypted_message, int d, int n) {
    std::string decrypted_message = "";
    for (int c : encrypted_message) {
        int decrypted_char = mod_exp(c, d, n);
        decrypted_message += static_cast<char>(decrypted_char);
    }
    return decrypted_message;
}

// array de frases para testar o algoritmo
std::string phrases[] = {
    "Hello, World!",
    "The quick brown fox jumps over the lazy dog.",
    "Lorem ipsum dolor sit amet, consectetur adipiscing elit",
    "The quick brown fox jumps over the lazy dog.",
    "Luiz Carlos Pessoa Albini"
};

int main(int argc, char *argv[]) {
    // set random seed
    srand(time(NULL));
    int e, n;

    // Lendo a chave pública do terminal
    std::cout << "Enter the public key (e, n): ";
    std::cin >> e >> n;

    auto [p, q] = factorize_n(n);

    std::cout << "p: " << p << std::endl;
    std::cout << "q: " << q << std::endl;

    int d = find_private_key(e, n, p, q);

    std::cout << "The private key is: " << d << std::endl;

    // Escolhendo uma frase aleatória para criptografar
    std::string random_phrase = phrases[rand() % 5];
    std::cout << "Random phrase to encrypt: " << random_phrase << std::endl;

    // Criptografando a frase
    std::vector<int> encrypted = encrypt(random_phrase, e, n);

    // Exibindo a frase criptografada
    std::cout << "Encrypted phrase: ";
    for (int num : encrypted) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    // Descriptografando a frase
    std::string decrypted = decrypt(encrypted, d, n);
    std::cout << "Decrypted phrase: " << decrypted << std::endl;

    return 0;
}
