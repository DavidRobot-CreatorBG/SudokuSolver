#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define SIZE 9

int sudoku[SIZE][SIZE] = {
    {5, 3, 0, 0, 7, 0, 0, 0, 0},
    {6, 0, 0, 1, 9, 5, 0, 0, 0},
    {0, 9, 8, 0, 0, 0, 0, 6, 0},
    {8, 0, 0, 0, 6, 0, 0, 0, 3},
    {4, 0, 0, 8, 0, 3, 0, 0, 1},
    {7, 0, 0, 0, 2, 0, 0, 0, 6},
    {0, 6, 0, 0, 0, 0, 2, 8, 0},
    {0, 0, 0, 4, 1, 9, 0, 0, 5},
    {0, 0, 0, 0, 8, 0, 0, 7, 9}
};

bool isRowPossible(int num, int row) {
    for (int i = 0; i < SIZE; i++) {
        if (num == sudoku[row][i])
            return false;
    }
    return true;
}

bool isColumnPossible(int num, int column) {
    for (int i = 0; i < SIZE; i++) {
        if (num == sudoku[i][column])
            return false;
    }
    return true;
}

bool isBoxPossible(int num, int startRow, int startCol) {
    int endRow = startRow + 2;
    int endCol = startCol + 2;

    for (int row = startRow; row <= endRow; row++) {
        for (int col = startCol; col <= endCol; col++) {
            if (sudoku[row][col] == num)
                return false;
        }
    }
    return true;
}

bool isPositionPossible(int num, int row, int col) {
    int startRow = row - (row % 3);
    int startCol = col - (col % 3);

    return isRowPossible(num, row) &&
           isColumnPossible(num, col) &&
           isBoxPossible(num, startRow, startCol);
}

bool solveSudoku() {
    for (int row = 0; row < SIZE; row++) {
        for (int col = 0; col < SIZE; col++) {
            if (sudoku[row][col] == 0) {
                for (int num = 1; num <= SIZE; num++) {
                    if (isPositionPossible(num, row, col)) {
                        sudoku[row][col] = num;
                        if (solveSudoku()) {
                            return true;
                        } else {
                            sudoku[row][col] = 0;
                        }
                    }
                }
                return false;
            }
        }
    }
    return true;
}

void print(int sudoku[SIZE][SIZE]) {
    printf("---------------------\n");
    for (int row = 0; row < SIZE; row++) {
        for (int column = 0; column < SIZE; column++) {
            if (column % 3 == 0 && column != 0)
                printf("| ");
            printf("%d ", sudoku[row][column]);
        }
        printf("\n");
        if ((row + 1) % 3 == 0 && row != SIZE - 1)
            printf("---------------------\n");
    }
    printf("---------------------\n");
}

int main() {
    printf("Sudoku:\n");
    print(sudoku);

    if (solveSudoku()) {
        printf("Solved:\n");
        print(sudoku);
    } else {
        printf("error\n");
    }

    return 0;
}
