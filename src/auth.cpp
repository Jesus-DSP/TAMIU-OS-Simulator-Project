#include "auth.h"
#include <iostream>
#include <string>

using namespace std;

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