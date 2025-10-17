#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include <vector>

enum ProcessState { NEW, READY, RUNNING, WAITING, TERMINATED };

class Process {
private:
    int pid;
    int arrival_time;
    int burst_time;
    int priority;
    ProcessState state;
    int remaining_time;
    int waiting_time;
    int turnaround_time;
    int memory_required;
    bool has_io;

public:
    Process(int pid, int arrival_time, int burst_time, int priority = 0, int memory = 0, bool io = false);
    
    void updateState(ProcessState newState);
    void decrementTime();
    
    // Getters
    int getPID() { return pid; }
    int getArrivalTime() { return arrival_time; }
    int getBurstTime() { return burst_time; }
    int getPriority() { return priority; }
    ProcessState getState() { return state; }
    int getRemainingTime() { return remaining_time; }
    int getWaitingTime() { return waiting_time; }
    int getTurnaroundTime() { return turnaround_time; }
    int getMemoryRequired() { return memory_required; }
    bool hasIO() { return has_io; }
    
    // Setters
    void setWaitingTime(int time) { waiting_time = time; }
    void setTurnaroundTime(int time) { turnaround_time = time; }
};

#endif
