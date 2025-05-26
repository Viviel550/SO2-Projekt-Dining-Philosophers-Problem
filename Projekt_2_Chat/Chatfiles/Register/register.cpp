#include "register.h"
#include "registerback.h"
#include <ncurses/ncurses.h>
#include <iostream>
#include <string>

void initializeRegisterScreen() {
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

void cleanupRegisterScreen() {
    endwin();
}

void drawRegisterInterface() {
    clear();
    
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);
    
    // Calculate center positions
    int center_x = max_x / 2;
    int center_y = max_y / 2;
    
    // Header
    std::string header = "===== USER REGISTRATION =====";
    int header_x = center_x - header.length() / 2;
    
    if (has_colors()) {
        attron(COLOR_PAIR(1));
    }
    mvprintw(center_y - 10, header_x, "%s", header.c_str());
    if (has_colors()) {
        attroff(COLOR_PAIR(1));
    }
    
    // Labels (positioned relative to center)
    mvprintw(center_y - 7, center_x - 20, "Username:");
    mvprintw(center_y - 5, center_x - 20, "Email:");
    mvprintw(center_y - 3, center_x - 20, "Password:");
    mvprintw(center_y - 1, center_x - 20, "Confirm Password:");
    
    // Instructions (centered at bottom)
    if (has_colors()) {
        attron(COLOR_PAIR(6));
    }
    
    std::string nav_title = "Navigation:";
    std::string nav1 = "- Use TAB or Arrow Keys to move between fields/buttons";
    std::string nav2 = "- Press ENTER or SPACE to activate selected button";
    std::string nav3 = "- Press ESC to cancel";
    
    mvprintw(center_y + 5, center_x - nav_title.length() / 2, "%s", nav_title.c_str());
    mvprintw(center_y + 6, center_x - nav1.length() / 2, "%s", nav1.c_str());
    mvprintw(center_y + 7, center_x - nav2.length() / 2, "%s", nav2.c_str());
    mvprintw(center_y + 8, center_x - nav3.length() / 2, "%s", nav3.c_str());
    
    if (has_colors()) {
        attroff(COLOR_PAIR(6));
    }
    
    refresh();
}

bool isValidEmail(const std::string& email) {
    // Check if email is empty
    if (email.empty()) {
        return false;
    }
    
    // Find @ symbol
    size_t atPos = email.find('@');
    if (atPos == std::string::npos || atPos == 0 || atPos == email.length() - 1) {
        return false; // No @ found, or @ is at the beginning or end
    }
    
    // Check if there's only one @ symbol
    if (email.find('@', atPos + 1) != std::string::npos) {
        return false; // Multiple @ symbols found
    }
    
    // Extract domain part (after @)
    std::string domain = email.substr(atPos + 1);
    
    // Check if domain has at least one dot
    size_t dotPos = domain.find('.');
    if (dotPos == std::string::npos || dotPos == 0 || dotPos == domain.length() - 1) {
        return false; // No dot in domain, or dot is at the beginning or end of domain
    }
    
    // Check if there's at least one character before the dot and at least 2 characters after the dot
    if (dotPos < 1 || domain.length() - dotPos - 1 < 2) {
        return false;
    }
    
    // Check for valid characters in local part (before @)
    std::string localPart = email.substr(0, atPos);
    for (char c : localPart) {
        if (!std::isalnum(c) && c != '.' && c != '_' && c != '-' && c != '+') {
            return false;
        }
    }
    
    // Check for valid characters in domain part
    for (char c : domain) {
        if (!std::isalnum(c) && c != '.' && c != '-') {
            return false;
        }
    }
    
    // Check that domain doesn't start or end with a dot or hyphen
    if (domain[0] == '.' || domain[0] == '-' || 
        domain[domain.length() - 1] == '.' || domain[domain.length() - 1] == '-') {
        return false;
    }
    
    return true;
}

