#include <cstdlib>
#include <errno.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <iostream>
#include <vector>
#include <string>

using namespace std;

int main(int argc, char* argv[])
{
	// Get the number of columns in the terminal
	int cols;
	{
		struct winsize size;

		if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &size) == -1)
		{
			int ioerr = errno;
			cerr << "ioctl() failed\n";

			if (ioerr == EBADF) cerr << "invalid file descriptor\n";
			else if (ioerr == EFAULT) cerr << "argp is inaccessible memory area\n";
			else if (ioerr == EINVAL) cerr << "request or argp is not valid\n";
			else if (ioerr == ENOTTY) cerr << "fd is not associated with character special device or specified request does not apply to that kind of file descriptor.\n";
			exit(EXIT_FAILURE);
		}
		cols = size.ws_col;
	}

	// Parse the arguments
	string fgColor = "black";
	string bgColor = "red";
	string label = "";
	{
		// I am lazy, lets use strings for ease
		// (Relax. Your speed anxiety is unfounded here.)
		for (int i = 1; i < argc; i++)
		{
			string arg(argv[i]);

			// Do we have a possible arg?
			if (arg[0] == '-' && (arg[1] == 'f' || arg[1] <= 'b'))
			{
				// Grab the color
				if (arg.size() == 2)
					(arg[1] == 'f' ? fgColor : bgColor) = argv[++i];
				else
					(arg[1] == 'f' ? fgColor : bgColor) = arg.substr(2, string::npos);
			}
			else if (arg[0] == '-' && arg[1] == 'h')
			{
				cout << "You need help..." << endl;
			}
			else
			{
				// Not an argument, append it to the label
				label.append(" ").append(arg);
			}
		}
	}

	// Evaluate the arguments to get what we need
	{
		// Finish padding the label if there is one
		if (label.size() > 0)
		{
			label.insert(0, "[");
			label.append(" ]");
		}

		// Find the foreground code
		if (fgColor == "black")
			fgColor = "30";
		else if (fgColor == "red")
			fgColor = "31";
		else if (fgColor == "green")
			fgColor = "32";
		else if (fgColor == "yellow")
			fgColor = "33";
		else if (fgColor == "blue")
			fgColor = "34";
		else if (fgColor == "magenta")
			fgColor = "35";
		else if (fgColor == "cyan")
			fgColor = "36";
		else if (fgColor == "white")
			fgColor = "37";
		else
			fgColor = "30";

		// Find the background code
		if (bgColor == "black")
			bgColor = "40";
		else if (bgColor == "red")
			bgColor = "41";
		else if (bgColor == "green")
			bgColor = "42";
		else if (bgColor == "yellow")
			bgColor = "43";
		else if (bgColor == "blue")
			bgColor = "44";
		else if (bgColor == "magenta")
			bgColor = "45";
		else if (bgColor == "cyan")
			bgColor = "46";
		else if (bgColor == "white")
			bgColor = "47";
		else
			bgColor = "47";
	}

	// Output the starting color code
	cout << "\033[" << fgColor << ";" << bgColor << "m";


	if (label.size() > 0 && label.size() <= cols)
	{
		int totalEquals = cols - label.size();
		int preEquals = totalEquals / 2;
		int postEquals = preEquals + (totalEquals % 2);

		for (int i = 0; i < preEquals; i++)
			cout << "=";
		cout << label;
		for (int i = 0; i < postEquals; i++)
			cout << "=";
	}
	else
	{
		if (label.size() > cols)
		{
			for (int i = 0; i < cols; i++)
				cout << "=";
			cout << endl << label << endl;
		}
		for (int i = 0; i < cols; i++)
			cout << "=";
	}

	// Output the reset color code
	cout << "\033[0m\n";

	return 0;
}
