# Process Scheduling Test Cases Documentation

This document explains the test cases implemented in the OS Simulator that demonstrate process creation, state changes, and execution across three CPU scheduling algorithms.

## Overview

The simulator creates process instances and demonstrates their lifecycle through various states (NEW, READY, RUNNING, TERMINATED) while executing three different scheduling strategies: FCFS, SJF, and Round Robin.

## Process Class Implementation

### Constructor
The Process class initializes all process attributes:

```cpp
Process::Process(int pid, int arrival_time, int burst_time, int priority, int memory, bool io) {
    this->pid = pid;
    this->arrival_time = arrival_time;
    this->burst_time = burst_time;
    this->priority = priority;
    this->state = NEW;
    this->remaining_time = burst_time;
    this->waiting_time = 0;
    this->turnaround_time = 0;
    this->memory_required = memory;
    this->has_io = io;
}
```

### State Management
Processes can transition between states using the updateState method:

```cpp
void Process::updateState(ProcessState newState) {
    state = newState;
}
```

### Execution Control
The decrementTime method reduces remaining execution time:

```cpp
void Process::decrementTime() {
    if (remaining_time > 0) {
        remaining_time--;
    }
}
```

## Test Case 1: First-Come, First-Served (FCFS)

### Process Creation
Seven processes are created with varying arrival times and burst times:

```cpp
Process* p1 = new Process(1, 2, 3);   // PID=1, Arrival=2, Burst=3
Process* p2 = new Process(2, 4, 2);   // PID=2, Arrival=4, Burst=2
Process* p3 = new Process(3, 5, 1);   // PID=3, Arrival=5, Burst=1
Process* p4 = new Process(4, 7, 4);   // PID=4, Arrival=7, Burst=4
Process* p5 = new Process(5, 9, 2);   // PID=5, Arrival=9, Burst=2
Process* p6 = new Process(6, 15, 6);  // PID=6, Arrival=15, Burst=6
Process* p7 = new Process(7, 16, 8);  // PID=7, Arrival=16, Burst=8
```

### State Transitions Demonstrated
The FCFS algorithm demonstrates the following state changes:

```cpp
// NEW to READY when process arrives
fcfsProcs[i]->updateState(READY);

// READY to RUNNING when scheduled
running->updateState(RUNNING);

// RUNNING to TERMINATED when complete
running->updateState(TERMINATED);
```

### Execution Cycle
Each time unit, the scheduler checks for arriving processes and executes:

```cpp
// Add processes to ready queue when they arrive
if (fcfsProcs[i]->getArrivalTime() <= time) {
    readyQueue.push_back(fcfsProcs[i]);
    fcfsProcs[i]->updateState(READY);
}

// Execute current process
if (running != NULL) {
    int executed = running->getBurstTime() - remainingBurst + 1;
    cout << "Time " << time << ": Process P" << running->getPID() 
         << " running (" << executed << "/" << running->getBurstTime() << ")" << endl;
    remainingBurst--;
}
```

## Test Case 2: Shortest-Job-First (SJF)

### Process Creation
Seven processes identical to FCFS are created:

```cpp
Process* s1 = new Process(1, 2, 3);
Process* s2 = new Process(2, 4, 2);
Process* s3 = new Process(3, 5, 1);
Process* s4 = new Process(4, 7, 4);
Process* s5 = new Process(5, 9, 2);
Process* s6 = new Process(6, 15, 6);
Process* s7 = new Process(7, 16, 8);
```

### Ready Queue Management
SJF maintains processes in sorted order by burst time:

```cpp
// Insert in sorted order by burst time
int insertPos = readyQueue.size();
for (int j = 0; j < readyQueue.size(); j++) {
    if (sjfProcs[i]->getBurstTime() < readyQueue[j]->getBurstTime()) {
        insertPos = j;
        break;
    }
}
readyQueue.insert(readyQueue.begin() + insertPos, sjfProcs[i]);
```

### State Changes
SJF demonstrates the same state transitions as FCFS but selects processes based on shortest burst time first.

## Test Case 3: Round Robin (RR)

### Process Creation
Seven processes with quantum time slice of 2:

```cpp
Process* rr1 = new Process(1, 2, 3);
Process* rr2 = new Process(2, 4, 2);
Process* rr3 = new Process(3, 5, 1);
Process* rr4 = new Process(4, 7, 4);
Process* rr5 = new Process(5, 9, 2);
Process* rr6 = new Process(6, 15, 6);
Process* rr7 = new Process(7, 16, 8);
```

### Preemptive Scheduling
Round Robin adds an additional state transition for preemption:

```cpp
// Execute and track time slice
running->decrementTime();
timeSlice++;

// Process complete
if (running->getRemainingTime() == 0) {
    running->updateState(TERMINATED);
    running = NULL;
}
// Quantum expired - preempt
else if (timeSlice >= quantum) {
    running->updateState(READY);
    readyQueue.push_back(running);
    running = NULL;
}
```

### State Transition Flow
Round Robin demonstrates:
- NEW to READY (arrival)
- READY to RUNNING (scheduled)
- RUNNING to READY (preempted after quantum expires)
- RUNNING to TERMINATED (completed)

## Time-Cycle Execution

All three algorithms run cycle-by-cycle from time 0, demonstrating:

### Idle Cycles
When no process has arrived:
```cpp
cout << "Time " << time << ": No process" << endl;
```

### Active Execution
When a process is running:
```cpp
int executed = running->getBurstTime() - running->getRemainingTime() + 1;
cout << "Time " << time << ": Process P" << running->getPID() 
     << " running (" << executed << "/" << running->getBurstTime() << ")" << endl;
```

## Summary

The test cases comprehensively demonstrate:

1. Process instance creation with constructor
2. State transitions using updateState method
3. Execution tracking using decrementTime method
4. Getter methods for process attributes
5. Three different scheduling algorithms
6. Complete process lifecycle from NEW to TERMINATED
7. Preemptive scheduling (Round Robin)
8. Non-preemptive scheduling (FCFS, SJF)
9. Cycle-by-cycle execution output
10. Ready queue management

Each test case creates 7 processes, totaling 21 process instances across all algorithms, providing extensive demonstration of the Process class functionality and CPU scheduling behavior.

