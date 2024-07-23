#include "gamif.h"

#define CMAX 50

struct person_t{
    char name[CMAX];
    char surname[CMAX];
    float salary;
    int *skills;
    int available;
};

struct people_t{
    person *vecPeople;
    int nPeople;
    int nSkills;
    char **skillNames;
};

struct symbTab_t{
    int dim;
    char **skills;
    int *req;
};

struct node{
    task taskEl;
    link next;
};

struct taskList_t{
    link head;
    link z;
    int nTasks;
};

struct task_t{
    char name[CMAX];
    ST tab;
    float budget;
    int nP;
};

struct solStruct_t{
    int *vec;
    int dim;
    float budget;
    float delta;
};


void readStaff(char *staffName, char *skillsName, people *staff){
    FILE *fileSkills = fopen(skillsName, "r");
    FILE *fileStaff = fopen(staffName, "r");

    if (fileStaff == NULL || fileSkills == NULL) {
        printf("Error while reading files");
        if (fileSkills) fclose(fileSkills);
        if (fileStaff) fclose(fileStaff);
        return;
    }
    int nS, nP;

    fscanf(fileSkills, "%d", &nS);
    fscanf(fileStaff, "%d", &nP);
    
    *staff = staffInit(nP, nS);

    if (*staff == NULL){
        printf("Error with staff initialization");
        fclose(fileSkills); fclose(fileStaff);
        return;
    }

    for (int i = 0; i < nS; i++)
        fscanf(fileSkills, "%s", (*staff)->skillNames[i]);
    fclose(fileSkills);

    for (int i = 0; i < nP; i++){
        if ((*staff)->vecPeople[i] == NULL) {
            printf("Memory allocation failed for vecPeople[%d]", i);
            fclose(fileStaff);
            return;
        }
        fscanf(fileStaff, "%s %s %f", (*staff)->vecPeople[i]->name, (*staff)->vecPeople[i]->surname, &((*staff)->vecPeople[i]->salary));
        (*staff)->vecPeople[i]->available = 1;

        for (int j = 0; j < nS; j++)
            fscanf(fileStaff, "%d", &((*staff)->vecPeople[i]->skills[j]));
    }

    fclose(fileStaff);
}

people staffInit(int nP, int nS){
    people staff = malloc(sizeof (struct people_t));

    staff->nSkills = nS;
    staff->nPeople = nP;

    staff->vecPeople = malloc(nP * sizeof (struct person_t*));

    for (int i = 0; i < nP; i++){
        staff->vecPeople[i] = malloc(sizeof (struct person_t));
        staff->vecPeople[i]->skills = calloc(nS, sizeof(int));
    }

    staff->skillNames = calloc(nS, sizeof (char *));

    for (int i = 0; i < nS; i++)
        staff->skillNames[i] = calloc(CMAX, sizeof (char));

    return staff;
}

taskList taskListInit(void){
    taskList tasks = malloc(sizeof (struct taskList_t));
    tasks->z = malloc(sizeof(struct node));

    tasks->z->taskEl = NULL;
    tasks->z->next = NULL;
    tasks->head = tasks->z;

    tasks->nTasks = 0;

    return tasks;
}

void addTask(taskList *tasks, task t){
    link newTaskNode = malloc(sizeof (struct node));
    newTaskNode->taskEl = t;
    (*tasks)->nTasks++;

    newTaskNode->next = (*tasks)->head;
    (*tasks)->head = newTaskNode;
}

void printStaff_fun(people staff, taskList tasks){
    link tmp;

    printf("Available people:\n");

    // Print people
    for (int i = 0; i < staff->nPeople; i++) {
        if (staff->vecPeople[i]->available == 1){
            printf("%d) %s %s %.2f\n\t", i + 1, staff->vecPeople[i]->name, staff->vecPeople[i]->surname, staff->vecPeople[i]->salary);

            for (int j = 0; j < staff->nSkills; j++)
                printf("%d ", staff->vecPeople[i]->skills[j]);
            printf("\n");
        }
    }
}

