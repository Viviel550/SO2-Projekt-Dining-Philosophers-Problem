#include "chatterminalf.h"
#include "chatterminalback.h"
#include "chatcommands.h"
#include "../FriendList/friendlistback.h"
#include "../FriendList/friendlist.h"
#include <ncurses/ncurses.h>
#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <chrono>
#include <algorithm>
#include <ctime>

// Global variables
std::mutex chatMutex;
WINDOW* chatWindow = nullptr;
WINDOW* inputWindow = nullptr;
WINDOW* headerWindow = nullptr;
WINDOW* buttonWindow = nullptr;
std::vector<Message> globalChatHistory;
int scrollPosition = 0;
bool needsRefresh = true;
int totalLoadedMessages = 0;
bool isLoadingOlderMessages = false;
bool hasMoreOlderMessages = true;
std::string currentUserDisplay = "";
std::string otherUserDisplay = "";
int selectedButton = 0;
bool isInButtonArea = false;
int currentChatUserId = 0;
static bool hasNewMessagesFromFriends = false;
static auto lastFriendMessageCheck = std::chrono::steady_clock::now();

// Function declarations
void loadOlderMessages(int userId, int otherUserId);
void updateRecentMessages(int userId, int otherUserId);
void handleKeyInput(int ch, std::string& currentInput, int userId, int otherUserId, bool& chatting);
void handleEnterKey(const std::string& currentInput, std::string& inputBuffer, int userId, int otherUserId, bool& chatting);
void handleBackspace(std::string& currentInput);
void handleScrollUp();
void handleScrollDown();
void refreshChatDisplay(int userId, const std::string& currentInput);
void drawButtonArea();
void drawChatHeader(const std::string& otherUserName);
void drawChatHistory(int userId);
void drawInputArea(const std::string& currentInput);

bool hasNewMessagesFromAnyFriend(int userId) {
    auto now = std::chrono::steady_clock::now();
    
    // Check for new messages every 3 seconds regardless of what the user is doing
    if (std::chrono::duration_cast<std::chrono::seconds>(now - lastFriendMessageCheck).count() >= 3) {
        try {
            std::vector<FriendInfo> friends = getFriendsList(userId);
            
            hasNewMessagesFromFriends = false;
            for (const auto& friendInfo : friends) {
                if (friendInfo.hasNewMessages) {
                    hasNewMessagesFromFriends = true;
                    break;
                }
            }
        } catch (const std::exception& e) {
            // Handle error silently to avoid breaking the UI
        }
        lastFriendMessageCheck = now;
    }
    
    return hasNewMessagesFromFriends;
}

// Terminal initialization
void initializeChatTerminal() {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(1);
    
    if (has_colors()) {
        start_color();
        init_pair(1, COLOR_CYAN, COLOR_BLACK);
        init_pair(2, COLOR_GREEN, COLOR_BLACK);
        init_pair(3, COLOR_YELLOW, COLOR_BLACK);
        init_pair(4, COLOR_WHITE, COLOR_BLACK);
        init_pair(5, COLOR_RED, COLOR_BLACK);
        init_pair(6, COLOR_BLUE, COLOR_BLACK);
        init_pair(7, COLOR_BLACK, COLOR_GREEN);
        init_pair(8, COLOR_WHITE, COLOR_BLACK);
    }
    
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);
    
    buttonWindow = newwin(3, max_x, 0, 0);
    headerWindow = newwin(3, max_x, 3, 0);
    chatWindow = newwin(max_y - 9, max_x, 6, 0);
    scrollok(chatWindow, TRUE);
    inputWindow = newwin(3, max_x, max_y - 3, 0);
    
    refresh();
}

void cleanupChatTerminal() {
    if (buttonWindow) delwin(buttonWindow);
    if (headerWindow) delwin(headerWindow);
    if (chatWindow) delwin(chatWindow);
    if (inputWindow) delwin(inputWindow);
    endwin();
}

// Button handling functions
void handleButtonExit(bool& chatting) {
    chatting = false;
}

void handleButtonFriends(int userId, bool& chatting) {
    chatting = false;
    cleanupChatTerminal();
    showFriendsListScreen(userId, currentUserDisplay, otherUserDisplay);
}

