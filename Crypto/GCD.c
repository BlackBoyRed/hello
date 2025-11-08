#include <stdio.h>

/**
 * @brief Finds the Greatest Common Divisor (GCD) of two integers
 * using the iterative Euclidean algorithm.
 * @param a The first integer.
 * @param b The second integer.
 * @return The GCD of a and b.
 */
int gcd(int a, int b) {
    int temp;

    // Make inputs positive
    if (a < 0) a = -a;
    if (b < 0) b = -b;

    while (b != 0) {
        temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

int main() {
    int num1, num2;

    printf("Enter two integers: ");
    scanf("%d %d", &num1, &num2);

    printf("The GCD of %d and %d is %d\n", num1, num2, gcd(num1, num2));

    return 0;
}