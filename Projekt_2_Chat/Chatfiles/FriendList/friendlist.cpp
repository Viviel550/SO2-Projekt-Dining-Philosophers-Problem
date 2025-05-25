#include "friendlist.h"
#include "friendlistback.h"
#include <ncurses/ncurses.h>
#include <iostream>
#include <string>
#include <vector>
#include "../ChatTerminal/chatterminalf.h"

void initializeFriendsListScreen() {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);
    
    if (has_colors()) {
        start_color();
        init_pair(1, COLOR_CYAN, COLOR_BLACK);    // Header
        init_pair(2, COLOR_GREEN, COLOR_BLACK);   // Success messages
        init_pair(3, COLOR_RED, COLOR_BLACK);     // Error messages
        init_pair(4, COLOR_YELLOW, COLOR_BLACK);  // Warning messages
        init_pair(5, COLOR_WHITE, COLOR_BLACK);   // Normal text
        init_pair(6, COLOR_BLACK, COLOR_GREEN);   // Active button/item
        init_pair(7, COLOR_WHITE, COLOR_BLACK);   // Inactive button/item
        init_pair(8, COLOR_BLUE, COLOR_BLACK);    // Instructions
    }
}

void cleanupFriendsListScreen() {
    endwin();
}

void drawFriendsListHeader() {
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);
    
    if (has_colors()) {
        attron(COLOR_PAIR(1));
    }
    
    std::string header = "===== FRIENDS LIST =====";
    int header_x = (max_x - header.length()) / 2;
    mvprintw(1, header_x, "%s", header.c_str());
    
    if (has_colors()) {
        attroff(COLOR_PAIR(1));
    }
    
    // Instructions
    if (has_colors()) {
        attron(COLOR_PAIR(8));
    }
    
    std::string nav1 = "UP/DOWN: Navigate | ENTER: Chat with friend | TAB: Switch to buttons";
    std::string nav2 = "ESC: Return to main menu";
    
    mvprintw(max_y - 3, (max_x - nav1.length()) / 2, "%s", nav1.c_str());
    mvprintw(max_y - 2, (max_x - nav2.length()) / 2, "%s", nav2.c_str());
    
    if (has_colors()) {
        attroff(COLOR_PAIR(8));
    }
}

void drawFriendsListArea(const std::vector<FriendInfo>& friends, int selectedFriend, bool isInFriendsList) {
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);
    
    // Friends list area (left side)
    int friends_area_width = max_x * 2 / 3;
    int friends_start_y = 4;
    int friends_end_y = max_y - 6;
    
    // Clear friends area
    for (int y = friends_start_y; y < friends_end_y; y++) {
        for (int x = 1; x < friends_area_width - 1; x++) {
            mvaddch(y, x, ' ');
        }
    }
    
    // Draw border around friends area
    for (int y = friends_start_y - 1; y <= friends_end_y; y++) {
        mvaddch(y, 0, '|');
        mvaddch(y, friends_area_width - 1, '|');
    }
    for (int x = 0; x < friends_area_width; x++) {
        mvaddch(friends_start_y - 1, x, '-');
        mvaddch(friends_end_y, x, '-');
    }
    mvaddch(friends_start_y - 1, 0, '+');
    mvaddch(friends_start_y - 1, friends_area_width - 1, '+');
    mvaddch(friends_end_y, 0, '+');
    mvaddch(friends_end_y, friends_area_width - 1, '+');
    
    // Friends list title
    std::string title = " Your Friends ";
    mvprintw(friends_start_y - 1, (friends_area_width - title.length()) / 2, "%s", title.c_str());
    
    if (friends.empty()) {
        if (has_colors()) {
            attron(COLOR_PAIR(4));
        }
        std::string no_friends = "No friends yet. Add some friends!";
        mvprintw(friends_start_y + 2, (friends_area_width - no_friends.length()) / 2, "%s", no_friends.c_str());
        if (has_colors()) {
            attroff(COLOR_PAIR(4));
        }
    } else {
        // Display friends list
        int visible_friends = friends_end_y - friends_start_y - 1;
        int start_index = std::max(0, selectedFriend - visible_friends / 2);
        int end_index = std::min((int)friends.size(), start_index + visible_friends);
        
        for (int i = start_index; i < end_index; i++) {
            int display_y = friends_start_y + 1 + (i - start_index);
            
            if (i == selectedFriend && isInFriendsList) {
                if (has_colors()) {
                    attron(COLOR_PAIR(6)); // Highlight selected friend
                }
            } else {
                if (has_colors()) {
                    attron(COLOR_PAIR(7));
                }
            }
            
            std::string friend_display = friends[i].nickname + "#" + friends[i].userNameId;
            if (friend_display.length() > friends_area_width - 4) {
                friend_display = friend_display.substr(0, friends_area_width - 7) + "...";
            }
            
            mvprintw(display_y, 2, "%-*s", friends_area_width - 4, friend_display.c_str());
            
            if (has_colors()) {
                if (i == selectedFriend && isInFriendsList) {
                    attroff(COLOR_PAIR(6));
                } else {
                    attroff(COLOR_PAIR(7));
                }
            }
        }
    }
}