void printTab(ST tab){
    printf("\n");
    for (int i = 0; i < tab->dim; i++){
        printf("%s %d\n", tab->skills[i], tab->req[i]);
    }
    printf("\n");
}

task readTaskFile(char *fileName){
    FILE *inFile = fopen(fileName, "r");

    int tabDim;

    if (inFile == NULL){
        printf("Error while opening file\n");
        return NULL;
    }

    task t1 = malloc(sizeof (struct task_t));


    fgets(t1->name, CMAX, inFile);

    fscanf(inFile, "%d %d %f", &tabDim, &(t1->nP), &(t1->budget));

    t1->tab = tabInit(tabDim);

    for (int i = 0; i < t1->tab->dim; i++)
        fscanf(inFile, "%s %d", t1->tab->skills[i], &(t1->tab->req[i]));

    fclose(inFile);

    return t1;
}

ST tabInit(int nSkills){
    ST tab = malloc(sizeof (struct symbTab_t));
    tab->dim = nSkills;
    tab->skills = calloc(nSkills, sizeof (char *));
    tab->req = calloc(nSkills, sizeof (int));
    
    for (int i = 0; i < nSkills; i++)
        tab->skills[i] = calloc(CMAX, sizeof (char));

    return tab;
}

void findBestSol(people staff, task t){
    int k;
    int maxNum;
    solStruct sol = malloc(sizeof (struct solStruct_t));
    solStruct bestSol = malloc(sizeof (struct solStruct_t));

    bestSol->budget = (float)INT_MAX;

    bestSol->delta = (float)INT_MAX;

    bestSol->dim = 0;

    sol->vec = calloc(t->nP, sizeof (int));
    bestSol->vec = calloc(t->nP, sizeof (int));

    for (int i = 0; i < t->nP; i++) {
        sol->vec[i] = -1;
        bestSol->vec[i] = -1;
    }

    maxNum = (t->nP < staff->nPeople) ? t->nP : staff->nPeople;

    for (k = 1; k <= maxNum; k++){
        sol->dim = k;
        sol->budget = 0;
        sol->delta = 0;

        combSimple(0, 0, sol, bestSol, staff, t);
    }

    printf("\nMiglior soluzione trovata:\n");
    printSol(bestSol, staff);

    free(sol->vec);
    free(bestSol->vec);
    free(sol);
    free(bestSol);
}

void printSol(solStruct sol, people staff){
    for (int i = 0; i < sol->dim; i++)
        printf("%d) %s %s %.2f\n", i+1, staff->vecPeople[sol->vec[i]]->name, staff->vecPeople[sol->vec[i]]->surname, staff->vecPeople[sol->vec[i]]->salary);

    printf("\n");
    printf("Delta: %.2f\n", sol->delta);
    printf("Budget richiesto: %.2f\n", sol->budget);
    printf("Numero di dipendenti: %d\n", sol->dim);
}

void combSimple(int pos, int start, solStruct sol, solStruct bestSol, people staff, task t){
    int i;

    if (pos >= sol->dim){
        sol->delta = calcDelta(sol->vec, sol->dim, staff, t);
        if(isBetter(sol, bestSol, t))
            copySol(bestSol, sol);
        return;
    }

    for (i = start; i < staff->nPeople; i++){
        sol->vec[pos] = i;
        sol->budget += staff->vecPeople[i]->salary;
        combSimple(pos+1, i+1, sol, bestSol, staff, t);
        sol->budget -= staff->vecPeople[i]->salary;
    }
}

float calcDelta(int *sol, int k, people staff, task t){
    float delta = 0;
    float partDelta;
    int skillIndex;

    for (int i = 0; i < k; i++){ // Ciclo lungo sol
        partDelta = 0;

        for (int j = 0; j < t->tab->dim; j++){
            skillIndex = getSkillIndex(t->tab->skills[j], staff->skillNames, staff->nSkills);

            if (skillIndex == -1)
                return (float)INT_MAX;

            partDelta += (t->tab->req[j] - staff->vecPeople[sol[i]]->skills[skillIndex]);
        }

        delta += partDelta;
    }

    return delta;
}

