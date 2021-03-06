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
static void runRoundRobin(PROCESS **processes, int quantum, int processCount);
static FILE* fileOut;
int main() {

    FILE* fileIn;

    fileIn = fopen("processes.in", "r");
    if (fileIn == NULL) {
        fprintf(fileOut,"Cannot open file processes.in!\n");
        exit(0);
    }

    fileOut = fopen("processes.out", "w");
    if (fileIn == NULL) {
        fprintf(fileOut,"Cannot open file processes.out!\n");
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
    // fprintf(fileOut,"timeUnits = %d \n", timeUnits);

    // Read in scheduler type
    char* schedulerType = readSchedulerType(fileIn);

    // Read in time quantum
    int timeQuantum = readTimeQuantum(fileIn);
    // fprintf(fileOut,"timeQuantum = %d \n", timeQuantum);

    // Create our array of processes
    PROCESS** processes = malloc(processCount * sizeof(PROCESS*));

    //fprintf(fileOut,"malloced\n");

    // Read each process info
    int i;
    for (i=0; i<processCount; i++) {
        // Read in the process info
        PROCESS* proc = readProcessInfo(fileIn);

        // Assign proc to proccess array
        processes[i] = proc;
        // fprintf(fileOut,"Proc values: %s %d %d\n", processes[i]->name, processes[i]->arrival, processes[i]->burst);
    }

    // Run the schedule typ
    if (strcmp(schedulerType, "fcfs") == 0) {
        // run first come first serve schedule
        runFirstComeFirstServed(processes, processCount, timeUnits);
    } else if (strcmp(schedulerType, "sjf") == 0) {

        // run shortest job first schedule
        runShortestJobFirst(processes, processCount, timeUnits);
    } else if (strcmp(schedulerType, "rr") == 0) {
        runRoundRobin(processes, timeQuantum, processCount);
    } else {
        fprintf(fileOut,"No valid scheduler type: %s\n", schedulerType);
    }

    // Deallocate our memory
    free(processes);

    fclose(fileIn);

    // Write out solution to fileOut then close the file
    fclose(fileOut);

    // Wait for user input
    //fprintf(fileOut,"Press any key to continue...\n");
    //getchar();

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


    // printf("quantum: %s\n", str);
    // Ignore line if it starts with #
    // if (strcmp(str, "#"))
    //     return 0;

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

    int originalOrder[processCount];

    for (i = 0; i < processCount; i++)
    {
        originalOrder[i] = i;
    }
    // Initial the arrays for calculating waiting and turnaround time
    for(i=0; i<processCount; i++)
    {
        next[i]=0;
        waiting[i]=0;
        arrival[i]=process[i]->arrival;
        burst[i]=process[i]->burst;
    }

    int swap = 0;


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

                swap = originalOrder[i];
                originalOrder[i] = originalOrder[j];
                originalOrder[j] = swap;
            }

            // If there are more than one processes having the same arrival time, check burst time
            else if(process[i]->arrival == process[j]->arrival)
            {
                if(process[i]->burst>process[j]->burst)
                {
                    tempStruct=process[i];
                    process[i]=process[j];
                    process[j]=tempStruct;

                    swap = originalOrder[i];
                    originalOrder[i] = originalOrder[j];
                    originalOrder[j] = swap;
                }
            }
        }
    }


    fprintf(fileOut,"%d processes\n", processCount);
    fprintf(fileOut,"Using First Come First Served\n\n");

    next[0]=process[0]->arrival+process[0]->burst;

    // Calculating complete time for each processes
    for(i=1; i<processCount; i++)
        for(j=i-1; j<i; j++)
            next[i]+=next[j]+process[i]->burst;



    // Calculating turnAround time
    for(i=0; i<processCount; i++)
      turnAround[i] = next[i] - process[i]->arrival;

    // Calculating turnAround time
    for(i=0; i<processCount; i++)
      waiting[i] = turnAround[i] - process[i]->burst;

          
    while(time <= timeUnits)
    {
        // First process in
        if(time==process[0]->arrival)
        {
            fprintf(fileOut,"Time %d: %s arrived\n", time, process[0]->name);
            fprintf(fileOut,"Time %d: %s selected (burst %d)\n", time, process[0]->name, process[0]->burst);
        }
        // Rest of the processes arrival
        for(i=1; i<processCount; i++)
        {
            if(time == process[i]->arrival)
                fprintf(fileOut,"Time %d: %s arrived\n", time, process[i]->name);
        }
        // when a process finished, another one will be selected
        for(i=0; i<processCount; i++)
        {
            if(time==next[i])
            {
                fprintf(fileOut,"Time %d: %s finished\n", time, process[i]->name);
                if(i<processCount-1)
                {
                    fprintf(fileOut,"Time %d: %s selected (burst %d)\n", time, process[i+1]->name, process[i+1]->burst);
                }
            }
        }

        // When there is no process
        if(time<process[0]->arrival || time>next[processCount-1])
        {
            fprintf(fileOut,"Time %d: Idle\n", time);
        }
        // Just a timer
        time++;
    };

    // waiing and turnaround info
    fprintf(fileOut,"Finished at time %d\n\n", timeUnits);
    for(i=0; i<processCount; i++)
    {
        for(j = 0; j < processCount; j++)
        {
            if (i == originalOrder[j])
            {
                //process[j]->wait = process[j]->turnaround-burst[i];

                fprintf(fileOut,"%s wait %d turnaround %d\n", process[j]->name, waiting[j], turnAround[j]);
            }
        }
    }
}

