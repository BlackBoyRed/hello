#include <stdio.h>
#include <stdlib.h> // For rand()
#include <time.h>   // For time()

// 1. Modular Exponentiation Function: (base^exp) % mod
// (Using long long to prevent overflow)
long long power(long long base, long long exp, long long mod) {
    long long res = 1;
    base %= mod;
    
    while (exp > 0) {
        if (exp % 2 == 1) {
            res = (res * base) % mod;
        }
        exp = exp / 2;
        base = (base * base) % mod;
    }
    return res;
}

// 2. Modular Multiplicative Inverse Function
// Finds 'x' such that (a * x) % m = 1
int modInverse(int a, int m) {
    a = a % m;
    for (int x = 1; x < m; x++) {
        if ((a * x) % m == 1) {
            return x;
        }
    }
    return -1; // Should not happen for this algorithm
}

// 3. Main ElGamal Function
int main() {
    long long p, g, msg;
    long long x; // Private key
    long long y; // Public key component
    
    // --- Setup ---
    // In a real system, p is a large prime and g is a
    // primitive root modulo p. We'll use small, known values.
    
    printf("Enter a prime number (p): ");
    scanf("%lld", &p);
    
    printf("Enter a primitive root (g) for p: ");
    scanf("%lld", &g);
    
    // --- Key Generation ---
    
    // 1. Choose a private key 'x' (1 < x < p-1)
    printf("Enter a private key (x < %lld): ", p - 1);
    scanf("%lld", &x);
    
    // 2. Calculate public key 'y'
    // y = g^x (mod p)
    y = power(g, x, p);
    
    printf("\n--- Key Generation ---\n");
    printf("Public Key (p, g, y) = (%lld, %lld, %lld)\n", p, g, y);
    printf("Private Key (x) = %lld\n", x);
    
    // --- Encryption ---
    
    printf("\nEnter a message (as a number < %lld): ", p);
    scanf("%lld", &msg);
    
    // 3. Choose a random ephemeral key 'k' (1 < k < p-1)
    // For simplicity, we ask for it. A real system would generate
    // a new, cryptographically secure random 'k' for every message.
    long long k;
    printf("Enter a random value 'k' for this message (< %lld): ", p - 1);
    scanf("%lld", &k);

    // 4. Calculate Ciphertext (C1, C2)
    // C1 = g^k (mod p)
    long long c1 = power(g, k, p);
    
    // C2 = (msg * y^k) (mod p)
    long long c2 = (msg * power(y, k, p)) % p;
    
    printf("\n--- Encryption ---\n");
    printf("Original Message: %lld\n", msg);
    printf("Ciphertext (C1, C2) = (%lld, %lld)\n", c1, c2);
    
    // --- Decryption ---
    
    // 5. Compute shared secret S = C1^x (mod p)
    long long s = power(c1, x, p);
    
    // 6. Compute S inverse (S^-1 mod p)
    long long s_inv = modInverse(s, p);
    
    // 7. Recover message M' = (C2 * S^-1) (mod p)
    long long decrypted_msg = (c2 * s_inv) % p;
    
    printf("\n--- Decryption ---\n");
    printf("Ciphertext (C1, C2) = (%lld, %lld)\n", c1, c2);
    printf("Decrypted Message: %lld\n", decrypted_msg);

    return 0;
}