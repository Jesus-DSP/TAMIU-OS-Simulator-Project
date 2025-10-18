#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "auth.h"
#include "process.h"

using namespace std;

// Constructor
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

    // Prompt the user for user and pass
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

    // 3. Display success or failure
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
            }
            
            // Execute current time cycle
            if (running != NULL) {
                int executed = running->getBurstTime() - running->getRemainingTime() + 1;
                cout << "Time " << time << ": Process P" << running->getPID() << " running (" << executed << "/" << running->getBurstTime() << ")" << endl;
                running->decrementTime();  // Using the Process class method
                if (running->getRemainingTime() == 0) {
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
    } else {
        cout << "\nAuthentication failed. Invalid username or password." << endl;
    }

    cout << "\nOS is shutting down." << endl;

    return 0;
}