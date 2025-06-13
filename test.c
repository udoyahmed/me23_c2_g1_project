#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>

int dataCount = 0;                                      // reset the value

void showTable(float* flowRate, float* head, float* power, float* efficiency) {
    printf("+-----------+--------+---------+------------+\n");
    printf("| Flow Rate |  Head  |  Power  | Efficiency |\n");
    printf("+-----------+--------+---------+------------+\n");

    for (int i = 0; i < dataCount; i++) {
        printf("| %9.2f | %6.2f | %7.2f | %10.2f |\n", flowRate[i], head[i], power[i], efficiency[i]);
    }
    printf("+-----------+--------+---------+------------+\n");
}

void sort(float* flowRate, float* head, float* power, float* efficiency) {
    for (int i = 1; i < dataCount; i++) {
        float cFR = flowRate[i], cH = head[i], cP = power[i], cE = efficiency[i];
        int j = i;
        for (; cFR < flowRate[j - 1] && j > 0; j--) {
            flowRate[j] = flowRate[j - 1];

            head[j] = head[j - 1];
            power[j] = power[j - 1];
            efficiency[j] = efficiency[j - 1];
        }
        flowRate[j] = cFR;
        head[j] = cH;
        power[j] = cP;
        efficiency[j] = cE;
    }
}

// void sort(float* flowRate, float* head, float* power, float* efficiency) {
//     for (int i = 0; i < dataCount - 1; i++) {
//         for (int j = 0; j < dataCount - i - 1; j++) {
//             if (flowRate[j] > flowRate[j + 1]) {
//                 float temp = flowRate[j];
//                 flowRate[j] = flowRate[j + 1];
//                 flowRate[j + 1] = temp;

//                 temp = head[j];
//                 head[j] = head[j + 1];
//                 head[j + 1] = temp;

//                 temp = power[j];
//                 power[j] = power[j + 1];
//                 power[j + 1] = temp;

//                 temp = efficiency[j];
//                 efficiency[j] = efficiency[j + 1];
//                 efficiency[j + 1] = temp;
//             }
//         }
//     }
// }

int main() {
    FILE* f1;

    struct data {
        float flowRate[110];
        float head[110];
        float power[110];
        float efficiency[110];
    } pump;

    char fileName[100];

    strcpy(fileName, "istiaq.txt");

    if ((f1 = fopen(fileName, "r")) == NULL) {
        printf("\nError opening the file.");
        exit(1);
    }

    for (int i = 0; i < 110; i++) {                     // can check for upto 110 lines
        int fscanfReturn = fscanf(f1, "%f %f %f %f", &pump.flowRate[i], &pump.head[i], &pump.power[i], &pump.efficiency[i]);
        if (fscanfReturn == EOF) break;
        if (fscanfReturn != 4) {
            printf("\nSomething went wrong while reading line %d.", i + 1);
            exit(1);
        }
        dataCount++;
    }

    sort(pump.flowRate, pump.head, pump.power, pump.efficiency);
    showTable(pump.flowRate, pump.head, pump.power, pump.efficiency);


    return 0;
}

// int main() {
//     float a[10], b[10], c[10], d[10], totalLines = 0;
//     FILE* f1, * f2;

//     if ((f1 = fopen("test1.txt", "r")) == NULL) {
//         printf("Error reading the file.");
//         exit(1);
//     }
//     if ((f2 = fopen("report_test.txt", "w")) == NULL) {
//         printf("Error creating report file.");
//         exit(1);
//     }

//     for (int i = 0; i < 10; i++) {
//         int returnValue = fscanf(f1, "%f %f %f %f", a + i, b + i, c + i, d + i);
//         if (returnValue == EOF) break;
//         totalLines++;
//     }

//     printf("+-----------+--------+---------+------------+\n");
//     printf("| Flow Rate |  Head  |  Power  | Efficiency |\n");
//     printf("+-----------+--------+---------+------------+\n");

//     fprintf(f2, "+-----------+--------+---------+------------+\n");
//     fprintf(f2, "| Flow Rate |  Head  |  Power  | Efficiency |\n");
//     fprintf(f2, "+-----------+--------+---------+------------+\n");

//     for (int i = 0; i < totalLines; i++) {
//         printf("| %9.2f | %6.2f | %7.2f | %10.2f |\n", a[i], b[i], c[i], d[i]);
//         fprintf(f2, "| %9.2f | %6.2f | %7.2f | %10.2f |\n", a[i], b[i], c[i], d[i]);
//     }
//     printf("+-----------+--------+---------+------------+\n");
//     fprintf(f2, "+-----------+--------+---------+------------+\n");

//     fclose(f1);
//     fclose(f2);
//     printf("\n");
//     return 0;
// }