#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <limits>
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

static void printResults(const vector<Process*>& procs, const vector<int>& completionTimes) {
    cout << "\n=== Scheduling Results ===" << endl;
    cout << "PID\tArrival\tBurst\tCompletion\tTurnaround\tWaiting" << endl;
    cout << "-----------------------------------------------------------" << endl;

    double totalTurnaround = 0;
    double totalWaiting = 0;

    for (int i = 0; i < (int)procs.size(); i++) {
        int completionTime = completionTimes[i];
        int turnaroundTime = completionTime - procs[i]->getArrivalTime();   // TAT = CT - AT
        int waitingTime = turnaroundTime - procs[i]->getBurstTime();        // WT  = TAT - BT
        // keep your original setters to store metrics
        procs[i]->setTurnaroundTime(turnaroundTime);
        procs[i]->setWaitingTime(waitingTime);

        totalTurnaround += turnaroundTime;
        totalWaiting += waitingTime;

        cout << "P" << procs[i]->getPID() << "\t"
             << procs[i]->getArrivalTime() << "\t"
             << procs[i]->getBurstTime() << "\t"
             << completionTime << "\t\t"
             << turnaroundTime << "\t\t"
             << waitingTime << endl;
    }

    cout << "-----------------------------------------------------------" << endl;
    cout << "Average Turnaround Time: " << (procs.empty() ? 0.0 : totalTurnaround / procs.size()) << endl;
    cout << "Average Waiting Time: " << (procs.empty() ? 0.0 : totalWaiting / procs.size()) << endl;
}

static vector<Process*> readInputOrDemo() {
    cout << "\nEnter number of processes (0 to use built-in demo): ";
    int n; cin >> n;
    vector<Process*> procs;

    if (n == 0) {
        // Demo data keeping your original values
        procs.push_back(new Process(1, 2, 3));
        procs.push_back(new Process(2, 4, 2));
        procs.push_back(new Process(3, 5, 1));
        procs.push_back(new Process(4, 7, 4));
        procs.push_back(new Process(5, 9, 2));
        procs.push_back(new Process(6, 15, 6));
        procs.push_back(new Process(7, 16, 8));
        cout << "Using demo processes." << endl;
    } else {
        cout << "Enter: (pid arrival burst) on each line for each process :" << endl;
        for (int i = 0; i < n; ++i) {
            int pid, at, bt; cin >> pid >> at >> bt;
            procs.push_back(new Process(pid, at, bt));
        }
    }

    cout << "Process Information:" << endl;
    for (int i = 0; i < (int)procs.size(); i++) {
        cout << "P" << procs[i]->getPID()
             << " - Arrival: " << procs[i]->getArrivalTime()
             << ", Burst: " << procs[i]->getBurstTime() << endl;
    }
    return procs;
}

static void runFCFS(vector<Process*>& fcfsProcs) {
    cout << "\n=== FCFS Scheduling ===" << endl;

    int time = 0;
    vector<Process*> readyQueue;
    int nextProc = 0;
    Process* running = NULL;
    bool allDone = false;
    vector<int> completionTimes(fcfsProcs.size(), 0);

    // Sort by arrival then PID to ensure deterministic intake
    sort(fcfsProcs.begin(), fcfsProcs.end(), [](Process* a, Process* b){
        if (a->getArrivalTime() != b->getArrivalTime()) return a->getArrivalTime() < b->getArrivalTime();
        return a->getPID() < b->getPID();
    });

    while (!allDone) {
        // Add processes that have arrived to ready queue
        for (int i = nextProc; i < (int)fcfsProcs.size(); i++) {
            if (fcfsProcs[i]->getArrivalTime() <= time) {
                readyQueue.push_back(fcfsProcs[i]);
                fcfsProcs[i]->updateState(READY);
                nextProc++;
            } else {
                break;
            }
        }

        // If no running process, get from ready queue
        if (running == NULL && !readyQueue.empty()) {
            running = readyQueue[0];
            readyQueue.erase(readyQueue.begin());
            running->updateState(RUNNING);
        }

        // Execute current time cycle
        if (running != NULL) {
            int executed = running->getBurstTime() - running->getRemainingTime() + 1;
            cout << "Time " << time << ": Process P" << running->getPID()
                 << " running (" << executed << "/" << running->getBurstTime() << ")" << endl;
            running->decrementTime();  // Using the Process class method
            if (running->getRemainingTime() == 0) {
                running->updateState(TERMINATED);
                // Store completion time (index by pid-1 as your original did)
                if (running->getPID() - 1 >= 0 && running->getPID() - 1 < (int)completionTimes.size()) {
                    completionTimes[running->getPID() - 1] = time + 1;
                }
                running = NULL;
            }
        } else {
            cout << "Time " << time << ": No process" << endl;
        }

        // Check if all done
        allDone = (nextProc >= (int)fcfsProcs.size() && running == NULL && readyQueue.empty());
        time++;
    }

    // Print results using same table/averages semantics
    printResults(fcfsProcs, completionTimes);
}

static void runSJF(vector<Process*>& procs) {
    cout << "\n=== SJF (Non-preemptive) Scheduling ===" << endl;

    int time = 0;
    vector<int> completionTimes(procs.size(), 0);
    vector<bool> finished(procs.size(), false);
    int doneCount = 0;

    while (doneCount < (int)procs.size()) {
        int idx = -1;
        int minBurst = INT_MAX;

        for (int i = 0; i < (int)procs.size(); i++) {
            if (!finished[i] && procs[i]->getArrivalTime() <= time) {
                if (procs[i]->getBurstTime() < minBurst) {
                    minBurst = procs[i]->getBurstTime();
                    idx = i;
                }
            }
        }

        if (idx == -1) {
            cout << "Time " << time << ": No process" << endl;
            time++;
            continue;
        }

        Process* p = procs[idx];
        p->updateState(RUNNING);
        for (int t = 0; t < p->getBurstTime(); t++) {
            cout << "Time " << time << ": Process P" << p->getPID()
                 << " running (" << t + 1 << "/" << p->getBurstTime() << ")" << endl;
            time++;
        }

        p->updateState(TERMINATED);
        finished[idx] = true;
        completionTimes[idx] = time;
        doneCount++;
    }

    printResults(procs, completionTimes);
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

        
        vector<Process*> procs = readInputOrDemo();

        //choose scheduling policy
        cout << "\nChoose scheduling policy: 1 = First Come First Serve, 2 = Shortest Job First (Non-preemptive): ";
        int choice = 1; cin >> choice;

        if (choice == 1) {
            runFCFS(procs);
        } else if (choice == 2) {
            runSJF(procs);
        } else {
            cout << "Invalid choice. Defaulting to FCFS." << endl;
            runFCFS(procs);
        }

        // Cleanup
        for (auto* p : procs) delete p;

    } else {
        cout << "\nAuthentication failed. Invalid username or password." << endl;
    }

    cout << "\nOS is shutting down." << endl;

    return 0;
}
