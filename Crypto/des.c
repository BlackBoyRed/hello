#include <stdio.h>

// S-DES Permutations and S-Boxes
// Note: Bits are numbered 1-10 or 1-8. We adjust by -1 for 0-based indexing.

// P10 (Input: 10 bits, Output: 10 bits)
const int P10[] = { 3, 5, 2, 7, 4, 10, 1, 9, 8, 6 };
// P8 (Input: 10 bits from LS-1/2, Output: 8 bits)
const int P8[] = { 6, 3, 7, 4, 8, 5, 10, 9 };

// Initial Permutation IP (Input: 8 bits, Output: 8 bits)
const int IP[] = { 2, 6, 3, 1, 4, 8, 5, 7 };
// Final Permutation IP-1 (Input: 8 bits, Output: 8 bits)
const int IP_INV[] = { 4, 1, 3, 5, 7, 2, 8, 6 };

// Expansion/Permutation E/P (Input: 4 bits, Output: 8 bits)
const int EP[] = { 4, 1, 2, 3, 2, 3, 4, 1 };
// P4 (Input: 4 bits from S-Boxes, Output: 4 bits)
const int P4[] = { 2, 4, 3, 1 };

// S-Boxes (Input: 4 bits, Output: 2 bits)
const int S0[4][4] = {
    { 1, 0, 3, 2 },
    { 3, 2, 1, 0 },
    { 0, 2, 1, 3 },
    { 3, 1, 3, 2 }
};

const int S1[4][4] = {
    { 0, 1, 2, 3 },
    { 2, 0, 1, 3 },
    { 3, 0, 1, 0 },
    { 2, 1, 0, 3 }
};

// --- Helper Functions ---

/**
 * @brief Performs a generic permutation.
 * @param input The input bit block (as an integer).
 * @param table The permutation table (e.g., P10, IP).
 * @param in_bits Max bit position in input (e.g., 10 for P10).
 * @param out_bits Number of bits in output (e.g., 8 for P8).
 * @return The permuted bit block.
 */
int permute(int input, const int *table, int in_bits, int out_bits) {
    int output = 0;
    for (int i = 0; i < out_bits; i++) {
        // Find the bit to pull
        int bit_pos = table[i];
        // Check if that bit is set in the input
        if ((input >> (in_bits - bit_pos)) & 1) {
            // Set the corresponding bit in the output
            output |= (1 << (out_bits - 1 - i));
        }
    }
    return output;
}

/**
 * @brief Performs a left shift (LS-1) on a 5-bit half of a key.
 * @param key_half 5-bit integer.
 * @return 5-bit integer, shifted left by 1.
 */
int left_shift_1(int key_half) {
    // 0b11111 -> 5-bit mask
    int shifted = ((key_half << 1) & 0b11110) | (key_half >> 4);
    return shifted;
}

/**
 * @brief Performs a left shift (LS-2) on a 5-bit half of a key.
 * @param key_half 5-bit integer.
 * @return 5-bit integer, shifted left by 2.
 */
int left_shift_2(int key_half) {
    int shifted = ((key_half << 2) & 0b11100) | (key_half >> 3);
    return shifted;
}

/**
 * @brief Generates K1 and K2 from a 10-bit master key.
 * @param key 10-bit master key.
 * @param k1 Pointer to store 8-bit K1.
 * @param k2 Pointer to store 8-bit K2.
 */
void generate_keys(int key, int *k1, int *k2) {
    // 1. P10
    int p10_key = permute(key, P10, 10, 10);
    
    // 2. Split into 5-bit halves
    int left = (p10_key >> 5) & 0b11111; // Mask for 5 bits
    int right = p10_key & 0b11111;
    
    // 3. LS-1
    left = left_shift_1(left);
    right = left_shift_1(right);
    
    // 4. Combine and P8 to get K1
    int combined_ls1 = (left << 5) | right;
    *k1 = permute(combined_ls1, P8, 10, 8);
    
    // 5. LS-2 (on the already shifted halves)
    left = left_shift_2(left);
    right = left_shift_2(right);
    
    // 6. Combine and P8 to get K2
    int combined_ls2 = (left << 5) | right;
    *k2 = permute(combined_ls2, P8, 10, 8);
}

