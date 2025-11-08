#include <stdio.h>
#include <stdlib.h>

// --- Curve & Point Definitions ---

// We define a simple curve: y^2 = x^3 + ax + b (mod p)
// Using small, known-good parameters for this example:
const int a = 1;
const int b = 1;
const int p = 23;

// Base Point G on the curve
const int Gx = 0;
const int Gy = 1;

// Define a structure for a point (x, y)
typedef struct {
    int x;
    int y;
} Point;

// --- Helper Functions ---

/**
 * @brief Custom modulo function to handle negative results.
 * (a % m) in C can be negative, but we need a positive result.
 */
int mod(int n, int m) {
    return (n % m + m) % m;
}

/**
 * @brief Finds the modular multiplicative inverse of 'a' mod 'm'.
 * Finds 'x' such that (a * x) % m = 1
 */
int modInverse(int n, int m) {
    for (int x = 1; x < m; x++) {
        if (mod(n * x, m) == 1) {
            return x;
        }
    }
    return -1; // Error
}

// --- Core ECC Operations ---

/**
 * @brief Adds two distinct points P and Q on the curve.
 * (This is the "Point Addition" rule)
 */
Point pointAdd(Point P, Point Q) {
    Point R;
    
    // 1. Calculate slope (s)
    // s = (Q.y - P.y) / (Q.x - P.x) mod p
    int numerator = mod(Q.y - P.y, p);
    int denominator = mod(Q.x - P.x, p);
    int s = mod(numerator * modInverse(denominator, p), p);
    
    // 2. Calculate new point R
    // R.x = (s^2 - P.x - Q.x) mod p
    R.x = mod(s * s - P.x - Q.x, p);
    // R.y = (s * (P.x - R.x) - P.y) mod p
    R.y = mod(s * (P.x - R.x) - P.y, p);
    
    return R;
}

/**
 * @brief Adds a point P to itself.
 * (This is the "Point Doubling" rule)
 */
Point pointDouble(Point P) {
    Point R;
    
    // 1. Calculate slope (s)
    // s = (3*P.x^2 + a) / (2*P.y) mod p
    int numerator = mod(3 * P.x * P.x + a, p);
    int denominator = mod(2 * P.y, p);
    int s = mod(numerator * modInverse(denominator, p), p);
    
    // 2. Calculate new point R
    // R.x = (s^2 - 2*P.x) mod p
    R.x = mod(s * s - 2 * P.x, p);
    // R.y = (s * (P.x - R.x) - P.y) mod p
    R.y = mod(s * (P.x - R.x) - P.y, p);
    
    return R;
}

/**
 * @brief Multiplies a point P by a scalar k.
 * (Calculates k*P using the efficient "double-and-add" algorithm)
 */
Point scalarMultiply(int k, Point P) {
    // 'result' is the "point at infinity" (identity)
    // For this simple code, we'll initialize with P
    // and start the loop from k-1.
    Point result = P;
    
    // This is a simple (less efficient) iterative add
    for(int i = 1; i < k; i++) {
        // We must check if P == Q
        // Since we add P to 'result' every time, we
        // check if result.x == P.x
        if (result.x == P.x && result.y == P.y) {
            result = pointDouble(result);
        } else {
            result = pointAdd(result, P);
        }
    }
    
    // A more efficient "double-and-add" is more complex.
    // This simple loop is shorter and demonstrates the concept.
    
    return result;
}


// --- Main Function (ECDH Demo) ---
int main() {
    printf("--- ECC Diffie-Hellman Key Exchange ---\n");
    printf("Curve: y^2 = x^3 + %dx + %d (mod %d)\n", a, b, p);
    printf("Base Point G = (%d, %d)\n\n", Gx, Gy);
    
    // Define the base point G
    Point G = {Gx, Gy};
    
    // 1. Alice chooses a private key (a number)
    int nA;
    printf("Enter Alice's private key (nA): ");
    scanf("%d", &nA);
    
    // 2. Bob chooses a private key (a number)
    int nB;
    printf("Enter Bob's private key (nB): ");
    scanf("%d", &nB);

    // 3. Calculate public keys
    Point PA = scalarMultiply(nA, G); // PA = nA * G
    Point PB = scalarMultiply(nB, G); // PB = nB * G
    
    printf("\n--- Public Key Calculation ---\n");
    printf("Alice's Public Key PA = (%d, %d)\n", PA.x, PA.y);
    printf("Bob's Public Key PB = (%d, %d)\n", PB.x, PB.y);
    
    // 4. Calculate shared secrets
    // Alice computes S = nA * PB
    Point S_Alice = scalarMultiply(nA, PB);
    
    // Bob computes S = nB * PA
    Point S_Bob = scalarMultiply(nB, PA);
    
    printf("\n--- Shared Secret Calculation ---\n");
    printf("Alice's Shared Secret: (%d, %d)\n", S_Alice.x, S_Alice.y);
    printf("Bob's Shared Secret:   (%d, %d)\n", S_Bob.x, S_Bob.y);
    
    // 5. Verification
    if (S_Alice.x == S_Bob.x && S_Alice.y == S_Bob.y) {
        printf("\nSuccess! Both parties computed the same secret.\n");
    } else {
        printf("\nError: Secrets do not match!\n");
    }

    return 0;
}