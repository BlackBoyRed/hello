#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Define the block size for the hash function
// (e.g., 64 bytes for SHA-1/SHA-256, 128 bytes for SHA-512)
#define HASH_BLOCK_SIZE 64

// --- Placeholder Hash Function ---
/**
 * @brief A simple, non-cryptographic placeholder hash.
 * In a real lab, replace this with SHA-1, MD5, or SHA-256.
 * @param data The input data to hash.
 * @param len The length of the data.
 * @return A 4-byte (32-bit) hash value.
 */
unsigned int simple_hash(unsigned char *data, int len) {
    unsigned int hash = 5381; // A common starting prime
    for (int i = 0; i < len; i++) {
        hash = ((hash << 5) + hash) + data[i]; // hash * 33 + c
    }
    return hash;
}
// ---------------------------------

/**
 * @brief Computes the HMAC of a message given a key.
 * @param key The secret key.
 * @param key_len Length of the key.
 * @param msg The message to authenticate.
 * @param msg_len Length of the message.
 * @param mac_out Buffer to store the resulting HMAC (hash output size).
 */
void hmac(unsigned char *key, int key_len, unsigned char *msg, int msg_len, unsigned int *mac_out) {
    
    unsigned char k_pad[HASH_BLOCK_SIZE];
    unsigned char ipad = 0x36;
    unsigned char opad = 0x5C;
    
    // 1. Prepare the key (k_pad)
    memset(k_pad, 0, HASH_BLOCK_SIZE);
    if (key_len > HASH_BLOCK_SIZE) {
        // If key > block size, hash key to get a block-sized key
        // (We skip this for this short example)
        printf("Error: Key length not handled. Assuming < 64 bytes.\n");
        memcpy(k_pad, key, key_len); // Not a proper implementation
    } else {
        memcpy(k_pad, key, key_len);
    }
    
    // 2. Create k_ipad and k_opad
    unsigned char k_ipad[HASH_BLOCK_SIZE];
    unsigned char k_opad[HASH_BLOCK_SIZE];
    for (int i = 0; i < HASH_BLOCK_SIZE; i++) {
        k_ipad[i] = k_pad[i] ^ ipad;
        k_opad[i] = k_pad[i] ^ opad;
    }

    // --- Inner Hash ---
    // 3. Concatenate (k_ipad || message)
    int inner_len = HASH_BLOCK_SIZE + msg_len;
    unsigned char *inner_data = (unsigned char *)malloc(inner_len);
    memcpy(inner_data, k_ipad, HASH_BLOCK_SIZE);
    memcpy(inner_data + HASH_BLOCK_SIZE, msg, msg_len);
    
    // 4. Hash the inner data
    unsigned int inner_hash = simple_hash(inner_data, inner_len);
    
    free(inner_data);

    // --- Outer Hash ---
    // 5. Concatenate (k_opad || inner_hash)
    // Size of inner_hash is 4 bytes (unsigned int)
    int outer_len = HASH_BLOCK_SIZE + sizeof(unsigned int);
    unsigned char *outer_data = (unsigned char *)malloc(outer_len);
    memcpy(outer_data, k_opad, HASH_BLOCK_SIZE);
    memcpy(outer_data + HASH_BLOCK_SIZE, &inner_hash, sizeof(unsigned int));
    
    // 6. Hash the outer data
    *mac_out = simple_hash(outer_data, outer_len);
    
    free(outer_data);
}

int main() {
    unsigned char key[] = "mysecretkey";
    unsigned char msg[] = "This is the message to authenticate.";
    unsigned int mac1, mac2;

    // 1. Generate MAC for the original message
    hmac(key, strlen((char*)key), msg, strlen((char*)msg), &mac1);
    printf("Original Message: '%s'\n", msg);
    printf("HMAC 1: %u\n\n", mac1);

    // 2. Tamper with the message
    unsigned char msg_tampered[] = "This is the message to authenticate!"; // Changed '.' to '!'
    
    // 3. Generate MAC for the tampered message
    hmac(key, strlen((char*)key), msg_tampered, strlen((char*)msg_tampered), &mac2);
    printf("Tampered Message: '%s'\n", msg_tampered);
    printf("HMAC 2: %u\n\n", mac2);
    
    // 4. Verify message tampering
    if (mac1 == mac2) {
        printf("VERIFICATION FAILED: MACs are identical.\n");
    } else {
        printf("VERIFICATION SUCCESS: MACs are different. Tampering detected.\n");
    }

    return 0;
}