#include <iostream>
#include <string>
#include <vector>
#include <conio.h>
#include <windows.h>
#include <strsafe.h>
#include <tchar.h>
#include <atlstr.h>

#include <fcntl.h>
#include <io.h>

#include <algorithm>

#include "globals.h"
#include "world.h"

#define BACKSPACE "\033[D\033[K"

#define _OFF "\033[0m"
#define CYAN_ "\033[1;36m"
#define RED_ "\033[0;31m"

void changeTitleCMD(const std::string &newTitle) {

	char data[4000];

	std::copy(newTitle.begin(), newTitle.end(), data);
	data[newTitle.size()] = '\0';
	USES_CONVERSION;
	TCHAR* Ttitle = A2T(data);
	TCHAR szOldTitle[MAX_PATH];
	TCHAR szNewTitle[MAX_PATH];

	// Save current console title.

	if (GetConsoleTitle(szOldTitle, MAX_PATH))
	{
		// Build new console title string.

		StringCchPrintf(szNewTitle, MAX_PATH, TEXT("%s"), Ttitle);

		// Set console title to new title
		if (!SetConsoleTitle(szNewTitle))
		{
			_tprintf(TEXT("SetConsoleTitle failed (%d)\n"), GetLastError());
			exit(99);
		}
	}

}

// -------------------------------------------------
int main()
{

	//Enable UTF-8 codification
	SetConsoleOutputCP(CP_UTF8);

	std::string title = "Russian Nightmare";
	changeTitleCMD(title);

	char key;
	std::string player_input;
	std::vector<std::string> args;
	std::string player_name = "Hero";
	std::string description = "You are an awesome adventurer!";
	bool defConfig = false;
	bool playerDead = false;
	bool playerExited = false;

	args.reserve(10);

	std::cout << CYAN_ "Welcome to " << title << "! \n" _OFF;
	std::cout << "------------------\n\n";

	World my_world(player_name, description);

	args.push_back("look");

	bool change_room = true;

	while (1)
	{
		if (_kbhit() != 0)
		{
			key = _getch();
			if (key == '\b') // backspace
			{
				if (player_input.length() > 0)
				{
					player_input.pop_back();
					//std::cout << BACKSPACE;
					std::cout << '\b';
					std::cout << " ";
					std::cout << '\b';
				}
			}
			else if (key != '\r') // return
			{
				player_input += key;
				std::cout << key;
			}
			else
				Tokenize(player_input, args);
		}

		if (args.size() > 0 && Same(args[0], "quit"))
			break;

		if (my_world.Tick(args, change_room, playerDead, playerExited) == false) {
			std::cout << "\nSorry, I do not understand your command.\n";
		}

		if (playerDead) {
			std::cout << "\nYou lose\n";
			break;
		}

		if (playerExited) {
			std::cout << "\nYou win\n";
			break;
		}

		if (change_room) {
			changeTitleCMD(my_world.takeActualRoomName());
			change_room = false;
		}

		if (args.size() > 0)
		{
			args.clear();
			player_input = "";
			std::cout << "> ";
		}
	}

	std::cout << "\nThanks for playing, Bye!\n";
	return 0;
}