// Specific Pokemon class

#ifndef CHARMANDER_H
#define CHARMANDER_H
#include <string>
#include <cstdlib> // for randomness
#include "Pokemon.h" // include base class
#include "Moves.h" // to give Pokemon moves

using namespace std;

class Charmander: public Pokemon {
	public:
		Charmander(); // default constructor
		~Charmander(); // non-default constructor

	private:
		// Include sprites here


};

#endif

Charmander::Charmander() { // default constructor
	
	// SET STATS
	
	Pokemon::setmaxHealth(100); // set max health
	Pokemon::setcurrHealth(100); // set curr to max Health
	Pokemon::setlevel(1);
	Pokemon::setexp(0);
	Pokemon::setmaxAttack((rand() % 7) + 3); // random attack stat
	Pokemon::setcurrAttack(Pokemon::getmaxAttack()); // set curr to max
	Pokemon::setmaxDef(rand() % 7); // random def stat
	Pokemon::setcurrDef(Pokemon::getmaxDef()); // set curr to random
	Pokemon::setmaxSpeed(rand() % 7); // random speed stat
	Pokemon::setcurrSpeed(Pokemon::getmaxSpeed()); // set curr to max
	Pokemon::setKO(0); // not KO'd
	Pokemon::settype("fire");

	// SET MOVES

	Pokemon::setMoves(1, 2, 4, 8, 14); // scratch,ember,cut,headbutt,flamethrower
}
