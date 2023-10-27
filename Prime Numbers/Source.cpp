#include <iostream>
#include <syncstream>
#include <vector>
#include <thread>
#include <mutex>
#include <fstream>
#include <condition_variable>
#include <queue>

#include <chrono>

bool isPrime(unsigned long long n) { //Uses Primality Test
    if (n <= 1) return false; //returns false since prime numbers are defined as integers greater than 1
    if (n <= 3) return true; //returns true because 2 and 3 are prime numbers.
    if (n % 2 == 0 || n % 3 == 0) return false; // returns false because prime numbers greater than 3 are not divisible by 2 or 3.


    /*
    The loop starts at i = 5 and iterates until i * i is less than or equal to n. This optimization reduces the number of iterations needed to check for divisors.

    In each iteration, it checks if n is divisible by i or i + 2 (6k ± 1 for integer k) using the modulo operator. If it is divisible by either of these numbers, the function returns false.

    If the loop completes without finding any divisors, the function returns true, indicating that the number is prime.
    */
    for (unsigned long long i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) return false;
    }

    return true;
}

/*void findPrimes(std::vector<int>& primes, int upperLimit, std::mutex& mtx) {
    for (int num = 2; num <= upperLimit; ++num) {
        bool is_num_prime = isPrime(num);

        if (is_num_prime) {
            std::lock_guard<std::mutex> lock(mtx);
            primes.push_back(num);
        }
    }
}*/

std::mutex coutMutex;

void findPrimes(std::vector<unsigned long long>& primes, unsigned long long upperLimit, std::mutex& mtx, std::queue<unsigned long long>& taskQueue, std::condition_variable& cv) {
    while (true) {
        unsigned long long num;
        {
            std::unique_lock<std::mutex> lock(mtx);
            cv.wait(lock, [&taskQueue] { return !taskQueue.empty(); });
            num = taskQueue.front();
            taskQueue.pop();
        }

        if (num == -1) {
            std::unique_lock<std::mutex> lock(coutMutex); // Lock the mutex
            std::cout << "Thread ID: " << std::this_thread::get_id() << " finishing." << std::endl;
            // This is a signal to exit the thread
            break;
        }

        bool is_num_prime = isPrime(num);

        if (is_num_prime) {
            std::lock_guard<std::mutex> lock(mtx);
            primes.push_back(num);
        }
    }
}

bool isNumberPrime(unsigned long long number, const std::vector<unsigned long long>& primes) {
    for (unsigned long long prime : primes) {
        if (number == prime) {
            return true;
        }
        if (number % prime == 0) {
            return false;
        }
    }
    return true;
}

void savePrimesToCSV(std::vector<unsigned long long>& primes, std::mutex& mtx, std::condition_variable& cv, bool& shouldExit) {
    std::ofstream outputFile("primes.csv");
    if (outputFile.is_open()) {
        while (true) {
            std::vector<unsigned long long> primesToSave;
            {
                std::unique_lock<std::mutex> lock(mtx);
                cv.wait(lock, [&primes, &primesToSave, &shouldExit] { return !primes.empty() || shouldExit; });
                if (!primes.empty()) {
                    primesToSave = primes;
                    primes.clear();
                }
            }

            for (unsigned long long prime : primesToSave) {
                outputFile << prime << "\n";
            }
            outputFile.flush();

            if (shouldExit) {
                break;
            }
        }
        outputFile.close();
    }
    else {
        std::cerr << "Error: Unable to open the CSV file for writing." << std::endl;
    }
}

int main() {
    unsigned long long lowerLimit = UINT16_MAX;
    unsigned long long upperLimit = UINT32_MAX;
    int numThreads = 8;
    bool shouldExit = false;  // Flag to signal the save thread to exit

    std::vector<unsigned long long> primes;
    std::mutex mtx;
    std::queue<unsigned long long> taskQueue;
    std::condition_variable cv;

    std::vector<std::thread> threads;

    // Start the threads
    for (int i = 0; i < numThreads; i++) {
        threads.emplace_back(findPrimes, std::ref(primes), upperLimit, std::ref(mtx), std::ref(taskQueue), std::ref(cv));
    }

    // Create and start the thread for saving primes
    std::thread saveThread(savePrimesToCSV, std::ref(primes), std::ref(mtx), std::ref(cv), std::ref(shouldExit));

    // Enqueue tasks for the threads
    for (unsigned long long num = 2; num <= upperLimit; ++num) {
        {
            std::lock_guard<std::mutex> lock(mtx);
            taskQueue.push(num);
        }
        cv.notify_one();
    }

    // Add termination signal to the task queue
    for (int i = 0; i < numThreads; i++) {
        {
            std::lock_guard<std::mutex> lock(mtx);
            taskQueue.push(-1);
        }
        cv.notify_one();
    }

    // Wait for all threads to finish
    for (std::thread& t : threads) {
        t.join();
    }

    // Signal the save thread to exit
    shouldExit = true;
    cv.notify_one();

    // Wait for the save thread to finish
    saveThread.join();

    unsigned long long numberToCheck = 37;

    if (isNumberPrime(numberToCheck, primes)) {
        std::cout << numberToCheck << " is a prime number." << std::endl;
    }
    else {
        std::cout << numberToCheck << " is not a prime number." << std::endl;
    }

    return 0;
}