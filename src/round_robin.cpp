#include <bits/stdc++.h>
using namespace std;

struct Process {
    string pid;
    int arrival, burst;
    int remaining;
    int first_start = -1;
    int completion = -1;
};

struct Slice { string pid; int start, end; };

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // ---- SAMPLE INPUT ----
    // N q
    // pid arrival burst
    // (Replace with your project’s input loader)
    int n, q;
    if (!(cin >> n >> q)) {
        cerr << "Usage: provide n q then n lines: <pid> <arrival> <burst>\n";
        return 1;
    }
    vector<Process> procs(n);
    for (int i = 0; i < n; ++i) {
        cin >> procs[i].pid >> procs[i].arrival >> procs[i].burst;
        procs[i].remaining = procs[i].burst;
    }

    // sort by arrival, stable keeps declared order for ties
    stable_sort(procs.begin(), procs.end(),
        [](const Process& a, const Process& b) {
            if (a.arrival != b.arrival) return a.arrival < b.arrival;
            return a.pid < b.pid;
        });

    queue<int> ready; // store indices
    int t = 0, i = 0;
    vector<Slice> gantt;

    auto enqueue_arrivals = [&](int now) {
        while (i < n && procs[i].arrival <= now) {
            ready.push(i++);
        }
    };

    enqueue_arrivals(t);

    while (!ready.empty() || i < n) {
        if (ready.empty()) {
            // jump to next arrival
            t = max(t, procs[i].arrival);
            enqueue_arrivals(t);
            continue;
        }

        int idx = ready.front(); ready.pop();
        auto &p = procs[idx];

        if (p.first_start == -1) p.first_start = t;

        int run = min(q, p.remaining);
        int start_t = t;

        // step time to catch mid-quantum arrivals
        for (int step = 0; step < run; ++step) {
            t += 1;
            p.remaining -= 1;
            enqueue_arrivals(t);
            if (p.remaining == 0) {
                p.completion = t;
                break;
            }
        }

        gantt.push_back({p.pid, start_t, t});

        if (p.remaining > 0) {
            ready.push(idx);
        }
    }

    // compute metrics
    cout << "\nGantt (start->end):\n";
    for (auto &s : gantt) {
        cout << "[" << s.start << "-" << s.end << "] " << s.pid << "\n";
    }

    cout << "\nPID  Arr  Burst  Comp  Turn  Wait  Resp\n";
    cout << fixed;
    double sumT=0, sumW=0, sumR=0;
    for (auto &p : procs) {
        int turn = p.completion - p.arrival;
        int wait = turn - p.burst;
        int resp = p.first_start - p.arrival;
        sumT += turn; sumW += wait; sumR += resp;
        cout << setw(4) << p.pid << "  "
             << setw(4) << p.arrival << "  "
             << setw(5) << p.burst << "  "
             << setw(4) << p.completion << "  "
             << setw(4) << turn << "  "
             << setw(4) << wait << "  "
             << setw(4) << resp << "\n";
    }
    cout << "\nAvg Turnaround: " << (sumT / n)
         << "\nAvg Waiting:    " << (sumW / n)
         << "\nAvg Response:   " << (sumR / n) << "\n";

    // Optional: throughput = n / (last CT - first arrival)
    int lastCT = 0, firstArr = INT_MAX;
    for (auto &p : procs){ lastCT = max(lastCT, p.completion); firstArr = min(firstArr, p.arrival); }
    double throughput = (double)n / (lastCT - firstArr);
    cout << "Throughput:     " << throughput << " proc/unit-time\n";
    return 0;
}
