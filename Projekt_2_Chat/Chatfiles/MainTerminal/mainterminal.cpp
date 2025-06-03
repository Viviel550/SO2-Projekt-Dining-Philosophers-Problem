#include "mainterminal.h"
#include "mainterminalback.h"
#include <ncurses/ncurses.h>
#include <iostream>
#include <string>
#include <limits>
#include "../ChatTerminal/chatterminalf.h" 
#include "../FriendList/friendlist.h"

void initializeMainTerminal() {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0); // Hide cursor for button navigation
    
    if (has_colors()) {
        start_color();
        init_pair(1, COLOR_CYAN, COLOR_BLACK);    // Cyan for headers
        init_pair(2, COLOR_GREEN, COLOR_BLACK);   // Green for success
        init_pair(3, COLOR_RED, COLOR_BLACK);     // Red for errors
        init_pair(4, COLOR_YELLOW, COLOR_BLACK);  // Yellow for warnings
        init_pair(5, COLOR_WHITE, COLOR_BLACK);   // White for normal text
        init_pair(6, COLOR_BLACK, COLOR_GREEN);   // Active button
        init_pair(7, COLOR_WHITE, COLOR_BLACK);   // Inactive button
        init_pair(8, COLOR_BLUE, COLOR_BLACK);    // Instructions
    }
}

void cleanupMainTerminal() {
    endwin();
}

void displayMainMenu(const std::string& name, const std::string& userNameId) {
    clear();
    
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);
    
    // Calculate center positions
    int center_x = max_x / 2;
    int center_y = max_y / 2;
    
    // Header
    std::string header = "===== MAIN MENU =====";
    int header_x = center_x - header.length() / 2;
    
    if (has_colors()) {
        attron(COLOR_PAIR(1));
    }
    mvprintw(center_y - 10, header_x, "%s", header.c_str());
    
    // User info
    std::string userInfo = "Logged in as: " + name + "#" + userNameId;
    int userInfo_x = center_x - userInfo.length() / 2;
    mvprintw(center_y - 8, userInfo_x, "%s", userInfo.c_str());
    
    if (has_colors()) {
        attroff(COLOR_PAIR(1));
    }
    
    // Instructions (centered at bottom)
    if (has_colors()) {
        attron(COLOR_PAIR(8));
    }
    
    std::string nav_title = "Navigation:";
    std::string nav1 = "- Use UP/DOWN Arrow Keys to select option";
    std::string nav2 = "- Press ENTER or SPACE to confirm selection";
    std::string nav3 = "- Press ESC to logout";
    
    mvprintw(center_y + 7, center_x - nav_title.length() / 2, "%s", nav_title.c_str());
    mvprintw(center_y + 8, center_x - nav1.length() / 2, "%s", nav1.c_str());
    mvprintw(center_y + 9, center_x - nav2.length() / 2, "%s", nav2.c_str());
    mvprintw(center_y + 10, center_x - nav3.length() / 2, "%s", nav3.c_str());
    
    if (has_colors()) {
        attroff(COLOR_PAIR(8));
    }
    
    refresh();
}

void drawMainTerminalButton(int y, int x, const std::string& text, bool active) {
    if (has_colors()) {
        if (active) {
            attron(COLOR_PAIR(6)); // Active button (black on green)
        } else {
            attron(COLOR_PAIR(7)); // Inactive button (white on black)
        }
    }
    
    // Draw button with padding
    std::string buttonText = "[ " + text + " ]";
    int button_x = x - buttonText.length() / 2; // Center the button text
    mvprintw(y, button_x, "%s", buttonText.c_str());
    
    if (has_colors()) {
        if (active) {
            attroff(COLOR_PAIR(6));
        } else {
            attroff(COLOR_PAIR(7));
        }
    }
    
    refresh();
}

void showMainTerminalMessage(const std::string& message, int colorPair) {
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);
    
    int center_x = max_x / 2;
    int message_y = max_y - 6;
    
    // Clear message area
    for (int i = 0; i < 3; i++) {
        mvprintw(message_y + i, 0, "%*s", max_x, ""); // Clear entire line
    }
    
    if (has_colors()) {
        attron(COLOR_PAIR(colorPair));
    }
    
    mvprintw(message_y, center_x - message.length() / 2, "%s", message.c_str());
    
    std::string continue_msg = "Press any key to continue...";
    mvprintw(message_y + 1, center_x - continue_msg.length() / 2, "%s", continue_msg.c_str());
    
    if (has_colors()) {
        attroff(COLOR_PAIR(colorPair));
    }
    
    refresh();
    getch();
}

void clearMainTerminalMessageArea() {
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);
    
    int message_y = max_y - 6;
    
    // Clear message area
    for (int i = 0; i < 3; i++) {
        mvprintw(message_y + i, 0, "%*s", max_x, "");
    }
    refresh();
}

std::string getMainTerminalStringInput(const std::string& prompt) {
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);
    
    int center_x = max_x / 2;
    int input_y = max_y - 6;
    
    // Clear input area
    for (int i = 0; i < 3; i++) {
        mvprintw(input_y + i, 0, "%*s", max_x, "");
    }
    
    // Show prompt
    mvprintw(input_y, center_x - prompt.length() / 2, "%s", prompt.c_str());
    refresh();
    
    // Position cursor for input
    move(input_y + 1, center_x - 20);
    curs_set(1); // Show cursor for input
    
    char input[256];
    echo();
    getstr(input);
    noecho();
    curs_set(0); // Hide cursor again
    
    return std::string(input);
}

