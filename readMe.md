**TaskMaster**
================

A simple staff management and task assignment system.


## Features

* Manage a staff with skills and salaries
* Assign tasks to staff members based on their skills
* Read and print staff and tasks
* Delete tasks
* Find the best solution to complete a task


## Algorithm

The algorithm's primary objective is to **find the best combination** of staff members (represented by `person_t` structures) who can complete given tasks (represented by task_t structures) by matching the **required skills** of the tasks with the **skills of the staff members**.
The algorithm generates combinations of staff members and finds the best group of them exploring all the possible solution.

### Data Structures
* `person_t`: Represents a staff member with attributes like name, number of skills, and an array of skills.
* `task_t`: Represents a task with attributes like name, number of required skills, and an array of required skills.
* `symbTab_t`: Used for mapping skill names and values for each task.
* `solStruct_t`: Used to store the local best solution and the global one at the end.
* `taskList_t`: Linked list storing all tasks read from files.

### Main Functions
* `readStaff`, `readTaskFile`: Functions used to perform file reading and storing values.
* `addTask`, `deleteTask_fun`, `copyTask`: Functions used to add/delete tasks or copying.
* `findBestSol`, `combSimple`: Functions used to find the best solution and generate combinations.

## Usage

1. Compile the program using `gcc main.c gamif.c -o gamif`
2. Run the program using `./gamif`
3. Follow the prompts to interact with the program

## Files
* `main.c`: Main file
* `gamif.h`: Header file containing function declarations
* `gamif.c`: File containing function 
* `README.md`: This file

## License

This program is licensed under the MIT License. See the LICENSE file for details.

### Credits

* **Salvatore Lo Sardo**: Author of the program

### Changelog

* [Version 1.0]: Initial release

### Contributing

Contributions are welcome! If you'd like to contribute to the program, please submit a pull request.

### Issues

If you encounter any issues with the program, please submit an issue report.