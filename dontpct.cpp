#include <chrono>
#include <cstdlib> // For exit
#include <fstream>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

#include "dontpct.h"

// Placeholder, need to add verbosity
#ifdef _DEBUG
constexpr bool DEBUG = true;
#else
constexpr bool DEBUG = true;
#endif

std::chrono::steady_clock::time_point time_begin;

void err(const std::string& message)
{
	std::cerr << "[error] " << message << '\n';
	exit(-1);
}

void report(const std::string& message)
{
	std::cerr << "[debug] " << message << '\n';
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
	bool verified = true;
	
	for (const auto& e : info.equations)
	{
		auto minuend = convert(e.minuend, permutation);
		auto subtrahend = convert(e.subtrahend, permutation);
		auto difference = convert(e.difference, permutation);

		if (subtrahend > minuend || difference > minuend)
			verified = false;

		if ((minuend - subtrahend) != difference)
			verified = false;
	}

	return verified;
}

void thread_begin(unsigned long long start, unsigned long long end, const Info& info)
{
	for (; start < end; start++)
	{
		// Key to check
		auto permutation = permute(start, info.key);

		if (attempt(info, permutation) && verify(info, permutation))
		{
			report("Solution verified: " + permutation);

			if (DEBUG)
			{
				auto time_end = std::chrono::high_resolution_clock::now();
				auto dur = std::chrono::duration_cast<std::chrono::milliseconds>(time_end - time_begin);

				report("Elapsed time: " + std::to_string(dur.count()) + "ms");
			}

			break;
		}
	}
}

int main()
{
	if (DEBUG)
		time_begin = std::chrono::high_resolution_clock::now();

	// Placeholder, until options are added
	std::string filename = "puzzle.txt";
	std::ifstream file{ filename };

	if (!file.is_open())
		err("Unable to open file '" + filename + "'.");
	else
		report("Opened file '" + filename + "'.");

	auto info = get_info(file);

	file.close();

	std::vector<std::thread> threads;
	// Placeholder, need to add functionality
	unsigned long long count = 8;
	// Placeholder, need to add checks
	auto scale = factorial(info.key.length()) / count;

	// Divide and conquer!
	for (unsigned long long t = 0; t < count; t++)
		threads.push_back(std::thread{ thread_begin, t * scale, (t + 1) * scale, info });

	for (auto& t : threads)
		t.join();

	return 0;
}