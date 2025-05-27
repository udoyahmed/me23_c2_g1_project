#include <stdio.h>
#include <stdlib.h>

struct data {
    float flowRate[110];
    float head[110];
    float power[110];
    float efficiency[110];
};

int main() {
    struct data pump1;

    FILE* f1;

    if ((f1 = fopen("performance_data_100.txt", "r")) == NULL) {
        printf("Error opening the file.");
        exit(1);
    }

    for (int i = 0; i < 100; i++) {
        if (fscanf(f1, "%f %f %f %f", &pump1.flowRate[i], &pump1.head[i], &pump1.power[i], &pump1.efficiency[i]) != 4) {
            printf("Something went wrong at line %d.", i + 1);
        }
    }

    fclose(f1);
    printf("\n");
    return 0;
}