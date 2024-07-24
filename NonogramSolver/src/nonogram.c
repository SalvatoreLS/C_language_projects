#include "nonogram.h"
#include <stdlib.h>
#include <stdio.h>

struct nonogram_t {
    int **matrix;
    int rows;
    int cols;
    int **row_clues;
    int *n_row_clues;
    int **col_clues;
    int *n_col_clues;
};

Nonogram nonogram_create(int rows, int cols){
    Nonogram nonogram = malloc(sizeof(struct nonogram_t));

    if (nonogram == NULL) return NULL;

    nonogram->rows = rows;
    nonogram->cols = cols;

    nonogram->matrix = calloc(rows, sizeof(int *));

    if (nonogram->matrix == NULL) {
        free(nonogram);
        return NULL;
    }

    for(int i = 0; i < rows; i++){
        nonogram->matrix[i] = calloc(cols, sizeof(int));
        if (nonogram->matrix[i] == NULL){
            for (int j = 0; j < i; j++){
                free(nonogram->matrix[j]);
            }
            free(nonogram->matrix);
            free(nonogram);
            return NULL;
        }
    }

    nonogram->n_row_clues = calloc(rows, sizeof(int));
    nonogram->n_col_clues = calloc(cols, sizeof(int));
    nonogram->row_clues = calloc(rows, sizeof(int *));
    nonogram->col_clues = calloc(cols, sizeof(int *));

    if (nonogram->n_row_clues == NULL || nonogram->n_col_clues == NULL
        || nonogram->row_clues == NULL || nonogram->col_clues == NULL){
        nonogram_destroy(nonogram);
        return NULL;
    }

    for (int i = 0; i < rows; i++){
        nonogram->row_clues[i] = calloc(rows, sizeof(int));
        if (nonogram->row_clues[i] == NULL){
            nonogram_destroy(nonogram);
            return NULL;
        }
    }

    for (int i = 0; i < cols; i++){
        nonogram->col_clues[i] = calloc(cols, sizeof(int));
        if (nonogram->col_clues[i] == NULL){
            nonogram_destroy(nonogram);
            return NULL;
        }
    }

    return nonogram;
}

void nonogram_destroy(Nonogram nonogram){

    if (nonogram == NULL) return;

    for(int i = 0; i < nonogram->rows; i++){
        free(nonogram->matrix[i]);
        free(nonogram->row_clues[i]);
    }
    free(nonogram->matrix);
    free(nonogram->row_clues);

    for(int i = 0; i < nonogram->cols; i++){
        free(nonogram->col_clues[i]);
    }
    free(nonogram->col_clues);

    free(nonogram->n_row_clues);
    free(nonogram->n_col_clues);
    free(nonogram);
}

void nonogram_print(Nonogram nonogram) {
    if (nonogram == NULL) {
        printf("nonogram is null\n");
        return;
    }

    for (int i = 0; i < nonogram->rows; i++) {
        for (int j = 0; j < nonogram->cols; j++) {
            if (nonogram->matrix[i][j] == -1)
                printf("0 ");
            else
                printf("%d ", nonogram->matrix[i][j]);
        }
        printf("\n");
    }
}

Nonogram nonogram_read(FILE *file){
    if (file == NULL)
        return NULL;

    int rows, cols;
    if (fscanf(file, "%d %d", &rows, &cols) != 2)
        return NULL;

    Nonogram nonogram = nonogram_create(rows, cols);
    if (nonogram == NULL)
        return NULL;

    char c;
    fscanf(file, "%c", &c); // read newline character (discard it)

    int n = 0;
    int i, j;

    // read row clues
    for (i = 0; i < rows; i++){
        j = 0;
        while ((c = fgetc(file)) != '\n' && c != EOF){
            if (c <= '9' && c >= '0'){
                n = n * 10 + (c - '0');
            } else if (c == ' ' && n > 0) {
                nonogram->row_clues[i][j++] = n;
                nonogram->n_row_clues[i]++;
                n = 0;
            }
        }
        if (n > 0) {
            nonogram->row_clues[i][j] = n;
            nonogram->n_row_clues[i]++;
            n = 0;
        }
    }

    // read column clues
    for (i = 0; i < cols; i++){
        j = 0;
        while ((c = fgetc(file)) != '\n' && c != EOF){
            if (c <= '9' && c >= '0'){
                n = n * 10 + (c - '0');
            } else if (c == ' ' && n > 0) {
                nonogram->col_clues[i][j++] = n;
                nonogram->n_col_clues[i]++;
                n = 0;
            }
        }
        if (n > 0) {
            nonogram->col_clues[i][j] = n;
            nonogram->n_col_clues[i]++;
            n = 0;
        }
    }

    return nonogram;
}

void nonogram_solve(Nonogram *nonogram){
    if (nonogram == NULL || *nonogram == NULL){
        printf("nonogram is null\n");
        return;
    }

    nonogram_optimization(nonogram);
    printf("nonogram after optimization:\n");
    nonogram_print(*nonogram);
    printf("\n\n");

    int result = nonogram_solve_recursive(*nonogram, 0, 0);

    if (!result){
        printf("no solution found\n");
    } else {
        printf("solution found\n");
        nonogram_print(*nonogram);
    }
}

