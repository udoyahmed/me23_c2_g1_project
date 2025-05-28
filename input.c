#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define clrscr() printf("\e[1;1H\e[2J")

#define ORIGIN_Y 0
#define SCALE_GRAPH 1

int dataCount = 0;

struct data {
    float flowRate[110];
    float head[110];
    float power[110];
    float efficiency[110];
};

void plot(float* arr_x, float* arr_y, float* specialLines);
void BEP(float* flowRate, float* efficiency, float* result);

int main() {
    FILE* f1;
    struct data pump;

    while (1) {
        int input1 = 0;
        do {
            printf("\n1. Enter a file,\n2. Exit.\n\nChoice: ");
            scanf("%d", &input1);
        } while (input1 < 1 || input1 > 2);

        if (input1 == 1) {
            dataCount = 0;                                      // reset the value
            char fileName[100];
            printf("\nEnter file name (with extension): ");

            while (getchar() != '\n');                          // clear the buffer
            fgets(fileName, sizeof(fileName), stdin);

            fileName[strcspn(fileName, "\n")] = '\0';

            if ((f1 = fopen(fileName, "r")) == NULL) {
                printf("\nError opening the file.");
                exit(1);
            }

            for (int i = 0; i < 110; i++) {                     // can check for upto 110 lines
                int fscanfReturn = fscanf(f1, "%f %f %f %f", &pump.flowRate[i], &pump.head[i], &pump.power[i], &pump.efficiency[i]);
                if (fscanfReturn == EOF) break;
                if (fscanfReturn != 4) {
                    printf("\nSomething went wrong at line %d.", i + 1);
                    exit(1);
                }
                dataCount++;
            }

            int input2 = 0;
            while (1) {
                do {
                    printf("\nPlot the following curves:\n1. Head vs Flow Rate\n2. Efficiency vs Flow Rate\n3. Power vs Flow Rate\n4. Exit\n\nChoice: ");
                    scanf("%d", &input2);
                } while (input2 < 1 && input2 > 4);

                if (input2 == 1) {
                    float specialLine[] = { -100, -100 };
                    plot(pump.flowRate, pump.head, specialLine);
                    for (int i = 0; i < dataCount / 2; i++) {
                        printf("  ");
                    }
                    printf("Head vs Flow Rate\n");
                }
                else if (input2 == 2) {
                    float bep[2];
                    BEP(pump.flowRate, pump.efficiency, bep);
                    plot(pump.flowRate, pump.efficiency, bep);
                    for (int i = 0; i < (dataCount / 2) - 10; i++) {
                        printf("  ");
                    }
                    printf("Efficiency vs Flow Rate\n");

                    for (int i = 0; i < (dataCount / 2) - 10; i++) {
                        printf("  ");
                    }
                    printf("Best efficiency point (BEP): %.2f\n", bep[1]);
                    for (int i = 0; i < (dataCount / 2) - 10; i++) {
                        printf("  ");
                    }
                    printf("The BEP is marked using the letter \"B\"");
                }
                else if (input2 == 3) {
                    float specialLine[] = { -100, -100 };
                    plot(pump.flowRate, pump.power, specialLine);
                    for (int i = 0; i < dataCount / 2; i++) {
                        printf("  ");
                    }
                    printf("Power vs Flow Rate\n");
                }
                else {
                    printf("\nYour can exit or input a new file: \n");
                    break;
                }
            }
        }

        else if (input1 == 2) {
            fclose(f1);
            return 0;
        }
    }

    fclose(f1);
    printf("\n");
    return 0;
}

void plot(float* arr_x, float* arr_y, float* specialLines) {
    clrscr();

    int highestYvalue = 0, lowestYvalue = 100;

    for (int i = 0; i < dataCount; i++) {
        if (arr_y[i] > highestYvalue) highestYvalue = arr_y[i];
        if (arr_y[i] < lowestYvalue) lowestYvalue = arr_y[i];
    }

    lowestYvalue /= 2;

    int ROW = highestYvalue - lowestYvalue + 5, COLUMN = dataCount + 5;
    int ORIGIN_X = ROW - 1;

    clrscr();

    for (int row = 0; row < ROW; row++) {                               // row is y value
        for (int column = 0; column < COLUMN; column++) {               // column is x value

            if (column == ORIGIN_Y && row == ORIGIN_X) {      // show origin if empty
                printf("o");
                continue;
            }

            if (column == ORIGIN_Y) {
                printf("|");
                continue;
            }

            int current_x = column - ORIGIN_Y;                          // convert the row and column value
            int current_y = -row + ORIGIN_X;                            // to actual points

            if (current_x == (int)(specialLines[1]) && current_y == (int)specialLines[0] - lowestYvalue) {
                printf("B ");
                continue;
            }

            int flag = 0;

            for (int i = 0; i < 100; i++) {
                if (current_x == ((int)arr_x[i]) && current_y == (int)arr_y[i] - lowestYvalue) {
                    printf("O ");
                    flag = 1;
                }
            }

            if (flag) continue;


            if (row == ORIGIN_X) {
                printf("--");
            }
            else printf("  ");
        }
        printf("\n");
    }

    printf("\n");
}

void BEP(float* flowRate, float* efficiency, float* result) {
    *(result) = 0;              // holds highest efficiency
    *(result + 1) = 0;          // holds flowrate
    for (int i = 0; i < dataCount; i++) {
        if (*(result) < efficiency[i]) {
            *(result) = efficiency[i];
            *(result + 1) = flowRate[i];
        }
    }
}