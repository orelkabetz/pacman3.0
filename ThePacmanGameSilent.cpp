﻿#include "ThePacmanGame.h"
#include "ThePacmanGameSilent.h"
#include "io_utils.h"

void ThePacmanGameSilent::start() //changed
{
	do {
		Board::colored = true;
		runByScreens();
	} while (wannaPlay);
}

void ThePacmanGameSilent::init() //changed
{
	lives = 3;
	score = 0;
	b.createFileList(Board::fileNamesList);
	if (!Board::fileNamesList.size())
		printNoBoards(); // exception
	return;
}

void ThePacmanGameSilent::run(string name)
{
	//clear_screen();
	//cout << "this is Save Mode";
	//Sleep(400);
	clear_screen();
	hideCursor();
	string stepsFileName, resultFileName;

	createNewfileNames(name, stepsFileName, resultFileName);
	ifstream stepsFile(stepsFileName);
	ifstream resultFile(resultFileName);

	char key = 0;
	char isActive = 0;
	char tmp;
	int dir = 0, x, y;
	int timeInGame = 1, timeInFile;
	bool wait = true, isCrumb = false, stop = false, test = true;

	GhostNovice gn0(UP, Board::GhostsPos[0]), gn1(DOWN, Board::GhostsPos[1]), gn2(UP, Board::GhostsPos[2]), gn3(DOWN, Board::GhostsPos[3]);
	Ghost* g[4];
	g[0] = &gn0; g[1] = &gn1; g[2] = &gn2; g[3] = &gn3;

	Pacman p(Point(Board::PacmanPos.getX(), Board::PacmanPos.getY()));
	Fruit f(Point(22, 13));
	Point next;

	// Print board, score and lives
	//remainedCrumbs = b.print() - 1;
	//remainedCrumbs = 100;

	//gotoxy(41, 24);
	//printScore();
	//printLives();

	if (Board::colored) { setColors(p, g, f); }
	while (!stepsFile.eof())
	{
		stepsFile >> timeInFile;
		gotoxy(60, 1);
		//cout << timeInGame; //possible to delete only for debug

		stepsFile >> key;
		handlePacmanMove(p, key, dir, next);

		stepsFile >> isActive;
		f.setVisible(charToBool(isActive));

		if (f.getVisible())
		{
			//stepsFile >> tmp;
			//f.setFigure(tmp);
			f.setFigure('0');

			stepsFile >> x;
			stepsFile >> y;
			Point pos = f.setPos(x, y);
		}
		//Ghost Move
		stepsFile >> isActive;
		wait = charToBool(isActive);
		for (int i = 0; i < Board::ghostCount; i++)
		{
			stepsFile >> key;
			//directionToKey(key);
			dir = charToDirection(key);
			g[i]->setDirection(dir);
		}
		ghostsMove(wait, stop, isCrumb, g, p);

		stepsFile >> key;
		//directionToKey(key);
		dir = charToDirection(key);
		f.setDirection(dir);
		if (f.getVisible())
		{
			fruitsMove(wait, stop, isCrumb, f, p);
		}

		//check Fruit VS Pacman
		pacmanVsFruit(p, f);
		//check Ghost VS Pacman
		pacmanVsGhosts(p, g, name);
		if (pacmanDied)
		{
			//resultFile << "Pacman has died in iteration: " << timeInGame << endl;
			resultFile >> timeInFile;
			if (timeInFile != timeInGame)
				test = false;
			// initializing positions
			pacmanDied = false;
			p.setPos(Board::PacmanPos);
			p.setDirection(STAY);
			for (int i = 0; i < Board::ghostCount; i++)
			{
				//g[i]->erase(isCrumb);
				g[i]->setPos(Board::GhostsPos[i]);
				if (i % 2 == 0)
					g[i]->setDirection(UP);
				else
					g[i]->setDirection(DOWN);
			}
		}
		//Sleep(200);
		timeInGame++;
		//stepsFile >> tmp;
	}
	if (remainedCrumbs == 0)
	{
		//resultFile << "Pacman has finished the screen in iteration: " << timeInGame - 1 << endl;
		resultFile >> timeInFile;
		if (timeInFile != (timeInGame - 1))
			test = false;
	}

	stepsFile.close();
	resultFile.close();
	clear_screen();
	if (test)
	{
		setTextColor(Color::GREEN);
		cout << "The Test has passed successfully";
	}
	else
	{
		setTextColor(Color::RED);
		cout << "The Test has not passed successfully";
	}
	//Sleep(2000);
	exit(0);
}

