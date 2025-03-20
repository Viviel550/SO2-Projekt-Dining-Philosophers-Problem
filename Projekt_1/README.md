# Dining Philosophers Problem

This project is an implementation of the Dining Philosophers Problem, created as a school project with the goal of understanding the basics of multithreading.

## Description

The Dining Philosophers Problem is a classic synchronization problem that illustrates the challenges of avoiding deadlock and ensuring proper synchronization in a concurrent system. In this problem, a number of philosophers sit around a table, each with a fork on either side. To eat, a philosopher needs to pick up both forks. Philosophers alternate between thinking and eating, and they must avoid deadlock by properly synchronizing their actions.

## Features

- Each philosopher reports their state (THINKING, HUNGRY, EATING) in the terminal.
- The program ensures that there is no deadlock.
- The number of philosophers is provided as input at the start of the program.
- The output is color-coded for better readability:
  - THINKING: Blue
  - HUNGRY: Yellow
  - EATING: Green
  - Putting down forks: Red

## Requirements

- C++11 or later
- A terminal that supports ANSI escape codes for colored output

## Usage

1. Clone the repository or download the source code.
2. Compile the code using a C++ compiler (e.g., `g++`):