int nonogram_solve_recursive(Nonogram nonogram, int row, int col) {
    // base case: if we've filled all cells, check if the solution is valid
    if (row == nonogram->rows) {
        for (int c = 0; c < nonogram->cols; c++) {
            if (!is_col_valid(nonogram, c)) {
                return 0;
            }
        }
        return 1;
    }

    // move to the next row if we've reached the end of this row
    if (col == nonogram->cols) {
        if (!is_row_valid(nonogram, row)) {
            return 0;
        }
        return nonogram_solve_recursive(nonogram, row + 1, 0);
    }

    // if the cell is already filled (1) or marked as empty (-1), skip it
    if (nonogram->matrix[row][col] == 1 || nonogram->matrix[row][col] == -1) {
        return nonogram_solve_recursive(nonogram, row, col + 1);
    }

    // try filling the current cell with 1 (filled)
    nonogram->matrix[row][col] = 1;
    if (nonogram_solve_recursive(nonogram, row, col + 1)) {
        return 1;
    }

    // if filling with 1 didn't work, try 0 (empty)
    nonogram->matrix[row][col] = 0;
    if (nonogram_solve_recursive(nonogram, row, col + 1)) {
        return 1;
    }

    // if neither worked, this path doesn't lead to a solution
    nonogram->matrix[row][col] = 0;  // reset the cell
    return 0;
}

// helper function to check if a row is valid according to its clues
int is_row_valid(Nonogram nonogram, int row) {
    int count = 0;
    int clue_index = 0;

    for (int col = 0; col <= nonogram->cols; col++) {
        if (col == nonogram->cols || nonogram->matrix[row][col] == 0 || nonogram->matrix[row][col] == -1) {
            if (count > 0) {
                if (clue_index >= nonogram->n_row_clues[row] ||
                    count != nonogram->row_clues[row][clue_index]) {
                    return 0;
                }
                clue_index++;
            }
            count = 0;
        } else if (nonogram->matrix[row][col] == 1) {
            count++;
        }
    }

    return clue_index == nonogram->n_row_clues[row];
}

// helper function to check if a column is valid according to its clues
int is_col_valid(Nonogram nonogram, int col) {
    int count = 0;
    int clue_index = 0;

    for (int row = 0; row <= nonogram->rows; row++) {
        if (row == nonogram->rows || nonogram->matrix[row][col] == 0 || nonogram->matrix[row][col] == -1) {
            if (count > 0) {
                if (clue_index >= nonogram->n_col_clues[col] ||
                    count != nonogram->col_clues[col][clue_index]) {
                    return 0;
                }
                clue_index++;
            }
            count = 0;
        } else if (nonogram->matrix[row][col] == 1) {
            count++;
        }
    }

    return clue_index == nonogram->n_col_clues[col];
}

void nonogram_optimization(Nonogram *nonogram){

    int sum_clues;

    for (int i = 0; i < (*nonogram)->rows; i++){
        sum_clues = 0;
        for (int j = 0; j < (*nonogram)->n_row_clues[i]; j++){
            sum_clues += (*nonogram)->row_clues[i][j] + 1;
        }

        sum_clues--; // remove the last space
        if (sum_clues == (*nonogram)->cols){
            fill_row(nonogram, i); // fill the row with the clues
        }
    }


    for (int j = 0; j < (*nonogram)->cols; j++){
        sum_clues  = 0;
        for (int i = 0; i < (*nonogram)->n_col_clues[j]; i++){
            sum_clues += (*nonogram)->col_clues[j][i] + 1;
        }

        sum_clues--; // remove the last space
        if(sum_clues == (*nonogram)->rows){
            fill_col(nonogram, j); // fill the column with the clues
        }
    }
}

void fill_row(Nonogram *nonogram, int index) {
    int col = 0;
    for (int i = 0; i < (*nonogram)->n_row_clues[index]; i++) {
        for (int k = 0; k < (*nonogram)->row_clues[index][i]; k++) {
            (*nonogram)->matrix[index][col++] = 1;
        }
        if (i < (*nonogram)->n_row_clues[index] - 1 && col < (*nonogram)->cols) {
            (*nonogram)->matrix[index][col++] = -1; // Add space only if it's not the last clue and there's room
        }
    }
    while (col < (*nonogram)->cols) {
        (*nonogram)->matrix[index][col++] = -1;
    }
}

void fill_col(Nonogram *nonogram, int index){
    int row = 0;
    for (int i = 0; i < (*nonogram)->n_col_clues[index]; i++){
        for (int k = 0; k < (*nonogram)->col_clues[index][i]; k++){
            (*nonogram)->matrix[row++][index] = 1;
        }
        if (row < (*nonogram)->rows)
            (*nonogram)->matrix[row++][index] = -1; // cell with a space
    }

    while (row < (*nonogram)->rows){
        (*nonogram)->matrix[row++][index] = -1;
    }
}
