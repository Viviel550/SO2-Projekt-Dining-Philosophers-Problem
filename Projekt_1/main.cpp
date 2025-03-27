#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <iomanip>

using namespace std;

// Enum to represent the state of each philosopher
enum State { THINKING, HUNGRY, EATING };

class DiningPhilosophers {
private:
    int num_philosophers; // Number of philosophers
    int loops; // Number of loops for each philosopher
    vector<State> state; // State of each philosopher
    vector<mutex> forks; // Mutex for each fork
    vector<condition_variable> conditions; // Condition variable for each philosopher
    mutex output_mutex; // Mutex for synchronized output

public:
    // Constructor to initialize the number of philosophers and their states
    DiningPhilosophers(int n, int l) : num_philosophers(n), loops(l), state(n, THINKING), forks(n), conditions(n) {}

    // Function representing the actions of a philosopher
    void philosopher(int id) {
        for (int i = 0; i < loops; ++i) { // Repeat actions for the specified number of loops
            think(id);
            pick_up_forks(id);
            eat(id);
            put_down_forks(id);
        }
    }

    // Function to simulate thinking
    void think(int id) {
        update_output(id, "THINKING", "\033[1;34m"); // Blue color for thinking
        this_thread::sleep_for(chrono::milliseconds(1000 + rand() % 2000));
    }

    // Function to simulate eating
    void eat(int id) {
        update_output(id, "EATING", "\033[1;32m"); // Green color for eating
        this_thread::sleep_for(chrono::milliseconds(1000 + rand() % 2000));
    }

    // Function to pick up forks
    void pick_up_forks(int id) {
        unique_lock<mutex> lock(forks[id]);
        state[id] = HUNGRY;
        update_output(id, "HUNGRY", "\033[1;33m"); // Yellow color for hungry
        // Try to acquire both forks
        test(id);
        if (state[id] != EATING) {
            conditions[id].wait(lock);
        }
    }

    // Function to put down forks
    void put_down_forks(int id) {
        unique_lock<mutex> lock(forks[id]);
        state[id] = THINKING;
        update_output(id, "puts down forks", "\033[1;31m"); // Red color for putting down forks
        // Notify neighbors
        test((id + num_philosophers - 1) % num_philosophers);
        test((id + 1) % num_philosophers);
        this_thread::sleep_for(chrono::milliseconds(1000 + rand() % 2000));
    }

    // Function to test if a philosopher can eat
    void test(int id) {
        if (state[id] == HUNGRY && state[(id + num_philosophers - 1) % num_philosophers] != EATING && state[(id + 1) % num_philosophers] != EATING) {
            state[id] = EATING;
            conditions[id].notify_one();
        }
    }

    // Function to update the output with the current state of a philosopher
    void update_output(int id, const string& action, const string& color) {
        unique_lock<mutex> lock(output_mutex);
        auto now = chrono::system_clock::now();
        auto now_c = chrono::system_clock::to_time_t(now);
        cout << "\033[" << (id + 1) << ";1H" << color << "Philosopher " << id << " is " 
        << action << " at " << put_time(localtime(&now_c), "%H:%M:%S") << "\033[0m   " << flush;
    }
};

int main() {
    int num_philosophers, loops;
    cout << "\nEnter the number of philosophers: ";
    cin >> num_philosophers;
    cout << "\nEnter the number of loops: ";
    cin >> loops;

    if (num_philosophers <= 0 || loops <= 0) {
        cerr << "The number of philosophers and loops must be greater than 0." << endl;
        return 1;
    }

    // Clear the screen and hide the cursor
    cout << "\033[2J\033[?25l";

    // Create the DiningPhilosophers object with the specified number of philosophers and loops
    DiningPhilosophers table(num_philosophers, loops);
    vector<thread> philosophers;

    // Create a thread for each philosopher
    for (int i = 0; i < num_philosophers; ++i) {
        philosophers.emplace_back(&DiningPhilosophers::philosopher, &table, i);
    }

    // Join all threads
    for (auto& t : philosophers) {
        t.join();
    }

    // Show the cursor again
    cout << "\033[?25h";

    return 0;
}