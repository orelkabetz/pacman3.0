#include <windows.h>
#include <iostream>
using namespace std;

#include "ThePacmanGame.h"
#include "ThePacmanGameSave.h"
#include "InputParser.h"
//#include "Board.h"

//void test()
//{
//	Board b;
//	char* namesList[10][25];
//	//b.createFileList();
//	//b.readScreens("pacman_1.screen.txt");
//	//b.readScreens("pacman_2.screen.txt");
//	//b.readScreens("pacman_3.screen.txt");
//}

int main(int argc, char** argv)
//int main()
{
	srand(time(NULL));
	ThePacmanGame* game;
	if (argc < 2) {
		ThePacmanGame simpleGame;
		game = &simpleGame;
		game->start();
	}
	else {
		InputParser input(argc, argv);
		//input.printFirstToken();
		input.checkMode();
		//if(input.mode!=NULL)
		//	cout << input.mode; 
		if (input.mode == SAVE)
		{
			ThePacmanGameSave saveGame;
			game = &saveGame;
			game->start();
		}
	}

	return 0;
}
