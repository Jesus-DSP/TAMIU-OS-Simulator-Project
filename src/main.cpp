#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <limits>
#include <cstdlib> 
#include <ctime>   
#include "auth.h"
#include "process.h"
#include <queue>
#include <unordered_map>

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
    const string correctUsername = "admin";
    const string correctPassword = "password123";
    string username, password;

    cout << "Please enter your username: ";
    cin >> username;
    cout << "Please enter your password: ";
    cin >> password;

    return (username == correctUsername && password == correctPassword);
}

static void printResults(const vector<Process*>& procs, const vector<int>& completionTimes) {
    cout << "\n=== Scheduling Results ===" << endl;
    cout << "PID\tArrival\tBurst\tCompletion\tTurnaround\tWaiting" << endl;
    cout << "-----------------------------------------------------------" << endl;

    double totalTurnaround = 0;
    double totalWaiting = 0;

    for (int i = 0; i < (int)procs.size(); i++) {
        int completionTime = completionTimes[i];
        int turnaroundTime = completionTime - procs[i]->getArrivalTime();
        int waitingTime = turnaroundTime - procs[i]->getBurstTime();
        
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
    int time = 0, nextProc = 0;
    vector<Process*> readyQueue;
    Process* running = NULL;
    bool allDone = false;
    vector<int> completionTimes(fcfsProcs.size(), 0);

    sort(fcfsProcs.begin(), fcfsProcs.end(), [](Process* a, Process* b){
        if (a->getArrivalTime() != b->getArrivalTime()) return a->getArrivalTime() < b->getArrivalTime();
        return a->getPID() < b->getPID();
    });

    while (!allDone) {
        for (int i = nextProc; i < (int)fcfsProcs.size(); i++) {
            if (fcfsProcs[i]->getArrivalTime() <= time) {
                readyQueue.push_back(fcfsProcs[i]);
                fcfsProcs[i]->updateState(READY);
                nextProc++;
            } else break;
        }

        if (running == NULL && !readyQueue.empty()) {
            running = readyQueue[0];
            readyQueue.erase(readyQueue.begin());
            running->updateState(RUNNING);
        }

        if (running != NULL) {
            int executed = running->getBurstTime() - running->getRemainingTime() + 1;
            cout << "Time " << time << ": Process P" << running->getPID()
                 << " running (" << executed << "/" << running->getBurstTime() << ")" << endl;
            running->decrementTime();
            if (running->getRemainingTime() == 0) {
                running->updateState(TERMINATED);
                if (running->getPID() - 1 >= 0 && running->getPID() - 1 < (int)completionTimes.size()) {
                    completionTimes[running->getPID() - 1] = time + 1;
                }
                running = NULL;
            }
        } else {
            cout << "Time " << time << ": No process" << endl;
        }

        allDone = (nextProc >= (int)fcfsProcs.size() && running == NULL && readyQueue.empty());
        time++;
    }
    printResults(fcfsProcs, completionTimes);
}

static void runSJF(vector<Process*>& procs) {
    cout << "\n=== SJF (Non-preemptive) Scheduling ===" << endl;
    int time = 0, doneCount = 0;
    vector<int> completionTimes(procs.size(), 0);
    vector<bool> finished(procs.size(), false);

    while (doneCount < (int)procs.size()) {
        int idx = -1;
        int minBurst = numeric_limits<int>::max();

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

static void runRR(std::vector<Process*>& procs, int q) {
    if (q <= 0) { cout << "\n[RR] Invalid quantum. Using q=1.\n"; q = 1; }
    cout << "\n=== Round Robin (q=" << q << ") ===\n";

    const int n = (int)procs.size();
    vector<int> completionTimes(n, 0);
    vector<int> order(n);
    for (int i = 0; i < n; ++i) order[i] = i;
    sort(order.begin(), order.end(), [&](int a, int b) {
        if (procs[a]->getArrivalTime() != procs[b]->getArrivalTime())
            return procs[a]->getArrivalTime() < procs[b]->getArrivalTime();
        return procs[a]->getPID() < procs[b]->getPID();
    });

    queue<int> ready;
    int t = 0, i = 0;

    auto enqueue_arrivals = [&](int now) {
        while (i < n && procs[order[i]]->getArrivalTime() <= now) {
            int idx = order[i++];
            procs[idx]->updateState(READY);
            ready.push(idx);
        }
    };

    if (n > 0 && procs[order[0]]->getArrivalTime() > 0)
        t = procs[order[0]]->getArrivalTime();
    enqueue_arrivals(t);

    while (!ready.empty() || i < n) {
        if (ready.empty()) {
            t = std::max(t, procs[order[i]]->getArrivalTime());
            enqueue_arrivals(t);
            continue;
        }

        int idx = ready.front(); ready.pop();
        Process* p = procs[idx];
        p->updateState(RUNNING);

        int slice = std::min(q, p->getRemainingTime());
        for (int step = 0; step < slice; ++step) {
            cout << "Time " << t << ": Process P" << p->getPID()
                      << " running (" << (p->getBurstTime() - (p->getRemainingTime()-1))
                      << "/" << p->getBurstTime() << ")\n";
            p->decrementTime();
            t += 1;
            enqueue_arrivals(t);
            if (p->getRemainingTime() == 0) break;
        }

        if (p->getRemainingTime() == 0) {
            p->updateState(TERMINATED);
            completionTimes[idx] = t;
        } else {
            p->updateState(READY);
            ready.push(idx);
        }
    }
    printResults(procs, completionTimes);
}

static void runVirtualMemory() {
    struct VProc { int pid, pgs; vector<int> pt; };
    vector<int> mem(100, 0); // 0 = free, otherwise PID
    vector<VProc> vps;

    cout << "\n=== Virtual Memory Simulation ===\nHow many processes? ";
    int n; cin >> n;
    for (int i = 0; i < n; ++i)
        vps.push_back({ i + 1, rand() % 50 + 1, {} });

    // Display created processes
    cout << "\n--- Created Processes ---\n";
    for (auto& v : vps)
        cout << "P" << v.pid << ": " << v.pgs << " pages\n";

    while (true) {
        cout << "\n1. Run PID, 2. Run Random, 3. Delete Process, 4. Back to Main Menu: ";
        int c; cin >> c;
        if (c == 4) break;

        // ---- New Option: Delete Process Manually ----
        if (c == 3) {
            int id; 
            cout << "Enter PID to delete: ";
            cin >> id;

            VProc* d = nullptr;
            for (auto& vp : vps)
                if (vp.pid == id) d = &vp;

            if (!d) {
                cout << "PID not found.\n";
            } else if (d->pt.empty()) {
                cout << "P" << id << " is not currently in memory.\n";
            } else {
                cout << "Freeing P" << id << endl;
                for (int f : d->pt) mem[f] = 0;
                d->pt.clear();

                // Show updated process list
                cout << "\n--- Current Processes ---\n";
                for (auto& v : vps) {
                    cout << "P" << v.pid << ": " << v.pgs << " pages ";
                    if (v.pt.empty()) {
                        cout << "[NOT IN MEMORY]\n";
                    } else {
                        cout << "[IN MEMORY] Frames: ";
                        for (size_t j = 0; j < v.pt.size(); ++j) {
                            cout << v.pt[j];
                            if (j + 1 < v.pt.size()) cout << ", ";
                        }
                        cout << endl;
                    }
                }
            }
            continue; // go back to menu
        }

        // ---- Run PID / Run Random ----
        VProc* p = nullptr;
        if (c == 2 && !vps.empty()) {
            p = &vps[rand() % vps.size()];
        } else if (c == 1) {
            int id; cout << "Enter PID: "; cin >> id;
            for (auto& vp : vps)
                if (vp.pid == id) p = &vp;
        } else {
            cout << "Invalid option.\n";
            continue;
        }

        if (!p) {
            cout << "Invalid PID.\n";
            continue;
        }
        if (!p->pt.empty()) {
            cout << "P" << p->pid << " already in memory.\n";
            continue;
        }

        // Allocation with retry loop
        while (true) {
            int free_f = 0;
            for (int m : mem) if (m == 0) free_f++;

            if (free_f >= p->pgs) {
                int start = -1;
                // Continuous Check
                for (int i = 0; i <= 100 - p->pgs; ++i) {
                    bool fit = true;
                    for (int k = 0; k < p->pgs; k++)
                        if (mem[i + k] != 0) { fit = false; break; }
                    if (fit) { start = i; break; }
                }

                cout << "Allocating P" << p->pid << " (" << p->pgs << " pgs)... ";
                if (start != -1) {
                    cout << "Continuous at " << start << endl;
                    for (int k = 0; k < p->pgs; k++) {
                        mem[start + k] = p->pid;
                        p->pt.push_back(start + k);   // store frame index
                    }
                } else {
                    cout << "Scattered." << endl;
                    for (int i = 0; i < 100 && (int)p->pt.size() < p->pgs; ++i) {
                        if (mem[i] == 0) {
                            mem[i] = p->pid;
                            p->pt.push_back(i);       // store frame index
                        }
                    }
                }

                // Show page table for just-allocated process
                cout << "\nPage table for P" << p->pid << ":\n";
                for (size_t i = 0; i < p->pt.size(); ++i) {
                    cout << "  Page " << i << " -> Frame " << p->pt[i] << endl;
                }

                // Display process list with their frames
                cout << "\n--- Current Processes ---\n";
                for (auto& v : vps) {
                    cout << "P" << v.pid << ": " << v.pgs << " pages ";
                    if (v.pt.empty()) {
                        cout << "[NOT IN MEMORY]\n";
                    } else {
                        cout << "[IN MEMORY] Frames: ";
                        for (size_t j = 0; j < v.pt.size(); ++j) {
                            cout << v.pt[j];
                            if (j + 1 < v.pt.size()) cout << ", ";
                        }
                        cout << endl;
                    }
                }

                break; // Success, exit retry loop
            }

            cout << "Not enough memory (" << free_f << " free, " << p->pgs << " needed).\n";
            cout << "Delete Process (1. PID, 2. Random): ";
            int dc; cin >> dc;
            VProc* d = nullptr;

            if (dc == 2) {
                vector<VProc*> l;
                for (auto& v : vps)
                    if (!v.pt.empty()) l.push_back(&v);
                if (!l.empty()) d = l[rand() % l.size()];
            } else if (dc == 1) {
                int k; cout << "Kill PID: "; cin >> k;
                for (auto& v : vps)
                    if (v.pid == k) d = &v;
            } else {
                cout << "Invalid choice.\n";
            }

            if (d && !d->pt.empty()) {
                cout << "Freeing P" << d->pid << endl;
                for (int f : d->pt) mem[f] = 0;
                d->pt.clear(); // retry allocation after freeing
            } else if (!d || d->pt.empty()) {
                cout << "Invalid target.\n";
                break;
            }
        }
    }
}

int main() {
    srand(time(0));
    cout << "\nOS is booting up..." << endl;
    cout << "\nWelcome to the OS command line interface." << endl;
    cout << "------------------------------------------" << endl << endl;

    if (authenticateUser()) {
        cout << "\nAuthentication successful. Welcome, admin!" << endl;

        while (true) {
            cout << "\n=== Main Menu ===\n";
            cout << "1. CPU Scheduling\n";
            cout << "2. Virtual Memory Simulation\n";
            cout << "3. Exit System\n";
            cout << "Select option: ";
            
            int mainChoice;
            if (!(cin >> mainChoice)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input.\n";
                continue;
            }

            if (mainChoice == 3) break;
            
            if (mainChoice == 2) {
                runVirtualMemory();
            } else if (mainChoice == 1) {
                // CPU Scheduling Sub-menu
                while (true) {
                    cout << "\n   --- CPU Scheduling Methods ---\n";
                    cout << "   1. First Come First Serve (FCFS)\n";
                    cout << "   2. Shortest Job First (SJF)\n";
                    cout << "   3. Round Robin (RR)\n";
                    cout << "   4. Back to Main Menu\n";
                    cout << "   Select method: ";
                    
                    int cpuChoice;
                    cin >> cpuChoice;
                    
                    if (cpuChoice == 4) break;
                    
                    if (cpuChoice >= 1 && cpuChoice <= 3) {
                        // Load data specifically for the chosen algorithm run
                        vector<Process*> procs = readInputOrDemo();
                        
                        if (cpuChoice == 1) {
                            runFCFS(procs);
                        } else if (cpuChoice == 2) {
                            runSJF(procs);
                        } else { // cpuChoice == 3
                            cout << "Enter time quantum q: ";
                            int q;
                            while (!(cin >> q)) {
                                cin.clear();
                                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                cout << "Invalid. Enter integer: ";
                            }
                            runRR(procs, q);
                        }
                        
                        // Cleanup processes after run
                        for (auto* p : procs) delete p;
                    } else {
                        cout << "Invalid CPU option.\n";
                    }
                }
            } else {
                cout << "Invalid option.\n";
            }
        }

    } else {
        cout << "\nAuthentication failed. Invalid username or password." << endl;
    }

    cout << "\nOS is shutting down." << endl;
    return 0;
}
