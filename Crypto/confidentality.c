#include <stdio.h>
#include <math.h>
#include <string.h>

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
unsigned int simple_hash(char *str) {
    unsigned int hash = 5381;
    int c;
    while ((c = *str++))
        hash = ((hash << 5) + hash) + c;
    return hash % 1000; // Keep hash small for this demo
}

// --- Key Structure ---
typedef struct {
    long long n; // Modulus
    long long e; // Public exponent
    long long d; // Private exponent
} KeyPair;

// --- Main Simulation ---
int main() {
    // We will simulate two users, Alice and Bob
    // We hardcode their keys for this simulation
    
    // Alice's Keys (p=7, q=13) -> n=91, phi=72
    KeyPair Alice = {91, 5, 29}; // (n, e, d)
    
    // Bob's Keys (p=11, q=17) -> n=187, phi=160
    KeyPair Bob = {187, 7, 23}; // (n, e, d)
    
    printf("--- Simulation Setup ---\n");
    printf("Alice's Public Key (e, n) = (%lld, %lld)\n", Alice.e, Alice.n);
    printf("Bob's Public Key (e, n) = (%lld, %lld)\n", Bob.e, Bob.n);


    // --- 1. CONFIDENTIALITY ---
    // Alice sends a secret message (a number) to Bob.
    // She must encrypt it with BOB's PUBLIC key.
    
    printf("\n--- 1. Confidentiality (Alice to Bob) ---\n");
    long long secret_msg = 42;
    printf("Alice's secret message for Bob: %lld\n", secret_msg);
    
    // Alice encrypts with Bob's public key (e=7, n=187)
    long long encrypted_msg = power(secret_msg, Bob.e, Bob.n);
    printf("Encrypted message in transit: %lld\n", encrypted_msg);
    
    // Bob decrypts with his own PRIVATE key (d=23, n=187)
    long long decrypted_msg = power(encrypted_msg, Bob.d, Bob.n);
    printf("Bob decrypts message: %lld\n", decrypted_msg);
    
    if (decrypted_msg == secret_msg) {
        printf("Success: Confidentiality achieved!\n");
    } else {
        printf("Failure: Message corrupted.\n");
    }

    
    // --- 2. DIGITAL SIGNATURE ---
    // Alice sends a public message and "signs" it.
    // She "encrypts" a hash of the message with her PRIVATE key.
    
    printf("\n--- 2. Digital Signature (Alice signs) ---\n");
    char *public_msg = "Hello Bob, this is from Alice.";
    printf("Alice's public message: '%s'\n", public_msg);
    
    // 1. Alice hashes the message
    unsigned int msg_hash = simple_hash(public_msg);
    printf("Message Hash: %u\n", msg_hash);
    
    // 2. Alice "signs" (encrypts) the hash with her PRIVATE key (d=29, n=91)
    long long signature = power(msg_hash, Alice.d, Alice.n);
    printf("Alice's Digital Signature: %lld\n", signature);
    
    // ... (Alice sends 'public_msg' and 'signature' to Bob) ...
    
    printf("\n(Bob receives message and signature...)\n");
    
    // 3. Bob verifies the signature.
    //    He "decrypts" the signature with Alice's PUBLIC key (e=5, n=91)
    long long decrypted_hash = power(signature, Alice.e, Alice.n);
    printf("Bob decrypts signature to get hash: %lld\n", decrypted_hash);
    
    // 4. Bob hashes the message he received
    unsigned int bob_hash = simple_hash(public_msg);
    printf("Bob's calculated hash: %u\n", bob_hash);
    
    // 5. Compare the hashes
    if (decrypted_hash == bob_hash) {
        printf("Success: Signature is VALID. (Authenticity and Integrity OK)\n");
    } else {
        printf("Failure: Signature is INVALID!\n");
    }
    
    return 0;
}