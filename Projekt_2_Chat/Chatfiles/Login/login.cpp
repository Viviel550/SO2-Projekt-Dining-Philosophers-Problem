#include "login.h"
#include "loginback.h"
#include <iostream>
#include <string>
#include "../MainTerminal/mainterminal.h"

void showLoginScreen() {
    std::string email, password;
    
    std::cout << "\033[2J"; // Clear screen
    std::cout << "\033[1;36m"; // Cyan color
    std::cout << "===== USER LOGIN =====\n\n";
    
    std::cout << "Enter email: ";
    std::cin >> email;
    
    std::cout << "Enter password: ";
    std::cin >> password;
    
    // Call the backend function to authenticate user
    if (authenticateUser(email, password)) {
        std::cout << "\033[1;32m"; // Green color
        std::cout << "Login successful!\n";
        std::cout << "\033[0m"; // Reset color
        UserData userID = getUserIdByEmail(email);
        // Here you would typically set up the user session or navigate to the chat screen
        if (userID.UserId > 0){
            showMainTerminal(email, std::to_string(userID.userNameId),userID.userName, userID.UserId);
        } else {
            std::cout << "\033[1;31m"; // Red color
            std::cout << "Failed to retrieve user ID. Please try again later.\n";
            std::cout << "\033[0m"; // Reset color
        }
        
    } else {
        std::cout << "\033[1;31m"; // Red color
        std::cout << "Login failed. Invalid email or password.\n";
        std::cout << "\033[0m"; // Reset color
    }
    
    // Wait for user to press enter
    std::cout << "\nPress Enter to continue...";
    std::cin.ignore();
    std::cin.get();
}