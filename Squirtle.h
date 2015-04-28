// Specific Pokemon class

#ifndef SQURITLE_H
#define SQURITLE_H
#include <string>
#include <cstdlib> // for randomness
#include "Pokemon.h" // include base class
#include "Moves.h" // to give Pokemon moves

using namespace std;

class Squirtle: public Pokemon {
	public:
		Squirtle(); // default constructor
		~Squirtle(); // non-default constructor

	private:
		// Include sprites here


};

#endif

Squirtle::Squirtle() { // default constructor
	
	// SET STATS
	
	Pokemon::setmaxHealth(30); // set max health
	Pokemon::setcurrHealth(30); // set curr to max Health
	Pokemon::setlevel(1);
	Pokemon::setexp(0);
	Pokemon::setmaxAttack(rand() % 7); // random attack stat
	Pokemon::setcurrAttack(Pokemon::getmaxAttack()); // set curr to max
	Pokemon::setmaxDef((rand() % 7) + 3); // random def stat
	Pokemon::setcurrDef(Pokemon::getmaxDef()); // set curr to random
	Pokemon::setmaxSpeed(rand() % 7); // random speed stat
	Pokemon::setcurrSpeed(Pokemon::getmaxSpeed()); // set curr to max
	Pokemon::setKO(0); // not KO'd
	Pokemon::setmoveLevel(8); // learns new move at lv 8
	Pokemon::settype("water");
	Pokemon::setname("Squirtle");

	// SET MOVES

	Pokemon::setMoves(0, 3, 6, 8, 10); // tackle,watergun,doubleslap,headbutt,bubblebeam
}
