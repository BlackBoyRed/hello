#include <stdio.h>
#include <string.h>
#include <math.h>

// --- RSA Helper Functions ---

// 1. GCD Function
int gcd(int a, int b) {
    int temp;
    while (b != 0) { temp = b; b = a % b; a = temp; }
    return a;
}

// 2. Modular Exponentiation: (base^exp) % mod
long long power(long long base, long long exp, long long mod) {
    long long res = 1;
    base %= mod;
    while (exp > 0) {
        if (exp % 2 == 1) res = (res * base) % mod;
        exp = exp / 2;
        base = (base * base) % mod;
    }
    return res;
}

// 3. Modular Inverse: (e * d) % phi = 1
int modInverse(int e, int phi) {
    for (int d = 1; d < phi; d++) {
        if ((e * d) % phi == 1) return d;
    }
    return -1;
}

// --- HASH Helper Function (Placeholder) ---
/**
 * @brief A simple, non-cryptographic placeholder hash.
 * @param str The message to hash.
 * @return A small integer hash.
 */
unsigned int simple_hash(char *str) {
    unsigned int hash = 5381;
    int c;
    while ((c = *str++))
        hash = ((hash << 5) + hash) + c;
    
    // Modulo n to ensure hash is < n
    return hash % 91; // Using 91 as n for this example
}

int main() {
    // --- Key Generation (Sender's Keys) ---
    // (p=7, q=13) -> n=91, phi=72
    long long n = 91;
    long long e = 5;  // Public Key Exponent
    long long d = 29; // Private Key Exponent
    
    printf("--- Sender's Keys ---\n");
    printf("Public Key (e, n) = (%lld, %lld)\n", e, n);
    printf("Private Key (d, n) = (%lld, %lld)\n\n", d, n);

    // --- Sender (Signing) ---
    printf("--- Sender Side ---\n");
    char *message = "This is a signed message.";
    printf("Original Message: '%s'\n", message);
    
    // 1. Hash the message
    unsigned int msg_hash = simple_hash(message);
    printf("Message Hash: %u\n", msg_hash);
    
    // 2. Sign (encrypt) the hash with the Private Key
    long long signature = power(msg_hash, d, n);
    printf("Digital Signature: %lld\n", signature);

    // --- Receiver (Verification) ---
    printf("\n--- Receiver Side ---\n");
    printf("Message Received: '%s'\n", message);
    printf("Signature Received: %lld\n", signature);
    
    // 1. Decrypt the signature with the Public Key
    long long decrypted_hash = power(signature, e, n);
    printf("Decrypted Hash: %lld\n", decrypted_hash);

    // 2. Hash the received message
    unsigned int received_hash = simple_hash(message);
    printf("Calculated Hash: %u\n", received_hash);
    
    // 3. Compare the hashes
    if (decrypted_hash == received_hash) {
        printf("\nResult: SIGNATURE IS VALID.\n");
        printf("The message is authentic and has not been tampered with.\n");
    } else {
        printf("\nResult: SIGNATURE IS INVALID.\n");
        printf("The message is not authentic or has been tampered with.\n");
    }

    return 0;
}