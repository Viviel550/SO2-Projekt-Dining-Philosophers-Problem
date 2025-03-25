# Problem Jedzących Filozofów (Dining Philosophers Problem)

## Opis problemu
Problem ucztujących filozofów jest klasycznym problemem synchronizacji w informatyce. Pięciu filozofów siedzi przy okrągłym stole, na którym znajduje się pięć widelców. Każdy filozof na przemian myśli i je. Aby jeść, filozof musi mieć dwa widelce - jeden z lewej i jeden z prawej strony. Problem polega na tym, aby zapewnić, że żaden filozof nie będzie głodny na zawsze (brak zakleszczeń) i że filozofowie będą mogli jeść, gdy tylko będą mieli na to ochotę (brak zagłodzenia).

## Wymagania
- C++ 11+
- Terminal obsługujący kody wyjścia ANSI dla kolorowych wyników 

## Instrukcje uruchomienia projektu
1. Pobierz repozytorium lub sklonuj
2. Skompiluj projekt:
    ```sh
    g++ -std=c++11 -o dining_philosophers main.cpp -lpthread
    ```
2. Uruchom skompilowany program:
    ```sh
    ./dining_philosophers
    ```
3. Podaj liczbę filozofów, gdy zostaniesz o to poproszony.

## Wątki i co reprezentują
- **Philosopher Threads**: Każdy filozof jest reprezentowany przez osobny wątek. Wątek ten wykonuje cyklicznie trzy czynności: myślenie, podnoszenie widelców i jedzenie.

## Sekcje krytyczne i ich rozwiązanie
- **Sekcja krytyczna**: Podnoszenie i odkładanie widelców przez filozofów.
- **Rozwiązanie**: Użycie mutexów i zmiennych warunkowych do synchronizacji dostępu do widelców. Każdy widelec jest chroniony przez mutex, a zmienne warunkowe są używane do powiadamiania filozofów, gdy mogą podnieść widelce i zacząć jeść.

### Kod odpowiedzialny za sekcje krytyczne
```cpp
// Funkcja podnosząca widelce
void pick_up_forks(int id) {
    unique_lock<mutex> lock(forks[id]);
    state[id] = HUNGRY;
    update_output(id, "HUNGRY", "\033[1;33m"); // Żółty kolor dla głodnego

    // Próba zdobycia obu widelców
    test(id);
    if (state[id] != EATING) {
        conditions[id].wait(lock);
    }
}

// Funkcja odkładająca widelce
void put_down_forks(int id) {
    unique_lock<mutex> lock(forks[id]);
    state[id] = THINKING;
    update_output(id, "puts down forks", "\033[1;31m"); // Czerwony kolor dla odkładania widelców

    // Powiadomienie sąsiadów
    test((id + num_philosophers - 1) % num_philosophers);
    test((id + 1) % num_philosophers);
}

// Funkcja testująca, czy filozof może jeść
void test(int id) {
    if (state[id] == HUNGRY && state[(id + num_philosophers - 1) % num_philosophers] != EATING && state[(id + 1) % num_philosophers] != EATING) {
        state[id] = EATING;
        conditions[id].notify_one();
    }
}
```

## Autor
- Daniel Turek
