#include <ncurses/ncurses.h>
#include <chrono>
#include <iomanip>
#include <limits>
#include "Chatfiles/Register/register.h"
#include "Chatfiles/Login/login.h"

using namespace std;

void initializeNCurses() {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0); // Hide cursor for button navigation
    
    if (has_colors()) {
        start_color();
        init_pair(1, COLOR_CYAN, COLOR_BLACK);    // Header color
        init_pair(2, COLOR_GREEN, COLOR_BLACK);   // Success messages
        init_pair(3, COLOR_RED, COLOR_BLACK);     // Error messages
        init_pair(4, COLOR_YELLOW, COLOR_BLACK);  // Warning messages
        init_pair(5, COLOR_WHITE, COLOR_BLACK);   // Normal text
        init_pair(6, COLOR_BLACK, COLOR_GREEN);   // Active button
        init_pair(7, COLOR_WHITE, COLOR_BLACK);   // Inactive button
        init_pair(8, COLOR_BLUE, COLOR_BLACK);    // Instructions
    }
}

void cleanupNCurses() {
    endwin();
}

void displayMainMenu() {
    clear();
    
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);
    
    // Calculate center positions
    int center_x = max_x / 2;
    int center_y = max_y / 2;
    
    // Header
    std::string header = "=== TERMINAL CHAT APPLICATION ===";
    int header_x = center_x - header.length() / 2;
    
    if (has_colors()) {
        attron(COLOR_PAIR(1));
    }
    mvprintw(center_y - 8, header_x, "%s", header.c_str());
    if (has_colors()) {
        attroff(COLOR_PAIR(1));
    }
    
    // Subtitle
    std::string subtitle = "Welcome! Please select an option:";
    int subtitle_x = center_x - subtitle.length() / 2;
    mvprintw(center_y - 5, subtitle_x, "%s", subtitle.c_str());
    
    // Instructions (centered at bottom)
    if (has_colors()) {
        attron(COLOR_PAIR(8));
    }
    
    std::string nav_title = "Navigation:";
    std::string nav1 = "- Use UP/DOWN Arrow Keys to select option";
    std::string nav2 = "- Press ENTER or SPACE to confirm selection";
    std::string nav3 = "- Press ESC to exit";
    
    mvprintw(center_y + 6, center_x - nav_title.length() / 2, "%s", nav_title.c_str());
    mvprintw(center_y + 7, center_x - nav1.length() / 2, "%s", nav1.c_str());
    mvprintw(center_y + 8, center_x - nav2.length() / 2, "%s", nav2.c_str());
    mvprintw(center_y + 9, center_x - nav3.length() / 2, "%s", nav3.c_str());
    
    if (has_colors()) {
        attroff(COLOR_PAIR(8));
    }
    
    refresh();
}

void drawMainMenuButton(int y, int x, const std::string& text, bool active) {
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

void showMainMessage(const std::string& message, int colorPair) {
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

int main() {
    initializeNCurses();
    
    int selectedOption = 0; // 0 = Login, 1 = Register, 2 = Exit
    const int NUM_OPTIONS = 3;
    
    while (true) {
        // Get current terminal size
        int max_y, max_x;
        getmaxyx(stdscr, max_y, max_x);
        
        // Calculate dynamic positions
        int center_x = max_x / 2;
        int center_y = max_y / 2;
        
        // Button positions (vertically centered)
        const int LOGIN_Y = center_y - 2;
        const int REGISTER_Y = center_y;
        const int EXIT_Y = center_y + 2;
        
        displayMainMenu();
        
        // Draw buttons
        drawMainMenuButton(LOGIN_Y, center_x, "LOGIN", selectedOption == 0);
        drawMainMenuButton(REGISTER_Y, center_x, "REGISTER", selectedOption == 1);
        drawMainMenuButton(EXIT_Y, center_x, "EXIT", selectedOption == 2);
        
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
                    case 0: // Login
                        {
                            // Clear screen before transitioning
                            clear();
                            refresh();
                            
                            // Show login screen
                            showLoginScreen();
                            
                            // Reinitialize main menu when returning
                            initializeNCurses();
                        }
                        break;
                        
                    case 1: // Register
                        {
                            // Clear screen before transitioning
                            clear();
                            refresh();
                            
                            // Show register screen
                            showRegisterScreen();
                            
                            // Reinitialize main menu when returning
                            initializeNCurses();
                        }
                        break;
                        
                    case 2: // Exit
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
                            
                            // Brief pause before exit
                            napms(1500); // 1.5 second pause
                            
                            cleanupNCurses();
                            return 0;
                        }
                        break;
                }
                break;
                
            case 27: // ESC
                {
                    // Same exit behavior as selecting Exit button
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
                    napms(1500);
                    
                    cleanupNCurses();
                    return 0;
                }
                break;
                
            default:
                // Ignore other keys
                break;
        }
    }
    
    cleanupNCurses();
    return 0;
}