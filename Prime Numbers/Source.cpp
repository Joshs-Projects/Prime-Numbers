#include <iostream>
#include <chrono>
#include "TextFile.h"

#include <vector>
#include <thread>

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

		if (prime == true) {
			std::cout << startNumber << " is a prime number." << std::endl;
		}
	}
	return prime;
}

int main() {
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
		if (FindIfPrimeDoWhile(count)) {
			std::cout << count << " is a prime number." << std::endl;
		}
	}
	end = std::chrono::steady_clock::now();

	std::cout << "It took " << std::chrono::duration_cast<std::chrono::seconds>(end - start).count() << " seconds to calculate all primes up to " << max << " using a do while loop." << std::endl;
	results.addLine("It took " + std::to_string(std::chrono::duration_cast<std::chrono::seconds>(end - start).count()) + " seconds to calculate all primes up to " + std::to_string(max) + " using a do while loop.");
	results.WriteTextFile();

	/*count = 0;
	start = std::chrono::steady_clock::now();
	while (true && count <= max) {
		count = count + 1;
		if (FindIfPrimeFor(count)) {
			std::cout << count << " is a prime number." << std::endl;
		}
	}
	end = std::chrono::steady_clock::now();

	std::cout << "It took " << std::chrono::duration_cast<std::chrono::seconds>(end - start).count() << " seconds to calculate all primes up to " << max << " using a for loop." << std::endl;
	results.addLine("It took " + std::to_string(std::chrono::duration_cast<std::chrono::seconds>(end - start).count()) + " seconds to calculate all primes up to " + std::to_string(max) + " using a for loop.");
	




	results.WriteTextFile();*/

}