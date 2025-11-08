#include <stdio.h>
#include <string.h>
#include <ctype.h>

// 5x5 Key Matrix
char keyMatrix[5][5];
// Used to mark letters that are already in the matrix
int charUsed[26] = {0};

// Stores the row and column of a character in the matrix
void getCharPosition(char ch, int *row, int *col) {
    if (ch == 'J') ch = 'I'; // Treat J as I
    
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            if (keyMatrix[i][j] == ch) {
                *row = i;
                *col = j;
                return;
            }
        }
    }
}

// Generates the 5x5 key matrix
void generateKeyMatrix(char *key) {
    int k = 0, l = 0;
    
    // 1. Fill with the key
    for (int i = 0; i < strlen(key); i++) {
        char ch = toupper(key[i]);
        if (ch == 'J') ch = 'I'; // Treat J as I
        
        if (charUsed[ch - 'A'] == 0) {
            keyMatrix[k][l] = ch;
            charUsed[ch - 'A'] = 1;
            l++;
            if (l == 5) {
                k++;
                l = 0;
            }
        }
    }
    
    // 2. Fill with remaining alphabet
    for (int i = 0; i < 26; i++) {
        char ch = 'A' + i;
        if (ch == 'J') continue; // Skip J
        
        if (charUsed[ch - 'A'] == 0) {
            keyMatrix[k][l] = ch;
            l++;
            if (l == 5) {
                k++;
                l = 0;
            }
        }
    }
}

// Prepares the plaintext string
int prepareText(char *text, char *prepared) {
    int j = 0;
    for (int i = 0; i < strlen(text); i++) {
        char ch = toupper(text[i]);
        if (ch < 'A' || ch > 'Z') continue; // Skip non-alphabetic
        if (ch == 'J') ch = 'I'; // Treat J as I
        
        // Check for duplicate in a pair
        if (j > 0 && prepared[j-1] == ch) {
            prepared[j++] = 'X'; // Add padding 'X'
        }
        prepared[j++] = ch;
    }
    
    // Add padding 'X' if length is odd
    if (j % 2 != 0) {
        prepared[j++] = 'X';
    }
    prepared[j] = '\0';
    return j; // Return new length
}

// Encrypts the prepared text
void encryptPlayfair(char *text, char *cipherText) {
    int r1, c1, r2, c2;
    int k = 0;
    
    for (int i = 0; i < strlen(text); i += 2) {
        getCharPosition(text[i], &r1, &c1);
        getCharPosition(text[i+1], &r2, &c2);
        
        if (r1 == r2) { // Same Row
            cipherText[k++] = keyMatrix[r1][(c1 + 1) % 5];
            cipherText[k++] = keyMatrix[r2][(c2 + 1) % 5];
        } else if (c1 == c2) { // Same Column
            cipherText[k++] = keyMatrix[(r1 + 1) % 5][c1];
            cipherText[k++] = keyMatrix[(r2 + 1) % 5][c2];
        } else { // Rectangle
            cipherText[k++] = keyMatrix[r1][c2];
            cipherText[k++] = keyMatrix[r2][c1];
        }
    }
    cipherText[k] = '\0';
}

int main() {
    char key[100], text[200], prepared[200], cipherText[200];
    
    printf("Enter key (e.g., MONARCHY): ");
    scanf("%s", key);
    
    printf("Enter plaintext (e.g., INSTRUMENTS): ");
    scanf("%s", text);
    
    generateKeyMatrix(key);
    
    printf("\nKey Matrix:\n");
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            printf("%c ", keyMatrix[i][j]);
        }
        printf("\n");
    }
    
    int len = prepareText(text, prepared);
    printf("\nPrepared Text: %s\n", prepared);
    
    encryptPlayfair(prepared, cipherText);
    printf("Encrypted Text: %s\n", cipherText);
    
    return 0;
}