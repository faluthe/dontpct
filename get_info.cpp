#include <fstream>
#include <string>

#include "dontpct.h"

void report(const std::string& message);
void err(const std::string& message);

int extra_letters = 0;

Equation::Equation(const std::string& m, const std::string& s, const std::string& d)
{
	minuend = m;
	subtrahend = s;
	difference = d;

	format(subtrahend);

	// First equation logic
	auto pos = minuend.find('|');
	if (pos != std::string::npos)
	{
		minuend.erase(0, pos + 2);
		extra_letters = minuend.length() - subtrahend.length();
		minuend.erase(minuend.end() - extra_letters, minuend.end());
	}
	else
	{
		format(minuend);
		extra_letters--;
	}

	format(difference);
	// If a letter is dropped down, disregard
	if (extra_letters > 0 && difference.length() > 0)
		difference.pop_back();
}

Info get_info(std::ifstream& file)
{
	Info info;
	std::string line;

	while (std::getline(file, line))
	{
		static std::string buf_minuend;
		static std::string buf_subtrahend;

		// Here the minuend/subtrahend will be in their respective buffers
		if (line.find('=') != std::string::npos)
		{
			// Get the difference
			std::getline(file, line);

			if (line.find('?') != std::string::npos)
			{
				info.solve4 = true;
				// Still need more info
				continue;
			}

			Equation e{ buf_minuend, buf_subtrahend, line };
			info.equations.push_back(e);

			report("Found equation: '" + e.minuend + "' - '" + e.subtrahend + "' = '" + e.difference + "'");
		}
		else if (line.find("base") != std::string::npos)
		{
			std::string substr = line.substr(line.find(':') + 2, line.length());
			info.base = std::atoi(substr.c_str());
			report("Found base: '" + substr + "'");
		}
		else if (line.find("letters") != std::string::npos)
		{
			std::string substr = line.substr(line.find(':') + 2, line.length());
			info.key = std::atoi(substr.c_str());
			report("Found key: '" + substr + "'");
		}

		// Update buffers (stores previous two lines)
		buf_minuend = buf_subtrahend;
		buf_subtrahend = line;
	}

	bool fail = false;
	if (!info.base)
	{
		report("Missing base");
		fail = true;
	}
	if (info.key.empty())
	{
		report("Missing key");
		fail = true;
	}
	if (info.equations.empty())
	{
		report("Missing equations");
		fail = true;
	}
	if (fail)
		err("Information unobtainable");

	return info;
}