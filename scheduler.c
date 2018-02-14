#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Process {
    char* name;
    int arrival;
    int burst;
};

static int readProcessCount(FILE* fileIn);
static int readTimeUnits(FILE* fileIn);
static char* readSchedulerType(FILE* fileIn);
static int readTimeQuantum(FILE* fileIn);
static void readProcessInfo(char* name[], int* arrival, int* burst, FILE* fileIn);

int main() {

    FILE* fileIn;
    FILE* fileOut;
    size_t read;

    fileIn = fopen("processes.in", "r");
    if (fileIn == NULL) {
        printf("Cannot open file processes.in!\n");
        exit(0);
    }

    fileOut = fopen("processes.out", "w");
    if (fileIn == NULL) {
        printf("Cannot open file processes.out!\n");
        exit(0);
    }

    printf("Readin in file...\n");

    char* line = NULL;
    size_t len = 0;
    // while ((read = getline(&line, &len, fileIn)) != -1) {
    //     printf("%s", line);
    //     //sscanf(line, "%d", )
    // }
    // getchar();
    // return 0;

    /*
    *
    * Read in the variables
    *
    */

    // Eaxmple:

    // processcout 2   # Read 5 processes
    // runfor 15       # Run for 15 time units
    // use rr          # Can be fcfs, sjf, or rr
    // quantum 2       # Time quantum - only if using rr
    // process name P1 arrival 3 burst 5
    // process name P2 arrival 0 burst 9

    // Read in process count
    int processCount = readProcessCount(fileIn);
    printf("processCount = %d \n", processCount);

    int timeUnits = readTimeUnits(fileIn);
    printf("timeUnits = %d \n", timeUnits);

    char* schedulerType = readSchedulerType(fileIn);
    printf("schedulerType = %s\n", schedulerType);
    
    int timeQuantum = readTimeQuantum(fileIn);
    printf("timeQuantum = %d \n", timeQuantum);

    //printf("read line 1");
    getchar();


    printf("END");
    getchar();

    return 0;
}

static int readProcessCount(FILE* fileIn) {
    char* line;
    size_t len = 32;
    size_t read;

    // Allocate space for our buffer
    line = (char *)malloc(len * sizeof(char));

    // Read in the line
    read = getline(&line, &len, fileIn);

    // Extract the processCount from the line
    int processCount;

    // read the first string in the line
    char* str = strtok(line, " ");

    // Ignore the first string
    str = strtok(NULL, " ");

    // Convert the string to an int
    processCount = atoi(str);

    return processCount;
}

static int readTimeUnits(FILE* fileIn) {
    char* line;
    size_t len = 32;
    size_t read;

    // Allocate space for our buffer
    line = (char *)malloc(len * sizeof(char));

    // Read in the line
    read = getline(&line, &len, fileIn);

    // Extract the processCount from the line
    int timeUnits;

    // read the first string in the line
    char* str = strtok(line, " ");

    // Ignore the first string
    str = strtok(NULL, " ");

    // Convert the string to an int
    timeUnits = atoi(str);

    return timeUnits;
}

static char* readSchedulerType(FILE* fileIn) {
    char* line;
    size_t len = 5;
    size_t read;

    // Allocate space for our buffer
    line = (char *)malloc(len * sizeof(char));

    // Read in the line
    read = getline(&line, &len, fileIn);

    // read the first string in the line
    char* str = strtok(line, " ");

    // Ignore the first string
    str = strtok(NULL, " ");

    return str;
}

static int readTimeQuantum(FILE* fileIn) {
    char* line;
    size_t len = 32;
    size_t read;

    // Allocate space for our buffer
    line = (char *)malloc(len * sizeof(char));

    // Read in the line
    read = getline(&line, &len, fileIn);

    // Extract the processCount from the line
    int timeQuantum;

    // read the first string in the line
    char* str = strtok(line, " ");

    // Ignore the first string
    str = strtok(NULL, " ");

    // Convert the string to an int
    timeQuantum = atoi(str);

    return timeQuantum;
}

static void readProcessInfo(char* name[], int* arrival, int* burst, FILE* fileIn) {
    // TODO
}