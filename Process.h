#define TRUE 1
#define FALSE 0
#ifndef PROCESS_H
#define PROCESS_H


typedef struct Process {
    int waiting;
    char* name;
    int arrival;
    int burst;
    int wait;
    int turnaround;
} PROCESS;

#endif