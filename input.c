#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// only for linux 

#include <termios.h>
#include <unistd.h>
int getch(void);

// comment these lines above for windows / codeblocks

#define ORIGIN_Y 0
#define SCALE_GRAPH 1

int dataCount = 0;

struct data {
    float flowRate[110];
    float head[110];
    float power[110];
    float efficiency[110];
};

void clearConsole(void);
void plot(float* arr_x, float* arr_y, float* specialLines, char plotID);
void minima_maxima(float* flowRate, float* power_head_eff, float* result);
void showTable(float* flowRate, float* head, float* power, float* efficiency);
void summaryTable(float* flowRate, float* head, float* power, float* efficiency);
void generateReport(float* flowRate, float* head, float* power, float* efficiency);
void cavitationOnset(float* flowRate, float* head, float* result);
void overloadCondition(float* flowRate, float* power, float* result);
void pressAnyKeytoContinue(void);

int main() {
    FILE* f1;
    struct data pump;

    while (1) {
        int input1 = 0;
        do {
            clearConsole();
            printf("\n1. Enter a file,\n2. Exit.\n\nChoice: ");
            scanf("%d", &input1);
        } while (input1 < 1 || input1 > 2);

        if (input1 == 1) {
            dataCount = 0;                                      // reset the value


            char fileName[100];
            // printf("\nEnter file name (with extension): ");

            // while (getchar() != '\n');                          // clear the buffer
            // fgets(fileName, sizeof(fileName), stdin);

            // fileName[strcspn(fileName, "\n")] = '\0';

            strcpy(fileName, "performance_data.txt");

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

            int input2 = 0;
            while (1) {
                do {
                    clearConsole();
                    printf("\nPlots:\n1. Head vs Flow Rate\n2. Efficiency vs Flow Rate\n3. Power vs Flow Rate\n4. Operating Range\n");
                    printf("\nTables:\n5. Show Table of the points\n6. Show Summery Table\n\n7. Generate Report\n8. Exit\n\nChoice: ");
                    scanf("%d", &input2);
                } while (input2 < 1 && input2 > 8);

                if (input2 == 1) {                                  // head vs flow rate
                    float cavitation[3];
                    cavitationOnset(pump.flowRate, pump.head, cavitation);
                    plot(pump.flowRate, pump.head, cavitation, 'H');
                    for (int i = 0; i < dataCount / 2 - 10; i++) {
                        printf("  ");
                    }
                    printf("Head vs Flow Rate\n");

                    if (cavitation[0] != -1) {
                        for (int i = 0; i < dataCount / 2 - 10; i++) {
                            printf("  ");
                        }
                        printf("Cavitation Onset at %.2f L/s\n", cavitation[1]);
                        for (int i = 0; i < (dataCount / 2) - 10; i++) {
                            printf("  ");
                        }
                        printf("The Cavitation Onset is marked using the letter \"H\"\n");
                    }
                    else {
                        for (int i = 0; i < (dataCount / 2) - 10; i++) {
                            printf("  ");
                        }
                        printf("No Cavitation Onset!\n");
                    }
                    pressAnyKeytoContinue();
                }

                else if (input2 == 2) {                             // efficiency vs flow rate
                    float bep[3];
                    minima_maxima(pump.flowRate, pump.efficiency, bep);
                    plot(pump.flowRate, pump.efficiency, bep, 'E');
                    for (int i = 0; i < (dataCount / 2) - 10; i++) {
                        printf("  ");
                    }
                    printf("Efficiency vs Flow Rate\n");

                    for (int i = 0; i < (dataCount / 2) - 10; i++) {
                        printf("  ");
                    }
                    printf("Best efficiency point (BEP): %.2f L/s\n", bep[1]);
                    for (int i = 0; i < (dataCount / 2) - 10; i++) {
                        printf("  ");
                    }
                    printf("The BEP is marked using the letter \"E\"\n");
                    pressAnyKeytoContinue();
                }

                else if (input2 == 3) {                             // power vs flow rate
                    float overload[3];
                    overloadCondition(pump.flowRate, pump.power, overload);
                    plot(pump.flowRate, pump.power, overload, 'P');
                    for (int i = 0; i < dataCount / 2 - 10; i++) {
                        printf("  ");
                    }
                    printf("Power vs Flow Rate\n");
                    if (overload[0] != -1) {
                        for (int i = 0; i < dataCount / 2 - 10; i++) {
                            printf("  ");
                        }
                        printf("Overload Point at %.2f L/s\n", overload[1]);
                        for (int i = 0; i < (dataCount / 2) - 10; i++) {
                            printf("  ");
                        }
                        printf("The Overload Point is marked using the letter \"P\"\n");
                    }
                    else {
                        for (int i = 0; i < (dataCount / 2) - 10; i++) {
                            printf("  ");
                        }
                        printf("No Overload Condition!\n");
                    }
                    pressAnyKeytoContinue();
                }
                else if (input2 == 4) {         // operating point 
                    float cavitation[3];
                    cavitationOnset(pump.flowRate, pump.head, cavitation);
                    plot(pump.flowRate, pump.head, cavitation, 'O');
                    for (int i = 0; i < dataCount / 2 - 10; i++) {
                        printf("  ");
                    }
                    printf("Head vs Flow Rate\n");
                    for (int i = 0; i < dataCount / 2 - 10; i++) {
                        printf("  ");
                    }
                    printf("Operating range is shown by the two straight lines.\n");
                    pressAnyKeytoContinue();
                }

                else if (input2 == 5) {         // show table
                    showTable(pump.flowRate, pump.head, pump.power, pump.efficiency);
                    pressAnyKeytoContinue();
                }

                else if (input2 == 6) {         // show summary table
                    summaryTable(pump.flowRate, pump.head, pump.power, pump.efficiency);
                    pressAnyKeytoContinue();
                }

                else if (input2 == 7) {         // generate report
                    generateReport(pump.flowRate, pump.head, pump.power, pump.efficiency);
                    printf("\nReport generated successfully at report.txt.\n\n");
                    pressAnyKeytoContinue();
                }

                else {
                    printf("\nYour can exit or input a new file: \n");
                    pressAnyKeytoContinue();
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

void plot(float* arr_x, float* arr_y, float* specialLines, char plotID) {
    clearConsole();

    int highestYvalue = 0, lowestYvalue = 100;

    for (int i = 0; i < dataCount; i++) {
        if (arr_y[i] > highestYvalue) highestYvalue = arr_y[i];
        if (arr_y[i] < lowestYvalue) lowestYvalue = arr_y[i];
    }

    lowestYvalue /= 2;

    int ROW = highestYvalue - lowestYvalue + 5, COLUMN = dataCount + 5;
    int ORIGIN_X = ROW - 1;

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

            if (plotID == 'E' || plotID == 'H' || plotID == 'P') {
                if (current_x == (int)(specialLines[1]) && current_y == (int)specialLines[0] - lowestYvalue) {
                    printf("%c ", plotID);
                    continue;
                }
            }

            if (plotID == 'O') {
                if (current_x == 2 || current_x == (int)specialLines[1] - 1) {
                    printf("+");
                    continue;
                }
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

void minima_maxima(float* flowRate, float* power_head_eff, float* result) {
    result[0] = 0;              // holds highest efficiency
    result[1] = 0;              // holds flowrate
    result[2] = 1000;           // holds lowest efficiency
    for (int i = 0; i < dataCount; i++) {
        if (result[0] < power_head_eff[i]) {
            result[0] = power_head_eff[i];
            result[1] = flowRate[i];
        }
        if (power_head_eff[i] < result[2]) {
            result[2] = power_head_eff[i];
        }
    }
}

void cavitationOnset(float* flowRate, float* head, float* result) {
    float averageHeadDrop = 0;
    int pointsCounted = 0;

    for (int i = 0; i < dataCount / 2 - 1; i++) {
        averageHeadDrop += (head[i] - head[i + 1]);
        pointsCounted++;
    }
    averageHeadDrop /= pointsCounted;

    for (int i = dataCount / 2; i < dataCount - 1; i++) {
        if ((head[i] - head[i + 1]) > (10 * averageHeadDrop)) {        // 10 is arbitrary
            result[0] = head[i + 1];
            result[1] = flowRate[i + 1];
            return;
        }
    }
    result[0] = -1;
    result[1] = flowRate[dataCount - 1];
    result[2] = -1;
}

void overloadCondition(float* flowRate, float* power, float* result) {
    float averagePowerIncrease = 0;
    int pointsCounted = 0;

    for (int i = 0; i < dataCount / 2 - 1; i++) {
        averagePowerIncrease += (power[i + 1] - power[i]);
        pointsCounted++;
    }
    averagePowerIncrease /= pointsCounted;

    for (int i = dataCount / 2; i < dataCount - 1; i++) {
        if ((power[i + 1] - power[i]) > (10 * averagePowerIncrease)) {        // 10 is arbitrary
            result[0] = power[i + 1];
            result[1] = flowRate[i + 1];
            return;
        }
    }
    result[0] = -1;
    result[1] = flowRate[dataCount - 1];
    result[2] = -1;
}

void showTable(float* flowRate, float* head, float* power, float* efficiency) {
    printf("+-----------+--------+---------+------------+\n");
    printf("| Flow Rate |  Head  |  Power  | Efficiency |\n");
    printf("+-----------+--------+---------+------------+\n");

    for (int i = 0; i < dataCount; i++) {
        printf("| %9.2f | %6.2f | %7.2f | %10.2f |\n", flowRate[i], head[i], power[i], efficiency[i]);
    }
    printf("+-----------+--------+---------+------------+\n");
}

void summaryTable(float* flowRate, float* head, float* power, float* efficiency) {
    float temp_result[3];
    minima_maxima(flowRate, head, temp_result);

    printf("+---------------------------+------------+\n");
    printf("| Maximum Head              | %6.2f m   |\n", temp_result[0]);
    printf("| Minimum Head              | %6.2f m   |\n", temp_result[2]);

    minima_maxima(flowRate, power, temp_result);

    printf("| Maximum Power             | %6.2f W   |\n", temp_result[0]);
    printf("| Minimum Power             | %6.2f W   |\n", temp_result[2]);

    minima_maxima(flowRate, efficiency, temp_result);

    printf("| Maximum Efficiency        | %6.2f %%   |\n", temp_result[0]);
    printf("| Minimum Efficieny         | %6.2f %%   |\n", temp_result[2]);
    printf("| BEP Flow Rate             | %6.2f L/s |\n", temp_result[1]);
    printf("| Shut-off Condition        | %6.2f m   |\n", flowRate[0]);

    overloadCondition(flowRate, power, temp_result);

    printf("| Overload Start Flowrate   | %6.2f L/s |\n", temp_result[1]);

    cavitationOnset(flowRate, head, temp_result);

    printf("| Cavitation Onset Flowrate | %6.2f L/s |\n", temp_result[1]);
    printf("+---------------------------+------------+\n");
}

void generateReport(float* flowRate, float* head, float* power, float* efficiency) {
    FILE* f2;

    if ((f2 = fopen("report.txt", "w")) == NULL) {
        printf("Error creating report.txt.");
        exit(1);
    }

    fprintf(f2, "+-----------+--------+---------+------------+\n");
    fprintf(f2, "| Flow Rate |  Head  |  Power  | Efficiency |\n");
    fprintf(f2, "+-----------+--------+---------+------------+\n");

    for (int i = 0; i < dataCount; i++) {
        fprintf(f2, "| %9.2f | %6.2f | %7.2f | %10.2f |\n", flowRate[i], head[i], power[i], efficiency[i]);
    }

    fprintf(f2, "+-----------+--------+---------+------------+\n");
    fprintf(f2, "\nSummary Table:\n\n");

    float temp_result[3];
    minima_maxima(flowRate, head, temp_result);

    fprintf(f2, "+---------------------------+------------+\n");
    fprintf(f2, "| Maximum Head              | %6.2f m   |\n", temp_result[0]);
    fprintf(f2, "| Minimum Head              | %6.2f m   |\n", temp_result[2]);

    minima_maxima(flowRate, power, temp_result);

    fprintf(f2, "| Maximum Power             | %6.2f W   |\n", temp_result[0]);
    fprintf(f2, "| Minimum Power             | %6.2f W   |\n", temp_result[2]);

    minima_maxima(flowRate, efficiency, temp_result);

    fprintf(f2, "| Maximum Efficiency        | %6.2f %%   |\n", temp_result[0]);
    fprintf(f2, "| Minimum Efficieny         | %6.2f %%   |\n", temp_result[2]);
    fprintf(f2, "| BEP Flow Rate             | %6.2f L/s |\n", temp_result[1]);
    fprintf(f2, "| Shut-off Condition        | %6.2f m   |\n", flowRate[0]);

    overloadCondition(flowRate, power, temp_result);

    fprintf(f2, "| Overload Start Flowrate   | %6.2f L/s |\n", temp_result[1]);

    cavitationOnset(flowRate, head, temp_result);

    fprintf(f2, "| Cavitation Onset Flowrate | %6.2f L/s |\n", temp_result[1]);
    fprintf(f2, "+---------------------------+------------+\n");

    fclose(f2);
}

void pressAnyKeytoContinue(void) {
    printf("Press any key to continue...");
    while (getchar() != '\n');
    getch();
}

void clearConsole(void) {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// comment this function out for windows / codeblocks

int getch(void) {
    struct termios oldt, newt;
    int ch;

    // get current terminal settings
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;

    // disable canonical mode and echo
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    // read character
    ch = getchar();

    // restore terminal settings
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

    return ch;
}