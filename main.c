#include "gamif.h"

#define CMAX 50

enum scelta{readData, printStaff, printTask, readTask, deleteTask, findSol, quit};

enum scelta userMenu(int x){
    switch (x){
        case 0:
            return readData;
        case 1:
            return printStaff;
        case 2:
            return printTask;
        case 3:
            return readTask;
        case 4:
            return deleteTask;
        case 5:
            return findSol;
        default:
            return quit;
    }
}

void getFileName(char *);
int printMenu(void);

int main(void){
    people Staff = NULL;
    taskList Tasks = NULL;
    task tmpTask = NULL;
    int choice;

    char fileStaff[CMAX];
    char fileSkills[CMAX];
    char taskName[CMAX];

    setbuf(stdout, 0);

    Tasks = taskListInit();

    while(1){
        choice = printMenu();

        switch (choice)
        {
            case readData:
                printf("Reading staff ...\n");
                getFileName(fileStaff);
                printf("Reading skills...\n");
                getFileName(fileSkills);
                readStaff(fileStaff, fileSkills, &Staff);
                break;
            case printStaff:
                printStaff_fun(Staff, Tasks);
                break;
            case printTask:
                printTasks(Tasks);
                break;
            case readTask:
                printf("Reading task...\n");
                getFileName(taskName);
                tmpTask = readTaskFile(taskName);
                if (tmpTask == NULL){
                    printf("\nTask reading failed\n\n");
                    break;
                }
                addTask(&Tasks, tmpTask);
                printf("\nTask successfully read\n\n");
                break;
            case deleteTask:
                printf("Deleting task...\n");
                printTasks(Tasks);
                printf("\nInsert number of the task to delete: ");
                scanf("%d", &choice);
                deleteTask_fun(&Tasks, choice);
                break;
            case findSol:
                printf("Finding solution...\n");
                printTasks(Tasks);
                printf("\nInsert the number of the task to complete: ");
                scanf("%d", &choice);
                tmpTask = copyTask(Tasks, choice - 1);
                if (tmpTask == NULL){
                    printf("\nTask not found\n\n");
                    break;
                }
                findBestSol(Staff, tmpTask);
                break;
            case quit:
                printf("Quitting...\n");
                if (Tasks != NULL)
                    freetaskList(Tasks);
                if (tmpTask != NULL)
                    freeTask(tmpTask);
                if (Staff != NULL)
                    freeStaff(Staff);
                printf("\n");
                return 0;
                break;
        }
    }
}

void getFileName(char *name){
    printf("\nType file name (Max %d characters): ", CMAX);
    scanf("%s", name);
    printf("\n");
}

int printMenu(void){
    int x;
    printf("\n\n\n0) Read staff and skills\n1) Print staff\n2) Print tasks\n3) Read task\n4) Delete task\n5) Find solution\n6) Quit\n\n");
    scanf("%d", &x);
    printf("\n\n");
    return x;
}