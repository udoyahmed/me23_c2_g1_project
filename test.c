#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#include <stdlib.h>

int main() {
    printf("+--------------------+-------+\n");
    printf("| Maximum Head       | 23.32 |\n");
    printf("| Minimum Head       | 23.32 |\n");
    printf("| Maximum Power      | 23.32 |\n");
    printf("| Minimum Power      | 23.32 |\n");
    printf("| Maximum Efficiency | 23.32 |\n");
    printf("| Minimum Efficieny  | 23.32 |\n");
    printf("| BEP Flow Rate      | 23.32 |\n");
    printf("| Shut-off Condition | 23.32 |\n");
    printf("| Overload Start     | 23.32 |\n");
    printf("| Cavitation Onset   | 23.32 |\n");
    printf("+--------------------+-------+\n");
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