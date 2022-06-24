#include <chrono>
#include <cmath> // For pow
#include <cstdlib> // For exit
#include <fstream>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

#include "dontpct.h"

std::chrono::steady_clock::time_point time_begin;
Options options;

void err(const std::string& message)
{
	std::cerr << "[error] " << message << '\n';
	exit(-1);
}

// Info is only displayed if the set verbosity level exceeds v
void report(const std::string& message, verboseness v)
{
	if (v <= options.verbosity)
		std::cout << message << '\n';
}

unsigned long long factorial(unsigned long long x)
{
	for (auto i = x - 1; i > 0; i--)
		x *= i;
	return x;
}

// https://github.com/faluthe/permutor
std::string permute(unsigned long long k, std::string s)
{
	for (unsigned long long j = 1; j < s.length(); j++)
	{
		std::swap(s[static_cast<std::size_t>(k % (j + 1))], s[static_cast<std::size_t>(j)]);
		k = k / (j + 1);
	}

	return s;
}

// Converts letters to a number, using a unique permutation as a key
unsigned int convert(const std::string& letters, const std::string& p)
{
	unsigned int x{};

	// I believe string::find() has quite a bit of overhead, suggestions are welcome!
	for (std::size_t i = 0; i < letters.length(); i++)
		x += static_cast<unsigned int>(p.find(letters[i])) * static_cast<unsigned int>(std::pow(p.length(), letters.length() - 1 - i));

	return x;
}

bool attempt(const Info& info, const std::string& permutation)
{
	for (const auto& e : info.equations)
	{
		auto minuend = convert(e.minuend, permutation);
		auto subtrahend = convert(e.subtrahend, permutation);
		auto difference = convert(e.difference, permutation);

		if (subtrahend > minuend || difference > minuend)
			return false;

		if ((minuend - subtrahend) == difference)
			return true;
	}
	
	return false;
}

bool verify(const Info& info, const std::string& permutation)
{
	for (const auto& e : info.equations)
	{
		auto minuend = convert(e.minuend, permutation);
		auto subtrahend = convert(e.subtrahend, permutation);
		auto difference = convert(e.difference, permutation);

		if (subtrahend > minuend || difference > minuend)
			return false;

		if ((minuend - subtrahend) != difference)
			return false;
	}

	return true;
}

void thread_begin(unsigned long long start, unsigned long long end, const Info& info)
{
	for (; start < end; start++)
	{
		// Key to check
		auto permutation = permute(start, info.key);

		if (attempt(info, permutation) && verify(info, permutation))
		{
			report("Solution verified:", verboseness::standard);
			report(permutation, verboseness::quiet);

			// Only used in debugging mode
			auto time_end = std::chrono::high_resolution_clock::now();
			auto dur = std::chrono::duration_cast<std::chrono::milliseconds>(time_end - time_begin);
			
			report("\nElapsed time: " + std::to_string(dur.count()) + "ms", verboseness::debug);

			break;
		}
	}
}

int main(int argc, char* argv[])
{
	// Used for measuring execution time in debugging mode
	time_begin = std::chrono::high_resolution_clock::now();

	options = get_options(argc, argv);

	std::ifstream file{ options.filename };

	if (!file.is_open())
		err("Unable to open file '" + options.filename + "'");
	else
		report("Opened file '" + options.filename + "'\n", verboseness::extra);

	auto info = get_info(file);

	file.close();

	std::vector<std::thread> threads;
	// Placeholder, need to add checks
	auto scale = factorial(info.key.length()) / options.thread_count;

	report("\nThread count: " + std::to_string(options.thread_count), verboseness::debug);
	report("Thread scale factor: " + std::to_string(scale) + "\n", verboseness::debug);

	// Divide and conquer!
	for (unsigned long long t = 0; t < options.thread_count; t++)
		threads.push_back(std::thread{ thread_begin, t * scale, (t + 1) * scale, info });

	for (auto& t : threads)
		t.join();

	return 0;
}