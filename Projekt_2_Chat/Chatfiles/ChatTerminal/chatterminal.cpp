#include "chatterminalf.h"
#include "chatterminalback.h"
#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <chrono>

std::mutex chatMutex; // Mutex for synchronizing access to chat history

void receiveMessages(int userId, int otherUserId, std::vector<Message>& chatHistory, bool& chatting) {
    std::vector<Message> lastDisplayedMessages; // Track the last displayed messages

    while (chatting) {
        try {
            std::this_thread::sleep_for(std::chrono::milliseconds(700)); // Check for new messages every 2 seconds

            // Fetch the latest messages
            std::vector<Message> newMessages = getChatHistory(userId, otherUserId, 30);

            // Lock the mutex to update the shared chat history
            std::lock_guard<std::mutex> lock(chatMutex);

            // Check if there are new messages
            if (newMessages == lastDisplayedMessages) {
                // No new messages, skip updating the terminal
                continue;
            }

            // Update the chat history and last displayed messages
            chatHistory = newMessages;
            lastDisplayedMessages = newMessages;

            // Save the current input
            std::cout << "\033[s"; // Save cursor position

            // Clear the chat history area
            std::cout << "\033[H"; // Move to the top of the terminal
            std::cout << "\033[J"; // Clear from cursor to the end of the screen

            // Display the chat history (newest messages at the bottom)
            for (auto it = chatHistory.rbegin(); it != chatHistory.rend(); ++it) { // Reverse iteration
                const auto& msg = *it;
                if (msg.senderId == userId) {
                    std::cout << "\033[1;32m"; // Green for user's messages
                    std::cout << "You: ";
                } else {
                    std::cout << "\033[1;33m"; // Yellow for other's messages
                    std::cout << "Other: ";
                }
                std::cout << msg.content << " [" << msg.timestamp << "]\n";
                std::cout << "\033[0m"; // Reset color
            }

            // Restore the input area
            std::cout << "\033[u"; // Restore cursor position
            std::cout.flush();

        } catch (const std::exception& e) {
            std::cerr << "Error fetching messages: " << e.what() << std::endl;
        }
    }
}

void sendMessages(int userId, int otherUserId, bool& chatting) {
    std::string message;
    while (chatting) {
        std::getline(std::cin, message); // Get user input

        if (message == "/exit") {
            chatting = false; // Stop the chat
        } else if (!message.empty()) {
            // Send the message
            if (sendMessage(userId, otherUserId, message)) {
                //std::cout << "\033[1;32mMessage sent!\033[0m\n";
            } else {
                std::cout << "\033[1;31mFailed to send message.\033[0m\n";
            }
        }
    }
}

int showChatTerminal(int userId, int otherUserId, const std::string& userName) {
    std::vector<Message> chatHistory; // Shared chat history
    bool chatting = true; // Flag to control the chat loop

    // Start the threads
    std::thread receiverThread(receiveMessages, userId, otherUserId, std::ref(chatHistory), std::ref(chatting));
    std::thread senderThread(sendMessages, userId, otherUserId, std::ref(chatting));

    // Wait for both threads to finish
    senderThread.join();
    chatting = false; // Ensure the receiver thread stops
    receiverThread.join();

    return 0;
}