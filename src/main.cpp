#include <iostream>
#include "auth.h" // Include the authentication header

using namespace std;

int main() {
    // 1. Simulate the boot up process
    cout << "SimOS is booting up..." << endl;
    cout << "Welcome to the SimOS command line interface." << endl;
    cout << "------------------------------------------" << endl << endl;

    // 2. Call the authentication function
    bool isAuthenticated = authenticateUser();

    // 3. Display success or failure message based on authentication result
    if (isAuthenticated) {
        cout << "\nAuthentication successful. Welcome, admin!" << endl;
    } else {
        cout << "\nAuthentication failed. Invalid username or password." << endl;
    }

    cout << "\nSimOS is shutting down." << endl;

    return 0;
}