void showInviteSuccessMessage() {
    if (has_colors()) {
        wattron(buttonWindow, COLOR_PAIR(2));
    }
    mvwprintw(buttonWindow, 0, 2, "Friend request sent!");
    if (has_colors()) {
        wattroff(buttonWindow, COLOR_PAIR(2));
    }
    wrefresh(buttonWindow);
    napms(1500);
}

void showInviteErrorMessage() {
    if (has_colors()) {
        wattron(buttonWindow, COLOR_PAIR(5));
    }
    mvwprintw(buttonWindow, 0, 2, "Failed to send friend request or already friends.");
    if (has_colors()) {
        wattroff(buttonWindow, COLOR_PAIR(5));
    }
    wrefresh(buttonWindow);
    napms(1500);
}

void handleButtonInvite(int userId, int otherUserId) {
    if (sendFriendRequest(userId, otherUserId)) {
        showInviteSuccessMessage();
    } else {
        showInviteErrorMessage();
    }
    needsRefresh = true;
}

void handleButtonSelection(int userId, int otherUserId, bool& chatting) {
    switch (selectedButton) {
        case 0:
            handleButtonExit(chatting);
            break;
        case 1:
            handleButtonFriends(userId, chatting);
            break;
        case 2:
            handleButtonInvite(userId, otherUserId);
            break;
    }
}

void moveButtonLeft() {
    selectedButton = (selectedButton == 0) ? 2 : selectedButton - 1;
    needsRefresh = true;
}

void moveButtonRight() {
    selectedButton = (selectedButton + 1) % 3;
    needsRefresh = true;
}

void handleButtonAreaKeys(int ch, int userId, int otherUserId, bool& chatting) {
    switch (ch) {
        case KEY_LEFT:
            moveButtonLeft();
            break;
        case KEY_RIGHT:
            moveButtonRight();
            break;
        case 10:
        case 13:
        case 32:
            handleButtonSelection(userId, otherUserId, chatting);
            break;
        case 27:
            chatting = false;
            break;
    }
}

// Area switching
void switchToButtonArea() {
    isInButtonArea = true;
    curs_set(0);
    needsRefresh = true;
}

void switchToChatArea() {
    isInButtonArea = false;
    curs_set(1);
    needsRefresh = true;
}

void handleTabKey() {
    if (isInButtonArea) {
        switchToChatArea();
    } else {
        switchToButtonArea();
    }
}

// Scrolling functions
bool shouldLoadOlderMessages() {
    return scrollPosition >= (int)globalChatHistory.size() - 10 && 
           hasMoreOlderMessages && 
           !isLoadingOlderMessages;
}

void handleScrollUp() {
    if (!isInButtonArea) {
        scrollPosition++;
        if (shouldLoadOlderMessages()) {
            isLoadingOlderMessages = true;
        }
    }
}

void handleScrollDown() {
    if (!isInButtonArea) {
        scrollPosition = std::max(0, scrollPosition - 1);
    }
}

void handleScrollKeys(int ch) {
    switch (ch) {
        case KEY_UP:
            handleScrollUp();
            needsRefresh = true;
            break;
        case KEY_DOWN:
            handleScrollDown();
            needsRefresh = true;
            break;
    }
}

// Input handling functions
void handleBackspace(std::string& currentInput) {
    if (!currentInput.empty() && !isInButtonArea) {
        currentInput.pop_back();
    }
}

void handleRegularCharacter(char ch, std::string& currentInput) {
    if (ch >= 32 && ch <= 126 && !isInButtonArea) {
        currentInput += ch;
    }
}

void processCommandInput(const std::string& currentInput, std::string& inputBuffer, int userId, int otherUserId, bool& chatting) {
    processCommand(currentInput, userId, otherUserId, inputBuffer, chatting, needsRefresh);
}

void updateChatAfterSendMessage(int userId, int otherUserId) {
    scrollPosition = 0;
    std::lock_guard<std::mutex> lock(chatMutex);
    globalChatHistory = getLatestChatHistory(userId, otherUserId, 50);
    totalLoadedMessages = std::max(totalLoadedMessages, (int)globalChatHistory.size());
}

