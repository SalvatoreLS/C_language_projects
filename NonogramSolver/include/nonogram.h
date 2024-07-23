#ifndef NONOGRAM_H
#define NONOGRAM_H

#include <stdio.h>
#include <stdlib.h>

typedef struct nonogram_t *Nonogram;

Nonogram nonogram_create(int rows, int cols);
void nonogram_destroy(Nonogram nonogram);
void nonogram_print(Nonogram nonogram);
Nonogram nonogram_read(FILE *file);
void nonogram_solve(Nonogram *nonogram);
int is_row_valid(Nonogram nonogram, int row);
int is_col_valid(Nonogram nonogram, int col);
int nonogram_solve_recursive(Nonogram nonogram, int row, int col);
void nonogram_optimization(Nonogram *nonogram);
void fill_row(Nonogram *nonogram, int index);
void fill_col(Nonogram *nonogram, int index);


#endif // NONOGRAM_H