void drawButtonArea(int selectedButton, bool isInButtonArea) {
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);
    
    // Button area (right side)
    int buttons_start_x = max_x * 2 / 3;
    int buttons_width = max_x - buttons_start_x;
    int buttons_start_y = 4;
    int buttons_end_y = max_y - 6;
    
    // Clear button area
    for (int y = buttons_start_y; y < buttons_end_y; y++) {
        for (int x = buttons_start_x + 1; x < max_x - 1; x++) {
            mvaddch(y, x, ' ');
        }
    }
    
    // Draw border around button area
    for (int y = buttons_start_y - 1; y <= buttons_end_y; y++) {
        mvaddch(y, buttons_start_x, '|');
        mvaddch(y, max_x - 1, '|');
    }
    for (int x = buttons_start_x; x < max_x; x++) {
        mvaddch(buttons_start_y - 1, x, '-');
        mvaddch(buttons_end_y, x, '-');
    }
    mvaddch(buttons_start_y - 1, buttons_start_x, '+');
    mvaddch(buttons_start_y - 1, max_x - 1, '+');
    mvaddch(buttons_end_y, buttons_start_x, '+');
    mvaddch(buttons_end_y, max_x - 1, '+');
    
    // Button area title
    std::string title = " Actions ";
    mvprintw(buttons_start_y - 1, buttons_start_x + (buttons_width - title.length()) / 2, "%s", title.c_str());
    
    // Button positions
    int button_y_start = buttons_start_y + 3;
    int button_center_x = buttons_start_x + buttons_width / 2;
    
    std::vector<std::string> button_texts = {"ADD NEW FRIEND", "DELETE FRIEND", "CANCEL"};
    
    for (int i = 0; i < 3; i++) {
        int button_y = button_y_start + i * 3;
        
        if (i == selectedButton && isInButtonArea) {
            if (has_colors()) {
                attron(COLOR_PAIR(6)); // Active button
            }
        } else {
            if (has_colors()) {
                attron(COLOR_PAIR(7)); // Inactive button
            }
        }
        
        std::string button_text = "[ " + button_texts[i] + " ]";
        mvprintw(button_y, button_center_x - button_text.length() / 2, "%s", button_text.c_str());
        
        if (has_colors()) {
            if (i == selectedButton && isInButtonArea) {
                attroff(COLOR_PAIR(6));
            } else {
                attroff(COLOR_PAIR(7));
            }
        }
    }
}

void showFriendsMessage(const std::string& message, int colorPair) {
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);
    
    int message_y = max_y - 5;
    
    // Clear message area
    mvprintw(message_y, 0, "%*s", max_x, "");
    
    if (has_colors()) {
        attron(COLOR_PAIR(colorPair));
    }
    
    mvprintw(message_y, (max_x - message.length()) / 2, "%s", message.c_str());
    
    if (has_colors()) {
        attroff(COLOR_PAIR(colorPair));
    }
    
    refresh();
    napms(1500); // Show message for 1.5 seconds
}

