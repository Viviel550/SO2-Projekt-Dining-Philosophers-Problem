#include "mainterminal.h"
#include "mainterminalback.h"
#include <iostream>
#include <string>
#include <limits>
#include "../ChatTerminal/chatterminalf.h" 

static int PrivateChat(const std::string& userNameId, const int userId, const std::string& userName) {
    std::string otherUserName, otherUserId;
    std::cout << "Enter Nickname of other User: ";
    std::cin >> otherUserName;
    std::cout << "Enter Other User ID ";
    std::cin >> otherUserId;
    if (otherUserName == userName && otherUserId == userNameId) {
        std::cout << "\033[1;31m"; // Red color
        std::cout << "You cannot chat with yourself.\n";
        std::cout << "\033[0m"; // Reset color
        return -1;
    } 
    // Get actual user ID from database using nickname and user_name_id
    int otherUserIdInt = pickUserChat(otherUserName, otherUserId);
    std::cout << "User ID: " << otherUserIdInt << "\n"; 
    if (otherUserIdInt > 0) {
                std::cout << "\033[1;32m"; // Green color
                std::cout << "Starting chat with " << otherUserName << "...\n";
                std::cout << "\033[0m"; // Reset color
                
                // Format the complete username (name#id) for display            
                // Forward to chat terminal
                showChatTerminal(userId, otherUserIdInt, userName);
            
        } else {
            std::cout << "\033[1;31m"; // Red color
            std::cout << "User not found. Check the Nickname and UserId\n";
            std::cout << "\033[0m"; // Reset color
        }
        
        // Wait for user to press enter
        std::cout << "\nPress Enter to continue...";
        std::cin.ignore();
        std::cin.get();
    return 0;
}

static int JoinChatRoom() {
    std::cout << "\033[1;33m"; // Yellow color
    std::cout << "Chat rooms not implemented yet.\n";
    std::cout << "\033[0m"; // Reset color
    
    // Wait for user to press enter
    std::cout << "\nPress Enter to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
    return 0;
}

int showMainTerminal(const std::string& userEmail, const std::string& userNameId,const std::string& Name, int userId) {
    int choice = 0;
    while (choice != 3) {
        std::cout << "\033[2J"; // Clear screen
        std::cout << "\033[1;36m"; // Cyan color
        std::cout << "===== MAIN MENU =====\n";
        std::cout << "Logged in as: " << Name << "#" << userNameId << ")\n\n";
        
        std::cout << "1. Start Private Chat\n";
        std::cout << "2. Join Chat Room (not implemented yet)\n";
        std::cout << "3. Logout\n4. Friends List (not implemented yet)\n";
        std::cout << "5. Exit\n";
        
        std::cout << "Select an option: ";
        std::cin >> choice;
        
        switch (choice) {
            case 1:
                PrivateChat(userNameId, userId, Name);
                break;
                
            case 2:
                JoinChatRoom();
                break;
                
            case 3:
                std::cout << "Logging out...\n";
                break;
            case 4:
                break;
            case 5:
                std::cout << "Exiting...\n";
                std::cout << "\033[0m"; // Reset color
                std::exit(0);
                break;
            default:
                std::cout << "\033[1;31m"; // Red color
                std::cout << "Invalid option. Please try again.\n";
                std::cout << "\033[0m"; // Reset color
                
                // Wait for user to press enter
                std::cout << "\nPress Enter to continue...";
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cin.get();
                break;
        }
    }
    
    return 0;
}