void processRegularMessage(const std::string& currentInput, std::string& inputBuffer, int userId, int otherUserId) {
    if (sendMessage(userId, otherUserId, currentInput)) {
        inputBuffer = "";
        updateChatAfterSendMessage(userId, otherUserId);
        needsRefresh = true;
    }
}

void handleEnterKey(const std::string& currentInput, std::string& inputBuffer, int userId, int otherUserId, bool& chatting) {
    if (currentInput.empty() || isInButtonArea) return;
    
    if (currentInput[0] == '/') {
        processCommandInput(currentInput, inputBuffer, userId, otherUserId, chatting);
    } else {
        processRegularMessage(currentInput, inputBuffer, userId, otherUserId);
    }
}

void handleChatInputKeys(int ch, std::string& currentInput, int userId, int otherUserId, bool& chatting) {
    switch (ch) {
        case 10:
        case 13:
            handleEnterKey(currentInput, currentInput, userId, otherUserId, chatting);
            needsRefresh = true;
            break;
        case KEY_BACKSPACE:
        case 127:
        case 8:
            handleBackspace(currentInput);
            needsRefresh = true;
            break;
        case 27:
            chatting = false;
            break;
    }
}

void handleChatAreaKeys(int ch, std::string& currentInput, int userId, int otherUserId, bool& chatting) {
    if (ch == KEY_UP || ch == KEY_DOWN) {
        handleScrollKeys(ch);
        return;
    }
    
    if (ch == 10 || ch == 13 || ch == KEY_BACKSPACE || ch == 127 || ch == 8 || ch == 27) {
        handleChatInputKeys(ch, currentInput, userId, otherUserId, chatting);
        return;
    }
    
    handleRegularCharacter((char)ch, currentInput);
    if (ch >= 32 && ch <= 126) {
        needsRefresh = true;
    }
}

// Main key input handler
void handleKeyInput(int ch, std::string& currentInput, int userId, int otherUserId, bool& chatting) {
    if (ch == 9) {
        handleTabKey();
        return;
    }
    
    if (isInButtonArea) {
        handleButtonAreaKeys(ch, userId, otherUserId, chatting);
    } else {
        handleChatAreaKeys(ch, currentInput, userId, otherUserId, chatting);
    }
}

// Drawing functions
void drawButtonArea() {
    if (!buttonWindow) return;
    
    wclear(buttonWindow);
    
    int max_x = getmaxx(buttonWindow);
    
    mvwprintw(buttonWindow, 2, 0, "%s", std::string(max_x, '-').c_str());
    
    int button_spacing = max_x / 4;
    std::vector<std::string> button_texts = {"EXIT", "FRIENDS", "INVITE"};
    
    // Check for new messages (this will update the global cache if needed)
    bool hasNewMessages = false;
    if (currentChatUserId > 0) {
        hasNewMessages = hasNewMessagesFromAnyFriend(currentChatUserId);
    }
    
    for (int i = 0; i < 3; i++) {
        int button_x = button_spacing * (i + 1) - button_texts[i].length() / 2 - 1;
        
        // Determine color for the button
        int colorPair = 8; // Default inactive color
        
        if (i == selectedButton && isInButtonArea) {
            colorPair = 7; // Active button (selected)
        } else if (i == 1 && hasNewMessages) { // FRIENDS button
            colorPair = 5; // Red color for new messages (COLOR_PAIR(5) is red)
        }
        
        if (has_colors()) {
            wattron(buttonWindow, COLOR_PAIR(colorPair));
        }
        
        std::string button_display = "[ " + button_texts[i] + " ]";
        
        // Add visual indicator for new messages
        if (i == 1 && hasNewMessages) {
            button_display = "[ " + button_texts[i] + " (!) ]";
        }
        
        mvwprintw(buttonWindow, 1, button_x, "%s", button_display.c_str());
        
        if (has_colors()) {
            wattroff(buttonWindow, COLOR_PAIR(colorPair));
        }
    }
    
    if (isInButtonArea) {
        if (has_colors()) {
            wattron(buttonWindow, COLOR_PAIR(6));
        }
        std::string instruction = "LEFT/RIGHT: Navigate buttons | ENTER: Select | TAB: Switch to chat";
        int instr_x = (max_x - instruction.length()) / 2;
        mvwprintw(buttonWindow, 0, instr_x, "%s", instruction.c_str());
        if (has_colors()) {
            wattroff(buttonWindow, COLOR_PAIR(6));
        }
    }
    
    wrefresh(buttonWindow);
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
    
    if (!isInButtonArea) {
        if (has_colors()) {
            wattron(headerWindow, COLOR_PAIR(6));
        }
        
        std::string instructions = "Type message and press ENTER | UP/DOWN to scroll | TAB for buttons";
        int instr_x = (max_x - instructions.length()) / 2;
        mvwprintw(headerWindow, 1, instr_x, "%s", instructions.c_str());
        
        if (has_colors()) {
            wattroff(headerWindow, COLOR_PAIR(6));
        }
    }
    
    mvwprintw(headerWindow, 2, 0, "%s", std::string(max_x, '-').c_str());
    
    wrefresh(headerWindow);
}