static void runShortestJobFirst(PROCESS** process, int processCount, int runFor) {
    fprintf(fileOut,"%d processes\n", processCount);
    fprintf(fileOut,"Using Shortest Job First (Pre)\n");
    fprintf(fileOut,"\n");
    // struct Queue* queue = createQueue(processCount);

    int cont = 0;
    int time = 0;
    int Idle = 0;

    PROCESS* currentProcess = NULL;

    while(time <= runFor){
        int selected = FALSE;
        // for each elemnt of our processes list
        int i;

        // DEBUG
        if (currentProcess) {
            // fprintf(fileOut, "Time %d: %s - burst: %d\n", time, currentProcess->name, currentProcess->burst);
        }

        for (i=0; i<processCount; i++) {
            if (process[i]->arrival == time) {
                fprintf(fileOut,"Time %d: %s arrived\n", time, process[i]->name);

                // If no process is running, make this process the current process
                if (currentProcess == NULL) {
                    currentProcess = process[i];
                    currentProcess->waiting = FALSE;
                    fprintf(fileOut,"Time %d: %s selected (burst %d)\n", time, currentProcess->name, currentProcess->burst);
                    selected = FALSE;
                } else {
                    // fprintf(fileOut,"compare %s with %s\n", process[i].name, currentProcess->name);
                    // fprintf(fileOut,"%d < %d", process[i].burst, currentProcess->burst);
                    if (process[i]->burst < currentProcess->burst) {
                        // TODO: queue currentProcess
                        // enqueue(queue, currentProcess);
                        currentProcess->waiting = TRUE;
                        currentProcess = process[i];
                        currentProcess->waiting = FALSE;
                        fprintf(fileOut,"Time %d: %s selected (burst %d)\n", time, currentProcess->name, currentProcess->burst);
                        selected = FALSE;
                    }
                }
            } else {
                process[i]->waiting = TRUE;
            }
            if (process[i]->arrival < time && process[i] != currentProcess && process[i]->burst > 0) {
                process[i]->wait++;
            }

        }

        if (currentProcess != NULL && selected == FALSE) {
            currentProcess->burst--;
            if (currentProcess->burst == -1) {
                currentProcess->turnaround = time - currentProcess->arrival;
                fprintf(fileOut,"Time %d: %s finished\n", time, currentProcess->name);
                // Check each waiting process and pick the next shortest job
                PROCESS* nextProcess = NULL;
                for (i=0; i<processCount; i++) {
                    if (process[i]->waiting = TRUE) {
                        // If this is the only process next or if this process is shorter than the next process we picked
                        if (process[i]->burst > -1 && (nextProcess == NULL || process[i]->burst < nextProcess->burst)) {
                            nextProcess = process[i];
                        }
                    }
                }
                currentProcess = nextProcess;
                if (currentProcess != NULL) {
                    currentProcess->waiting = FALSE;
                    fprintf(fileOut,"Time %d: %s selected (burst %d)\n", time, currentProcess->name, currentProcess->burst);
                    currentProcess->burst--;
                } else {
                    fprintf(fileOut,"Time %d: IDLE\n", time);
                }

                // int empty = isEmpty(queue);
                // if (empty == FALSE) {
                //     PROCESS* next = dequeue(queue);
                //     currentProcess = next;
                //     fprintf(fileOut,"Time %d: %s selected (burst %d)\n", time, currentProcess->name, currentProcess->burst);
                // } else {
                //     fprintf(fileOut,"Time %d: IDLE\n", time);
                // }
            }
        } else if (time != runFor) {
            fprintf(fileOut,"Time %d: IDLE\n", time);
        }

        if (time == runFor) {
            fprintf(fileOut,"Finished at time %d\n", time);
        }
        time++;
    };

    fprintf(fileOut,"\n");

    int i;
    for (i=0; i<processCount; i++) {
        fprintf(fileOut,"%s wait %d turnaround %d\n", process[i]->name, process[i]->wait, process[i]->turnaround);
    }
}

