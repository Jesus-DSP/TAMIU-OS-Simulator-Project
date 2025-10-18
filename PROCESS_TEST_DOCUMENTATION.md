# Process Scheduling Test Cases Documentation

This document explains the test cases implemented in the OS Simulator that demonstrate process creation, state changes, and execution with First Come First Served Scheduling Algorithm.

## Overview

The simulator creates processes and shows state changes (NEW, READY, RUNNING, TERMINATED) with FCFS algorithm.

## Process Class Implementation

### Class Overview
The Process class represents a process in our operating system simulator. It had essential attributes and methods of a process, including:

#### Attributes
- `pid`: Process identifier
- `arrival_time`: Time when the process enters the system
- `burst_time`: Total CPU time required by the process
- `priority`: Process priority level (default: 0)
- `state`: Current state of the process (NEW, READY, RUNNING, TERMINATED)
- `remaining_time`: Time left for completion
- `waiting_time`: Total time spent waiting
- `turnaround_time`: Total time from arrival to completion
- `memory_required`: Amount of memory needed by the process
- `has_io`: Flag indicating if process requires I/O operations

#### Methods
1. **State Management**
   - `updateState(ProcessState newState)`: Updates the state
   - `getState()`: Returns current process state

2. **Time Management**
   - `decrementTime()`: Reduces remaining execution time
   - `getRemainingTime()`: Returns remaining execution time
   - `getWaitingTime()`: Returns total waiting time
   - `getTurnaroundTime()`: Returns total turnaround time
   - `setWaitingTime(int time)`: Sets the waiting time
   - `setTurnaroundTime(int time)`: Sets the turnaround time

3. **Process Information**
   - `getPID()`: Returns process ID
   - `getArrivalTime()`: Returns arrival time
   - `getBurstTime()`: Returns total CPU burst time
   - `getPriority()`: Returns process priority
   - `getMemoryRequired()`: Returns required memory
   - `hasIO()`: Returns whether process has I/O operations

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
The Process class manages its own execution time through internal methods. The `decrementTime()` method reduces the remaining execution time, while `getRemainingTime()` allows checking the current progress:

```cpp
// Reduces remaining execution time
void Process::decrementTime() {
    if (remaining_time > 0) {
        remaining_time--;
    }
}

// Returns current remaining time
int Process::getRemainingTime() { 
    return remaining_time; 
}
```

This encapsulation ensures that time management is handled consistently within the Process class itself, rather than being managed externally. The scheduler uses these methods to:
1. Check process completion (`getRemainingTime() == 0`)
2. Track execution progress (`getBurstTime() - getRemainingTime()`)
3. Update remaining time (`decrementTime()`)

## Test Case 1: First Come First Served (FCFS)

### Process Creation
Seven processes are created with differnt arrival times and burst times:

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
    int executed = running->getBurstTime() - running->getRemainingTime() + 1;
    cout << "Time " << time << ": Process P" << running->getPID() 
         << " running (" << executed << "/" << running->getBurstTime() << ")" << endl;
    running->decrementTime();
}
```

## Summary

The test cases comprehensively demonstrate:

1. Process instance creation with constructor
2. State transitions using updateState method
3. Execution using decrementTime method
4. Test case with First come, First Served 
5. State changes from NEW to READY to TERMINATED, also WAITING

## Program Output

Below is the complete output from running the OS Simulator:

```
OS is booting up...

Welcome to the OS command line interface.
------------------------------------------

Please enter your username: admin
Please enter your password: password123

Authentication successful. Welcome, admin!

=== FCFS Scheduling ===
Process Information:
P1 - Arrival: 2, Burst: 3
P2 - Arrival: 4, Burst: 2
P3 - Arrival: 5, Burst: 1
P4 - Arrival: 7, Burst: 4
P5 - Arrival: 9, Burst: 2
P6 - Arrival: 15, Burst: 6
P7 - Arrival: 16, Burst: 8
Time 0: No process
Time 1: No process
Time 2: Process P1 running (1/3)
Time 3: Process P1 running (2/3)
Time 4: Process P1 running (3/3)
Time 5: Process P2 running (1/2)
Time 6: Process P2 running (2/2)
Time 7: Process P3 running (1/1)
Time 8: Process P4 running (1/4)
Time 9: Process P4 running (2/4)
Time 10: Process P4 running (3/4)
Time 11: Process P4 running (4/4)
Time 12: Process P5 running (1/2)
Time 13: Process P5 running (2/2)
Time 14: No process
Time 15: Process P6 running (1/6)
Time 16: Process P6 running (2/6)
Time 17: Process P6 running (3/6)
Time 18: Process P6 running (4/6)
Time 19: Process P6 running (5/6)
Time 20: Process P6 running (6/6)
Time 21: Process P7 running (1/8)
Time 22: Process P7 running (2/8)
Time 23: Process P7 running (3/8)
Time 24: Process P7 running (4/8)
Time 25: Process P7 running (5/8)
Time 26: Process P7 running (6/8)
Time 27: Process P7 running (7/8)
Time 28: Process P7 running (8/8)


OS is shutting down.
```

