# Nonogram Solver
==================

## Features

* Nonogram file reading
* Nonogram solution finder by implementing optimization and backtracking

## Project structure

The project consists of two main files:
1. `main.c`: file that calls functions and displays the result
2. `nonogram.h`: Header file
3. `nonogram.c`: containing functions used in main

## Algoritm

The solver uses a recursive backtracking algorithm to find a solution that satisfies both row and column constraints. It works as follows:

1. Traverses the grid cell by cell.
2. For each cell, it tries filling it (1) or leaving it empty (0).
3. It recursively proceeds to the next cell.
4. If a contradiction is found, it backtracks.
5. The process continues until a valid solution is found or all possibilities are exhausted.

I also implemented a function called `nonogram_optimization` in order to make the algorithm be more efficient.

## Data Structures

* `nonogram_t`: It contains all the relevant data about nonogram matrix and clues that has to be used to solve it

## License
This program is licensed under the MIT License. See the LICENSE file for details.

### Credits

* **Salvatore Lo Sardo**: Author