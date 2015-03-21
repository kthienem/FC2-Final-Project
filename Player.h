// Player.h
// Interface file for Pokemon Player class

#ifndef PLAYER_H
#define PLAYER_H
#include "Pokemon.h"

class Player {

	public:
		Player(); // constructor
		void Battle(); // initiate battle sequence
		void Menu(); // initiate menu interface
		void Roam(); // initiate field interface (free roam)

	private:
		Pokemon *myPoke[6]; // array of pointers to pokemon of player's pokemon		

};

#endif
