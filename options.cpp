#include <cstdlib>
#include <string>
#include <sstream>

#include "dontpct.h"

void display_useage()
{
	std::stringstream msg;

	msg << "useage: dontpct [-h | --help] [-f <filename>] [-t <thread count>]\n";
	msg << "        [-v <verbosity>] [--quiet] [--debug]\n";

	report(msg.str(), verboseness::quiet);
}

void help()
{
	report("dontpct -- A bruteforce solution for letter division puzzles\n", verboseness::quiet);

	display_useage();

	std::stringstream msg;

	msg << "Help:      use -h or --help to display program information.\n\n";
	msg << "Files:     use -f and specify a filename to change what file the\n";
	msg << "           program will look for. By default this value is 'puzzle.txt'\n\n";
	msg << "Threads:   use -t and specify the number of threads you'd like to create.\n";
	msg << "           By default this value is the number of concurrent threads your\n";
	msg << "           system will support. Changing this number will drastically change\n";
	msg << "           execution speeds, so it's best left as the default value.\n\n";
	msg << "Verbosity: change how much information you'd like the program to print\n";
	msg << "           by using -v and specifiying one of the following options:\n\n";
	msg << "           quiet    - Only display verified solutions, with no surrounding text\n";
	msg << "           standard - Display verified solution, with surrounding text (default)\n";
	msg << "           extra    - Display file information and standard operations\n";
	msg << "           debug    - Display all information: execution time, threading information, etc..\n\n";
	msg << "Examples:\n";
	msg << "Open different file: ./dontpct -f 'puzzle_copy.txt'\n";
	msg << "Use 4 threads and change verbosity: ./dontpct -tv 4 'debug'\n";
	msg << "                                    ./dontpct -t 4 -v 'debug'\n";

	report(msg.str(), verboseness::quiet);

	exit(0);
}

char* get_arg(char* arg, char c)
{
	if (!arg) err("Missing argument for option " + c);
	return arg;
}

verboseness get_verbosity(const std::string& arg)
{
	if (arg == "quiet") return verboseness::quiet;
	if (arg == "standard") return verboseness::standard;
	if (arg == "extra") return verboseness::extra;
	if (arg == "debug") return verboseness::debug;
	
	err("Verbosity mode not recongized. Options are: 'quiet', 'standard', 'extra', or 'debug'");

	return verboseness::standard;
}

void get_long_opt(Options& opt, std::string argv)
{
	auto optstr = argv.substr(1, argv.length() - 1);

	if (optstr == "help") help();
	else if (optstr == "quiet") opt.verbosity = verboseness::quiet;
	else if (optstr == "debug") opt.verbosity = verboseness::debug;
	else err("Option '" + argv + "' not recognized");
}

void get_short_opt(Options& opt, char* argv[], int i)
{
	char c;
	short m = 1;
	while ((c = *++argv[i]))
	{
		switch (c)
		{
		// Long options handled here
		case '-':
			get_long_opt(opt, argv[i]);
			return;
		case 'f':
			// Allows for multiple arguments with a list of short options: '-abc 1 2 3'
			opt.filename = get_arg(argv[i + m++], c);
			break;
		case 'h':
			help();
		case 't':
			opt.thread_count = std::atoi(get_arg(argv[i + m++], c));
			break;
		case 'v':
			opt.verbosity = get_verbosity(get_arg(argv[i + m++], c));
			break;
		default:
			display_useage();
			err("Invalid option: " + std::to_string(c));
		}
	}
}

// Cross-platform option handler
Options get_options(int argc, char* argv[])
{
	Options opt{};

	for (int i = 1; i < argc; i++)
	{
		// Long options are handled in the function
		if (*argv[i] == '-')
			get_short_opt(opt, argv, i);
	}

	// thread::hardware_concurrency, thread_count's initializer, returns 0 on error
	if (opt.thread_count == 0)
		err("Invalid thread count: " + opt.thread_count);

	return opt;
}