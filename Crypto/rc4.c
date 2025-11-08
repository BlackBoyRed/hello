#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Helper macro to swap two bytes
#define SWAP(a, b) do { unsigned char temp = a; a = b; b = temp; } while (0)

// 1. Key-Scheduling Algorithm (KSA)
// Initializes the state array 'S'
void ksa(unsigned char S[256], unsigned char *key, int key_len) {
    // Initialize S[i] = i
    for (int i = 0; i < 256; i++) {
        S[i] = i;
    }

    // Shuffle S based on the key
    int j = 0;
    for (int i = 0; i < 256; i++) {
        j = (j + S[i] + key[i % key_len]) % 256;
        SWAP(S[i], S[j]);
    }
}

// 2. Pseudo-Random Generation Algorithm (PRGA)
// Generates the keystream and XORs it with the data
void prga(unsigned char S[256], unsigned char *data, unsigned char *output, int data_len) {
    int i = 0;
    int j = 0;

    for (int k = 0; k < data_len; k++) {
        i = (i + 1) % 256;
        j = (j + S[i]) % 256;
        
        SWAP(S[i], S[j]);
        
        // Generate the keystream byte
        int t = (S[i] + S[j]) % 256;
        unsigned char keystream_byte = S[t];
        
        // XOR with data
        output[k] = data[k] ^ keystream_byte;
    }
}

// Helper to print hex values
void print_hex(const char *label, unsigned char *data, int len) {
    printf("%s: ", label);
    for (int i = 0; i < len; i++) {
        printf("%02X ", data[i]);
    }
    printf("\n");
}

// Main function
int main() {
    unsigned char key[256];
    unsigned char plaintext[1024];
    
    printf("Enter Key (e.g., 'Key'): ");
    fgets((char*)key, sizeof(key), stdin);
    key[strcspn((char*)key, "\n")] = 0; // Remove newline

    printf("Enter Plaintext (e.g., 'Plaintext'): ");
    fgets((char*)plaintext, sizeof(plaintext), stdin);
    plaintext[strcspn((char*)plaintext, "\n")] = 0; // Remove newline

    int key_len = strlen((char*)key);
    int text_len = strlen((char*)plaintext);
    
    unsigned char S[256];
    unsigned char ciphertext[1024];
    unsigned char decryptedtext[1024];

    // --- Encryption ---
    // 1. Initialize S-Box
    ksa(S, key, key_len);
    
    // 2. Encrypt (PRGA)
    prga(S, plaintext, ciphertext, text_len);
    
    printf("\nPlaintext: %s\n", plaintext);
    print_hex("Ciphertext (Hex)", ciphertext, text_len);

    // --- Decryption ---
    // 1. Re-initialize S-Box (CRITICAL!)
    // The S-Box state is changed by the PRGA.
    // It must be reset to the original KSA state.
    ksa(S, key, key_len); 
    
    // 2. Decrypt (PRGA)
    prga(S, ciphertext, decryptedtext, text_len);
    
    // Null-terminate the decrypted string
    decryptedtext[text_len] = '\0';
    printf("Decrypted: %s\n", decryptedtext);

    return 0;
}