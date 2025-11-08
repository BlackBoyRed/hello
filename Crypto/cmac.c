#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Define block size (16 bytes = 128 bits for AES)
#define BLOCK_SIZE 16
// Constant for 128-bit subkey generation
const unsigned char R_b = 0x87; 

// --- Placeholder Block Cipher ---
/**
 * @brief A simple, non-secure placeholder for AES-128.
 * @param in 16-byte input block.
 * @param out 16-byte output block.
 * @param key 16-byte key.
 */
void simple_encrypt(unsigned char *in, unsigned char *out, unsigned char *key) {
    // This is NOT encryption. It's a stand-in.
    // A real implementation would have the full AES algorithm here.
    for (int i = 0; i < BLOCK_SIZE; i++) {
        out[i] = in[i] ^ key[i];
    }
}
// ---------------------------------

// --- Helper Functions ---
void xor_blocks(unsigned char *a, unsigned char *b, int len) {
    for (int i = 0; i < len; i++) {
        a[i] = a[i] ^ b[i];
    }
}

// Helper to perform (block << 1)
void left_shift_one_bit(unsigned char *arr) {
    unsigned char carry = 0;
    for (int i = BLOCK_SIZE - 1; i >= 0; i--) {
        unsigned char next_carry = (arr[i] & 0x80) ? 1 : 0;
        arr[i] = (arr[i] << 1) | carry;
        carry = next_carry;
    }
}

/**
 * @brief Generates CMAC subkeys K1 and K2.
 */
void generate_subkeys(unsigned char *key, unsigned char *K1, unsigned char *K2) {
    unsigned char L[BLOCK_SIZE];
    unsigned char Z[BLOCK_SIZE];
    memset(Z, 0, BLOCK_SIZE);
    
    // 1. L = encrypt(K, Zeros)
    simple_encrypt(Z, L, key);

    // 2. Calculate K1
    memcpy(K1, L, BLOCK_SIZE);
    unsigned char msb = K1[0] & 0x80;
    left_shift_one_bit(K1);
    if (msb) {
        K1[BLOCK_SIZE - 1] ^= R_b;
    }

    // 3. Calculate K2
    memcpy(K2, K1, BLOCK_SIZE);
    msb = K2[0] & 0x80;
    left_shift_one_bit(K2);
    if (msb) {
        K2[BLOCK_SIZE - 1] ^= R_b;
    }
}

/**
 * @brief Computes the CMAC of a message.
 */
void cmac(unsigned char *key, unsigned char *msg, int msg_len, unsigned char *mac_out) {
    unsigned char K1[BLOCK_SIZE], K2[BLOCK_SIZE];
    generate_subkeys(key, K1, K2);

    int n_blocks = (msg_len + BLOCK_SIZE - 1) / BLOCK_SIZE;
    if (n_blocks == 0) n_blocks = 1; // Must process at least one block
    
    int last_block_complete = (msg_len % BLOCK_SIZE == 0) && (msg_len != 0);
    
    unsigned char Y[BLOCK_SIZE];
    memset(Y, 0, BLOCK_SIZE);
    
    // Process all but the last block
    for (int i = 0; i < n_blocks - 1; i++) {
        xor_blocks(Y, msg + i * BLOCK_SIZE, BLOCK_SIZE);
        simple_encrypt(Y, Y, key);
    }

    // --- Process Last Block ---
    unsigned char M_last[BLOCK_SIZE];
    int last_block_len = msg_len - (n_blocks - 1) * BLOCK_SIZE;
    
    if (last_block_complete) {
        memcpy(M_last, msg + (n_blocks - 1) * BLOCK_SIZE, BLOCK_SIZE);
        xor_blocks(M_last, K1, BLOCK_SIZE);
    } else {
        // Pad the last block
        memset(M_last, 0, BLOCK_SIZE);
        memcpy(M_last, msg + (n_blocks - 1) * BLOCK_SIZE, last_block_len);
        M_last[last_block_len] = 0x80; // This is the '10...' padding
        
        xor_blocks(M_last, K2, BLOCK_SIZE);
    }
    
    // Final CBC step
    xor_blocks(Y, M_last, BLOCK_SIZE);
    simple_encrypt(Y, mac_out, key);
}

// Helper to print a MAC
void print_mac(char *label, unsigned char *mac) {
    printf("%s: ", label);
    for (int i = 0; i < BLOCK_SIZE; i++) {
        printf("%02X", mac[i]);
    }
    printf("\n");
}

int main() {
    // Key must be 16 bytes for this example
    unsigned char key[] = "0123456789ABCDEF"; 
    unsigned char msg[] = "This is the message.";
    unsigned char mac1[BLOCK_SIZE];
    
    // 1. Generate MAC
    cmac(key, msg, strlen((char*)msg), mac1);
    printf("Original Message: '%s'\n", msg);
    print_mac("CMAC 1", mac1);

    // 2. Tamper with message
    unsigned char msg_tampered[] = "This is the message!";
    unsigned char mac2[BLOCK_SIZE];

    // 3. Generate MAC for tampered message
    cmac(key, msg_tampered, strlen((char*)msg_tampered), mac2);
    printf("\nTampered Message: '%s'\n", msg_tampered);
    print_mac("CMAC 2", mac2);
    
    // 4. Verify (using memcmp for byte arrays)
    if (memcmp(mac1, mac2, BLOCK_SIZE) == 0) {
        printf("\nVERIFICATION FAILED: MACs are identical.\n");
    } else {
        printf("\nVERIFICATION SUCCESS: MACs are different. Tampering detected.\n");
    }

    return 0;
}