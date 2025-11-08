#include <stdio.h>
#include <string.h>

// Encrypts text using Rail Fence Cipher
void encryptRailFence(char *text, int key, char *cipherText) {
    int len = strlen(text);
    // Create a 2D array (matrix) to simulate the rails
    char rail[key][len];
    
    // 1. Initialize the rail matrix with newline characters
    for (int i = 0; i < key; i++) {
        for (int j = 0; j < len; j++) {
            rail[i][j] = '\n';
        }
    }

    // 2. Fill the matrix in a zig-zag pattern
    int row = 0;
    int dir_down = 0; // 0 for down, 1 for up

    for (int i = 0; i < len; i++) {
        // Fill the character
        rail[row][i] = text[i];

        // Check for direction change
        if (row == 0) {
            dir_down = 0; // Start moving down
        } else if (row == key - 1) {
            dir_down = 1; // Start moving up
        }

        // Move to the next row
        if (dir_down == 0) {
            row++;
        } else {
            row--;
        }
    }

    // 3. Read the matrix row by row to create the ciphertext
    int k = 0;
    for (int i = 0; i < key; i++) {
        for (int j = 0; j < len; j++) {
            if (rail[i][j] != '\n') {
                cipherText[k++] = rail[i][j];
            }
        }
    }
    cipherText[k] = '\0';
}

int main() {
    char text[200], cipherText[200];
    int key; // The number of rails

    printf("Enter plaintext: ");
    // Use fgets to read string with spaces, then remove newline
    fgets(text, sizeof(text), stdin);
    text[strcspn(text, "\n")] = 0; // Remove trailing newline

    printf("Enter key (number of rails): ");
    scanf("%d", &key);

    encryptRailFence(text, key, cipherText);

    printf("Encrypted Text: %s\n", cipherText);

    return 0;
}