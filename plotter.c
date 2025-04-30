#include <stdio.h>
#include <math.h>

#define clrscr() printf("\e[1;1H\e[2J")

#define ROW 39                              // 43 max for full screen, 39 best
#define COLUMN 65
#define ORIGIN_X ((ROW-1)/2)                // also the Y limit
#define ORIGIN_Y ((COLUMN-1)/2)             // also the X limit

#define TRUE 1
#define FALSE 0

#define X_INIT -ORIGIN_Y
#define X_FINAL ORIGIN_Y
#define X_COUNT (X_FINAL - X_INIT + 1)

int f(int x) {
    return 5 * sin(x);
}

int contains(int x_value, int y_value) {
    int j = 0;
    int x[X_COUNT];
    for (int i = X_INIT; i <= X_FINAL; i++, j++) {
        x[j] = i;
    }

    int x_size = sizeof(x) / 4;
    int y[x_size];

    for (int i = 0; i < x_size; i++) {
        y[i] = f(x[i]);
    }

    for (int i = 0; i < x_size; i++) {
        if (x[i] == x_value && y[i] == y_value) {
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
                printf("* ");
                continue;
            }
            if (column == ORIGIN_Y && row == ORIGIN_X) printf("o");     // show origin if empty
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