static int PrivateChat(const std::string& userNameId, const int userId, const std::string& userName) {
    clearMainTerminalMessageArea();
    
    std::string otherUserName = getMainTerminalStringInput("Enter Nickname of other User:");
    clearMainTerminalMessageArea();
    
    std::string otherUserId = getMainTerminalStringInput("Enter Other User ID:");
    clearMainTerminalMessageArea();
    
    if (otherUserName == userName && otherUserId == userNameId) {
        showMainTerminalMessage("You cannot chat with yourself.", 3); // Red color
        return -1;
    } 
    
    // Get actual user ID from database using nickname and user_name_id
    int otherUserIdInt = pickUserChat(otherUserName, otherUserId);
    
    if (otherUserIdInt > 0) {
        std::string successMsg = "Starting chat with " + otherUserName + "...";
        showMainTerminalMessage(successMsg, 2); // Green color
        
        // Cleanup main terminal before starting chat
        cleanupMainTerminal();
        
        // Forward to chat terminal with user display information
        showChatTerminal(userId, otherUserIdInt, userName, userNameId, otherUserName, otherUserId);
        
        // Reinitialize main terminal when returning from chat
        initializeMainTerminal();
    } else {
        showMainTerminalMessage("User not found. Check the Nickname and UserId", 3); // Red color
    }
    
    return 0;
}

static int JoinChatRoom() {
    clearMainTerminalMessageArea();
    showMainTerminalMessage("Chat rooms not implemented yet.", 4); // Yellow color
    return 0;
}

static int FriendsList(const int userId, const std::string& userName, const std::string& userNameId) {
    clearMainTerminalMessageArea();
    
    // Cleanup main terminal before starting friends list
    cleanupMainTerminal();
    
    // Show friends list screen
    showFriendsListScreen(userId, userName, userNameId);
    
    // Reinitialize main terminal when returning
    initializeMainTerminal();
    
    return 0;
}

int showMainTerminal(const std::string& userEmail, const std::string& userNameId, const std::string& Name, int userId) {
    initializeMainTerminal();
    
    int selectedOption = 0; // 0 = Private Chat, 1 = Chat Room, 2 = Friends, 3 = Logout, 4 = Exit
    const int NUM_OPTIONS = 5;
    
    while (true) {
        // Get current terminal size
        int max_y, max_x;
        getmaxyx(stdscr, max_y, max_x);
        
        // Calculate dynamic positions
        int center_x = max_x / 2;
        int center_y = max_y / 2;
        
        // Button positions (vertically centered)
        const int PRIVATE_CHAT_Y = center_y - 4;
        const int CHAT_ROOM_Y = center_y - 2;
        const int FRIENDS_Y = center_y;
        const int LOGOUT_Y = center_y + 2;
        const int EXIT_Y = center_y + 4;
        
        displayMainMenu(Name, userNameId);
        
        // Draw buttons
        drawMainTerminalButton(PRIVATE_CHAT_Y, center_x, "START PRIVATE CHAT", selectedOption == 0);
        drawMainTerminalButton(CHAT_ROOM_Y, center_x, "JOIN CHAT ROOM", selectedOption == 1);
        drawMainTerminalButton(FRIENDS_Y, center_x, "FRIENDS LIST", selectedOption == 2);
        drawMainTerminalButton(LOGOUT_Y, center_x, "LOGOUT", selectedOption == 3);
        drawMainTerminalButton(EXIT_Y, center_x, "EXIT", selectedOption == 4);
        
        int ch = getch();
        
        switch (ch) {
            case KEY_UP:
                selectedOption = (selectedOption == 0) ? NUM_OPTIONS - 1 : selectedOption - 1;
                break;
                
            case KEY_DOWN:
                selectedOption = (selectedOption + 1) % NUM_OPTIONS;
                break;
                
            case 10: // ENTER
            case 13: // ENTER
            case 32: // SPACE
                switch (selectedOption) {
                    case 0: // Private Chat
                        PrivateChat(userNameId, userId, Name);
                        break;
                        
                    case 1: // Chat Room
                        JoinChatRoom();
                        break;
                        
                    case 2: // Friends List
                        FriendsList(userId, Name, userNameId);
                        break;
                        
                    case 3: // Logout
                        clearMainTerminalMessageArea();
                        showMainTerminalMessage("Logging out...", 4); // Yellow color
                        cleanupMainTerminal();
                        return 0; // Return to main menu
                        
                    case 4: // Exit
                        {
                            // Confirmation message
                            clear();
                            int max_y, max_x;
                            getmaxyx(stdscr, max_y, max_x);
                            int center_x = max_x / 2;
                            int center_y = max_y / 2;
                            
                            std::string goodbye = "Thank you for using Terminal Chat Application!";
                            mvprintw(center_y, center_x - goodbye.length() / 2, "%s", goodbye.c_str());
                            
                            std::string goodbye2 = "Goodbye!";
                            mvprintw(center_y + 1, center_x - goodbye2.length() / 2, "%s", goodbye2.c_str());
                            
                            refresh();
                            napms(1500); // 1.5 second pause
                            
                            cleanupMainTerminal();
                            std::exit(0);
                        }
                        break;
                }
                break;
                
            case 27: // ESC (Logout)
                clearMainTerminalMessageArea();
                showMainTerminalMessage("Logging out...", 4); // Yellow color
                cleanupMainTerminal();
                return 0;
                
            default:
                // Ignore other keys
                break;
        }
    }
    
    cleanupMainTerminal();
    return 0;
}