#include <stdio.h>

// 1. Define LCG parameters
// These values are from the ANSI C standard
#define M 2147483648  // Modulus (2^31)
#define A 1103515245  // Multiplier
#define C 12345       // Increment

// Global variable to hold the current random number
static unsigned long long next_num = 1;

/**
 * @brief Sets the seed for the random number generator
 * @param seed The starting value
 */
void my_srand(unsigned int seed) {
    next_num = seed;
}

/**
 * @brief Generates the next pseudorandom number
 * @return An integer between 0 and M-1
 */
unsigned int my_rand(void) {
    // Apply the LCG formula
    next_num = (A * next_num + C) % M;
    return next_num;
}

// 2. Main function to test the generator
int main() {
    unsigned int seed;
    int count;

    printf("Enter a seed value (a starting integer): ");
    scanf("%u", &seed);
    my_srand(seed);

    printf("How many random numbers to generate? ");
    scanf("%d", &count);

    printf("\nGenerating %d numbers:\n", count);
    for (int i = 0; i < count; i++) {
        // We can scale the output to a smaller range, e.g., 0-99
        printf("%d\n", my_rand() % 100); 
    }

    return 0;
}