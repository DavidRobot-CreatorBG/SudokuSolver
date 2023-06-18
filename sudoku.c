#include <stdio.h>
#include <stdbool.h>

#define SIZE 9
#define MAX_NAMES 50
#define MAX_LENGTH 50

int sudoku[SIZE][SIZE] = {
    {8, 5, 0, 0, 0, 2, 4, 0, 0},
    {7, 2, 0, 0, 0, 0, 0, 0, 9},
    {0, 0, 4, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 1, 0, 7, 0, 0, 2},
    {3, 0, 5, 0, 0, 0, 9, 0, 0},
    {0, 4, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 8, 0, 0, 7, 0},
    {0, 1, 7, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 3, 6, 0, 4, 0}
};

bool isRowPossible(int sudoku[SIZE][SIZE], int num, int row) {
    for (int i = 0; i < SIZE; i++) {
        if (num == sudoku[row][i])
            return false;
    }
    return true;
}

bool isColumnPossible(int sudoku[SIZE][SIZE], int num, int column) {
    for (int i = 0; i < SIZE; i++) {
        if (num == sudoku[i][column])
            return false;
    }
    return true;
}

bool isBoxPossible(int sudoku[SIZE][SIZE], int num, int column, int row) {
    int r, c;
    r = row - row % 3;
    c = column - column % 3;

    for (int i = r; i < r + 3; i++) {
        for (int j = c; j < c + 3; j++) {
            if (sudoku[i][j] == num)
                return false;
        }
    }
    return true;
}

bool isPositionPossible(int sudoku[SIZE][SIZE], int num, int column, int row) {
    return  isColumnPossible(sudoku, num, column) &&
            isRowPossible(sudoku, num, row) &&
            isBoxPossible(sudoku, num, column, row);
}

bool solveSudoku(int sudoku[SIZE][SIZE])
{
    for (int row = 0; row < SIZE; row++) {
        for (int column = 0; column < SIZE; column++)
            {
            if (sudoku[row][column] == 0) {
                for (int num = 1; num <= SIZE; num++) {
                    if (isPositionPossible(sudoku, num, column, row)) {
                        sudoku[row][column] = num;
                        if (solveSudoku(sudoku)) {
                            return true;
                        } else {
                            sudoku[row][column] = 0;
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

bool readSudokuFromFile( char* filePath, int grid[SIZE][SIZE]) {
    FILE* file = fopen(filePath, "r");
    if (file == NULL) {
        printf("Error opening the file: %s\n", filePath);
        return false;
    }
    char line[20];

    int row = 0;
    while (fgets(line, sizeof(line), file) != NULL) {
        if (line[0] == '*' || line[0] == '-' || line[0] == '+')
            continue;
        int col = 0;
        int charIndex = 0;
        while (col < SIZE) {
            char ch = line[charIndex];
            if (ch == '|' || ch == '+') {
                charIndex++;
                continue;
            }
            if (ch == '.') {
                grid[row][col] = 0;
                col++;
                charIndex++;
                continue;
            }
            if (ch == '-') {
                col = SIZE;
                break;
            }
            if (isdigit(ch)) {
                grid[row][col] = ch - '0';
                col++;
                charIndex++;
            }
        }
        if (col != SIZE) {
            printf("Invalid col length. \n");
            fclose(file);
            return false;
        }
        row++;
        if (row == SIZE)
            break;
    }
    fclose(file);
    return true;
}

void writeSudokuToFile(const char* filePath, const int grid[SIZE][SIZE]) {
    FILE* file = fopen(filePath, "a");
    if (file == NULL) {
        printf("Error opening the file: %s\n", filePath);
        exit(1);
    }

    fprintf(file, " *-----------*\n");
    for (int row = 0; row < SIZE; row++) {
        fprintf(file, " |");
        for (int col = 0; col < SIZE; col++) {
            if (grid[row][col] != 0)
                fprintf(file, "%d", grid[row][col]);
            else
                fprintf(file, ".");

            if ((col + 1) % 3 == 0 && col != SIZE - 1)
                fprintf(file, "|");
        }
        fprintf(file, "|\n");

        if ((row + 1) % 3 == 0 && row != SIZE - 1)
            fprintf(file, " |---+---+---|\n");
    }
    fprintf(file, " *-----------*\n");
    fprintf(file, "\n");

    fclose(file);
}
int main() {
    char filenames[MAX_NAMES][MAX_LENGTH];
    for (int i = 0; i < MAX_NAMES; i++) {
        printf("Filename %d: ", i + 1);
        scanf("%s", filenames[i]);

        printf("Unsolved: \n");
        if (readSudokuFromFile(filenames[i], sudoku)) {
            print(sudoku);
            printf("Solved: \n");
            solveSudoku(sudoku);
            print(sudoku);
            writeSudokuToFile("ivan.ss", sudoku);
        } else {
            printf("Failed to read from the file: %s\n", filenames[i]);
        }
        printf("\nNEXT: \n");
    }

    return 0;
}
