#include "GhostGood.h"
#include "GhostBest.h"
#include "GhostNovice.h"


void GhostGood::move(const bool& isCrumb, const Point& pacmanPos, const Board& b)
{
	static bool isSmart = true;
	static int count = 0;

	if (!isSmart) //isSmart is false and he need to move novice for 5 moves
	{
		Point next = calculateNext(pacmanPos);
		while (b.mati[next.getY()][next.getX()] == '#')
		{
			setDirection(rand() % 4); // Stupid
			next = calculateNext(pacmanPos);
		}

		erase(isCrumb);
		//_pos.moveGhost(_direction);
		step();
		//bestGhost(_direction, pacmanPos);
		setTextColor(_color);
		_pos.draw(_figure);

		if (count == 5)
		{
			isSmart = true;
			count = 0;
		}
		else
			count++;
	}
	else
		if (rand() % 20 == 1)// randomally once in 20
			isSmart = false;


	setDirection(calculateNextBest(pacmanPos, b));
	Point next = nextPos(_pos, _direction);
	while (b.mati[next.getY()][next.getX()] == '#')
	{
		setDirection(calculateNextBest(pacmanPos, b));
		next = nextPos(_pos, _direction);
	}
	erase(isCrumb);
	step();
	setTextColor(_color);
	_pos.draw(_figure);
	// The function erase the ghost from the cuurent position, 
	// change it's position to a random direction and draw it again there.


}

/*

int GhostGood::calculateNextGood(const Point& pacmanPos, const Board& b)
{
	int pacmanX = pacmanPos.getX(), pacmanY = pacmanPos.getY();
	int ghostX = _pos.getX(), ghostY = _pos.getY();
	double pathUp = MAXPATH, pathRight = MAXPATH, pathDown = MAXPATH, pathLeft = MAXPATH;
	if (isValid(UP, b) && _direction != DOWN)
		pathUp = sqrt(pow(abs(pacmanX - ghostX), 2) + pow(abs(pacmanY - (ghostY - 1)), 2));
	if (isValid(RIGHT, b) && _direction != LEFT)
		pathRight = sqrt(pow(abs(pacmanX - (ghostX + 1)), 2) + pow((abs(pacmanY - ghostY)), 2));
	if (isValid(DOWN, b) && _direction != UP)
		pathDown = sqrt(pow(abs(pacmanX - ghostX), 2) + pow(abs(pacmanY - (ghostY + 1)), 2));
	if (isValid(LEFT, b) && _direction != RIGHT)
		pathLeft = sqrt(pow(abs(pacmanX - (ghostX - 1)), 2) + pow((abs(pacmanY - ghostY)), 2));

	int newDir = checkMinimal(pathUp, pathRight, pathDown, pathLeft);
	//if (newDir = _direction)
	//	newDir = abs(newDir - 2);
	return newDir;
}

bool GhostGood::isValid(int dir, const Board& b)
{
	Point next = nextPos(_pos, dir);
	if (b.mati[next.getY()][next.getX()] == '#')
		return false;
	else
		return true;
}
*/