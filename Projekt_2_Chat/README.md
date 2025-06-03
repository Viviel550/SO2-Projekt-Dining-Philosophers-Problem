# Terminalowy Chat PostgreSQL

## Opis projektu
Aplikacja czatu terminalowego umożliwia komunikację tekstową pomiędzy użytkownikami z wykorzystaniem bazy danych PostgreSQL. Program pozwala na rejestrację, logowanie, wybór rozmówcy po jego ID oraz prowadzenie rozmowy w czasie rzeczywistym. Interfejs działa w terminalu i obsługuje kolorowe komunikaty.

## Funkcjonalności
- **Rejestracja** – zakładanie nowego konta użytkownika
- **Logowanie** – uwierzytelnianie użytkownika
- **Dodawania do Przyjaciół** - umożliwia szybką komunikacje z znanymi użytkonikami
- **Wybór rozmówcy po ID** – możliwość wyboru użytkownika do rozmowy
- **Pisanie i odbieranie wiadomości** – dwukierunkowa komunikacja tekstowa

## Wymagania
- C++ 17+
- CMake 3.10+
- PostgreSQL 12+
- Biblioteka `libpq` lub `libpqxx` (klient PostgreSQL dla C++)
- Kompilator: MinGW-w64 GCC lub MSVC z obsługą C++17
- Generator: Ninja (zalecany) lub Make
- NCurses - interfejs terminalowy z obsługą Unicode

## Platforma
- Windows: MSYS2/MinGW64 (zalecane)
- Terminal obsługujący:
    - Kody ANSI (kolorowe wyniki)
    - Unicode/UTF-8
    - Klawisze funkcyjne (strzałki, Tab, Enter)

## Instrukcja uruchomienia projektu
1. Pobierz repozytorium lub sklonuj:
    ```sh
    git clone https://github.com/Viviel550/SO2-Projekt-Dining-Philosophers-Problem.git
    cd SO2-Projekt-Dining-Philosophers-Problem\Projekt_2_Chat
    ```
2. Utwórz bazę danych i załaduj strukturę:
    ```sh
    createdb Chat
    psql Chat < Chat.sql
    ```
3. Skompiluj projekt:
    1. **Bezpośrednio**
       - **Windows (MSYS2/MinGW):**
        ```sh
        g++ -std=c++17 main.cpp Chatfiles/Login/*.cpp Chatfiles/Register/*.cpp Chatfiles/MainTerminal/*.cpp Chatfiles/ChatTerminal/*.cpp Chatfiles/FriendList/*.cpp  -o chat_app.exe -lpqxx -lpq -lncursesw
        ```
    2. **Przy użyciu CMakeList z generatorem Ninja**
        ```sh
        mkdir build
        cd build
        cmake -G Ninja .. -DCMAKE_BUILD_TYPE=Release
        cmake --build .
        ```
    3. **Przy użyciu makefile**
        ```sh
        make
        ```
4. Uruchom program:
    ```sh
    ./chat_app
    ```

## Wielowątkowość
Aplikacja będzie wykorzystywać wielowątkowość, aby umożliwić jednoczesne pisanie/wysyłanie wiadomości i ich odbieranie. Jeden wątek odpowiada za obsługę wejścia użytkownika i wysyłanie wiadomości, a drugi w tle cyklicznie sprawdza, czy pojawiły się nowe wiadomości od rozmówcy.

## Sekcje krytyczne i ich rozwiązanie
- **Sekcja krytyczna:** Jednoczesny dostęp do bazy danych przez wiele wątków (np. wysyłanie i odbieranie wiadomości).
- **Rozwiązanie:** Synchronizacja dostępu do bazy danych za pomocą mutexów.

