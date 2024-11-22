#include <cmath>
#include <iostream>
// #include <omp.h>
#include <vector>
#include <getopt.h>
#include <algorithm>

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

// Função para verificar se um número é primo
bool is_prime(int num) {
    if (num < 2) return false;
    for (int i = 2; i * i <= num; i++) {
        if (num % i == 0) return false;
    }
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
std::pair<int, int> generate_private_key() {
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
    int phi_n = (p - 1) * (q - 1);

    // Escolher e pequeno que seja coprimo com phi_n
    int e = 3;
    while (std::__gcd(e, phi_n) != 1 && e < phi_n) {
        e += 2; // Incrementar apenas números ímpares
    }
    if (e >= phi_n) {
        std::cout << "Error: Could not generate a valid public key!\n";
        exit(1);
    }
    return std::make_pair(e, n);
}
int main(int argc, char *argv[]) {
    int opt;
    std::string message;
    std::pair <int, int> private_keys, public_keys;
    int d, n, e, p, q, phi_n;
    std::vector<int> encrypted, encrypted_message;
    std::string decrypted, num_str;

    while ((opt = getopt(argc, argv, "hged")) != -1) {
        switch (opt) {
            case 'h':
                std::cout << "Usage: " << argv[0] << " [-h] [-g]\n";
                std::cout << "Options:\n";
                std::cout << "  -h  Show this help message and exit\n";
                std::cout << "  -g  Generate a public key\n";
                return 0;
            case 'g':
                std::cout << "Generating a key pair...\n";

                private_keys = generate_private_key();
                p = private_keys.first;
                q = private_keys.second;

                std::cout << "Private key primes (p, q): " << p << " " << q << std::endl;

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
            case 'e':
                std::cout << "Enter your private key (d, n): ";
                std::cin >> d >> n;
                std::cout << "Encrypting a message...\n";
                // lendo a frase criptografada
                std::cout << "Enter the message to encrypt: ";
                // std::cin.ignore();
                // std::getline(std::cin, message);
                message = "eu amo o dvd!";
                encrypted = encrypt(message, d, n);
                std::cout << "Encrypted message: ";
                for (int num : encrypted) {
                    std::cout << num << " ";
                }
                std::cout << std::endl;
                return 0;
            case 'd':
                std::cout << "Enter your public key (e, n): ";
                std::cin >> e >> n;
                std::cout << "Decrypting a message...\n";
                std::cout << "Enter the message to decrypt: ";
                std::cin.ignore();
                std::getline(std::cin, message);
                num_str = "";
                for (char ch : message) {
                    if (ch == ' ') {
                        encrypted_message.push_back(std::stoi(num_str));
                        num_str = "";
                    } else {
                        num_str += ch;
                    }
                }
                encrypted_message.push_back(std::stoi(num_str));
                decrypted = decrypt(encrypted_message, e, n);
                std::cout << "Decrypted message: " << decrypted << std::endl;
                return 0;
            case '?':
                std::cerr << "Usage: " << argv[0] << " [-h] [-g]\n";
                return 1;
        }
    }
    // set random seed
    srand(time(NULL));

    // Lendo a chave pública do terminal
    std::cout << "Enter the public key (e, n): ";
    std::cin >> e >> n;

    auto [p_, q_] = factorize_n(n);

    std::cout << "p: " << p << std::endl;
    std::cout << "q: " << q << std::endl;

    d = find_private_key(e, n, p, q);

    std::cout << "The private key is: " << d << std::endl;

    // Escolhendo uma frase aleatória para criptografar
    std::string random_phrase = phrases[rand() % 5];
    std::cout << "Random phrase to encrypt: " << random_phrase << std::endl;

    // Criptografando a frase
    encrypted = encrypt(random_phrase, e, n);

    // Exibindo a frase criptografada
    std::cout << "Encrypted phrase: ";
    for (int num : encrypted) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    // Descriptografando a frase
    decrypted = decrypt(encrypted, d, n);
    std::cout << "Decrypted phrase: " << decrypted << std::endl;

    return 0;
}
