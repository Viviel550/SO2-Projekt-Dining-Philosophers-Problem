#include "register.h"
#include "registerback.h"
#include <iostream>
#include <limits>
#include <string>

void showRegisterScreen() {
    std::string username, password, confirmPassword, email;
    
    std::cout << "\033[2J"; // Clear screen
    std::cout << "\033[1;36m"; // Cyan color
    std::cout << "===== USER REGISTRATION =====\n\n";
    
    std::cout << "Enter username: ";
    std::cin >> username;
    std::cout << "Enter email: ";
    std::cin >> email;
    // Check if username already exists
    if (checkEmailExists(email)) {
        std::cout << "\033[1;31m"; // Red color
        std::cout << "Username already exists. Please choose another one.\n";
        std::cout << "\033[0m"; // Reset color
        return;
    }
    
    std::cout << "Enter password: ";
    std::cin >> password;
    
    std::cout << "Confirm password: ";
    std::cin >> confirmPassword;
    
    if (password != confirmPassword) {
        std::cout << "\033[1;31m"; // Red color
        std::cout << "Passwords do not match. Registration failed.\n";
        std::cout << "\033[0m"; // Reset color
        return;
    }
    
    // Call the backend function to register user
    if (registerUser(username, password, email)) {
        std::cout << "\033[1;32m"; // Green color
        std::cout << "Registration successful!\n";
        std::cout << "\033[0m"; // Reset color
    } else {
        std::cout << "\033[1;31m"; // Red color
        std::cout << "Registration failed. Please try again later.\n";
        std::cout << "\033[0m"; // Reset color
    }
    
    // Wait for user to press enter
    std::cout << "\nPress Enter to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}