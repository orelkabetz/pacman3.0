#include "GhostBest.h"



void GhostBest::move(const bool& isCrumb, const Point& pacmanPos, const Board& b)
{
	// The function erase the ghost from the cuurent position, 
	// change it's position to a random direction and draw it again there.

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
}
/*
int GhostBest::calculateNextBest(const Point& pacmanPos, const Board& b)
{
	int pacmanX = pacmanPos.getX(), pacmanY = pacmanPos.getY();
	int ghostX = _pos.getX(), ghostY = _pos.getY();
	double pathUp = MAXPATH, pathRight = MAXPATH, pathDown = MAXPATH, pathLeft = MAXPATH;
	if (isValid(UP, b)&& _direction!=DOWN)
		pathUp = sqrt(pow(abs(pacmanX - ghostX), 2) + pow(abs(pacmanY - (ghostY-1)), 2));
	if (isValid(RIGHT, b) && _direction != LEFT)
		pathRight = sqrt(pow(abs(pacmanX - (ghostX+1)), 2) + pow((abs(pacmanY - ghostY)), 2));
	if (isValid(DOWN, b) && _direction != UP)
		pathDown = sqrt(pow(abs(pacmanX - ghostX), 2) + pow(abs(pacmanY - (ghostY+1)), 2));
	if (isValid(LEFT, b) && _direction != RIGHT)
		pathLeft = sqrt(pow(abs(pacmanX - (ghostX-1)), 2) + pow((abs(pacmanY - ghostY)), 2));

	int newDir = checkMinimal(pathUp, pathRight, pathDown, pathLeft);
	//if (newDir = _direction)
	//	newDir = abs(newDir - 2);
	return newDir;
}

bool GhostBest::isValid(int dir, const Board& b)
{
	Point next = nextPos(_pos, dir);
	if (b.mati[next.getY()][next.getX()] == '#')
		return false;
	else
		return true;
}
*/