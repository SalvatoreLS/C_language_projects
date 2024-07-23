#ifndef GAMIFICATION_GAMIF_H
#define GAMIFICATION_GAMIF_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

typedef struct people_t *people;
typedef struct person_t *person;
typedef struct symbTab_t *ST;
typedef struct node *link, node_t;
typedef struct task_t *task;
typedef struct taskList_t *taskList;
typedef struct solStruct_t *solStruct;

// INIT
people staffInit(int, int);
taskList taskListInit();
ST tabInit(int);

// FREE
void freeStaff(people);
void freeTask(task);
void freetaskList(taskList);
void freeTab(ST);

// INSERT/DELETE
void addTask(taskList *, task);
void deleteTask_fun(taskList *, int);
task copyTask(taskList, int);

// READ FILE
void readStaff(char *, char *, people *);
task readTaskFile(char *);

// PRINT
void printStaff_fun(people, taskList);
void printTab(ST);
void printTasks(taskList);
void printSol(solStruct, people);

// ALGORITHM
void findBestSol(people, task);
void combSimple(int, int, solStruct, solStruct, people, task);
float calcBudget(int *, int, people, task);
float calcDelta(int *, int, people, task);
int isBetter(solStruct, solStruct, task);
int getSkillIndex(char *, char **, int);
void copySol(solStruct, solStruct);

#endif //GAMIFICATION_GAMIF_H