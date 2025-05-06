#include <stdio.h>
#include <math.h>

#define clrscr() printf("\e[1;1H\e[2J")

#define ROW 99                              // 43 max for full screen, 39 best
#define COLUMN 135
#define ORIGIN_X ((ROW-1)/2)                // also the Y limit
#define ORIGIN_Y ((COLUMN-1)/2)             // also the X limit
#define SCALE_GRAPH 10                      // every 10 box equals 1

#define TRUE 1
#define FALSE 0

#define TEST_POINTS 500                          // number of x points to be taken
#define X_INIT -ORIGIN_Y                        // where the x value will start from
#define X_FINAL ORIGIN_Y                        // where the x value will end
#define TOTAL_X_POINTS (X_FINAL - X_INIT + 1) 

float f(float x) {
    return sin(x);
}

int contains(int x_value, int y_value) {
    int j = 0;
    float increment = (TOTAL_X_POINTS / (float)TEST_POINTS);
    float x[TEST_POINTS + 10];
    for (float i = X_INIT; i <= X_FINAL; i += increment, j++) {
        x[j] = i;
    }

    float y[TEST_POINTS + 10];

    for (int i = 0; i < TEST_POINTS; i++) {
        y[i] = f(x[i]);
    }

    for (int i = 0; i < TEST_POINTS; i++) {
        if ((int)(x[i] * SCALE_GRAPH) == x_value && (int)(y[i] * SCALE_GRAPH) == y_value) {
            return TRUE;
        }
    }
    return FALSE;
}

void plot(void) {
    clrscr();

    for (int row = 0; row < ROW; row++) {                               // row is y value
        for (int column = 0; column < COLUMN; column++) {               // column is x value

            if (row == 0 || row == ROW - 1) {
                printf("__");
                continue;
            }

            int current_x = column - ORIGIN_Y;                          // convert the row and column value
            int current_y = -row + ORIGIN_X;                            // to actual points

            if (contains(current_x, current_y)) {                       // check if the point is to be plotted
                printf("O ");
                continue;
            }
            if (column == ORIGIN_Y && row == ORIGIN_X) {      // show origin if empty
                printf("o");
                continue;
            }
            if (row == ORIGIN_X) {
                printf("--");
                continue;
            }
            if (column == ORIGIN_Y) {
                printf("|");
            }
            else printf("  ");
        }
        printf("\n");
    }

    printf("\n");
}

int main() {
    plot();

    return 0;
}