void drawEmptyChat() {
    if (has_colors()) {
        wattron(chatWindow, COLOR_PAIR(4));
    }
    mvwprintw(chatWindow, 0, 2, "No messages yet. Start typing to begin the conversation!");
    if (has_colors()) {
        wattroff(chatWindow, COLOR_PAIR(4));
    }
    wrefresh(chatWindow);
}

void calculateMessageRange(int totalMessages, int max_y, int& startIndex, int& endIndex) {
    int visibleMessages = max_y - 1;
    
    scrollPosition = std::max(0, std::min(scrollPosition, std::max(0, totalMessages - visibleMessages)));
    
    if (scrollPosition >= totalMessages - visibleMessages && hasMoreOlderMessages && !isLoadingOlderMessages) {
        isLoadingOlderMessages = true;
    }
    
    endIndex = totalMessages - scrollPosition;
    startIndex = std::max(0, endIndex - visibleMessages);
    
    endIndex = std::min(totalMessages, endIndex);
    startIndex = std::max(0, startIndex);
}

void drawMessage(const Message& msg, int userId, int line, int max_x) {
    std::string displayLine;
    std::string timeStr = msg.timestamp.length() >= 19 ? msg.timestamp.substr(11, 8) : msg.timestamp;
    
    if (msg.senderId == userId) {
        if (has_colors()) {
            wattron(chatWindow, COLOR_PAIR(2));
        }
        displayLine = "[" + timeStr + "] " + currentUserDisplay + ": " + msg.content;
    } else {
        if (has_colors()) {
            wattron(chatWindow, COLOR_PAIR(3));
        }
        displayLine = "[" + timeStr + "] " + otherUserDisplay + ": " + msg.content;
    }
    
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
}

