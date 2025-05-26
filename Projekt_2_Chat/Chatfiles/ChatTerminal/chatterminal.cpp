#include "chatterminalf.h"
#include "chatterminalback.h"
#include "chatcommands.h"
#include <ncurses/ncurses.h>
#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <chrono>
#include <algorithm>
#include <chrono>
#include <ctime>

std::mutex chatMutex; // Mutex for synchronizing access to chat history
WINDOW* chatWindow = nullptr;
WINDOW* inputWindow = nullptr;
WINDOW* headerWindow = nullptr;
std::vector<Message> globalChatHistory;
int scrollPosition = 0;
bool needsRefresh = true;
int totalLoadedMessages = 0;
bool isLoadingOlderMessages = false;
bool hasMoreOlderMessages = true;

void initializeChatTerminal() {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(1);
    
    if (has_colors()) {
        start_color();
        init_pair(1, COLOR_CYAN, COLOR_BLACK);    // Header
        init_pair(2, COLOR_GREEN, COLOR_BLACK);   // User messages
        init_pair(3, COLOR_YELLOW, COLOR_BLACK);  // Other messages
        init_pair(4, COLOR_WHITE, COLOR_BLACK);   // Normal text
        init_pair(5, COLOR_RED, COLOR_BLACK);     // Error messages
        init_pair(6, COLOR_BLUE, COLOR_BLACK);    // Instructions
    }
    
    // Get terminal dimensions
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);
    
    // Create windows
    // Header window (top 3 lines)
    headerWindow = newwin(3, max_x, 0, 0);
    
    // Chat history window (middle area, scrollable)
    chatWindow = newwin(max_y - 6, max_x, 3, 0);
    scrollok(chatWindow, TRUE);
    
    // Input window (bottom 3 lines)
    inputWindow = newwin(3, max_x, max_y - 3, 0);
    
    refresh();
}

void cleanupChatTerminal() {
    if (headerWindow) delwin(headerWindow);
    if (chatWindow) delwin(chatWindow);
    if (inputWindow) delwin(inputWindow);
    endwin();
}

void drawChatHeader(const std::string& otherUserName) {
    if (!headerWindow) return;
    
    wclear(headerWindow);
    
    if (has_colors()) {
        wattron(headerWindow, COLOR_PAIR(1));
    }
    
    int max_x = getmaxx(headerWindow);
    std::string title = "=== CHAT WITH " + otherUserName + " ===";
    int title_x = (max_x - title.length()) / 2;
    
    mvwprintw(headerWindow, 0, title_x, "%s", title.c_str());
    
    if (has_colors()) {
        wattroff(headerWindow, COLOR_PAIR(1));
    }
    
    // Instructions
    if (has_colors()) {
        wattron(headerWindow, COLOR_PAIR(6));
    }
    
    std::string instructions = "Type message and press ENTER | UP/DOWN to scroll | /exit or ESC to quit";
    int instr_x = (max_x - instructions.length()) / 2;
    mvwprintw(headerWindow, 1, instr_x, "%s", instructions.c_str());
    
    if (has_colors()) {
        wattroff(headerWindow, COLOR_PAIR(6));
    }
    
    // Draw separator line
    mvwprintw(headerWindow, 2, 0, "%s", std::string(max_x, '-').c_str());
    
    wrefresh(headerWindow);
}

