#include <stdio.h>

// 1. GCD Function (finds Greatest Common Divisor)
int gcd(int a, int b) {
    int temp;
    while (b != 0) {
        temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

// 2. Euler's Totient (phi) Function
// Counts positive integers <= n that are relatively prime to n
int phi(int n) {
    int count = 0;
    for (int i = 1; i <= n; i++) {
        if (gcd(i, n) == 1) {
            count++;
        }
    }
    return count;
}

// 3. Modular Exponentiation Function: (base^exp) % mod
// Uses (a * b) % m = ((a % m) * (b % m)) % m to prevent overflow
long long power(long long base, int exp, int mod) {
    long long res = 1;
    base %= mod; // Apply modulo to base
    
    while (exp > 0) {
        // If exp is odd, multiply base with result
        if (exp % 2 == 1) {
            res = (res * base) % mod;
        }
        
        // exp must be even now
        exp = exp / 2; // Equivalent to exp >>= 1
        base = (base * base) % mod; // Change base to base^2
    }
    return res;
}

// 4. Main function to verify the theorem
int main() {
    int a, n;
    
    printf("Enter base (a): ");
    scanf("%d", &a);
    printf("Enter modulus (n): ");
    scanf("%d", &n);

    // Step 1: Check if a and n are coprime
    if (gcd(a, n) != 1) {
        printf("Error: %d and %d are not coprime.\n", a, n);
        printf("Euler's Theorem does not apply.\n");
        return 1;
    }

    // Step 2: Calculate phi(n)
    int phin = phi(n);
    printf("Euler's Totient phi(%d) = %d\n", n, phin);

    // Step 3: Calculate (a^phi(n)) % n
    long long result = power(a, phin, n);

    // Step 4: Verify the theorem
    printf("Verifying: %d^%d (mod %d) = %lld\n", a, phin, n, result);

    if (result == 1) {
        printf("Theorem Verified: Result is 1.\n");
    } else {
        printf("Theorem Verification Failed.\n");
    }

    return 0;
}