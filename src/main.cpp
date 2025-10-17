#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "auth.h" // Include the authentication header
#include "process.h"

using namespace std;

// Process class implementation
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

void Process::updateState(ProcessState newState) {
    state = newState;
}

void Process::decrementTime() {
    if (remaining_time > 0) {
        remaining_time--;
    }
}

// Function Definition
bool authenticateUser() {
    // Admin credentials
    const string correctUsername = "admin";
    const string correctPassword = "password123";

    // User input
    string username;
    string password;

    // Prompt the user for credentials
    cout << "Please enter your username: ";
    cin >> username;
    cout << "Please enter your password: ";
    cin >> password;

    // Verify the input against the credential values
    if (username == correctUsername && password == correctPassword) {
        return true; // Authentication successful
    } else {
        return false; // Authentication failed
    }
}

int main() {
    // 1. Simulate the boot up process
    cout << "\nOS is booting up..." << endl;
    cout << "\nWelcome to the OS command line interface." << endl;
    cout << "------------------------------------------" << endl << endl;

    // 2. Call the authentication function
    bool isAuthenticated = authenticateUser();

    // 3. Display success or failure message based on authentication result
    if (isAuthenticated) {
        cout << "\nAuthentication successful. Welcome, admin!" << endl;
        
        // FCFS Scheduling
        cout << "\n=== FCFS Scheduling ===" << endl;
        Process* p1 = new Process(1, 2, 3);
        Process* p2 = new Process(2, 4, 2);
        Process* p3 = new Process(3, 5, 1);
        Process* p4 = new Process(4, 7, 4);
        Process* p5 = new Process(5, 9, 2);
        Process* p6 = new Process(6, 15, 6);
        Process* p7 = new Process(7, 16, 8);
        vector<Process*> fcfsProcs;
        fcfsProcs.push_back(p1);
        fcfsProcs.push_back(p2);
        fcfsProcs.push_back(p3);
        fcfsProcs.push_back(p4);
        fcfsProcs.push_back(p5);
        fcfsProcs.push_back(p6);
        fcfsProcs.push_back(p7);
        
        cout << "Process Information:" << endl;
        for (int i = 0; i < fcfsProcs.size(); i++) {
            cout << "P" << fcfsProcs[i]->getPID() 
                 << " - Arrival: " << fcfsProcs[i]->getArrivalTime() 
                 << ", Burst: " << fcfsProcs[i]->getBurstTime() << endl;
        }
        
        int time = 0;
        vector<Process*> readyQueue;
        int nextProc = 0;
        Process* running = NULL;
        int remainingBurst = 0;
        bool allDone = false;
        
        while (!allDone) {
            // Add processes that have arrived to ready queue
            for (int i = nextProc; i < fcfsProcs.size(); i++) {
                if (fcfsProcs[i]->getArrivalTime() <= time) {
                    readyQueue.push_back(fcfsProcs[i]);
                    fcfsProcs[i]->updateState(READY);
                    nextProc++;
                } else {
                    break;
                }
            }
            
            // If no running process, get from ready queue
            if (running == NULL && readyQueue.size() > 0) {
                running = readyQueue[0];
                readyQueue.erase(readyQueue.begin());
                running->updateState(RUNNING);
                remainingBurst = running->getBurstTime();
            }
            
            // Execute current time cycle
            if (running != NULL) {
                int executed = running->getBurstTime() - remainingBurst + 1;
                cout << "Time " << time << ": Process P" << running->getPID() << " running (" << executed << "/" << running->getBurstTime() << ")" << endl;
                remainingBurst--;
                if (remainingBurst == 0) {
                    running->updateState(TERMINATED);
                    running = NULL;
                }
            } else {
                cout << "Time " << time << ": No process" << endl;
            }
            
            // Check if all done
            allDone = (nextProc >= fcfsProcs.size() && running == NULL && readyQueue.size() == 0);
            time++;
        }
        
        delete p1;
        delete p2;
        delete p3;
        delete p4;
        delete p5;
        delete p6;
        delete p7;
        
        // SJF Scheduling
        cout << "\n=== SJF Scheduling ===" << endl;
        Process* s1 = new Process(1, 2, 3);
        Process* s2 = new Process(2, 4, 2);
        Process* s3 = new Process(3, 5, 1);
        Process* s4 = new Process(4, 7, 4);
        Process* s5 = new Process(5, 9, 2);
        Process* s6 = new Process(6, 15, 6);
        Process* s7 = new Process(7, 16, 8);
        vector<Process*> sjfProcs;
        sjfProcs.push_back(s1);
        sjfProcs.push_back(s2);
        sjfProcs.push_back(s3);
        sjfProcs.push_back(s4);
        sjfProcs.push_back(s5);
        sjfProcs.push_back(s6);
        sjfProcs.push_back(s7);
        
        cout << "Process Information:" << endl;
        for (int i = 0; i < sjfProcs.size(); i++) {
            cout << "P" << sjfProcs[i]->getPID() 
                 << " - Arrival: " << sjfProcs[i]->getArrivalTime() 
                 << ", Burst: " << sjfProcs[i]->getBurstTime() << endl;
        }
        
        time = 0;
        readyQueue.clear();
        nextProc = 0;
        running = NULL;
        remainingBurst = 0;
        allDone = false;
        
        while (!allDone) {
            // Add processes that have arrived to ready queue
            for (int i = nextProc; i < sjfProcs.size(); i++) {
                if (sjfProcs[i]->getArrivalTime() <= time) {
                    sjfProcs[i]->updateState(READY);
                    // Insert in sorted order by burst time
                    int insertPos = readyQueue.size();
                    for (int j = 0; j < readyQueue.size(); j++) {
                        if (sjfProcs[i]->getBurstTime() < readyQueue[j]->getBurstTime()) {
                            insertPos = j;
                            break;
                        }
                    }
                    readyQueue.insert(readyQueue.begin() + insertPos, sjfProcs[i]);
                    nextProc++;
                } else {
                    break;
                }
            }
            
            // If no running process, get from ready queue
            if (running == NULL && readyQueue.size() > 0) {
                running = readyQueue[0];
                readyQueue.erase(readyQueue.begin());
                running->updateState(RUNNING);
                remainingBurst = running->getBurstTime();
            }
            
            // Execute current time cycle
            if (running != NULL) {
                int executed = running->getBurstTime() - remainingBurst + 1;
                cout << "Time " << time << ": Process P" << running->getPID() << " running (" << executed << "/" << running->getBurstTime() << ")" << endl;
                remainingBurst--;
                if (remainingBurst == 0) {
                    running->updateState(TERMINATED);
                    running = NULL;
                }
            } else {
                cout << "Time " << time << ": No process" << endl;
            }
            
            // Check if all done
            allDone = (nextProc >= sjfProcs.size() && running == NULL && readyQueue.size() == 0);
            time++;
        }
        
        delete s1;
        delete s2;
        delete s3;
        delete s4;
        delete s5;
        delete s6;
        delete s7;
        
        // Round Robin Scheduling
        cout << "\n=== Round Robin (quantum=2) ===" << endl;
        Process* rr1 = new Process(1, 2, 3);
        Process* rr2 = new Process(2, 4, 2);
        Process* rr3 = new Process(3, 5, 1);
        Process* rr4 = new Process(4, 7, 4);
        Process* rr5 = new Process(5, 9, 2);
        Process* rr6 = new Process(6, 15, 6);
        Process* rr7 = new Process(7, 16, 8);
        vector<Process*> rrProcs;
        rrProcs.push_back(rr1);
        rrProcs.push_back(rr2);
        rrProcs.push_back(rr3);
        rrProcs.push_back(rr4);
        rrProcs.push_back(rr5);
        rrProcs.push_back(rr6);
        rrProcs.push_back(rr7);
        
        cout << "Process Information:" << endl;
        for (int i = 0; i < rrProcs.size(); i++) {
            cout << "P" << rrProcs[i]->getPID() 
                 << " - Arrival: " << rrProcs[i]->getArrivalTime() 
                 << ", Burst: " << rrProcs[i]->getBurstTime() << endl;
        }
        
        time = 0;
        readyQueue.clear();
        nextProc = 0;
        running = NULL;
        int quantum = 2;
        int timeSlice = 0;
        allDone = false;
        
        while (!allDone) {
            // Add processes that have arrived to ready queue
            for (int i = nextProc; i < rrProcs.size(); i++) {
                if (rrProcs[i]->getArrivalTime() <= time) {
                    readyQueue.push_back(rrProcs[i]);
                    rrProcs[i]->updateState(READY);
                    nextProc++;
                } else {
                    break;
                }
            }
            
            // If no running process, get from ready queue
            if (running == NULL && readyQueue.size() > 0) {
                running = readyQueue[0];
                readyQueue.erase(readyQueue.begin());
                running->updateState(RUNNING);
                timeSlice = 0;
            }
            
            // Execute current time cycle
            if (running != NULL) {
                int executed = running->getBurstTime() - running->getRemainingTime() + 1;
                cout << "Time " << time << ": Process P" << running->getPID() << " running (" << executed << "/" << running->getBurstTime() << ")" << endl;
                running->decrementTime();
                timeSlice++;
                
                // Check if process done or quantum expired
                if (running->getRemainingTime() == 0) {
                    running->updateState(TERMINATED);
                    running = NULL;
                    timeSlice = 0;
                } else if (timeSlice >= quantum) {
                    running->updateState(READY);
                    readyQueue.push_back(running);
                    running = NULL;
                    timeSlice = 0;
                }
            } else {
                cout << "Time " << time << ": No process" << endl;
            }
            
            // Check if all done
            allDone = (nextProc >= rrProcs.size() && running == NULL && readyQueue.size() == 0);
            time++;
        }
        
        delete rr1;
        delete rr2;
        delete rr3;
        delete rr4;
        delete rr5;
        delete rr6;
        delete rr7;
    } else {
        cout << "\nAuthentication failed. Invalid username or password." << endl;
    }

    cout << "\nOS is shutting down." << endl;

    return 0;
}