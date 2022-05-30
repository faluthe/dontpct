#pragma once
#include <string>
#include <vector>

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

Info get_info(std::ifstream& file);