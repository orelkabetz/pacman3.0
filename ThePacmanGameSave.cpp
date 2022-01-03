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
	bool wait = true, isCrumb = false, stop = false;

	GhostBest gb0(UP, Board::GhostsPos[0]), gb1(DOWN, Board::GhostsPos[1]), gb2(UP, Board::GhostsPos[2]), gb3(DOWN, Board::GhostsPos[3]);
	GhostGood gg0(UP, Board::GhostsPos[0]), gg1(DOWN, Board::GhostsPos[1]), gg2(UP, Board::GhostsPos[2]), gg3(DOWN, Board::GhostsPos[3]);
	GhostNovice gn0(UP, Board::GhostsPos[0]), gn1(DOWN, Board::GhostsPos[1]), gn2(UP, Board::GhostsPos[2]), gn3(DOWN, Board::GhostsPos[3]);

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
	//remainedCrumbs = 100;

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
				exit(0); //exception to close files
			}
		}

		if (key == ESC)
		{
			int dir = p.getDirection();
			stop = true;
			timeInGame--;
			continue;
		}

		//stepsFile << "Iteration: " << timeInGame << " - ";
		stepsFile << timeInGame << " ";
		gotoxy(60, 1);
		cout << timeInGame; //possible to delete only for debug

		handlePacmanMove(p, key, dir, next);
		//stepsFile << "pacman: " << DirectionToChar(p.getDirection()) << ", ";
		stepsFile <<  DirectionToChar(p.getDirection()) << " ";

		handleFruitActivity(f);
		//stepsFile << "fruit exists: " << yesOrNo(f.getVisible()) << ", ";
		stepsFile << yesOrNo(f.getVisible()) << " ";
		if (f.getVisible())
		{
			//stepsFile << "fruit position: (" << f.getPos().getX() << ", " << f.getPos().getY() << "), ";
			//stepsFile << f.getFigure() << " ";
			stepsFile << f.getPos().getX() << " " << f.getPos().getY() << " ";
		}

		//Ghost Move
		stepsFile << yesOrNo(wait) << " ";
		ghostsMove(wait, stop, isCrumb, g, p);
		//stepsFile << "ghosts + fruit move: " << yesOrNo(wait) << ", ";

		for (int i = 0; i < Board::ghostCount; i++)
		{
			//stepsFile << "ghost " << i+1 << ": " << DirectionToChar(g[i]->getDirection()) << ", ";
			stepsFile << DirectionToChar(g[i]->getDirection()) << " ";
		}

		if (f.getVisible())
			fruitsMove(wait, stop, isCrumb, f, p);
		//stepsFile << "fruit: " << DirectionToChar(f.getDirection()) << "." << endl;
		//stepsFile << DirectionToChar(f.getDirection()) << " " << endl;
		stepsFile << DirectionToChar(f.getDirection()) << endl;

		//check Fruit VS Pacman
		pacmanVsFruit(p, f);
		//check Ghost VS Pacman
		pacmanVsGhosts(p, g ,name);
		if (pacmanDied)
		{
			//resultFile << "Pacman has lost life in iteration: " << timeInGame << endl;
			resultFile << timeInGame << " ";
			
			// initializing positions
			pacmanDied = false;
			p.setPos(Board::PacmanPos);
			p.setDirection(STAY);
			for (int i = 0; i < Board::ghostCount; i++)
			{
				g[i]->erase(isCrumb);
				g[i]->setPos(Board::GhostsPos[i]);
				if (i % 2 == 0)
					g[i]->setDirection(UP);
				else
					g[i]->setDirection(DOWN);
			}
		}
		Sleep(200);
		timeInGame++;
	}
	if (remainedCrumbs == 0)
	{
		//resultFile << "Pacman has finished the screen in iteration: " << timeInGame - 1 << endl;
		resultFile << timeInGame - 1 << " ";
	}

	stepsFile.close();
	resultFile.close();
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
	default:
		return NULL; //exception?
	}
}

char ThePacmanGameSave::yesOrNo(const bool argument)
{
	if (argument)
		return 'Y';
	else
		return 'N';
}