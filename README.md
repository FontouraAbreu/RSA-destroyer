# RSA-destroyer

The eater of encryptions

## Description

This is a Uni project for the course of Cryptography. The goal is to implement a program that can infer the private key of an RSA encryption given a 10bit public key. The program is written in C++.

## Original assignment (PT-BR)

Dato um texto cifrado com o RSA e sabendo que p e q são números primos menores que 1024 (10 bits), faça um programa que encontre a chave privada d sendo que a chave pública {e , n} é conhecida.

## Original assignment (EN)

Given a text encrypted with RSA and knowing that p and q are prime numbers smaller than 1024 (10 bits), make a program that finds the private key d given that the public key {e, n} is known.

## How it works

Given [link](https://condor.depaul.edu/ichu/csc415/notes/notes4/rsa.html): The public key is `{e, n}` and the private key is `{d, n}`. We need to find `d` which is given by the formula `d = e^-1 mod (p-1)(q-1)`.

There exists two prime numbers `p` and `q` such that `n = p * q`, so first we need to find `p` and `q`. We can do this by testing all prime numbers smaller than 1024 (2^10 bits) and checking if `n` is divisible by them. Once we find `p` and `q`, we can find `d` using the formula above.

Then we can decrypt a message using the formula `m = c^d mod n`, where `m` is the decrypted message and `c` is the encrypted message.

## How to run

To run the program, you need to have a C++ compiler installed. You can compile the program using the following command:

```bash
g++ main.cpp -o main
```

## Authors

Vinícius Fontoura de Abreu
David Lucas Pereira Gomes
