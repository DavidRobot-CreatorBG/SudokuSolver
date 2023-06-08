#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define SIZE 9
#define MAX_NAMES 50
#define MAX_LENGTH 50
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
void readSudokuFromFile(const char* filePath, int grid[SIZE][SIZE]) {
    FILE* file = fopen(filePath, "r");
    if (file == NULL) {
        printf("Error opening the file: %s\n", filePath);
        exit(1);
    }
    char line[20];

    int row = 0;
    while (fgets(line, sizeof(line), file) != NULL) {
        if (line[0] == '*'|| line[0] == '-' || line[0] == '+')
            continue;  // Skip whole lines as a separator lines

    //    printf("\nrow %d --> %s \n",row,line);
       int col = 0;
        int charIndex = 0;  // Start from index 1 to skip the '|'
        while (col < SIZE) {
            char ch = line[charIndex];
     //         printf("ch %c ",ch);
              if(ch=='|' || ch=='+')  {charIndex ++; continue; }// Skip separator lines |
              if(ch=='.')  {grid[row][col] = 0; col++; charIndex ++; continue; }// Skip separator lines |
              if(ch=='-')  {col = SIZE; row--; break; }// Break if find '-' as separator lines |
            if (isdigit(ch)) {
                grid[row][col] = ch - '0';  // Convert character to integer
                col++;
                charIndex ++;  // Skip the next character and the '|'
            }
        }
        if (col != SIZE) {
            printf("Invalid col length in the Sudoku grid.\n");
            exit(1);
        }
        row++;
        if (row == SIZE)
            break;  // Stop reading after reaching the expected grid size
    }
    fclose(file);
}
void writeSudokuToFile(const char* filePath, const int grid[SIZE][SIZE]) {
    FILE* file = fopen(filePath, "w");
    if (file == NULL) {
        printf("Error creating the output file.\n");
        exit(1);
    }

    // Writing Sudoku to file in the specified format
    fprintf(file, " *-----------*\n");
    for (int row = 0; row < SIZE; row++) {
        fprintf(file, " |");
        for (int col = 0; col < SIZE; col++) {
            int value = grid[row][col];
            if (value != 0)
                fprintf(file, "%d", value);
            else
                fprintf(file, ".");

            if ((col + 1) % 3 == 0 && col != SIZE - 1)
                fprintf(file, "|");
        }
        fprintf(file, "|\n");

        if ((row + 1) % 3 == 0 && row != SIZE - 1)
            fprintf(file, " |---+---+---|\n");
    }
    fprintf(file, " *-----------*");

    fclose(file);
}
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

bool isPositionPossible(int sudoku[SIZE][SIZE], int num, int column,int row) {
    return  isColumnPossible(sudoku, num, column) &&
            isRowPossible(sudoku, num, row) &&
            isBoxPossible(sudoku, num, column, row);

}
bool solveSudoku(int sudoku[SIZE][SIZE])
{
    for (int row = 0; row < SIZE; row++) {
        for (int column = 0; column < SIZE; column++)
        {
            if (sudoku[row][column] == 0){
                for (int k = 1; k <= SIZE; k++) {
                    if (isPositionPossible(sudoku, k, column, row)){
                        sudoku[row][column] = k;
                        if (solveSudoku(sudoku)){
                            return true;
                        }
                        else {
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
int main() {
    char filenames[MAX_NAMES][MAX_LENGTH];

    for (int i = 0; i < MAX_NAMES; i++) {
        printf("Filename %d: ", i + 1);
        scanf("%s", filenames[i]);

        printf("Unsolved: \n");
        readSudokuFromFile(filenames[i], sudoku);
        print(sudoku);
        printf("Solved: \n");

        solveSudoku(sudoku);
        print(sudoku);
        writeSudokuToFile("colors1.ss", sudoku);
        printf("\nNEXT: \n");
    }

    return 0;
}
