#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Process.h"
#include "Queue.h"

static int readProcessCount(FILE* fileIn);
static int readTimeUnits(FILE* fileIn);
static char* readSchedulerType(FILE* fileIn);
static int readTimeQuantum(FILE* fileIn);
static PROCESS* readProcessInfo(FILE* fileIn);
static void runFirstComeFirstServed(PROCESS** process, int processCount, int timeUnits);
static void runShortestJobFirst(PROCESS** process, int processCount, int runFor);

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
    size_t processCount = readProcessCount(fileIn);

    // Read in time units
    int timeUnits = readTimeUnits(fileIn);
    // printf("timeUnits = %d \n", timeUnits);

    // Read in scheduler type
    char* schedulerType = readSchedulerType(fileIn);

    // Read in time quantum
    int timeQuantum = readTimeQuantum(fileIn);
    // printf("timeQuantum = %d \n", timeQuantum);

    // Create our array of processes
    PROCESS** processes = malloc(processCount * sizeof(PROCESS*));

    //printf("malloced\n");

    // Read each process info
    int i;
    for (i=0; i<processCount; i++) {
        // Read in the process info
        PROCESS* proc = readProcessInfo(fileIn);

        // Assign proc to proccess array
        processes[i] = proc;
        // printf("Proc values: %s %d %d\n", processes[i]->name, processes[i]->arrival, processes[i]->burst);
    }

    // Run the schedule typ
    if (strcmp(schedulerType, "fcfs") == 0) {
        // run first come first serve schedule
        runFirstComeFirstServed(processes, processCount, timeUnits);
    } else if (strcmp(schedulerType, "sjf") == 0) {

        // run shortest job first schedule
        runShortestJobFirst(processes, processCount, timeUnits);
    } else if (strcmp(schedulerType, "rr") == 0) {
        // run round robin schedule
    } else {
        printf("No valid scheduler type: %s\n", schedulerType);
    }

    // Deallocate our memory
    free(processes);

    fclose(fileIn);

    // Write out solution to fileOut then close the file
    fclose(fileOut);

    // Wait for user input
    printf("Press any key to continue...\n");
    getchar();

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

static PROCESS* readProcessInfo(FILE* fileIn) {
    char* line;
    char* name;
    int arrival;
    int burst;
    size_t len = 32;

    // Read in process name
    line = (char *)malloc(len * sizeof(char));

    // Read in the line
    getline(&line, &len, fileIn);

    // Read in the first two strings in the line
    char* str = strtok(line, " ");
    str = strtok(NULL, " ");
    str = strtok(NULL, " ");

    // Assign value to variable
    name = str;

    // Read in the process arrival
    str = strtok(NULL, " ");
    str = strtok(NULL, " ");

    // Assign value to variable
    arrival = atoi(str);

    // Read in the process burst
    str = strtok(NULL, " ");
    str = strtok(NULL, " ");

    // Assign value to variable
    burst = atoi(str);

    // Return the processs
    PROCESS* proc = malloc(sizeof(PROCESS));
    proc->name = name;
    proc->arrival = arrival;
    proc->burst = burst;
    proc->wait = 0;
    proc->turnaround = 0;

    return proc;
}

