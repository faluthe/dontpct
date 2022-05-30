#include <cstdlib> // For exit
#include <fstream>
#include <iostream>
#include <string>

#include "dontpct.h"

// Placeholder, need to add verbosity
#ifdef _DEBUG
constexpr bool DEBUG = true;
#elif
constexpr bool DEBUG = false;
#endif

void err(const std::string& message)
{
	std::cerr << "[error] " << message << '\n';
	exit(-1);
}

void report(const std::string& message)
{
	if (DEBUG)
		std::cerr << "[debug] " << message << '\n';
}

int main()
{
	// Placeholder, until options are added
	std::string filename = "puzzle.txt";
	std::ifstream file{ filename };

	if (!file.is_open())
		err("Unable to open file '" + filename + "'.");
	else
		report("Opened file '" + filename + "'.");

	get_info(file);

	file.close();

	return 0;
}