#include "Fruit.h"
#include "Board.h"
#include "ThePacmanGame.h"

void Fruit::move(bool isCrumb)
{
	// The function erase the ghost from the cuurent position, 
	// change it's position to a random direction and draw it again there.
	erase(isCrumb);
	step(_direction);
	setTextColor(_color);
	_pos.draw(_figure);
}

void Fruit::step(int& dir){
	// Checking cases according to the direction
	switch (dir) {
	case UP: // UP
		_pos.setY(_pos.getY() - 1);
		if (_pos.getY() < 0) { _pos.setY(1); dir = DOWN; }
		break;
	case DOWN: // DOWN
		_pos.setY(_pos.getY() + 1);
		if (_pos.getY() > Board::maxY - 1) { _pos.setY(Board::maxY - 2); dir = UP; }
		break;
	case LEFT: // LEFT
		_pos.setX(_pos.getX() - 1);
		if (_pos.getX() < 1) { _pos.setX(2); dir = RIGHT; }
		break;
	case RIGHT: // RIGHT
		_pos.setX(_pos.getX() + 1);
		if (_pos.getX() > Board::maxX) { _pos.setX(Board::maxX - 1); dir = LEFT; }
		break;
	case STAY: // STAY
		break;
	}
}

void Fruit::erase(bool isCrumb)
{
	// Function to draw whatever was in a position before the ghost visit: crumb or space
	if (isCrumb)
	{
		if (Board::colored) { setTextColor(Color::BROWN); }
		_pos.draw('.');
	}
	else
	{
		_pos.draw(' ');
	}
}


void Fruit::randValue()
{
	int figure= (rand() % (MAXVALUE - MINVALUE + 1) + MINVALUE);  
	//_figure = figure - '0';
	_figure = figure + 48;
}

Point Fruit:: randPosition()
{
	int x = rand() % (Board::maxX);
	int y = rand() % (Board::maxY); 
	return Point(x, y);
}


