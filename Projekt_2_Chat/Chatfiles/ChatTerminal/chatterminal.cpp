#include "chatterminal.h"
#include "chatterminalback.h"
#include <iostream>
#include <string>
#include <vector>

int showChatTerminal(int userId, int otherUserId, const std::string& userName, const std::string& otherUserName) {
    std::string message;
    bool chatting = true;
    
    while (chatting) {
        std::cout << "\033[2J"; // Clear screen
        std::cout << "\033[1;36m"; // Cyan color
        std::cout << "===== CHAT WITH " << otherUserName << " =====\n\n";
        std::cout << "\033[0m"; // Reset color
        
        // Get chat history
        std::vector<Message> history = getChatHistory(userId, otherUserId);
        
        // Display chat history
        for (const auto& msg : history) {
            if (msg.senderId == userId) {
                std::cout << "\033[1;32m"; // Green for user's messages
                std::cout << "You: ";
            } else {
                std::cout << "\033[1;33m"; // Yellow for other's messages
                std::cout << otherUserName << ": ";
            }
            std::cout << msg.content << " [" << msg.timestamp << "]\n";
            std::cout << "\033[0m"; // Reset color
        }
        
        std::cout << "\n----------------------------------------\n";
        std::cout << "Enter your message (or type '/exit' to return to main menu):\n";
        
        std::cin.ignore(); // Clear input buffer
        std::getline(std::cin, message);
        
        if (message == "/exit") {
            chatting = false;
        } else if (!message.empty()) {
            // Send the message
            if (sendMessage(userId, otherUserId, message)) {
                std::cout << "\033[1;32m"; // Green color
                std::cout << "Message sent!\n";
                std::cout << "\033[0m"; // Reset color
            } else {
                std::cout << "\033[1;31m"; // Red color
                std::cout << "Failed to send message.\n";
                std::cout << "\033[0m"; // Reset color
            }
        }
    }
    
    return 0;
}