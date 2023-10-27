#include <iostream>
#include <chrono>
#include "TextFile.h"

#include <vector>
#include <thread>

#include <mutex>

bool FindIfPrimeFor(int number) {
	bool prime = true;
	for (int i = 2; i < number; i++) {
		if (number % i == 0) {
			prime = false;
		}
	}
	return prime;
}

bool FindIfPrimeDoWhile(int number) {
	bool prime = true;
	int i = 2;
	do {

		if (number % i == 0) {
			prime = false;
		}
		i = i + 1;

	} while (prime != false && i < number);

	return prime;
}


bool FindIfPrimeDoWhileThreads(int startNumber, int endNumber) {
	bool prime = true;
	int i = 2;
	for (int q = startNumber; q <= endNumber; q++) {
		i = 2;
		do {

			if (startNumber % i == 0) {
				prime = false;
			}
			i = i + 1;

		} while (prime != false && i < startNumber);

		/*if (prime == true) {
			std::cout << startNumber << " is a prime number." << std::endl;
		}*/
	}
	return prime;
}

std::vector<double> allPrimes;
std::mutex g_allPrimes;

void primesList() {
	std::lock_guard<std::mutex> guard(g_allPrimes);
}

bool mutableFindPrime() {
	g_allPrimes.lock();
	//ADD A RETURN BECAUSE OTHERWISE ERROR in debug
	return true;
}

int mainOld() {
	const int max = UINT_MAX; //set to UINT_MAX if you want to get to the biggest unsigned integer
	TextFile results;
	results.SetFileName("Results");

	const int numOfThreads = 100;
	std::thread threads[numOfThreads];
	
	int howManyNumbersPerThread = 10;

	unsigned int count = 1;
	auto start = std::chrono::steady_clock::now();
	while (true && count <= max) {
		/*count = count + 1;
		if (FindIfPrimeDoWhile(count)) {
			std::cout << count << " is a prime number." << std::endl;
		}*/
		for (int i = 0; i < numOfThreads; i++) {
			threads[i] = std::thread(FindIfPrimeDoWhileThreads, count, count + howManyNumbersPerThread);
			count = count + 1 + howManyNumbersPerThread;
		}

		for (int i = 0; i < numOfThreads; i++) {
			threads[i].join();
		}
	}
	
	auto end = std::chrono::steady_clock::now();

	std::cout << "It took " << std::chrono::duration_cast<std::chrono::seconds>(end - start).count() << " seconds to calculate all primes up to " << max << " using a do while loop with threads." << std::endl;
	results.addLine("It took " + std::to_string(std::chrono::duration_cast<std::chrono::seconds>(end - start).count()) + " seconds to calculate all primes up to " + std::to_string(max) + " using a do while loop with threads.");
	results.WriteTextFile();


	count = 0;
	start = std::chrono::steady_clock::now();
	while (true && count <= max) {
		count = count + 1;
		FindIfPrimeDoWhile(count);
		/*if (FindIfPrimeDoWhile(count)) {
			std::cout << count << " is a prime number." << std::endl;
		}*/
	}
	end = std::chrono::steady_clock::now();

	std::cout << "It took " << std::chrono::duration_cast<std::chrono::seconds>(end - start).count() << " seconds to calculate all primes up to " << max << " using a do while loop." << std::endl;
	results.addLine("It took " + std::to_string(std::chrono::duration_cast<std::chrono::seconds>(end - start).count()) + " seconds to calculate all primes up to " + std::to_string(max) + " using a do while loop.");
	results.WriteTextFile();

	//Saves prime numbers
	TextFile ThreadResults;
	ThreadResults.SetFileName("ThreadResults");
	//To get a list of primes.
	count = 0;
	start = std::chrono::steady_clock::now();
	while (true && count <= max) {
		count = count + 1;
		if (FindIfPrimeDoWhile(count)) {
			//std::cout << count << " is a prime number." << std::endl;
			ThreadResults.addLine(std::to_string(count));
		}
	}
	end = std::chrono::steady_clock::now();

	ThreadResults.WriteTextFile();

	std::cout << "It took " << std::chrono::duration_cast<std::chrono::seconds>(end - start).count() << " seconds to calculate all primes up to " << max << " using a do while loop and saving." << std::endl;
	results.addLine("It took " + std::to_string(std::chrono::duration_cast<std::chrono::seconds>(end - start).count()) + " seconds to calculate all primes up to " + std::to_string(max) + " using a do while loop and saving.");

	results.WriteTextFile();
	//stops saving prime numbers
	/*
	count = 0;
	start = std::chrono::steady_clock::now();
	while (true && count <= max) {
		count = count + 1;
		FindIfPrimeFor(count);
		//if (FindIfPrimeFor(count)) {
			//std::cout << count << " is a prime number." << std::endl;
		//}
	}
	end = std::chrono::steady_clock::now();

	std::cout << "It took " << std::chrono::duration_cast<std::chrono::seconds>(end - start).count() << " seconds to calculate all primes up to " << max << " using a for loop." << std::endl;
	results.addLine("It took " + std::to_string(std::chrono::duration_cast<std::chrono::seconds>(end - start).count()) + " seconds to calculate all primes up to " + std::to_string(max) + " using a for loop.");

	results.WriteTextFile();
	*/

	return 0;
}