### kod odpowiedzialny za sekcje krytyczne
```cpp
// Kod odpowiedzialny za sekcje krytyczne - synchronizacja dostępu do zasobów współdzielonych
// Ten kod zapewnia bezpieczny dostęp do historii wiadomości, puli połączeń z bazą danych oraz kolejek zadań asynchronicznych. Używa mutexów, zmiennych warunkowych i atomowych
// operacji do ochrony przed wyścigami wątków podczas jednoczesnego wysyłania/odbierania
// wiadomości oraz zarządzania połączeniami z bazą danych.

// 1. SEKCJA KRYTYCZNA: Dostęp do historii czatu 
// Plik: Chatfiles/ChatTerminal/chatterminal.cpp
std::mutex chatMutex; // Mutex chroniący globalną historię czatu

// Bezpieczne ładowanie starszych wiadomości
void loadOlderMessages(int userId, int otherUserId) {
    if (!hasMoreOlderMessages) return;
    
    std::vector<Message> olderMessages = getOlderChatHistory(userId, otherUserId, 50, totalLoadedMessages);
    
    // SEKCJA KRYTYCZNA - dostęp do współdzielonej historii wiadomości
    std::lock_guard<std::mutex> lock(chatMutex);
    
    if (!olderMessages.empty()) {
        insertOlderMessages(olderMessages);
    } else {
        hasMoreOlderMessages = false;
    }
    isLoadingOlderMessages = false;
    needsRefresh = true;
}

// Bezpieczne rysowanie historii czatu
// Plik: Chatfiles/ChatTerminal/chatterminal.cpp
void drawChatHistory(int userId) {
    if (!chatWindow) return;
    
    // SEKCJA KRYTYCZNA - odczyt współdzielonej historii wiadomości
    std::lock_guard<std::mutex> lock(chatMutex);
    
    wclear(chatWindow);
    
    if (globalChatHistory.empty()) {
        drawEmptyChat();
        return;
    }
    
    int max_y = getmaxy(chatWindow);
    int max_x = getmaxx(chatWindow);
    // Bezpieczny dostęp do globalChatHistory podczas renderowania
    int totalMessages = globalChatHistory.size();
    // ... renderowanie wiadomości
}

// 2. SEKCJA KRYTYCZNA: Pula połączeń z bazą danych
// Połączenie Pliku: Chatfiles/Database/ConnectionPool.cpp i Chatfiles/Database/ConnectionPool.h
class ConnectionPool {
private:
    std::queue<std::unique_ptr<pqxx::connection>> pool;
    std::mutex poolMutex; // Mutex chroniący pulę połączeń
    std::condition_variable condition; // Zmienna warunkowa dla oczekujących wątków

public:
    // Bezpieczne pobieranie połączenia z puli
    std::unique_ptr<pqxx::connection> getConnection() {
        std::unique_lock<std::mutex> lock(poolMutex);
        
        // SEKCJA KRYTYCZNA - oczekiwanie na dostępne połączenie
        while (pool.empty() && currentConnections >= maxConnections) {
            condition.wait(lock);
        }
        
        if (!pool.empty()) {
            auto conn = std::move(pool.front());
            pool.pop();
            return conn;
        }
        return createConnection();
    }
    
    // Bezpieczne zwracanie połączenia do puli
    void returnConnection(std::unique_ptr<pqxx::connection> conn) {
        if (conn && conn->is_open()) {
            // SEKCJA KRYTYCZNA - zwracanie połączenia do puli
            std::lock_guard<std::mutex> lock(poolMutex);
            pool.push(std::move(conn));
            condition.notify_one();
        }
    }
};

// 3. SEKCJA KRYTYCZNA: Kolejka zadań asynchronicznych
//  Połączenie Pliku: Chatfiles/Database/AsyncQueryManager.cpp
///                   Chatfiles/Database/AsyncQueryManager.h
class AsyncQueryManager {
private:
    std::queue<std::function<void()>> tasks;
    std::mutex taskMutex; // Mutex chroniący kolejkę zadań
    std::condition_variable condition;
    std::atomic<bool> stop;

public:
    // Bezpieczne dodawanie zadania do kolejki
    template<class F, class... Args>
    auto enqueue(F&& f, Args&&... args) {
        // SEKCJA KRYTYCZNA - dodawanie zadania do kolejki
        std::unique_lock<std::mutex> lock(taskMutex);
        if (stop) {
            throw std::runtime_error("enqueue on stopped AsyncQueryManager");
        }
        
        tasks.emplace([task](){ (*task)(); });
        condition.notify_one();
        
        return res;
    }
    
    // Worker thread - bezpieczne pobieranie zadań z kolejki
    void worker() {
        for (;;) {
            std::function<void()> task;
            
            {
                // SEKCJA KRYTYCZNA - pobieranie zadania z kolejki
                std::unique_lock<std::mutex> lock(taskMutex);
                condition.wait(lock, [this] { return stop || !tasks.empty(); });
                
                if (stop && tasks.empty()) return;
                
                task = std::move(tasks.front());
                tasks.pop();
            }
            
            task(); // Wykonanie zadania poza sekcją krytyczną
        }
    }
};

// 4. SEKCJA KRYTYCZNA: Sprawdzanie nowych wiadomości
// Plik: Chatfiles/FriendList/friendlist.cpp
static std::atomic<bool> messageCheckRunning{false}; // Atomowa kontrola stanu
static std::atomic<bool> messageCheckInterrupt{false};

// Bezpieczne sprawdzanie nowych wiadomości w tle
void checkForNewMessagesOptimized(int userId, std::vector<FriendInfo>* friends, std::atomic<bool>* needsRefresh) {
    while (messageCheckRunning) {
        for (int i = 0; i < 30 && messageCheckRunning && !messageCheckInterrupt; ++i) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        
        if (messageCheckInterrupt.exchange(false) || !messageCheckRunning) {
            break;
        }
        
        if (friends && needsRefresh && !friends->empty()) {
            // SEKCJA KRYTYCZNA - batch sprawdzanie wiadomości dla wszystkich znajomych
            std::vector<std::future<bool>> futures;
            
            for (size_t i = 0; i < friends->size(); ++i) {
                futures.push_back(
                    queryManager->hasNewMessagesFromAsync(userId, (*friends)[i].userId)
                );
            }
            
            // Collect results
            bool anyChanges = false;
            for (size_t i = 0; i < futures.size() && i < friends->size(); ++i) {
                try {
                    bool hasNew = futures[i].get();
                    if (hasNew != (*friends)[i].hasNewMessages) {
                        (*friends)[i].hasNewMessages = hasNew;
                        anyChanges = true;
                    }
                } catch (const std::exception& e) {
                    // Handle error
                }
            }
            
            if (anyChanges) {
                needsRefresh->store(true);
            }
        }
    }
}

// 5. SEKCJA KRYTYCZNA: Asynchroniczne oznaczanie wiadomości jako przeczytane
// Plik: Chatfiles/Database/AsyncQueryManager.cpp
std::future<void> AsyncQueryManager::markMessagesAsReadAsync(int userId, int friendId) {
    return enqueue([this, userId, friendId]() {
        // Bezpieczne pobieranie połączenia z puli
        auto conn = pool->getConnection();
        try {
            // SEKCJA KRYTYCZNA - operacja na bazie danych
            pqxx::work txn(*conn);
            txn.exec(
                "UPDATE user_chat SET read_status = TRUE "
                "WHERE sender_id = $1 AND receiver_id = $2 AND read_status = FALSE",
                pqxx::params(friendId, userId)
            );
            txn.commit();
        } catch (const std::exception& e) {
            // Logowanie błędu
        }
        // Bezpieczne zwracanie połączenia do puli
        pool->returnConnection(std::move(conn));
    });
}
```

