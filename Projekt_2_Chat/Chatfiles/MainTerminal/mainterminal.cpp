#include "mainterminal.h"
#include "mainterminalback.h"
#include <iostream>
#include <string>
#include "../ChatTerminal/chatterminal.h" 

int showMainTerminal(const std::string& userName, const std::string& userId) {
    int choice = 0;
    std::string otherUserName, otherUserId;
    
    while (choice != 3) {
        std::cout << "\033[2J"; // Clear screen
        std::cout << "\033[1;36m"; // Cyan color
        std::cout << "===== MAIN MENU =====\n";
        std::cout << "Logged in as: " << userName << " (ID: " << userId << ")\n\n";
        
        std::cout << "1. Start Private Chat\n";
        std::cout << "2. Join Chat Room (not implemented yet)\n";
        std::cout << "3. Logout\n\n";
        
        std::cout << "Select an option: ";
        std::cin >> choice;
        
        switch (choice) {
            case 1:
                // Private chat mode
                std::cout << "Enter Nickname of other User: ";
                std::cin >> otherUserName;
                std::cout << "Enter Other User ID ";
                std::cin >> otherUserId;
                
                {
                    
                    // Get actual user ID from database using nickname and user_name_id
                    int otherUserIdInt = pickUserChat(otherUserName, otherUserId);
                    
                    if (otherUserIdInt > 0) {
                        if (otherUserIdInt == std::stoi(userId)) {
                            std::cout << "\033[1;31m"; // Red color
                            std::cout << "You cannot chat with yourself.\n";
                            std::cout << "\033[0m"; // Reset color
                        } else {
                            std::cout << "\033[1;32m"; // Green color
                            std::cout << "Starting chat with " << otherUserName << "...\n";
                            std::cout << "\033[0m"; // Reset color
                            
                            // Format the complete username (name#id) for display
                            std::string fullOtherUsername = otherUserName + "#" + otherUserId;
                            
                            // Forward to chat terminal
                            //showChatTerminal(std::stoi(userId), otherUserIdInt, userName, fullOtherUsername);
                        }
                    } else {
                        std::cout << "\033[1;31m"; // Red color
                        std::cout << "User not found. Check the nickname and user_name_id.\n";
                        std::cout << "\033[0m"; // Reset color
                    }
                    
                    // Wait for user to press enter
                    std::cout << "\nPress Enter to continue...";
                    std::cin.ignore();
                    std::cin.get();
                }
                break;
                
            case 2:
                std::cout << "\033[1;33m"; // Yellow color
                std::cout << "Chat rooms not implemented yet.\n";
                std::cout << "\033[0m"; // Reset color
                
                // Wait for user to press enter
                std::cout << "\nPress Enter to continue...";
                std::cin.ignore();
                std::cin.get();
                break;
                
            case 3:
                std::cout << "Logging out...\n";
                break;
                
            default:
                std::cout << "\033[1;31m"; // Red color
                std::cout << "Invalid option. Please try again.\n";
                std::cout << "\033[0m"; // Reset color
                
                // Wait for user to press enter
                std::cout << "\nPress Enter to continue...";
                std::cin.ignore();
                std::cin.get();
                break;
        }
    }
    
    return 0;
}