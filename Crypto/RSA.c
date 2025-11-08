#include <stdio.h>
#include <math.h>

// 1. GCD Function (finds Greatest Common Divisor)
// (Used to find 'e')
int gcd(int a, int b) {
    int temp;
    while (b != 0) {
        temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

// 2. Modular Exponentiation Function: (base^exp) % mod
// (Used for both encryption and decryption)
// Note: Using long long to prevent overflow during intermediate steps
long long power(long long base, long long exp, long long mod) {
    long long res = 1;
    base %= mod;
    
    while (exp > 0) {
        if (exp % 2 == 1) { // If exp is odd
            res = (res * base) % mod;
        }
        exp = exp / 2; // exp >>= 1
        base = (base * base) % mod;
    }
    return res;
}

// 3. Modular Multiplicative Inverse Function
// Finds 'd' such that (e * d) % phi = 1
// Uses a simple linear search. Extended Euclidean Algorithm is
// more efficient but complex. This is shorter for a lab.
int modInverse(int e, int phi) {
    for (int d = 1; d < phi; d++) {
        if ((e * d) % phi == 1) {
            return d;
        }
    }
    return -1; // Should not happen if e is valid
}

// 4. Main RSA Function
int main() {
    // Use small prime numbers for this example
    // (In reality, these are hundreds of digits long)
    int p, q;
    printf("Enter a prime number (p): ");
    scanf("%d", &p);
    printf("Enter another prime number (q): ");
    scanf("%d", &q);
    
    // --- Key Generation ---
    
    // 1. Calculate n = p * q
    long long n = (long long)p * q;
    
    // 2. Calculate Euler's Totient (phi)
    long long phi = (long long)(p - 1) * (q - 1);
    
    // 3. Find 'e' (public exponent)
    // e must be 1 < e < phi and gcd(e, phi) = 1
    long long e = 2; // Start checking from 2
    while (e < phi) {
        if (gcd(e, phi) == 1) {
            break; // Found a valid 'e'
        }
        e++;
    }
    
    // 4. Find 'd' (private exponent)
    // d is the modular inverse of e mod phi
    long long d = modInverse(e, phi);
    
    printf("\n--- Key Generation ---\n");
    printf("p = %d, q = %d\n", p, q);
    printf("n (p*q) = %lld\n", n);
    printf("phi = %lld\n", phi);
    printf("Public Key (e, n) = (%lld, %lld)\n", e, n);
    printf("Private Key (d, n) = (%lld, %lld)\n", d, n);
    
    // --- Encryption & Decryption ---
    
    long long msg;
    printf("\nEnter a message (as a number < %lld): ", n);
    scanf("%lld", &msg);
    
    if (msg >= n) {
        printf("Error: Message must be smaller than n.\n");
        return 1;
    }
    
    // 5. Encryption: C = (msg^e) % n
    long long ciphertext = power(msg, e, n);
    printf("\n--- Encryption ---\n");
    printf("Original Message: %lld\n", msg);
    printf("Ciphertext: %lld\n", ciphertext);
    
    // 6. Decryption: M' = (ciphertext^d) % n
    long long decrypted_msg = power(ciphertext, d, n);
    printf("\n--- Decryption ---\n");
    printf("Ciphertext: %lld\n", ciphertext);
    printf("Decrypted Message: %lld\n", decrypted_msg);

    return 0;
}