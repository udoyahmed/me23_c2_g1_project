#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// only for linux 

#include <termios.h>
#include <unistd.h>
int getch(void);

// comment these lines above for windows / codeblocks

#define EXPECTED_STANDARD_DEVIATION 10
#define SHOULD_SCALE_WHEN_STD_X 5
#define SHOULD_SCALE_WHEN_STD_Y 1

void addSpaces(void);
void clearConsole(void);
void pressAnyKeytoContinue(void);
void maxima_minima_plot(float* array, float* result);
void cavitationOnset(float* flowRate, float* head, float* result);
void overloadCondition(float* flowRate, float* power, float* result);
void plot(float* arr_x, float* arr_y, float* specialLines, char plotID);
void sort(float* flowRate, float* head, float* power, float* efficiency);
void minima_maxima(float* flowRate, float* power_head_eff, float* result);
void showTable(float* flowRate, float* head, float* power, float* efficiency);
void summaryTable(float* flowRate, float* head, float* power, float* efficiency);
void generateReport(float* flowRate, float* head, float* power, float* efficiency);
float average(float* points);
float standard_deviation(float* array);

int dataCount = 0;

struct data {
    float flowRate[110];
    float head[110];
    float power[110];
    float efficiency[110];
};

int main() {
    FILE* f1;
    struct data pump;

    while (1) {
        int input1 = 0;
        do {
            clearConsole();
            printf("\n1. Enter a file\n2. Exit.\n\nChoice: ");
            scanf("%d", &input1);
        } while (input1 < 1 || input1 > 2);

        if (input1 == 1) {
            dataCount = 0;                                      // reset the value

            char fileName[100];
            printf("\nEnter file name (with extension): ");

            int c;
            while ((c = getchar()) != '\n' && c != EOF);        // clear the buffer
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
                    printf("\nSomething went wrong while reading line %d.", i + 1);
                    exit(1);
                }
                dataCount++;
            }

            sort(pump.flowRate, pump.head, pump.power, pump.efficiency);

            int input2 = 0;
            while (1) {
                do {
                    clearConsole();
                    printf("\nPlots:\n1. Head vs Flow Rate\n2. Efficiency vs Flow Rate\n3. Power vs Flow Rate\n4. Operating Range\n");
                    printf("\nTables:\n5. Show Table of the points\n6. Show Summary Table\n\n7. Generate Report\n8. Exit\n\nChoice: ");
                    scanf("%d", &input2);
                } while (input2 < 1 || input2 > 8);

                if (input2 == 1) {                                  // head vs flow rate
                    float cavitation[3];
                    cavitationOnset(pump.flowRate, pump.head, cavitation);
                    plot(pump.flowRate, pump.head, cavitation, 'H');
                    addSpaces();
                    printf("Head vs Flow Rate\n");

                    if (cavitation[0] != -1) {
                        addSpaces();
                        printf("Cavitation Onset at %.2f L/s\n", cavitation[1]);
                        addSpaces();
                        printf("The Cavitation Onset is marked using the letter \"H\"\n");
                    }
                    else {
                        addSpaces();
                        printf("No Cavitation Onset!\n");
                    }
                    pressAnyKeytoContinue();
                }

                else if (input2 == 2) {                             // efficiency vs flow rate
                    float bep[3];
                    minima_maxima(pump.flowRate, pump.efficiency, bep);
                    plot(pump.flowRate, pump.efficiency, bep, 'E');

                    addSpaces();
                    printf("Efficiency vs Flow Rate\n");

                    addSpaces();
                    printf("Best efficiency point (BEP): %.2f L/s\n", bep[1]);

                    addSpaces();
                    printf("The BEP is marked using the letter \"E\"\n");
                    pressAnyKeytoContinue();
                }

                else if (input2 == 3) {                             // power vs flow rate
                    float overload[3];
                    overloadCondition(pump.flowRate, pump.power, overload);
                    plot(pump.flowRate, pump.power, overload, 'P');

                    addSpaces();
                    printf("Power vs Flow Rate\n");
                    if (overload[0] != -1) {
                        addSpaces();
                        printf("Overload Point at %.2f L/s\n", overload[1]);

                        addSpaces();
                        printf("The Overload Point is marked using the letter \"P\"\n");
                    }
                    else {
                        addSpaces();
                        printf("No Overload Condition!\n");
                    }
                    pressAnyKeytoContinue();
                }
                else if (input2 == 4) {         // operating point 
                    float cavitation[3];
                    cavitationOnset(pump.flowRate, pump.head, cavitation);
                    plot(pump.flowRate, pump.head, cavitation, 'O');

                    addSpaces();
                    printf("Head vs Flow Rate\n");

                    addSpaces();
                    printf("Operating range is shown by the two straight lines made of ""+"".\n");

                    addSpaces();
                    printf("Minimum Threshold: %.2f m, Maximum Threshold: %.2f m.\n", pump.head[1], cavitation[1]);

                    addSpaces();
                    printf("Unsafe operating zones are indicated by the letter ""U"".\n");
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
}

void plot(float* arr_x, float* arr_y, float* specialLines, char plotID) {
    clearConsole();

    // scaling for y axis

    float std_y = standard_deviation(arr_y);
    float multiplying_factor = (std_y < SHOULD_SCALE_WHEN_STD_Y) ? EXPECTED_STANDARD_DEVIATION / std_y : 1;

    for (int i = 0; i < dataCount; i++) {
        arr_y[i] *= multiplying_factor;
    }

    // scaling for x axis

    float std_x = standard_deviation(arr_x);
    multiplying_factor = (std_x < SHOULD_SCALE_WHEN_STD_X) ? EXPECTED_STANDARD_DEVIATION / std_x : 1;

    for (int i = 0; i < dataCount; i++) {
        arr_x[i] *= multiplying_factor;
    }

    float max_min_y[3], max_min_x[3];
    maxima_minima_plot(arr_y, max_min_y);
    maxima_minima_plot(arr_x, max_min_x);

    int yAxisStartFrom = (max_min_y[0] > 5) ? 5 : 1;

    int ROW = max_min_y[1] - max_min_y[0] + yAxisStartFrom + 5;
    int COLUMN = max_min_x[1] - max_min_x[0] + 5;


    int X_CENTER = 2, Y_CENTER = ROW - 1;

    for (int row = 0; row < ROW; row++) {                               // row is y value
        for (int column = 0; column < COLUMN; column++) {               // column is x value

            int current_x = column - X_CENTER;                          // convert the row and column value
            int current_y = -row + Y_CENTER;                            // to actual points

            if (row == Y_CENTER && column == X_CENTER) {
                printf("O");
                continue;
            }
            if (row == Y_CENTER) {
                printf("--");
                continue;
            }
            if (column == X_CENTER) {

                printf("|");
                continue;
            }
            if (row <= ROW - 3 && row >= 4 && !(row % 4)) {
                if (column == 0) {              // 4 because we are taking 2 characters/block
                    printf("%4d", current_y + (int)max_min_y[0] - yAxisStartFrom);
                    continue;
                }
                if (column == 1) continue;
                if (column == 2) continue;
            }

            if (plotID == 'E' || plotID == 'H' || plotID == 'P') {
                if (current_x == (int)(specialLines[1] - max_min_x[0] + 1) && current_y == (int)(specialLines[0] - max_min_y[0] + yAxisStartFrom)) {
                    printf("%c ", plotID);
                    continue;
                }
            }

            if (plotID == 'O') {
                if (current_x == (int)(arr_x[1] - max_min_x[0] + 1) || current_x == (int)(specialLines[1] - max_min_x[0] + 1)) {
                    printf("+ ");
                    continue;
                }
                if (current_x < (int)(arr_x[1] - max_min_x[0] + 1) || current_x >(int)(specialLines[1] - max_min_x[0] + 1)) {
                    if (current_x > 0) {
                        printf("U ");
                        continue;
                    }
                }
            }

            int flag = 0;
            for (int i = 0; i < dataCount; i++) {
                if (current_x == (int)(arr_x[i] - max_min_x[0] + 1) && current_y == (int)(arr_y[i] - max_min_y[0] + yAxisStartFrom)) {
                    printf("● ");       // use * for codeblocks
                    flag = 1;
                    break;
                }
            }
            if (flag) continue;

            printf("  ");
        }
        printf("\n");
    }

    printf("\n");
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

void minima_maxima(float* flowRate, float* power_head_eff, float* result) {
    result[0] = 0;              // holds the highest efficiency/power/head
    result[1] = 0;              // holds the corresponding flow rate
    result[2] = 1000;           // holds the lowest efficiency/power/head
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

float average(float* points) {
    float sum = 0;
    for (int i = 0; i < dataCount; i++) {
        sum += points[i];
    }
    return sum / dataCount;
}

float standard_deviation(float* array) {
    float mean = average(array), sum = 0;
    for (int i = 0; i < dataCount; i++) {
        sum += ((array[i] - mean) * (array[i] - mean));
    }
    sum /= (dataCount - 1);     // sample standard deviation not population
    return sqrt(sum);
}

void maxima_minima_plot(float* array, float* result) {
    float max = 0, min = 1000;
    for (int i = 0; i < dataCount; i++) {
        if (array[i] > max) max = array[i];
        if (array[i] < min) min = array[i];
    }
    result[0] = min;            // holds the minimum value
    result[1] = max;            // holds the maximum value
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
    result[0] = -100;
    result[1] = flowRate[dataCount - 1];
    result[2] = -100;
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
    clearConsole();

    printf("+-----------+--------+---------+------------+\n");
    printf("| Flow Rate |  Head  |  Power  | Efficiency |\n");
    printf("+-----------+--------+---------+------------+\n");

    for (int i = 0; i < dataCount; i++) {
        printf("| %9.2f | %6.2f | %7.2f | %10.2f |\n", flowRate[i], head[i], power[i], efficiency[i]);
    }
    printf("+-----------+--------+---------+------------+\n");
}

void summaryTable(float* flowRate, float* head, float* power, float* efficiency) {
    clearConsole();

    float temp_result[3];
    minima_maxima(flowRate, head, temp_result);

    printf("Summary Table:\n\n");

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

    printf("| Shut-off Condition        | %6.2f m   |\n", head[0]);

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
    fprintf(f2, "| Minimum Efficiency        | %6.2f %%   |\n", temp_result[2]);
    fprintf(f2, "| BEP Flow Rate             | %6.2f L/s |\n", temp_result[1]);
    fprintf(f2, "| Shut-off Condition        | %6.2f m   |\n", head[0]);

    overloadCondition(flowRate, power, temp_result);

    fprintf(f2, "| Overload Start Flowrate   | %6.2f L/s |\n", temp_result[1]);

    cavitationOnset(flowRate, head, temp_result);

    fprintf(f2, "| Cavitation Onset Flowrate | %6.2f L/s |\n", temp_result[1]);
    fprintf(f2, "+---------------------------+------------+\n");

    fclose(f2);
}

void addSpaces(void) {
    for (int i = 0; i < dataCount / 2 - 10; i++) {
        printf("  ");
    }
}

void pressAnyKeytoContinue(void) {
    printf("Press any key to continue...");
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
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