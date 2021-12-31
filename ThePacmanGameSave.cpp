#include "ThePacmanGameSave.h"
#include "ThePacmanGame.h"
#include "io_utils.h"


void ThePacmanGameSave::run(string name)
{
	//clear_screen();
	//cout << "this is Save Mode";
	//Sleep(400);
	clear_screen();
	hideCursor();
	string stepsFileName, resultFileName;

	createNewfileNames(name, stepsFileName, resultFileName);
	ofstream stepsFile(stepsFileName);
	ofstream resultFile(resultFileName);

	char key = 0;
	int dir = 0;
	int timeInGame = 1;
	bool wait = true, isCrumb = false, stop = false, died = false;

	GhostBest gb0(UP, Board::GhostsPos[0]), gb1(DOWN, Board::GhostsPos[1]), gb2(DOWN, Board::GhostsPos[2]), gb3(UP, Board::GhostsPos[3]);
	GhostGood gg0(UP, Board::GhostsPos[0]), gg1(DOWN, Board::GhostsPos[1]), gg2(DOWN, Board::GhostsPos[2]), gg3(UP, Board::GhostsPos[3]);
	GhostNovice gn0(UP, Board::GhostsPos[0]), gn1(DOWN, Board::GhostsPos[1]), gn2(DOWN, Board::GhostsPos[2]), gn3(UP, Board::GhostsPos[3]);

	Ghost* g[4];

	if (ghostLevel == NOVICE)
	{
		g[0] = &gn0; g[1] = &gn1; g[2] = &gn2; g[3] = &gn3;
	}
	else if (ghostLevel == GOOD)
	{
		g[0] = &gg0; g[1] = &gg1; g[2] = &gg2; g[3] = &gg3;
	}
	else // BEST
	{
		g[0] = &gb0; g[1] = &gb1; g[2] = &gb2; g[3] = &gb3;
	}

	Pacman p(Point(Board::PacmanPos.getX(), Board::PacmanPos.getY()));
	Fruit f(Point(22, 13));
	Point next;

	// Print board, score and lives
	remainedCrumbs = b.print() - 1;
	remainedCrumbs = 20;

	gotoxy(41, 24);
	printScore();
	printLives();

	if (Board::colored) { setColors(p, g, f); }
	while ((lives != 0) && (remainedCrumbs != 0))
	{
		if (stop)
		{
			printPause();
			stop = false;
			key = _getch();
			if (key != ESC)
			{
				printContinue();
				stop = false;
				timeInGame--;
				continue;
			}
			else
			{
				printGoodBye();
				exit(0);
			}
		}

		if (key == ESC)
		{
			int dir = p.getDirection();
			stop = true;
			timeInGame--;
			continue;
		}
		stepsFile << "Iteration: " << timeInGame << " - ";
		gotoxy(60, 1);
		cout << timeInGame;

		handlePacmanMove(p, key, dir, next);
		stepsFile << "pacman: " << DirectionToChar(p.getDirection()) << ", ";

		handleFruitActivity(f);
		stepsFile << "fruit exists: " << f.getVisible() << ", ";

		//Ghost Move
		ghostsMove(wait, stop, isCrumb, g, p);
		for (int i = 0; i < Board::ghostCount; i++)
		{
			stepsFile << "ghost " << i+1 << ": " << DirectionToChar(g[i]->getDirection()) << ", ";
		}

		if (f.getVisible())
			fruitsMove(wait, stop, isCrumb, f, p);
		stepsFile << "fruit: " << DirectionToChar(f.getDirection()) << "." << endl;

		//check Fruit VS Pacman
		pacmanVsFruit(p, f);
		//check Ghost VS Pacman
		pacmanVsGhosts(p, g ,name);
		if (pacmanDied)
		{
			resultFile << "Pacman has died in iteration: " << timeInGame << endl;
			// initializing positions
			pacmanDied = false;
			p.setPos(Board::PacmanPos);
			for (int i = 0; i < Board::ghostCount; i++)
			{
				g[i]->erase(isCrumb);
				g[i]->setPos(Board::GhostsPos[i]);
			}
		}
		Sleep(200);
		timeInGame++;
	}
	if(remainedCrumbs==0)
		resultFile << "Pacman has finished the screen in iteration: " << timeInGame-1 << endl;

	stepsFile.close();
	resultFile.close();
}

void ThePacmanGameSave::createNewfileNames(string name, string& stepsFileName, string& resultFileName)
{
	string delimiter = ".screen.txt";
	stepsFileName = name.substr(0, name.find(delimiter)) + ".steps.txt";
	resultFileName = name.substr(0, name.find(delimiter)) + ".result.txt";
}

char ThePacmanGameSave::DirectionToChar(const int direction)
{
	switch (direction)
	{
	case UP:
		return 'U';
	case DOWN:
		return 'D';
	case LEFT:
		return 'L';
	case RIGHT:
		return 'R';
	case STAY:
		return 'S';
	}
}