#include<stdio.h>
#include<stdlib.h>

int main() {
    int col1[10], col2[10], col3[10], lines = 0;;

    FILE* f1;

    if ((f1 = fopen("test1.txt", "r")) == NULL) {
        printf("Error opening the file.");
        exit(1);
    }

    for (int i = 0; i < 10; i++) {
        if ((fscanf(f1, "%d %d %d", col1 + i, col2 + i, col3 + i) == EOF)) {
            break;
        }
        lines++;
    }

    for (int i = 0; i < lines; i++) {
        printf("%d %d %d\n", col1[i], col2[i], col3[i]);
    }

    printf("\n");
    fclose(f1);
    return 0;
}