/**
 * @brief Performs S-Box lookup.
 * @param input 4-bit input.
 * @param sbox The S-Box (S0 or S1) to use.
 * @return 2-bit output.
 */
int s_box_lookup(int input, const int sbox[4][4]) {
    // Row: bit 1 and 4
    int row = ((input & 0b1000) >> 2) | (input & 0b0001);
    // Col: bit 2 and 3
    int col = (input & 0b0110) >> 1;
    return sbox[row][col];
}

/**
 * @brief The S-DES Feistel function F(right_half, subkey).
 * @param half 4-bit half of the data.
 * @param key 8-bit subkey (K1 or K2).
 * @return 4-bit result.
 */
int feistel_f(int half, int key) {
    // 1. E/P Expansion
    int expanded = permute(half, EP, 4, 8);
    
    // 2. XOR with key
    int xored = expanded ^ key;
    
    // 3. Split into 4-bit halves
    int left_4 = (xored >> 4) & 0b1111;
    int right_4 = xored & 0b1111;
    
    // 4. S-Box substitutions
    int s0_out = s_box_lookup(left_4, S0);
    int s1_out = s_box_lookup(right_4, S1);
    
    // 5. Combine 2-bit outputs
    int s_combined = (s0_out << 2) | s1_out;
    
    // 6. P4
    return permute(s_combined, P4, 4, 4);
}

/**
 * @brief Encrypts or Decrypts an 8-bit block.
 * @param plaintext 8-bit data.
 * @param k1 8-bit subkey 1.
 * @param k2 8-bit subkey 2.
 * @param is_encrypt 1 for encrypt, 0 for decrypt.
 * @return 8-bit result (ciphertext or plaintext).
 */
int sdes_crypt(int data, int k1, int k2, int is_encrypt) {
    // 1. Initial Permutation (IP)
    int ip_data = permute(data, IP, 8, 8);
    
    // 2. Split into 4-bit halves
    int left = (ip_data >> 4) & 0b1111;
    int right = ip_data & 0b1111;

    // Use keys in correct order
    int first_key = is_encrypt ? k1 : k2;
    int second_key = is_encrypt ? k2 : k1;
    
    // 3. Round 1 (f_k with K1 for encrypt, K2 for decrypt)
    int f_result = feistel_f(right, first_key);
    int r1_left = left ^ f_result;
    int r1_right = right;
    
    // 4. Switch (SW) - r1_right becomes new left, r1_left becomes new right
    // No actual swap, just pass to next round in swapped order
    
    // 5. Round 2 (f_k with K2 for encrypt, K1 for decrypt)
    f_result = feistel_f(r1_left, second_key);
    int r2_left = r1_right ^ f_result;
    int r2_right = r1_left;
    
    // 6. Combine final halves
    int final_data = (r2_left << 4) | r2_right;
    
    // 7. Final Permutation (IP-1)
    return permute(final_data, IP_INV, 8, 8);
}


// --- Main Function ---
int main() {
    // Use integers to represent bit patterns
    // e.g., 0b1010000010 (binary) = 642 (decimal)
    // e.g., 0b10010111 (binary) = 151 (decimal)
    
    int key, plaintext;
    
    printf("Enter 10-bit key (0-1023): ");
    scanf("%d", &key);
    
    printf("Enter 8-bit plaintext (0-255): ");
    scanf("%d", &plaintext);
    
    if (key < 0 || key > 1023 || plaintext < 0 || plaintext > 255) {
        printf("Invalid input range.\n");
        return 1;
    }

    int k1, k2;
    generate_keys(key, &k1, &k2);
    
    printf("\nMaster Key: %d\n", key);
    printf("Generated K1: %d\n", k1);
    printf("Generated K2: %d\n", k2);
    
    // --- Encryption ---
    int ciphertext = sdes_crypt(plaintext, k1, k2, 1);
    printf("\nPlaintext:  %d\n", plaintext);
    printf("Ciphertext: %d\n", ciphertext);
    
    // --- Decryption ---
    int decrypted_text = sdes_crypt(ciphertext, k1, k2, 0);
    printf("Decrypted:  %d\n", decrypted_text);

    if (decrypted_text == plaintext) {
        printf("\nSuccess: Decryption matches original plaintext!\n");
    } else {
        printf("\nError: Decryption failed!\n");
    }
    
    return 0;
}