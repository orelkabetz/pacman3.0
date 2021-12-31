#include "Point.h"
#include "Pacman.h"
#include "Enum.h"

void Pacman::move(bool isCrumb)
{
	// The function erase pacman from the cuurent position, 
	// change it's position to the chosen direction and draw it again there.
	_pos.draw(' ');
	_pos.movePacman(_direction);
	setTextColor(_color);
	_pos.draw(_figure);
}

int Pacman::changeDirection(char key)
{
	// Firstly we check if it's lower or upper case and changing to lower if needed
	if (isCapital(key))
		changeToLower(key);
	switch (key) {
	case 'w': // UP
		return UP;
	case 'x': // DOWN
		return DOWN;
	case 'a': // LEFT
		return LEFT;
	case 'd': // RIGHT
		return RIGHT;
	case 's': // STAY
		return STAY;
	default:
		return _direction;
	}
}

bool Pacman::isCapital(char key)
{
	// Function that checks if a letter is upper case
	if ((key > 'A') && (key < 'Z'))
		return true;
	else
		return false;
}

void Pacman::changeToLower(char& key)
{
	// Function that changes upper case letters to lower case
	switch (key)
	{
	case 'W':
		key = 'w';
	break;
	case 'A':
		key = 'a';
		break;
	case 'D':
		key = 'd';
		break;
	case 'X':
		key = 'x';
		break;
	case 'S':
		key = 's';
		break;
	}
}

void Pacman::loseLife(bool colored)
{
	for (int i = 0; i < 4; i++)
	{
		gotoxy(_pos.getX(), _pos.getY());

		cout << " ";
		Sleep(200);
		gotoxy(_pos.getX(), _pos.getY());
		if (colored) { setTextColor(Color::RED); }
		cout << "C";
		Sleep(200);
	}
}