# Multi-threaded Fibonacci Calculation with Memoization

## Overview

This project demonstrates the use of memoization, dynamic programming, and multi-threading to efficiently handle repeated heavy tasks and optimize performance. The results of computations are cached for quick retrieval, and parallel processing accelerates computation.

## Features

- **Memoization**: Stores results of expensive function calls and returns the cached result when the same inputs occur again.
- **Dynamic Programming**: Breaks down complex problems into simpler subproblems and solves them just once, storing their solutions.
- **Multi-threading**: Uses multiple threads for the computation, improving execution speed.

## Requirements

- **C++ Standard**: The code is compatible with both C++17 and C++20.
- **Compiler**: The project can be compiled using MinGW G++ compiler or Microsoft Visual Studio MSVC compiler.
- **Libraries**: 
  - `<iostream>`
  - `<unordered_map>`
  - `<exception>`
  - `<chrono>`
  - `<future>`
  - `<vector>`
  - `<mutex>`
  - `<condition_variable>`
  - `<cmath>`
  - `<stdexcept>`
  - `<cassert>`

## Installation

### Using MinGW G++

1. Ensure MinGW is installed on your system.
2. Open the command prompt and navigate to the project directory.
3. Compile the code using the following command:
    ```bash
    g++ -std=c++20 -o fib_multithread main.cpp -pthread
    ```

### Using Microsoft Visual Studio

1. Open Microsoft Visual Studio.
2. Create a new C++ project and add the `main.cpp` file to the project.
3. Configure the project to use C++17 or C++20.
4. Build and run the project using the built-in compiler.

## Usage

Run the compiled executable. The program calculates the Fibonacci number for given inputs using memoization and multi-threading.

### Sample Output
The total time duration to calculate the Fib of 8 is: 70 microseconds
The total time duration to calculate the Fib of 21 is: 13 microseconds
The number of repeated processes to find Fib of 8 is: 0 (memoized)
Fibonacci(8) = 21

## Code Explanation

### Header Files
- **<iostream>**: For input and output operations.
- **<unordered_map>**: For storing the memoization cache.
- **<exception>**: For handling exceptions.
- **<chrono>**: For measuring execution time.
- **<future>**: For handling asynchronous operations.
- **<vector>**: For storing futures.
- **<mutex>**: For thread synchronization.
- **<condition_variable>**: For thread synchronization.
- **<cmath>**: For mathematical operations.
- **<stdexcept>**: For throwing runtime errors.
- **<cassert>**: For assertions.

### Functions
- `void fibLoop(...)`: Calculates Fibonacci numbers in chunks using multiple threads.
- `int fib(int target, const int CPUPool, std::unordered_map<int, int>& memo)`: Calculates Fibonacci number with memoization and multi-threading.
- `int main()`: Entry point of the program. Initializes the memoization cache, sets up the thread pool, and handles exceptions.

### Multi-threading
- **Mutex**: Used to synchronize access to shared data (memoization cache).
- **Condition Variable**: Used to synchronize the completion of tasks.

## License

This project is licensed under the NUI License.

## Contributing

Feel free to fork this repository and contribute by submitting pull requests. For major changes, please open an issue first to discuss what you would like to change.

## Contact

For any questions or suggestions, please contact Masoud Farsi at farsi.masoud@gmail.com 