void drawChatHistory(int userId) {
    if (!chatWindow) return;
    
    std::lock_guard<std::mutex> lock(chatMutex);
    
    wclear(chatWindow);
    
    if (globalChatHistory.empty()) {
        if (has_colors()) {
            wattron(chatWindow, COLOR_PAIR(4));
        }
        mvwprintw(chatWindow, 0, 2, "No messages yet. Start typing to begin the conversation!");
        if (has_colors()) {
            wattroff(chatWindow, COLOR_PAIR(4));
        }
        wrefresh(chatWindow);
        return;
    }
    
    int max_y = getmaxy(chatWindow);
    int max_x = getmaxx(chatWindow);
    
    // Calculate which messages to display based on scroll position
    int totalMessages = globalChatHistory.size();
    int visibleMessages = max_y - 1; // Leave one line for padding
    
    // Ensure scroll position is within bounds
    scrollPosition = std::max(0, std::min(scrollPosition, std::max(0, totalMessages - visibleMessages)));
    
    // Check if user scrolled to the top and we need to load more messages
    if (scrollPosition >= totalMessages - visibleMessages && hasMoreOlderMessages && !isLoadingOlderMessages) {
        isLoadingOlderMessages = true;
        // This will be handled in the main loop
    }
    
    // Calculate the range of messages to display
    int endIndex = totalMessages - scrollPosition;
    int startIndex = std::max(0, endIndex - visibleMessages);
    
    // Ensure we don't go beyond bounds
    endIndex = std::min(totalMessages, endIndex);
    startIndex = std::max(0, startIndex);
    
    int line = 0;
    for (int i = startIndex; i < endIndex && line < max_y - 1; i++) {
        const Message& msg = globalChatHistory[i];
        
        std::string displayLine;
        std::string timeStr = msg.timestamp.length() >= 19 ? msg.timestamp.substr(11, 8) : msg.timestamp;
        
        if (msg.senderId == userId) {
            if (has_colors()) {
                wattron(chatWindow, COLOR_PAIR(2));
            }
            displayLine = "[" + timeStr + "] You: " + msg.content;
        } else {
            if (has_colors()) {
                wattron(chatWindow, COLOR_PAIR(3));
            }
            displayLine = "[" + timeStr + "] Other: " + msg.content;
        }
        
        // Word wrap if message is too long
        if (displayLine.length() > max_x - 2) {
            displayLine = displayLine.substr(0, max_x - 5) + "...";
        }
        
        mvwprintw(chatWindow, line, 1, "%s", displayLine.c_str());
        
        if (has_colors()) {
            if (msg.senderId == userId) {
                wattroff(chatWindow, COLOR_PAIR(2));
            } else {
                wattroff(chatWindow, COLOR_PAIR(3));
            }
        }
        
        line++;
    }
    
    // Show loading indicator if loading older messages
    if (isLoadingOlderMessages) {
        if (has_colors()) {
            wattron(chatWindow, COLOR_PAIR(6));
        }
        mvwprintw(chatWindow, max_y - 1, max_x / 2 - 10, "Loading older messages...");
        if (has_colors()) {
            wattroff(chatWindow, COLOR_PAIR(6));
        }
    } else {
        // Show scroll indicator if there are more messages above
        if (scrollPosition > 0 && hasMoreOlderMessages) {
            if (has_colors()) {
                wattron(chatWindow, COLOR_PAIR(6));
            }
            mvwprintw(chatWindow, max_y - 1, max_x - 15, "↑ Scroll for more ↑");
            if (has_colors()) {
                wattroff(chatWindow, COLOR_PAIR(6));
            }
        }
    }
    
    // Show scroll indicator if there are more messages below (when scrolled up)
    if (endIndex < totalMessages) {
        if (has_colors()) {
            wattron(chatWindow, COLOR_PAIR(6));
        }
        mvwprintw(chatWindow, max_y - 1, 1, "↓ More below ↓");
        if (has_colors()) {
            wattroff(chatWindow, COLOR_PAIR(6));
        }
    }
    
    wrefresh(chatWindow);
}


void drawInputArea(const std::string& currentInput) {
    if (!inputWindow) return;
    
    wclear(inputWindow);
    
    int max_x = getmaxx(inputWindow);
    
    // Draw separator line
    mvwprintw(inputWindow, 0, 0, "%s", std::string(max_x, '-').c_str());
    
    // Input prompt
    std::string prompt = "Message: ";
    mvwprintw(inputWindow, 1, 1, "%s", prompt.c_str());
    
    // Current input text
    std::string displayInput = currentInput;
    int maxInputLength = max_x - prompt.length() - 3;
    
    if (displayInput.length() > maxInputLength) {
        displayInput = "..." + displayInput.substr(displayInput.length() - maxInputLength + 3);
    }
    
    mvwprintw(inputWindow, 1, prompt.length() + 1, "%s", displayInput.c_str());
    
    // Position cursor
    int cursorPos = std::min((int)displayInput.length(), maxInputLength);
    wmove(inputWindow, 1, prompt.length() + 1 + cursorPos);
    
    wrefresh(inputWindow);
}



