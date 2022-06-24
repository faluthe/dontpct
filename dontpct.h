#pragma once
#include <string>
#include <thread>
#include <vector>

enum class verboseness
{
	quiet, standard, extra, debug
};

struct Equation
{
	std::string minuend;
	std::string subtrahend;
	std::string difference;

	Equation(const std::string& m, const std::string& s, const std::string& d);
private:
	// Create alphabetical substring
	void format(std::string& str)
	{
		std::string s;
		for (auto c : str)
		{
			if (std::isalpha(c))
				s += c;
		}
		str = s;
	}
};

struct Info
{
	int base = 0;
	bool solve4 = false;
	std::string key;
	std::vector<Equation> equations;
};

struct Options
{
	verboseness verbosity = verboseness::standard;
	std::string filename = "puzzle.txt";
	// Error checked in get_options
	unsigned int thread_count = std::thread::hardware_concurrency();
};

void report(const std::string& message, verboseness v);
void err(const std::string& message);
Info get_info(std::ifstream& file);
Options get_options(int argc, char* argv[]);