void drawScrollIndicators(int max_y, int max_x, int endIndex, int totalMessages) {
    if (isLoadingOlderMessages) {
        if (has_colors()) {
            wattron(chatWindow, COLOR_PAIR(6));
        }
        mvwprintw(chatWindow, max_y - 1, max_x / 2 - 10, "Loading older messages...");
        if (has_colors()) {
            wattroff(chatWindow, COLOR_PAIR(6));
        }
    } else {
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
    
    if (endIndex < totalMessages) {
        if (has_colors()) {
            wattron(chatWindow, COLOR_PAIR(6));
        }
        mvwprintw(chatWindow, max_y - 1, 1, "↓ More below ↓");
        if (has_colors()) {
            wattroff(chatWindow, COLOR_PAIR(6));
        }
    }
}

void drawChatHistory(int userId) {
    if (!chatWindow) return;
    
    std::lock_guard<std::mutex> lock(chatMutex);
    
    wclear(chatWindow);
    
    if (globalChatHistory.empty()) {
        drawEmptyChat();
        return;
    }
    
    int max_y = getmaxy(chatWindow);
    int max_x = getmaxx(chatWindow);
    
    int totalMessages = globalChatHistory.size();
    int startIndex, endIndex;
    
    calculateMessageRange(totalMessages, max_y, startIndex, endIndex);
    
    int line = 0;
    for (int i = startIndex; i < endIndex && line < max_y - 1; i++) {
        const Message& msg = globalChatHistory[i];
        drawMessage(msg, userId, line, max_x);
        line++;
    }
    
    drawScrollIndicators(max_y, max_x, endIndex, totalMessages);
    
    wrefresh(chatWindow);
}

void drawInputArea(const std::string& currentInput) {
    if (!inputWindow) return;
    
    wclear(inputWindow);
    
    int max_x = getmaxx(inputWindow);
    
    mvwprintw(inputWindow, 0, 0, "%s", std::string(max_x, '-').c_str());
    
    std::string prompt = "Message: ";
    mvwprintw(inputWindow, 1, 1, "%s", prompt.c_str());
    
    std::string displayInput = currentInput;
    int maxInputLength = max_x - prompt.length() - 3;
    
    if (displayInput.length() > maxInputLength) {
        displayInput = "..." + displayInput.substr(displayInput.length() - maxInputLength + 3);
    }
    
    mvwprintw(inputWindow, 1, prompt.length() + 1, "%s", displayInput.c_str());
    
    if (!isInButtonArea) {
        int cursorPos = std::min((int)displayInput.length(), maxInputLength);
        wmove(inputWindow, 1, prompt.length() + 1 + cursorPos);
    }
    
    wrefresh(inputWindow);
}

void refreshChatDisplay(int userId, const std::string& currentInput) {
    drawButtonArea();
    drawChatHeader(otherUserDisplay);
    drawChatHistory(userId);
    drawInputArea(currentInput);
}

// Message loading functions
void insertOlderMessages(const std::vector<Message>& olderMessages) {
    globalChatHistory.insert(globalChatHistory.begin(), olderMessages.begin(), olderMessages.end());
    totalLoadedMessages += olderMessages.size();
    scrollPosition += olderMessages.size();
    
    if (olderMessages.size() < 50) {
        hasMoreOlderMessages = false;
    }
}

void loadOlderMessages(int userId, int otherUserId) {
    if (!hasMoreOlderMessages) return;
    
    std::vector<Message> olderMessages = getOlderChatHistory(userId, otherUserId, 50, totalLoadedMessages);
    
    std::lock_guard<std::mutex> lock(chatMutex);
    
    if (!olderMessages.empty()) {
        insertOlderMessages(olderMessages);
    } else {
        hasMoreOlderMessages = false;
    }
    
    isLoadingOlderMessages = false;
    needsRefresh = true;
}

bool checkForNewMessages(const std::vector<Message>& latestMessages) {
    if (latestMessages.size() != globalChatHistory.size()) {
        return true;
    }
    
    if (!latestMessages.empty() && !globalChatHistory.empty()) {
        const Message& newLast = latestMessages.back();
        const Message& oldLast = globalChatHistory.back();
        
        if (newLast.content != oldLast.content || 
            newLast.timestamp != oldLast.timestamp) {
            return true;
        }
    }
    
    return false;
}

void updateChatHistory(const std::vector<Message>& latestMessages) {
    int oldSize = globalChatHistory.size();
    
    if (totalLoadedMessages > 50) {
        globalChatHistory.erase(globalChatHistory.end() - std::min(50, oldSize), globalChatHistory.end());
        globalChatHistory.insert(globalChatHistory.end(), latestMessages.begin(), latestMessages.end());
    } else {
        globalChatHistory = latestMessages;
        totalLoadedMessages = latestMessages.size();
    }
}

void updateRecentMessages(int userId, int otherUserId) {
    if (scrollPosition > 5) return;
    
    std::vector<Message> latestMessages = getLatestChatHistory(userId, otherUserId, 50);
    
    std::lock_guard<std::mutex> lock(chatMutex);
    
    bool hasNewMessages = checkForNewMessages(latestMessages);
    
    if (hasNewMessages) {
        updateChatHistory(latestMessages);
        needsRefresh = true;
    }
}

void processMessageUpdates(int userId, int otherUserId) {
    if (isLoadingOlderMessages && hasMoreOlderMessages) {
        loadOlderMessages(userId, otherUserId);
    } else {
        updateRecentMessages(userId, otherUserId);
    }
}

void receiveMessages(int userId, int otherUserId, bool& chatting) {
    while (chatting) {
        try {
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            processMessageUpdates(userId, otherUserId);
            
            // Also check for friend messages and trigger refresh if needed
            static bool lastFriendMessageState = false;
            bool currentFriendMessageState = hasNewMessagesFromAnyFriend(userId);
            
            if (currentFriendMessageState != lastFriendMessageState) {
                needsRefresh = true;
                lastFriendMessageState = currentFriendMessageState;
            }
            
        } catch (const std::exception& e) {
            isLoadingOlderMessages = false;
        }
    }
}

// Initialization functions
void initializeChatVariables() {
    totalLoadedMessages = 0;
    isLoadingOlderMessages = false;
    hasMoreOlderMessages = true;
    scrollPosition = 0;
}

void loadInitialChatHistory(int userId, int otherUserId) {
    std::lock_guard<std::mutex> lock(chatMutex);
    globalChatHistory = getLatestChatHistory(userId, otherUserId, 50);
    totalLoadedMessages = globalChatHistory.size();
    
    if (globalChatHistory.size() < 50) {
        hasMoreOlderMessages = false;
    }
}

void initializeChatSession(int userId, int otherUserId) {
    initializeChatVariables();
    loadInitialChatHistory(userId, otherUserId);
}

void setCurrentUserDisplay(const std::string& userName, const std::string& userNameId) {
    currentUserDisplay = userName + "#" + userNameId;
}

void setOtherUserDisplay(const std::string& otherUserName, const std::string& otherUserNameId, int otherUserId) {
    if (!otherUserName.empty() && !otherUserNameId.empty()) {
        otherUserDisplay = otherUserName + "#" + otherUserNameId;
    } else {
        UserInfo otherUser = getUserInfo(otherUserId);
        otherUserDisplay = otherUser.userName + "#" + otherUser.userNameId;
    }
}

void setupUserDisplayNames(const std::string& userName, const std::string& userNameId, 
                          const std::string& otherUserName, const std::string& otherUserNameId, int otherUserId) {
    setCurrentUserDisplay(userName, userNameId);
    setOtherUserDisplay(otherUserName, otherUserNameId, otherUserId);
}

void initializeChatState() {
    isInButtonArea = false;
    selectedButton = 0;
}

// Main loop functions
void processUserInput(int ch, std::string& currentInput, int userId, int otherUserId, bool& chatting) {
    if (ch != ERR) {
        handleKeyInput(ch, currentInput, userId, otherUserId, chatting);
    }
}

void updateDisplayIfNeeded(int userId, const std::string& currentInput) {
    if (needsRefresh) {
        needsRefresh = false;
        refreshChatDisplay(userId, currentInput);
    }
}

void runChatLoop(int userId, int otherUserId, bool& chatting) {
    std::string currentInput = "";
    
    timeout(500);
    
    while (chatting) {
        int ch = getch();
        
        processUserInput(ch, currentInput, userId, otherUserId, chatting);
        updateDisplayIfNeeded(userId, currentInput);
    }
    
    timeout(-1);
}

void startMessageReceiver(int userId, int otherUserId, bool& chatting, std::thread& receiverThread) {
    receiverThread = std::thread(receiveMessages, userId, otherUserId, std::ref(chatting));
}

void cleanupChatSession(bool& chatting, std::thread& receiverThread) {
    chatting = false;
    receiverThread.join();
    cleanupChatTerminal();
}

// Main function
int showChatTerminal(int userId, int otherUserId, const std::string& userName, const std::string& userNameId, 
                    const std::string& otherUserName, const std::string& otherUserNameId) {
    // Set the global user ID for button color checking
    currentChatUserId = userId;
    
    initializeChatTerminal();
    
    setupUserDisplayNames(userName, userNameId, otherUserName, otherUserNameId, otherUserId);
    initializeChatSession(userId, otherUserId);
    
    bool chatting = true;
    initializeChatState();
    
    std::thread receiverThread;
    startMessageReceiver(userId, otherUserId, chatting, receiverThread);
    
    refreshChatDisplay(userId, "");
    
    runChatLoop(userId, otherUserId, chatting);
    
    cleanupChatSession(chatting, receiverThread);
    
    // Reset the global user ID when leaving chat
    currentChatUserId = 0;
    
    return 0;
}