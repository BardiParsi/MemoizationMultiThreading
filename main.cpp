#include <iostream>
#include <unordered_map>
#include <exception>
#include <chrono>
#include <future>
#include <vector>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <atomic>
#include <cmath>
#include <stdexcept>
#include <cassert>

using std::cout;
using std::endl;
using std::cin;
// Fibonacci is used only as a simulator or example of a heavily used function for demonstration purposes
/*
This is a sample work of "Memoization" not "Memorization" in multi-threading process
This technique is used to save the result of the most repeated functions and tasks inside a cache
If the cache already has the result of 'n' the result will be returned from the cache
If the result of 'n+1' is needed; memoization uses the result of 'n' from the cache and with O(1) calculates the
result for 'n+1'.
*/
const size_t CPUThread = std::thread::hardware_concurrency();
std::mutex mtx; // To use inside Fib function
std::mutex mtxMain; // For threads in main() function
std::mutex mtxPool;
std::condition_variable condVar;
static std::atomic<int> POOLLAUNCHER{0};

void fibLoop(std::unordered_map<int, int>& memo, int loopLimit, int start, int endChunk, int& counter){
    if (start <= 1) {
        memo[0] = 0;
        memo[1] = 1;
        start = 2; // Skip the first two initialized numbers
    }
    for (int i = start; i <= endChunk; i++) {
        std::lock_guard<std::mutex> lock(mtx);
        if (memo.find(i) == memo.end()) {
           if (i >= loopLimit){
                break;
                }
            else {
                memo[i] = memo[i - 1] + memo[i - 2];
                counter++;
                }
        }
    }
}

// Function to calculate Fibonacci number with memoization using dynamic programming
int fib(int target, const int CPUPool, std::unordered_map<int, int>& memo) {
    auto now = std::chrono::high_resolution_clock::now();
    // Base cases
    if (target < 0) {
        throw std::invalid_argument("'n' Must be a positve number");
    }
    //assert(n >= 0); // for additional debug checks, if needed
    if (target <= 1) return target;
    // Check if result for n is already memoized
    auto it = memo.find(target);
    if (it != memo.end()) {
        auto then = std::chrono::high_resolution_clock::now();
        auto microSecond = std::chrono::duration_cast<std::chrono::microseconds>(then - now);
        std::lock_guard<std::mutex> lock(mtxMain);
        cout << "The total time duration to calculate the Fib of " << target << " is: " << microSecond.count() << " microseconds" << endl;
        cout << "The number of repeated process to find Fib of " << target << " is: 0 (memoized)" << endl;
        return it->second; // Return memoized result
    }
    // To check if we have initialized the task before
    if (memo.empty()) {
        memo[0] = 0;
        memo[1] = 1;
    }
    int prev = memo.size() - 1; // Index of the last computed Fibonacci number
    if (prev < 1) {
        prev = 1; // Adjust for the case where only memo[0] and memo[1] are initialized
    }
    int counter{0}; // To count the number of processing times
    // Calculate Fibonacci numbers starting from where we left off
    int start = prev + 1;
    int loopLimit = target + 1;
    int chunkSize = static_cast<int>(std::ceil(static_cast<double>(target - prev) / CPUPool));;
    std::vector<std::future<void>> futVec;

    for (int i = 0; i < CPUPool; ++i) {
        int chunkStart = start + (chunkSize * i);
        int chunkEnd = chunkStart + (chunkSize - 1);
        if (i == CPUPool - 1) {
        chunkEnd = target;
        }
        std::lock_guard<std::mutex> lock(mtx);
        futVec.push_back(std::async(std::launch::deferred, fibLoop, std::ref(memo), loopLimit, chunkStart, chunkEnd, std::ref(counter)));
    }
    for (auto& future : futVec) {
        future.get();
    }
    auto then = std::chrono::high_resolution_clock::now();
    auto microSecond = std::chrono::duration_cast<std::chrono::microseconds>(then - now);
    std::lock_guard<std::mutex> lock(mtxMain);
    cout << "The total time duration to calculate the Fib of " << target << " is: " << microSecond.count() << " microseconds" << endl;
    cout << "The number of repeated process to find Fib of " << target << " is: " << counter << endl;
    // Return the result for n
    return memo[target];
}

int main() {
    std::unordered_map<int, int> memo; // Memoization cache
    // The thread pool must be always less than 'n' operations by two
    // Initial data input into 'Memoization' should start with less operation counts than the thread pool by 2
    const int CPUPool = static_cast<int>(CPUThread/4);
    // Main Thread pool for multi calling the function for excessive use purposes
    const int ThreadPool = static_cast<int>(CPUThread/2);
    // We cannot run more threads than the number of calculations.
    assert (CPUPool < 7 && "Alert! CPUPool must me less than the smallest calculation.\n");
    // Make sure we dedicate more threads for concurrency function calling
    assert (ThreadPool > CPUPool);
    std::vector<std::jthread> threads1;

    for (int i = 0; i < ThreadPool; i++) {
        try {
            threads1.emplace_back([&, i](){
                int result = fib(i, CPUPool ,memo);
                std::lock_guard<std::mutex> lock(mtxMain);
                std::cout << "Fibonacci(" << (i) << ") = " << result << std::endl;
            });
        }
        catch(const std::runtime_error& e) {
        std::cerr << "The number of threads inside a pool must be less than the numbers of processes" << e.what() << endl;
        }
        catch(const std::exception& e){
        std::cerr << "The exception is caught!" << e.what() << endl;
        }
        catch(...) {
        std::cerr << "Unknown Error!" << endl;
        }
    }
    for (auto& thread : threads1) {
        thread.join();
    }
    POOLLAUNCHER.fetch_add(1, std::memory_order::relaxed);
    {
        std::unique_lock<std::mutex> poolLock(mtxPool);
        condVar.wait(poolLock, [&](){ return POOLLAUNCHER == 1; });
        poolLock.unlock();
        condVar.notify_one();
        POOLLAUNCHER.fetch_add(1, std::memory_order::relaxed);
    }
    std::vector<std::jthread> threads2;
    for (int i = 0; i < ThreadPool; i++) {
        try {
            int val = i * 10;
            threads2.emplace_back([&, val](){
                int result = fib(val, CPUPool ,memo);
                std::lock_guard<std::mutex> lock(mtxMain);
                std::cout << "Fibonacci(" << (val) << ") = " << result << std::endl;
            });
        }
        catch(const std::runtime_error& e) {
        std::cerr << "The number of threads inside a pool must be less than the numbers of processes" << e.what() << endl;
        }
        catch(const std::exception& e){
        std::cerr << "The exception is caught!" << e.what() << endl;
        }
        catch(...) {
        std::cerr << "Unknown Error!" << endl;
        }
    }
    for (auto& thread : threads2) {
        thread.join();
    }

    cin.get();

    return 0;
}