void receiveMessages(int userId, int otherUserId, bool& chatting) {
    while (chatting) {
        try {
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));

            // Check if we need to load older messages
            if (isLoadingOlderMessages && hasMoreOlderMessages) {
                std::vector<Message> olderMessages = getOlderChatHistory(userId, otherUserId, 50, totalLoadedMessages);
                
                std::lock_guard<std::mutex> lock(chatMutex);
                
                if (!olderMessages.empty()) {
                    // Insert older messages at the beginning
                    globalChatHistory.insert(globalChatHistory.begin(), olderMessages.begin(), olderMessages.end());
                    totalLoadedMessages += olderMessages.size();
                    
                    // Adjust scroll position to maintain user's view
                    scrollPosition += olderMessages.size();
                    
                    if (olderMessages.size() < 50) {
                        hasMoreOlderMessages = false; // No more older messages
                    }
                } else {
                    hasMoreOlderMessages = false; // No more older messages
                }
                
                isLoadingOlderMessages = false;
                needsRefresh = true;
            } else {
                // Regular update - get latest messages
                std::vector<Message> latestMessages = getLatestChatHistory(userId, otherUserId, 50);
                
                std::lock_guard<std::mutex> lock(chatMutex);
                
                // Only update if we're showing recent messages (not scrolled up too far)
                if (scrollPosition <= 5) { // Only update if close to bottom
                    bool hasNewMessages = false;
                    
                    if (latestMessages.size() != globalChatHistory.size()) {
                        hasNewMessages = true;
                    } else if (!latestMessages.empty() && !globalChatHistory.empty()) {
                        // Check if the last message is different
                        const Message& newLast = latestMessages.back();
                        const Message& oldLast = globalChatHistory.back();
                        
                        if (newLast.content != oldLast.content || 
                            newLast.timestamp != oldLast.timestamp) {
                            hasNewMessages = true;
                        }
                    }
                    
                    if (hasNewMessages) {
                        // Replace with latest messages only if we're near the bottom
                        int oldSize = globalChatHistory.size();
                        
                        // Keep older messages if we have them
                        if (totalLoadedMessages > 50) {
                            // Remove old recent messages and add new ones
                            globalChatHistory.erase(globalChatHistory.end() - std::min(50, oldSize), globalChatHistory.end());
                            globalChatHistory.insert(globalChatHistory.end(), latestMessages.begin(), latestMessages.end());
                        } else {
                            globalChatHistory = latestMessages;
                            totalLoadedMessages = latestMessages.size();
                        }
                        
                        needsRefresh = true;
                    }
                }
            }

        } catch (const std::exception& e) {
            // Handle error silently
            isLoadingOlderMessages = false;
        }
    }
}


// Replace the showChatTerminal function with this version:
int showChatTerminal(int userId, int otherUserId, const std::string& userName) {
    initializeChatTerminal();
    
    // Reset pagination variables
    totalLoadedMessages = 0;
    isLoadingOlderMessages = false;
    hasMoreOlderMessages = true;
    scrollPosition = 0;
    
    // Get other user's name
    std::string otherUserName = "User#" + std::to_string(otherUserId);
    
    bool chatting = true;
    std::string currentInput = "";
    
    // Initial load of chat history (most recent 50 messages)
    {
        std::lock_guard<std::mutex> lock(chatMutex);
        globalChatHistory = getLatestChatHistory(userId, otherUserId, 50);
        totalLoadedMessages = globalChatHistory.size();
        
        // If we got less than 50 messages, there are no older messages
        if (globalChatHistory.size() < 50) {
            hasMoreOlderMessages = false;
        }
    }
    
    // Rest of the function remains the same...
    std::thread receiverThread(receiveMessages, userId, otherUserId, std::ref(chatting));
    
    // Initial display
    drawChatHeader(otherUserName);
    drawChatHistory(userId);
    drawInputArea(currentInput);
    
    // Set non-blocking input with timeout
    timeout(500);
    
    // Main input loop (rest remains the same as before)
    int ch;
    while (chatting) {
        ch = getch();
        
        if (ch != ERR) {
            switch (ch) {
                case KEY_UP:
                    scrollPosition++;
                    drawChatHistory(userId);
                    drawInputArea(currentInput);
                    break;
                    
                case KEY_DOWN:
                    scrollPosition = std::max(0, scrollPosition - 1);
                    drawChatHistory(userId);
                    drawInputArea(currentInput);
                    break;
                    
                case 10: // ENTER
                case 13: // ENTER
                    if (currentInput[0] == '/') {
                        processCommand(currentInput, userId, otherUserId, currentInput, chatting, needsRefresh);
                    } else if (!currentInput.empty()) {
                        if (sendMessage(userId, otherUserId, currentInput)) {
                            currentInput = "";
                            scrollPosition = 0;
                            {
                                std::lock_guard<std::mutex> lock(chatMutex);
                                globalChatHistory = getLatestChatHistory(userId, otherUserId, 50);
                                totalLoadedMessages = std::max(totalLoadedMessages, (int)globalChatHistory.size());
                            }
                            needsRefresh = true;
                        }
                    }
                    drawInputArea(currentInput);
                    break;
                    
                case KEY_BACKSPACE:
                case 127:
                case 8:
                    if (!currentInput.empty()) {
                        currentInput.pop_back();
                        drawInputArea(currentInput);
                    }
                    break;
                    
                case 27: // ESC
                    chatting = false;
                    break;
                    
                default:
                    if (ch >= 32 && ch <= 126) {
                        currentInput += (char)ch;
                        drawInputArea(currentInput);
                    }
                    break;
            }
        }
        
        if (needsRefresh) {
            needsRefresh = false;
            drawChatHeader(otherUserName);
            drawChatHistory(userId);
            drawInputArea(currentInput);
        }
    }
    
    timeout(-1);
    chatting = false;
    receiverThread.join();
    cleanupChatTerminal();
    
    return 0;
}