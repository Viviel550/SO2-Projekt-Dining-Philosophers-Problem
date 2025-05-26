#include "login.h"
#include "loginback.h"
#include <ncurses/ncurses.h>
#include <iostream>
#include <string>
#include "../MainTerminal/mainterminal.h"

void initializeLoginScreen() {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(1);
    
    if (has_colors()) {
        start_color();
        init_pair(1, COLOR_CYAN, COLOR_BLACK);    // Header color
        init_pair(2, COLOR_WHITE, COLOR_BLUE);    // Active field
        init_pair(3, COLOR_BLACK, COLOR_WHITE);   // Inactive field
        init_pair(4, COLOR_GREEN, COLOR_BLACK);   // Success messages
        init_pair(5, COLOR_RED, COLOR_BLACK);     // Error messages
        init_pair(6, COLOR_YELLOW, COLOR_BLACK);  // Instructions
        init_pair(7, COLOR_BLACK, COLOR_GREEN);   // Active button
        init_pair(8, COLOR_WHITE, COLOR_BLACK);   // Inactive button
    }
}

void cleanupLoginScreen() {
    endwin();
}

void drawLoginInterface() {
    clear();
    
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);
    
    // Calculate center positions
    int center_x = max_x / 2;
    int center_y = max_y / 2;
    
    // Header
    std::string header = "===== USER LOGIN =====";
    int header_x = center_x - header.length() / 2;
    
    if (has_colors()) {
        attron(COLOR_PAIR(1));
    }
    mvprintw(center_y - 8, header_x, "%s", header.c_str());
    if (has_colors()) {
        attroff(COLOR_PAIR(1));
    }
    
    // Labels (positioned relative to center)
    mvprintw(center_y - 5, center_x - 20, "Email:");
    mvprintw(center_y - 2, center_x - 20, "Password:");
    
    // Instructions (centered at bottom)
    if (has_colors()) {
        attron(COLOR_PAIR(6));
    }
    
    std::string nav_title = "Navigation:";
    std::string nav1 = "- Use TAB or Arrow Keys to move between fields/buttons";
    std::string nav2 = "- Press ENTER or SPACE to activate selected button";
    std::string nav3 = "- Press ESC to cancel";
    
    mvprintw(center_y + 4, center_x - nav_title.length() / 2, "%s", nav_title.c_str());
    mvprintw(center_y + 5, center_x - nav1.length() / 2, "%s", nav1.c_str());
    mvprintw(center_y + 6, center_x - nav2.length() / 2, "%s", nav2.c_str());
    mvprintw(center_y + 7, center_x - nav3.length() / 2, "%s", nav3.c_str());
    
    if (has_colors()) {
        attroff(COLOR_PAIR(6));
    }
    
    refresh();
}

void drawLoginInputField(int y, int x, int width, const std::string& content, bool active) {
    // Draw field background
    if (has_colors()) {
        if (active) {
            attron(COLOR_PAIR(2)); // Active field (white on blue)
        } else {
            attron(COLOR_PAIR(3)); // Inactive field (black on white)
        }
    }
    
    // Clear the field area
    for (int i = 0; i < width; i++) {
        mvaddch(y, x + i, ' ');
    }
    
    // Display content
    mvprintw(y, x, "%-*s", width - 1, content.c_str());
    
    if (has_colors()) {
        if (active) {
            attroff(COLOR_PAIR(2));
        } else {
            attroff(COLOR_PAIR(3));
        }
    }
    
    // Position cursor at the end of content in active field
    if (active) {
        int cursor_pos = std::min((int)content.length(), width - 2);
        move(y, x + cursor_pos);
    }
    
    refresh();
}

void drawButton(int y, int x, const std::string& text, bool active) {
    if (has_colors()) {
        if (active) {
            attron(COLOR_PAIR(7)); // Active button (black on green)
        } else {
            attron(COLOR_PAIR(8)); // Inactive button (white on black)
        }
    }
    
    // Draw button with padding
    std::string buttonText = "[ " + text + " ]";
    mvprintw(y, x, "%s", buttonText.c_str());
    
    if (has_colors()) {
        if (active) {
            attroff(COLOR_PAIR(7));
        } else {
            attroff(COLOR_PAIR(8));
        }
    }
    
    refresh();
}

void showLoginMessage(const std::string& message, int colorPair) {
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);
    
    int center_x = max_x / 2;
    int message_y = max_y - 4;
    
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