static void runRoundRobin(PROCESS** processes, int quantum, int processCount)
{

    PROCESS* tempStruct;
    int swap;
    int i, j;
    //int arrival[processCount];

    int burst[processCount];

    //used to keep track of the original order to get correct final output.
    int originalOrder[processCount];

    for (i = 0; i < processCount; i++)
    {
        burst[i]=processes[i]->burst;
        originalOrder[i] = i;
    }

    // Sorting the process struct to ascending order
    for(i=0; i<processCount; ++i)
    {
        for(j=i+1; j<processCount; ++j)
        {
            if(processes[i]->arrival > processes[j]->arrival)
            {
                tempStruct=processes[i];
                processes[i]=processes[j];
                processes[j]=tempStruct;

                swap = originalOrder[i];
                originalOrder[i] = originalOrder[j];
                originalOrder[j] = swap;
            }
        }
    }

    fprintf(fileOut,"%d processes\n", processCount);
    fprintf(fileOut,"Using Round-Robin\n");
    fprintf(fileOut,"Quantum %d\n\n",quantum);

    //printProcesses(processes, processCount);

    PROCESS* currentProcess = NULL;

    //To track if a process is selected or not
    int selected = -1;

    //Keep track of if we have announced the arrival of a process or not
    int arrival_said[processCount];
    for(i = 0; i < processCount; i++)
    {
        arrival_said[processCount] = 0;
    }

    int current_quantum;
    int time;
    int next = 0;
    int running = 1;
    int finished = 0;
    for( time = 0; running == 1; time++)
    {
        //Check for processes arriving.
        for(j = 0; j < processCount; j++)
        {
            if(processes[j]->arrival == time)
            {
                fprintf(fileOut,"Time %d: %s arrived\n", time, processes[j]->name);
                arrival_said[j] = 1;
                if (selected  < 0)
                {
                    current_quantum = quantum;
                    currentProcess = processes[j];
                    fprintf(fileOut,"Time %d: %s selected (burst %d)\n", time, currentProcess->name, currentProcess->burst);
                    selected = j;
                }
            }
        }

        if( next == 1)
        {
            //Try and pick a process

            // loop around, if you find a valid one select it
            int searching = 1;
            int i = 0;
            int peek = selected+1;
            while(searching == 1)
            {
                if (peek > processCount-1)
                    peek = 0;

                //If its not finished, and it has arrived, bingo!
                if(processes[peek]->burst > 0 && arrival_said[peek] == 1)
                {
                    current_quantum = quantum;
                    currentProcess = processes[peek];
                    fprintf(fileOut,"Time %d: %s selected (burst %d)\n", time, currentProcess->name, currentProcess->burst);
                    selected = peek;
                    searching = 0;
                    next = 0;
                }

                if (i > processCount+1)
                {
                    //No processes are ready
                    selected = -1;
                    searching = 0;
                    next = 1;
                }

                peek++;
                i++;
            }
        }

        if (selected >= 0)
        {

            //If its finished, move to the next process (if that process has arrived
            if(currentProcess->burst == 0)
            {
                fprintf(fileOut,"Time %d: %s finished\n", time, currentProcess->name);
                currentProcess->turnaround = time - currentProcess->arrival;
                finished++;

                int searching = 1;
                int i = 0;
                int peek = selected+1;
                while(searching == 1)
                {
                    if (peek > processCount-1) // ghetto modulus
                        peek = 0;

                    //If its not finished, and it has arrived, bingo!
                    if(processes[peek]->burst > 0 && arrival_said[peek] == 1)
                    {
                        current_quantum = quantum;
                        currentProcess = processes[peek];
                        fprintf(fileOut,"Time %d: %s selected (burst %d)\n", time, currentProcess->name, currentProcess->burst);
                        selected = peek;
                        searching = 0;
                    }

                    if (i > processCount+1)
                    {
                        //No processes are ready
                        selected = -1;
                        searching = 0;
                    }

                    peek++;
                    i++;
                }
                // move to the next process that has arrived
                //next = 1;

            }

            //A process to run this time is valid, run it and subtract
            currentProcess->burst--;
            current_quantum--;


            if(current_quantum == 0)
            {
                next = 1;
            }

        }

        // IDLE
        if (selected == -1)
        {
            fprintf(fileOut,"Time %d: IDLE\n", time);
        }

        if(finished == processCount)
        {
            running = 0;
        }
    }

    // waiing and turnaround info
    fprintf(fileOut,"Finished at time %d\n\n", time);
    for(i=0; i<processCount; ++i)
    {
        for(j = 0; j < processCount; j++)
        {
            if (i == originalOrder[j])
            {
                processes[j]->wait = processes[j]->turnaround-burst[i];

                fprintf(fileOut,"%s wait %d turnaround %d\n", processes[j]->name, processes[j]->wait, processes[j]->turnaround);
            }
        }
    }
}