int getSkillIndex(char *skillName, char **tabNames, int dim){
    for (int i = 0; i < dim; i++)
        if (strcmp(skillName, tabNames[i]) == 0)
            return i;

    return -1;
}

float calcBudget(int *sol, int k, people staff, task t){ // Calcola il nuovo budget rimanente con quella sol
    float budget = 0;
    
    for (int i = 0; i < k; i++)
        budget += staff->vecPeople[sol[i]]->salary;

    return budget;
}

int isBetter(solStruct sol, solStruct bestSol, task t){

    // Se il budget richiesto dalla sol è maggiore, allora no
    if (t->budget - sol->budget < 0)
        return 0;

    if (sol->budget < bestSol->budget)
        if (sol->delta < bestSol->delta)
            return 1;

    return 0;
}

void copySol(solStruct bestSol, solStruct sol){
    for (int i = 0; i < sol->dim; i++)
        bestSol->vec[i] = sol->vec[i];

    bestSol->budget = sol->budget;
    bestSol->delta  = sol->delta;
    bestSol->dim = sol->dim;
}

void deleteTask_fun(taskList *tasks, int index){
    link tmp = (*tasks)->head;
    link prev = NULL;
    int i = 0;

    for (; tmp != (*tasks)->z && i < index; prev = tmp, tmp = tmp->next, i++);

    if (tmp == (*tasks)->z && i < index){
        printf("Error while deleting task. Element not existing\n");
        return;
    }
    
    if (prev == NULL){
        (*tasks)->head = (*tasks)->head->next;
        freeTask(tmp->taskEl);
        free(tmp);
        printf("Task deleted successfully\n");
        return;
    }

    prev->next = tmp->next;
    freeTask(tmp->taskEl);
    free(tmp);
    printf("Task deleted successfully\n");
}

void freeStaff(people staff){

    for (int i = 0; i < staff->nSkills; i++){
        free(staff->skillNames[i]);
    }

    free(staff->skillNames);

    for (int i = 0; i < staff->nPeople; i++){
        free(staff->vecPeople[i]->skills);
        free(staff->vecPeople[i]);
    }

    free(staff->vecPeople);

    free(staff);
}

void freeTask(task t){
    if (t == NULL)
        return;

    freeTab(t->tab);
    free(t);
}

void freetaskList(taskList tasks){
    if (tasks == NULL)
        return;
    
    link tmp = tasks->head;
    link next = NULL;

    while (tmp != tasks->z){
        next = tmp->next;
        freeTask(tmp->taskEl);
        free(tmp);
        tmp = next;
    }

    free(tasks->z);
    free(tasks);
}

void freeTab(ST t){
    if (t == NULL)
        return;

    for (int i = 0; i < t->dim; i++)
        free(t->skills[i]);

    free(t->req);
    free(t->skills);
    free(t);
}

void printTasks(taskList tasks){
    link tmp = tasks->head;
    int i = 1;

    if (tmp == tasks->z){
        printf("\n\nNo tasks in memory\n\n\n");
        return;
    }

    for (; tmp != tasks->z; tmp = tmp->next, i++){
        printf("%d) %s", i, tmp->taskEl->name);
        printf("Required skills:");
        printTab(tmp->taskEl->tab);
        printf("Budget: %.2f", tmp->taskEl->budget);
        printf("\n\n");
    }
}

task copyTask(taskList tasks, int index){
    link tmp = tasks->head;
    link prev = NULL;
    int i = 0;

    for (; tmp != tasks->z && i < index; prev = tmp, tmp = tmp->next, i++);

    if (tmp == tasks->z && i < index){
        printf("Error while finding task. Element not existing\n");
        return NULL;
    }

    return tmp->taskEl;
}