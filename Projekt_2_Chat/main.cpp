#include <iostream>
//#include <thread>
#include <vector>
//#include <mutex>
//#include <condition_variable>
#include <chrono>
#include <iomanip>
#include "Chatfiles/Register/register.h" // Updated include path

using namespace std;

int main(){
    int choice;
    cout<<"\033[2J\033[?25l"; // Clear the screen and hide the cursor
    cout << "\033[1;34m"; // Set text color to blue

    cout << "1. Login\n2. Register\n3. Exit\n";
    cout<<"What operation you wish to perform?\n";
    cin >> choice;
    switch(choice){
        case 1:
            cout << "Login selected.\n";
            // Add login functionality here
            break;
        case 2:
            cout << "Register selected.\n";
            showRegisterScreen(); // Call the register frontend function
            break;
        case 3:
            cout << "Exiting.\n";
            cout << "\033[0m\033[?25h"; // Reset color and show cursor
            break;
        default:
            cout << "Invalid choice. Please select 1, 2, or 3.\n";
            break;
    }
    return 0;
}