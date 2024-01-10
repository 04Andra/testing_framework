#include <stdio.h>

int maximumNumber(int vector[50], int n) {
    int max = 0, i;
    for (i = 0; i < n; i++) {
        if (vector[i] > max) {
            max = vector[i];
        }
    }
    return max;
}

int main () {
    int vector[50] = {1, 4, 5, 23, 2, 543, 6};
    int n = sizeof (vector) / sizeof (vector[0]);

    printf("%d", maximumNumber(vector, n));

    return 0;
}