static void runFirstComeFirstServed(PROCESS** process, int processCount, int timeUnits) {

    PROCESS* tempStruct;
    int i, j, result, time=0, flag=1;
    int arrival[processCount], burst[processCount], waiting[processCount], turnAround[processCount], next[processCount];

    // Initial the arrays for calculating waiting and turnaround time
    for(i=0; i<processCount; i++)
    {
      next[i]=0;
      waiting[i]=0;
      arrival[i]=process[i]->arrival;
      burst[i]=process[i]->burst;
    }

    // Sorting the process struct to ascending order
    for(i=0; i<processCount; ++i)
    {
      for(j=i+1; j<processCount; ++j)
      {
        if(process[i]->arrival > process[j]->arrival)
        {
          tempStruct=process[i];
          process[i]=process[j];
          process[j]=tempStruct;
        }

        // If there are more than one processes having the same arrival time, check burst time
        else if(process[i]->arrival == process[j]->arrival)
        {
          if(process[i]->burst>process[j]->burst)
          {
            tempStruct=process[i];
            process[i]=process[j];
            process[j]=tempStruct;
          }
        }
      }
    }

    // Calculating waiting time
    for(i=0; i<processCount; ++i)
    {
      result=0;
        for(j=0; j<i; ++j)
          result+=burst[j];
      waiting[i]=result-arrival[i];
    }

    // Calculating turnaround time
    for(i=0; i<processCount; ++i)
      turnAround[i]=burst[i]+waiting[i];

    printf("%d processes\n", processCount);
    printf("Using First-Come First-Served\n\n\n");

    next[0]=process[0]->arrival+process[0]->burst;

    // Calculating complete time for each processes
    for(i=1; i<processCount; i++)
      for(j=i-1; j<i; j++)
        next[i]+=next[j]+process[i]->burst;


    while(time <= timeUnits)
    {
      // First process in
      if(time==process[0]->arrival)
      {
        printf("Time %d: %s arrived\n", time, process[0]->name);
        printf("Time %d: %s selected (burst %d)\n", time, process[0]->name, process[0]->burst);
      }
      // Rest of the processes arrival
      for(i=1; i<processCount; i++)
      {
        if(time == process[i]->arrival)
          printf("Time %d: %s arrived\n", time, process[i]->name);
      }
      // when a process finished, another one will be selected
      for(i=0; i<processCount; i++)
      {
        if(time==next[i])
        {
          printf("Time %d: %s finished\n", time, process[i]->name);
          if(i<processCount-1)
          {
            printf("Time %d: %s selected (burst %d)\n", time, process[i+1]->name, process[i+1]->burst);
          }
        }
      }

      // When there is no process
      if(time<process[0]->arrival || time>next[processCount-1])
      {
        printf("Time %d: Idle\n", time);
      }
      // Just a timer
      time++;
    };

    // waiing and turnaround info
    printf("Finished at time %d\n\n", timeUnits);
    for(i=0; i<processCount; ++i)
    {
      printf("%s wait %d turnaround %d\n", process[i]->name, waiting[i], turnAround[i]);
    }
}

static void runShortestJobFirst(PROCESS** process, int processCount, int runFor) {
    printf("%d processes\n", processCount);
    printf("Using Shortest Job First\n");
    printf("\n");
    struct Queue* queue = createQueue(processCount);

    int cont = 0;
    int time = 0;
    int Idle = 0;

    PROCESS* currentProcess = NULL;

    while(time <= runFor){
        // for each elemnt of our processes list
        int i;
        for (i=0; i<processCount; i++) {
            if (process[i]->arrival == time) {
                printf("Time %d: %s arrived\n", time, process[i]->name);

                // If no process is running, make this process the current process
                if (currentProcess == NULL) {
                    currentProcess = process[i];
                    printf("Time %d: %s selected (burst %d)\n", time, currentProcess->name, currentProcess->burst);
                } else {
                    // printf("compare %s with %s\n", process[i].name, currentProcess->name);
                    // printf("%d < %d", process[i].burst, currentProcess->burst);
                    if (process[i]->burst < currentProcess->burst) {
                        // TODO: queue currentProcess
                        enqueue(queue, currentProcess);
                        currentProcess = process[i];
                        printf("Time %d: %s selected (burst %d)\n", time, currentProcess->name, currentProcess->burst);
                    }
                }
            }
            if (process[i]->arrival < time && process[i] != currentProcess && process[i]->burst > 0) {
                process[i]->wait++;
            }

        }

        if (currentProcess != NULL) {
            currentProcess->burst--;
            if (currentProcess->burst == 0) {
                currentProcess->turnaround = time - currentProcess->arrival + 1;
                printf("Time %d: %s finished\n", time, currentProcess->name);
                // TODO: dequeue the next process
                int empty = isEmpty(queue);
                if (empty == FALSE) {
                    PROCESS* next = dequeue(queue);
                    currentProcess = next;
                    printf("Time %d: %s selected (burst %d)\n", time, currentProcess->name, currentProcess->burst);
                } else {
                    printf("Time %d: Idle\n", time);
                }
            }
        }

        if (time == runFor) {
            printf("Finished at time %d\n", time);
        }
        time++;
    };

    printf("\n");

    int i;
    for (i=0; i<processCount; i++) {
        printf("%s wait %d turnaround %d\n", process[i]->name, process[i]->wait, process[i]->turnaround);
    }
}
