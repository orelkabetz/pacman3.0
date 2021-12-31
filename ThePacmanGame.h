#ifndef _THEPACMANGAME_H_
#define _THEPACMANGAME_H_

#include "io_utils.h"
#include "Ghost.h"
#include "GhostBest.h"
#include "GhostGood.h"
#include "GhostNovice.h"
#include "Pacman.h"
#include "Fruit.h"
#include "Board.h"

class ThePacmanGame {
protected:
	// Data members
	Board b;
	// Game properties
	int remainedCrumbs;
	int score = 0;
	int lives = 3;
	int ghostLevel=1;
	int screens;
	bool wannaPlay = true;
	bool pacmanDied = false;

//private:
	// The game functions
	void init();
	virtual void run(string name);
	// Printing functions
	void printMenu() const;
	void printFrame() const;
	void printInstructions();
	void printScore() const;
	void printLives() const;
	void printPause() const;
	void printContinue() const;
	void printGoodBye();
	void printNoBoards() const;
	void printLevel(int i) const;
	void printGhostLevel();
	void printScreenChoice();
	// Function to check what the player have chosen in the menu
	void checkChoice();
	void runByScreens();
	bool isBoardValid();
	// Win or Lose functions
	void win();
	void gameOver();
	// Function to set the game on colored mode
	void setColors(Pacman& p, Ghost* g[], Fruit f);
	// Function to calculate the next position of a character
	Point nextPos(Point pos, int dir) const;
	// Function to check if there is a bread crumb in a given point
	bool isBreadCrumb(Point pos);

	// Function to check if a position is free for a fruit
	bool isNotOccupied(Point pos);
	// Function to check whethear pacman and a ghost met
	void pacmanVsGhosts(Pacman& p, Ghost* g[], string name);
	void pacmanVsFruit(Pacman& p, Fruit& f);
	void printMotivation();
	void ghostsVsGhost(int i, Ghost* g[]);
	// Functions to handle the ghosts and fruit moves
	void ghostsMove(bool& wait, bool& stop, bool& isCrumb, Ghost* g[], Pacman& p);
	void fruitsMove(bool& wait, bool& stop, bool& isCrumb, Fruit& f, Pacman& p);
	void handleFruitActivity(Fruit& f);
	void handlePacmanMove(Pacman& p, char& key, int& dir, Point& next);
public:
	void start();
};

#endif