void showLoginScreen() {
    initializeLoginScreen();
    
    std::string email = "";
    std::string password = "";
    int currentField = 0; // 0 = email, 1 = password, 2 = login button, 3 = cancel button
    
    while (true) {
        // Get current terminal size
        int max_y, max_x;
        getmaxyx(stdscr, max_y, max_x);
        
        // Calculate dynamic positions based on terminal size
        int center_x = max_x / 2;
        int center_y = max_y / 2;
        
        const int FIELD_WIDTH = std::min(40, max_x - 10); // Responsive field width
        const int EMAIL_Y = center_y - 4;
        const int PASSWORD_Y = center_y - 1;
        const int FIELD_X = center_x - FIELD_WIDTH / 2;
        const int BUTTON_Y = center_y + 2;
        
        // Center the buttons
        std::string login_text = "[ LOGIN ]";
        std::string cancel_text = "[ CANCEL ]";
        int total_button_width = login_text.length() + cancel_text.length() + 4; // 4 spaces between
        int LOGIN_BUTTON_X = center_x - total_button_width / 2;
        int CANCEL_BUTTON_X = LOGIN_BUTTON_X + login_text.length() + 4;
        
        drawLoginInterface();
        
        // Draw input fields
        drawLoginInputField(EMAIL_Y, FIELD_X, FIELD_WIDTH, email, currentField == 0);
        drawLoginInputField(PASSWORD_Y, FIELD_X, FIELD_WIDTH, std::string(password.length(), '*'), currentField == 1);
        
        // Draw buttons
        drawButton(BUTTON_Y, LOGIN_BUTTON_X, "LOGIN", currentField == 2);
        drawButton(BUTTON_Y, CANCEL_BUTTON_X, "CANCEL", currentField == 3);
        
        // Position cursor appropriately
        if (currentField == 0) {
            move(EMAIL_Y, FIELD_X + std::min((int)email.length(), FIELD_WIDTH - 2));
        } else if (currentField == 1) {
            move(PASSWORD_Y, FIELD_X + std::min((int)password.length(), FIELD_WIDTH - 2));
        } else if (currentField == 2) {
            move(BUTTON_Y, LOGIN_BUTTON_X + 1);
            curs_set(0); // Hide cursor on buttons
        } else if (currentField == 3) {
            move(BUTTON_Y, CANCEL_BUTTON_X + 1);
            curs_set(0); // Hide cursor on buttons
        }
        
        if (currentField <= 1) {
            curs_set(1); // Show cursor on input fields
        }
        
        int ch = getch();
        
        // Replace the switch statement navigation section with this corrected version:
        switch (ch) {
            case KEY_UP:
                // Move up in sequence: CANCEL → LOGIN → Password → Email
                if (currentField == 0) {
                    currentField = 3; // Email → CANCEL (wrap around)
                } else {
                    currentField--; // Move to previous field
                }
                break;
                
            case KEY_DOWN:
                // Move down in sequence: Email → Password → LOGIN → CANCEL
                if (currentField == 3) {
                    currentField = 0; // CANCEL → Email (wrap around)
                } else {
                    currentField++; // Move to next field
                }
                break;
                
            case KEY_LEFT:
                if (currentField >= 2) {
                    // Only allow left/right navigation between buttons
                    currentField = (currentField == 3) ? 2 : 3; // Toggle between LOGIN and CANCEL
                }
                break;
                
            case KEY_RIGHT:
                if (currentField >= 2) {
                    // Only allow left/right navigation between buttons
                    currentField = (currentField == 2) ? 3 : 2; // Toggle between LOGIN and CANCEL
                }
                break;
                
            case 9: // TAB
                // Tab cycles through all fields in order: Email → Password → LOGIN → CANCEL
                currentField = (currentField + 1) % 4;
                break;
                
            case 10: // ENTER
            case 13: // ENTER  
            case 32: // SPACE
                if (currentField == 2) { // LOGIN button
                    if (!email.empty() && !password.empty()) {
                        // Show processing message
                        mvprintw(center_y + 4, center_x - 7, "Authenticating...");
                        refresh();
                        
                        if (authenticateUser(email, password)) {
                            showLoginMessage("Login successful!", 4); // Green
                            
                            UserData userID = getUserIdByEmail(email);
                            if (userID.UserId > 0) {
                                cleanupLoginScreen();
                                showMainTerminal(email, std::to_string(userID.userNameId), userID.userName, userID.UserId);
                                return;
                            } else {
                                showLoginMessage("Failed to retrieve user data. Please try again.", 5); // Red
                            }
                        } else {
                            showLoginMessage("Login failed. Invalid email or password.", 5); // Red
                        }
                    } else {
                        showLoginMessage("Please fill in both email and password fields.", 5); // Red
                    }
                } else if (currentField == 3) { // CANCEL button
                    cleanupLoginScreen();
                    return;
                }
                // If ENTER is pressed on input fields (0 or 1), move to next field
                else if (currentField == 0) {
                    currentField = 1; // Email → Password
                } else if (currentField == 1) {
                    currentField = 2; // Password → LOGIN button
                }
                break;
                
            case 27: // ESC
                cleanupLoginScreen();
                return;
                
            default:
                // Handle input only for input fields (not buttons)
                if (currentField == 0) {
                    // Email field
                    if (ch == KEY_BACKSPACE || ch == 127 || ch == 8) {
                        if (!email.empty()) {
                            email.pop_back();
                        }
                    } else if (ch >= 32 && ch <= 126 && email.length() < FIELD_WIDTH - 2) {
                        email += (char)ch;
                    }
                } else if (currentField == 1) {
                    // Password field
                    if (ch == KEY_BACKSPACE || ch == 127 || ch == 8) {
                        if (!password.empty()) {
                            password.pop_back();
                        }
                    } else if (ch >= 32 && ch <= 126 && password.length() < FIELD_WIDTH - 2) {
                        password += (char)ch;
                    }
                }
                break;
            }
        }
}