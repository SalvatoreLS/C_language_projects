#include "nonogram.h"
#include <stdio.h>

int main(int argc, char **argv){

  FILE *file = fopen("../data/hard_nonogram.txt", "r");

  if (file == NULL){
    printf("Error opening file\n");
    return 1;
  }

  Nonogram nonogram = nonogram_read(file);

  fclose(file);

  if (nonogram == NULL){
    printf("Error reading nonogram\n");
    return 1;
  }

  nonogram_solve(&nonogram); // Prints out the solution

  nonogram_destroy(nonogram);

  return 0;
}