int ThePacmanGameSilent::charToDirection(const char& tmp)
{
	switch (tmp)
	{
	case 'U':
		return UP;
	case 'D':
		return DOWN;
	case 'L':
		return LEFT;
	case 'R':
		return RIGHT;
	case 'S':
		return STAY;
	default:
		return NULL; //exception?
	}
}

bool ThePacmanGameSilent::charToBool(const char& tmp)
{
	if (tmp == 'Y')
		return true;
	else // tmp== 'N'
		return false;
}

void ThePacmanGameSilent::runByScreens()
{
	init();
	for (int i = 0; i < Board::fileNamesList.size(); i++)
	{
		//printLevel(i);
		remainedCrumbs = b.readScreens(Board::fileNamesList[i]);
		if (!isBoardValid())
		{
			wannaPlay = false;
			return;
		}
		run(Board::fileNamesList[i]);
		if (!lives)
		{
			wannaPlay = false;
			return;
		}
		Board::pacmanCount = 0; Board::ghostCount = 0;
	}
	return;
}


void ThePacmanGameSilent::pacmanVsGhosts(Pacman& p, Ghost* g[], string name)
{
	for (int i = 0; i < 4; i++)
	{
		if (p.getPos() == g[i]->getPos())
		{
			pacmanDied = true;
			ThePacmanGame::lives--;
			//if (lives)
			//	p.loseLife(Board::colored);

			Sleep(200);
			//run(name); // öøéê ìùðåú ìreturn
			return;
		}
	}
}

void ThePacmanGameSilent::ghostsMove(bool& wait, bool& stop, bool& isCrumb, Ghost* g[], Pacman& p)
{
	Point next;
	if ((!wait) && (!stop))
	{
		for (int i = 0; i < Board::ghostCount; i++)
		{
			isCrumb = (isBreadCrumb(g[i]->getPos()));
			//g[i]->simpleMove(isCrumb, p.getPos(), b);
			g[i]->step();
			ghostsVsGhost(i, g);
		}
		wait = true;
	}
	else
		wait = false;
}

void ThePacmanGameSilent::fruitsMove(bool& wait, bool& stop, bool& isCrumb, Fruit& f, Pacman& p)
{
	//Point next;

	//if ((!wait) && (!stop))
	//{
	//	isCrumb = (isBreadCrumb(f.getPos()));
	//	next = nextPos(f.getPos(), f.getDirection());

	//	if (b.getPoint(next) == '#')
	//	{
	//		//f.erase(isCrumb);
	//		return;
	//	}
	//	//isCrumb = (isBreadCrumb(f.getPos()));
	//	f.move(isCrumb);
	//}
}

void ThePacmanGameSilent::handlePacmanMove(Pacman& p, char& key, int& dir, Point& next)
{
	//Pacman move
	directionToKey(key);
	dir = p.changeDirection(key);
	p.setDirection(dir);

	//check wall
	next = nextPos(p.getPos(), p.getDirection());
	if (b.getPoint(next) == '#')
		p.setDirection(STAY);
	else
		p.step();

	//check crumb for pacman
	//if (b.getPoint(p.getPos()) == '.')
	//{
	//	b.setPoint(p.getPos(), '%');
	//	score++;
	//	remainedCrumbs--;
	//	printScore();
	//}
	return;
}

void ThePacmanGameSilent::directionToKey(char& key)
{
	switch (key)
	{
	case 'U':
		key = 'w';
		break;
	case 'D':
		key = 'x';
		break;
	case 'L':
		key = 'a';
		break;
	case 'R':
		key = 'd';
		break;
	case 'S':
		key = 's';
		break;
	default:
		break;
	}
}
