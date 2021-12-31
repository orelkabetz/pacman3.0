#include "ThePacmanGame.h"
#include "ThePacmanGameLoad.h"
#include "io_utils.h"

void ThePacmanGameLoad::start() //changed
{
	do{
		Board::colored = true;
		runByScreens();
	} while (wannaPlay);
}

void ThePacmanGameLoad::init() //changed
{
	lives = 3;
	score = 0;
	b.createFileList(Board::fileNamesList);
	if (!Board::fileNamesList.size())
		printNoBoards(); // exception
	return;
}

void ThePacmanGameLoad::run(string name)
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
	bool wait = true, isCrumb = false, stop = false;

	GhostNovice gn0(UP, Board::GhostsPos[0]), gn1(DOWN, Board::GhostsPos[1]), gn2(DOWN, Board::GhostsPos[2]), gn3(UP, Board::GhostsPos[3]);
	Ghost* g[4];
	g[0] = &gn0; g[1] = &gn1; g[2] = &gn2; g[3] = &gn3;

	Pacman p(Point(Board::PacmanPos.getX(), Board::PacmanPos.getY()));
	Fruit f(Point(22, 13));
	Point next;

	// Print board, score and lives
	remainedCrumbs = b.print() - 1;
	//remainedCrumbs = 20;

	gotoxy(41, 24);
	printScore();
	printLives();

	if (Board::colored) { setColors(p, g, f); }
	while (!stepsFile.eof())
	{
		stepsFile >> timeInFile;
		gotoxy(60, 1);
		cout << timeInGame; //possible to delete only for debug

		stepsFile >> key;
		handlePacmanMove(p, key, dir, next);

		stepsFile >> isActive;
		f.setVisible(charToBool(isActive));

		if (f.getVisible())
		{
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
			dir = charToDirection(key);
			g[i]->setDirection(dir);
		}
		ghostsMove(wait, stop, isCrumb, g, p);


		if (f.getVisible())
		{
			stepsFile >> dir;
			f.setDirection(dir);
			fruitsMove(wait, stop, isCrumb, f, p);
		}

		//check Fruit VS Pacman
		pacmanVsFruit(p, f);
		//check Ghost VS Pacman
		pacmanVsGhosts(p, g, name);
		//if (pacmanDied)
		//{
		//	resultFile << "Pacman has died in iteration: " << timeInGame << endl;
		//	// initializing positions
		//	pacmanDied = false;
		//	p.setPos(Board::PacmanPos);
		//	for (int i = 0; i < Board::ghostCount; i++)
		//	{
		//		g[i]->erase(isCrumb);
		//		g[i]->setPos(Board::GhostsPos[i]);
		//	}
		//}
		Sleep(200);
		timeInGame++;
		stepsFile >> tmp;
	}
	if (remainedCrumbs == 0)
		//resultFile << "Pacman has finished the screen in iteration: " << timeInGame - 1 << endl;

	stepsFile.close();
	resultFile.close();
}

int ThePacmanGameLoad::charToDirection(const char& tmp)
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

bool ThePacmanGameLoad::charToBool(const char& tmp)
{
	if (tmp == 'Y')
		return true;
	else // tmp== 'N'
		return false;
}

void ThePacmanGameLoad::runByScreens()
{
	init();
	for (int i = 0; i < Board::fileNamesList.size(); i++)
	{
		printLevel(i);
		b.readScreens(Board::fileNamesList[i]);
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


void ThePacmanGameLoad::pacmanVsGhosts(Pacman& p, Ghost* g[], string name)
{
	for (int i = 0; i < 4; i++)
	{
		if (p.getPos() == g[i]->getPos())
		{
			pacmanDied = true;
			ThePacmanGame::lives--;
			if (lives)
				p.loseLife(Board::colored);

			Sleep(200);
			//run(name); // öøéê ìùðåú ìreturn
			return;
		}
	}
}

void ThePacmanGameLoad::ghostsMove(bool& wait, bool& stop, bool& isCrumb, Ghost* g[], Pacman& p)
{
	Point next;
	if ((!wait) && (!stop))
	{
		for (int i = 0; i < Board::ghostCount; i++)
		{
			isCrumb = (isBreadCrumb(g[i]->getPos()));
			g[i]->simpleMove(isCrumb, p.getPos(), b);
			ghostsVsGhost(i, g);
		}
		wait = true;
	}
	else
		wait = false;
}

void ThePacmanGameLoad::fruitsMove(bool& wait, bool& stop, bool& isCrumb, Fruit& f, Pacman& p)
{
	Point next;
	if ((!wait) && (!stop))
	{
		isCrumb = (isBreadCrumb(f.getPos()));
		f.move(isCrumb);
	}
}

void ThePacmanGameLoad::handlePacmanMove(Pacman& p, char& key, int& dir, Point& next)
{
	//Pacman move
	dir = p.changeDirection(key);
	p.setDirection(dir);

	//check wall
	next = nextPos(p.getPos(), p.getDirection());
	if (b.getPoint(next) == '#')
		p.setDirection(4);
	else
		p.move();

	//check crumb for pacman
	if (b.getPoint(p.getPos()) == '.')
	{
		b.setPoint(p.getPos(), '%');
		score++;
		remainedCrumbs--;
		printScore();
	}
}
