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
static void readProcessInfo(char* name, int arrival, int burst, FILE* fileIn);

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

    // Read in time units
    int timeUnits = readTimeUnits(fileIn);
    printf("timeUnits = %d \n", timeUnits);

    // Read in scheduler type
    char* schedulerType = readSchedulerType(fileIn);
    printf("schedulerType = %s\n", schedulerType);
    
    // Read in time quantum
    int timeQuantum = readTimeQuantum(fileIn);
    printf("timeQuantum = %d \n", timeQuantum);

    // Read each process info
    for (int i=0; i<processCount; i++) {
        char* name;
        int arrival;
        int burst;
        // TODO: Need to make this return a Process struct
        // and save it to the array of processes of size processCount
        readProcessInfo(name, arrival, burst, fileIn);

        if (strcmp(schedulerType, "fcfs") == 0) {
            // run first come first serve schedule
        } else if (strcmp(schedulerType, "sjf") == 0) {
            // run shortest job first schedule
        } else if (strcmp(schedulerType, "rr") == 0) {
            // run round robin schedule
        }
    }

    fclose(fileIn);

    // Wait for user input
    getchar();

    // Write out solution to fileOut then close the file
    fclose(fileOut);

    return 0;
}

static int readProcessCount(FILE* fileIn) {
    char* line;
    size_t len = 32;

    // Allocate space for our buffer
    line = (char *)malloc(len * sizeof(char));

    // Read in the line
    getline(&line, &len, fileIn);

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

    // Allocate space for our buffer
    line = (char *)malloc(len * sizeof(char));

    // Read in the line
    getline(&line, &len, fileIn);

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

    // Allocate space for our buffer
    line = (char *)malloc(len * sizeof(char));

    // Read in the line
    getline(&line, &len, fileIn);

    // read the first string in the line
    char* str = strtok(line, " ");

    // Ignore the first string
    str = strtok(NULL, " ");

    return str;
}

static int readTimeQuantum(FILE* fileIn) {
    char* line;
    size_t len = 32;

    // Allocate space for our buffer
    line = (char *)malloc(len * sizeof(char));

    // Read in the line
    getline(&line, &len, fileIn);

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

static void readProcessInfo(char* name, int arrival, int burst, FILE* fileIn) {
    char* line;
    size_t len = 32;

    // Read in process name
    line = (char *)malloc(len * sizeof(char));

    // Read in the line
    getline(&line, &len, fileIn);

    // Read in the first two strings in the line
    char* str = strtok(line, " ");
    str = strtok(NULL, " ");
    str = strtok(NULL, " ");

    name = str;
    printf("Name: %s\n", name);

    // Read in the process arrival
    str = strtok(NULL, " ");
    str = strtok(NULL, " ");

    arrival = atoi(str);
    printf("Arrival: %d\n", arrival);

    // Read in the process burst
    str = strtok(NULL, " ");
    str = strtok(NULL, " ");

    burst = atoi(str);
    printf("Burst: %d\n", burst);

}