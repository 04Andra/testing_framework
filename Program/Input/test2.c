#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char* concatMethod(char a[50], char b[50]) {
    char result[101];
    strcpy(result, a);
    strcat(result, " ");
    strcat(result, b);

    char* concatenated = strdup(result);
    return concatenated;
}

int main() {
    char str1[50] = "Hello";
    char str2[50] = "World";

    char* result = concatMethod(str1, str2);
    printf("%s\n", result);

    free(result);

    return 0;
}

