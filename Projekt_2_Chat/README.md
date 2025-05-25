# Terminalowy Chat PostgreSQL

## Opis projektu
Aplikacja czatu terminalowego umożliwia komunikację tekstową pomiędzy użytkownikami z wykorzystaniem bazy danych PostgreSQL. Program pozwala na rejestrację, logowanie, wybór rozmówcy po jego ID oraz prowadzenie rozmowy w czasie rzeczywistym. Interfejs działa w terminalu i obsługuje kolorowe komunikaty.

## Funkcjonalności
- **Rejestracja** – zakładanie nowego konta użytkownika
- **Logowanie** – uwierzytelnianie użytkownika
- **Wybór rozmówcy po ID** – możliwość wyboru użytkownika do rozmowy
- **Pisanie i odbieranie wiadomości** – dwukierunkowa komunikacja tekstowa

## Wymagania
- C++ 11+
- PostgreSQL 12+
- Biblioteka `libpq` lub `libpqxx` (klient PostgreSQL dla C++)
- Terminal obsługujący kody ANSI (kolorowe wyniki)

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
        g++ -std=c++17 main.cpp Chatfiles/Login/*.cpp Chatfiles/Register/*.cpp Chatfiles/MainTerminal/*.cpp Chatfiles/ChatTerminal/*.cpp -o chat_app.exe -lpqxx -lpq -lncursesw
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
```sh
    brak na chwile obecną
```
## Struktura bazy danych
- users – przechowuje dane użytkowników (user_id, user_name, user_name_id, password)
- user_chat – przechowuje wiadomości (log_id, sender_id, receiver_id, content, send_date), partycjonowana po dacie

## Autor
- Daniel Turek