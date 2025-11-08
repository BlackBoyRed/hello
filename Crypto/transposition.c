#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

int main() {
    char key[100], text[200], cipherText[200];
    int keyLen, textLen, rows, k, c;

    printf("Enter key (e.g., ZEBRA): ");
    scanf("%s", key);
    keyLen = strlen(key);

    printf("Enter plaintext (no spaces): ");
    scanf("%s", text);
    textLen = strlen(text);

    // 1. Calculate grid dimensions
    // Ceiling division to get number of rows
    rows = (textLen + keyLen - 1) / keyLen;
    char matrix[rows][keyLen];

    // 2. Fill the matrix with plaintext
    k = 0; // Index for plaintext
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < keyLen; j++) {
            if (k < textLen) {
                matrix[i][j] = text[k++];
            } else {
                // Pad with a filler character
                matrix[i][j] = 'X'; 
            }
        }
    }

    // 3. Create the column order map
    // We need to know which column to read first, second, etc.
    // e.g., ZEBRA -> 5 2 1 4 3
    int colOrder[keyLen];
    int visited[keyLen];
    for(int i=0; i < keyLen; i++) visited[i] = 0;

    c = 1; // Start with order '1'
    for (int i = 0; i < 26; i++) { // Loop through alphabet A-Z
        for (int j = 0; j < keyLen; j++) {
            if (visited[j] == 0 && toupper(key[j]) == ('A' + i)) {
                colOrder[j] = c++;
                visited[j] = 1;
            }
        }
    }

    // 4. Read the ciphertext by column based on the order
    k = 0; // Index for ciphertext
    for (int order = 1; order <= keyLen; order++) {
        for (int j = 0; j < keyLen; j++) {
            // Find the column 'j' that corresponds to the current 'order'
            if (colOrder[j] == order) {
                // Read all rows in that column
                for (int i = 0; i < rows; i++) {
                    cipherText[k++] = matrix[i][j];
                }
            }
        }
    }
    cipherText[k] = '\0';

    // Print the intermediate matrix (optional, for verification)
    printf("\nIntermediate Matrix:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < keyLen; j++) {
            printf("%c ", matrix[i][j]);
        }
        printf("\n");
    }
    
    printf("\nKey Order: ");
    for(int i=0; i<keyLen; i++) printf("%d ", colOrder[i]);
    
    printf("\n\nEncrypted Text: %s\n", cipherText);

    return 0;
}