**Mechanizmy synchronizacji zastosowane w aplikacji:**

1. **std::mutex** - Podstawowa synchronizacja dostępu do współdzielonych zasobów (historia czatu, pula połączeń)
2. **std::lock_guard** - RAII wrapper dla mutexów zapewniający automatyczne zwolnienie blokady
3. **std::condition_variable** - Efektywne oczekiwanie na spełnienie warunków (dostępność połączenia w puli)
4. **std::atomic** - Atomowe operacje na prostych typach danych (flagi kontrolne, stany wątków)
5. **std::future/std::promise** - Asynchroniczne przetwarzanie zadań z możliwością pobierania wyników
6. **Connection Pooling** - Ograniczenie liczby jednoczesnych połączeń z bazą danych

**Pliki zawierające kod sekcji krytycznych:**
- `Chatfiles/ChatTerminal/chatterminal.cpp` - Synchronizacja historii czatu
- `Chatfiles/Database/ConnectionPool.cpp` - Zarządzanie pulą połączeń
- `Chatfiles/Database/AsyncQueryManager.cpp` - Kolejka zadań asynchronicznych
- `Chatfiles/FriendList/friendlist.cpp` - Sprawdzanie nowych wiadomości

## Struktura bazy danych
- users – przechowuje dane użytkowników (user_id, user_name, user_name_id, password)
- user_chat – przechowuje wiadomości (log_id, sender_id, receiver_id, content, send_date), partycjonowana po dacie
- friends – przechowuje informacje na temat listy znajomych użytkowników (id, user_1, user_2, accepted, created_at, accepted_at), funckje odpowiedzialne za akceptowanie zaproszeń nie została zaimplementowana

## Autor
- Daniel Turek