void drawRegisterInputField(int y, int x, int width, const std::string& content, bool active, bool isPassword = false) {
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
    
    // Display content (masked if password)
    if (isPassword) {
        std::string maskedContent(content.length(), '*');
        mvprintw(y, x, "%-*s", width - 1, maskedContent.c_str());
    } else {
        mvprintw(y, x, "%-*s", width - 1, content.c_str());
    }
    
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

void drawRegisterButton(int y, int x, const std::string& text, bool active) {
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

void showRegisterMessage(const std::string& message, int colorPair) {
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

void showRegisterScreen() {
    initializeRegisterScreen();
    
    std::string username = "";
    std::string email = "";
    std::string password = "";
    std::string confirmPassword = "";
    int currentField = 0; // 0 = username, 1 = email, 2 = password, 3 = confirm password, 4 = register button, 5 = cancel button
    
    while (true) {
        // Get current terminal size
        int max_y, max_x;
        getmaxyx(stdscr, max_y, max_x);
        
        // Calculate dynamic positions based on terminal size
        int center_x = max_x / 2;
        int center_y = max_y / 2;
        
        const int FIELD_WIDTH = std::min(40, max_x - 10); // Responsive field width
        const int USERNAME_Y = center_y - 6;
        const int EMAIL_Y = center_y - 4;
        const int PASSWORD_Y = center_y - 2;
        const int CONFIRM_PASSWORD_Y = center_y;
        const int FIELD_X = center_x - FIELD_WIDTH / 2;
        const int BUTTON_Y = center_y + 3;
        
        // Center the buttons
        std::string register_text = "[ REGISTER ]";
        std::string cancel_text = "[ CANCEL ]";
        int total_button_width = register_text.length() + cancel_text.length() + 4; // 4 spaces between
        int REGISTER_BUTTON_X = center_x - total_button_width / 2;
        int CANCEL_BUTTON_X = REGISTER_BUTTON_X + register_text.length() + 4;
        
        drawRegisterInterface();
        
        // Draw input fields
        drawRegisterInputField(USERNAME_Y, FIELD_X, FIELD_WIDTH, username, currentField == 0);
        drawRegisterInputField(EMAIL_Y, FIELD_X, FIELD_WIDTH, email, currentField == 1);
        drawRegisterInputField(PASSWORD_Y, FIELD_X, FIELD_WIDTH, password, currentField == 2, true);
        drawRegisterInputField(CONFIRM_PASSWORD_Y, FIELD_X, FIELD_WIDTH, confirmPassword, currentField == 3, true);
        
        // Draw buttons
        drawRegisterButton(BUTTON_Y, REGISTER_BUTTON_X, "REGISTER", currentField == 4);
        drawRegisterButton(BUTTON_Y, CANCEL_BUTTON_X, "CANCEL", currentField == 5);
        
        // Position cursor appropriately
        if (currentField == 0) {
            move(USERNAME_Y, FIELD_X + std::min((int)username.length(), FIELD_WIDTH - 2));
        } else if (currentField == 1) {
            move(EMAIL_Y, FIELD_X + std::min((int)email.length(), FIELD_WIDTH - 2));
        } else if (currentField == 2) {
            move(PASSWORD_Y, FIELD_X + std::min((int)password.length(), FIELD_WIDTH - 2));
        } else if (currentField == 3) {
            move(CONFIRM_PASSWORD_Y, FIELD_X + std::min((int)confirmPassword.length(), FIELD_WIDTH - 2));
        } else if (currentField == 4) {
            move(BUTTON_Y, REGISTER_BUTTON_X + 1);
            curs_set(0); // Hide cursor on buttons
        } else if (currentField == 5) {
            move(BUTTON_Y, CANCEL_BUTTON_X + 1);
            curs_set(0); // Hide cursor on buttons
        }
        
        if (currentField <= 3) {
            curs_set(1); // Show cursor on input fields
        }
        
        int ch = getch();
        
        switch (ch) {
            case KEY_UP:
                // Move up in sequence
                if (currentField == 0) {
                    currentField = 5; // Username → CANCEL (wrap around)
                } else {
                    currentField--; // Move to previous field
                }
                break;
                
            case KEY_DOWN:
                // Move down in sequence: Username → Email → Password → Confirm → REGISTER → CANCEL
                if (currentField == 5) {
                    currentField = 0; // CANCEL → Username (wrap around)
                } else {
                    currentField++; // Move to next field
                }
                break;
                
            case KEY_LEFT:
                if (currentField >= 4) {
                    // Only allow left/right navigation between buttons
                    currentField = (currentField == 5) ? 4 : 5; // Toggle between REGISTER and CANCEL
                }
                break;
                
            case KEY_RIGHT:
                if (currentField >= 4) {
                    // Only allow left/right navigation between buttons
                    currentField = (currentField == 4) ? 5 : 4; // Toggle between REGISTER and CANCEL
                }
                break;
                
            case 9: // TAB
                // Tab cycles through all fields in order
                currentField = (currentField + 1) % 6;
                break;
                
            case 10: // ENTER
            case 13: // ENTER  
            case 32: // SPACE
                if (currentField == 4) { // REGISTER button
                    if (username.empty() || email.empty() || password.empty() || confirmPassword.empty()) {
                        showRegisterMessage("Please fill in all fields.", 5); // Red
                    }else if (!isValidEmail(email)) {
                        showRegisterMessage("Invalid email format. Please try again.", 5); // Red
                    } else if (username.length() < 3 || username.length() > 20) {
                        showRegisterMessage("Username must be between 3 and 20 characters.", 5); // Red
                    } else if (password.length() < 6 || password.length() > 20) {
                        showRegisterMessage("Password must be between 6 and 20 characters.", 5); // Red
                    } else if (password != confirmPassword) {
                        showRegisterMessage("Passwords do not match. Please try again.", 5); // Red
                    } else {
                        // Show processing message
                        mvprintw(center_y + 5, center_x - 10, "Processing registration...");
                        refresh();
                        
                        // Check if email already exists
                        if (checkEmailExists(email)) {
                            showRegisterMessage("Email already exists. Please choose another one.", 5); // Red
                        } else {
                            // Attempt registration
                            if (registerUser(username, password, email)) {
                                showRegisterMessage("Registration successful! You can now login.", 4); // Green
                                cleanupRegisterScreen();
                                return;
                            } else {
                                showRegisterMessage("Registration failed. Please try again later.", 5); // Red
                            }
                        }
                    }
                } else if (currentField == 5) { // CANCEL button
                    cleanupRegisterScreen();
                    return;
                }
                // If ENTER is pressed on input fields, move to next field
                else if (currentField < 4) {
                    currentField++; // Move to next field
                }
                break;
                
            case 27: // ESC
                cleanupRegisterScreen();
                return;
                
            default:
                // Handle input only for input fields (not buttons)
                if (currentField == 0) {
                    // Username field
                    if (ch == KEY_BACKSPACE || ch == 127 || ch == 8) {
                        if (!username.empty()) {
                            username.pop_back();
                        }
                    } else if (ch >= 32 && ch <= 126 && username.length() < FIELD_WIDTH - 2) {
                        username += (char)ch;
                    }
                } else if (currentField == 1) {
                    // Email field
                    if (ch == KEY_BACKSPACE || ch == 127 || ch == 8) {
                        if (!email.empty()) {
                            email.pop_back();
                        }
                    } else if (ch >= 32 && ch <= 126 && email.length() < FIELD_WIDTH - 2) {
                        email += (char)ch;
                    }
                } else if (currentField == 2) {
                    // Password field
                    if (ch == KEY_BACKSPACE || ch == 127 || ch == 8) {
                        if (!password.empty()) {
                            password.pop_back();
                        }
                    } else if (ch >= 32 && ch <= 126 && password.length() < FIELD_WIDTH - 2) {
                        password += (char)ch;
                    }
                } else if (currentField == 3) {
                    // Confirm Password field
                    if (ch == KEY_BACKSPACE || ch == 127 || ch == 8) {
                        if (!confirmPassword.empty()) {
                            confirmPassword.pop_back();
                        }
                    } else if (ch >= 32 && ch <= 126 && confirmPassword.length() < FIELD_WIDTH - 2) {
                        confirmPassword += (char)ch;
                    }
                }
                break;
        }
    }
}