std::string getFriendsInput(const std::string& prompt) {
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);
    
    int input_y = max_y / 2;
    
    // Clear input area
    for (int i = 0; i < 5; i++) {
        mvprintw(input_y + i, 0, "%*s", max_x, "");
    }
    
    // Show prompt
    mvprintw(input_y, (max_x - prompt.length()) / 2, "%s", prompt.c_str());
    mvprintw(input_y + 1, max_x / 2 - 20, "Enter: ");
    
    refresh();
    move(input_y + 1, max_x / 2 - 13);
    curs_set(1);
    
    char input[256];
    echo();
    getstr(input);
    noecho();
    curs_set(0);
    
    return std::string(input);
}

int showFriendsListScreen(int userId, const std::string& userName) {
    initializeFriendsListScreen();
    
    std::vector<FriendInfo> friends = getFriendsList(userId);
    int selectedFriend = 0;
    int selectedButton = 0;
    bool isInFriendsList = true; // true = in friends list, false = in buttons
    
    while (true) {
        clear();
        
        drawFriendsListHeader();
        drawFriendsListArea(friends, selectedFriend, isInFriendsList);
        drawButtonArea(selectedButton, !isInFriendsList);
        
        refresh();
        
        int ch = getch();
        
        switch (ch) {
            case KEY_UP:
                if (isInFriendsList) {
                    if (!friends.empty()) {
                        selectedFriend = (selectedFriend == 0) ? friends.size() - 1 : selectedFriend - 1;
                    }
                } else {
                    selectedButton = (selectedButton == 0) ? 2 : selectedButton - 1;
                }
                break;
                
            case KEY_DOWN:
                if (isInFriendsList) {
                    if (!friends.empty()) {
                        selectedFriend = (selectedFriend + 1) % friends.size();
                    }
                } else {
                    selectedButton = (selectedButton + 1) % 3;
                }
                break;
                
            case 9: // TAB - Switch between areas
                isInFriendsList = !isInFriendsList;
                break;
                
            case 10: // ENTER
            case 13: // ENTER
            case 32: // SPACE
                if (isInFriendsList) {
                    // Chat with selected friend
                    if (!friends.empty() && selectedFriend < friends.size()) {
                        cleanupFriendsListScreen();
                        showChatTerminal(userId, friends[selectedFriend].userId, userName);
                        initializeFriendsListScreen();
                        // Refresh friends list when returning
                        friends = getFriendsList(userId);
                        selectedFriend = std::min(selectedFriend, (int)friends.size() - 1);
                        if (selectedFriend < 0) selectedFriend = 0;
                    }
                } else {
                    // Handle button actions
                    switch (selectedButton) {
                        case 0: // ADD NEW FRIEND
                        {
                            std::string nickname = getFriendsInput("Enter friend's nickname:");
                            if (!nickname.empty()) {
                                std::string userNameId = getFriendsInput("Enter friend's user ID:");
                                if (!userNameId.empty()) {
                                    int friendUserId = findUserByNicknameAndId(nickname, userNameId);
                                    if (friendUserId > 0) {
                                        if (sendFriendRequest(userId, friendUserId)) {
                                            showFriendsMessage("Friend added successfully!", 2);
                                            friends = getFriendsList(userId); // Refresh list
                                        } else {
                                            showFriendsMessage("Failed to add friend or already friends.", 3);
                                        }
                                    } else {
                                        std::string errorMessage = "User not found: " + nickname + "#" + userNameId + ". " + std::to_string(friendUserId);
                                        showFriendsMessage(errorMessage, 3);
                                    }
                                }
                            }
                        }
                        break;
                        
                        case 1: // DELETE FRIEND
                            showFriendsMessage("Delete friend feature not implemented yet.", 4);
                            break;
                            
                        case 2: // CANCEL
                            cleanupFriendsListScreen();
                            return 0;
                    }
                }
                break;
                
            case 27: // ESC
                cleanupFriendsListScreen();
                return 0;
                
            default:
                break;
        }
    }
    
    cleanupFriendsListScreen();
    return 0;
}