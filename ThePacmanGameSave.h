#ifndef _THEPACMANGAMESAVE_H_
#define _THEPACMANGAMESAVE_H_

#include "ThePacmanGame.h"

//ThePacmanGameSave
class ThePacmanGameSave : public ThePacmanGame {
private:
	// The game functions
	//void init();
	void run(string name) override;
	void createNewfileNames(string name, string& stepsFileName, string& resultFileName);
	char DirectionToChar(const int direction);

public:
	//void start();
};

#endif