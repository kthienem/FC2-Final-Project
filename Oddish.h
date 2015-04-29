// Specific Pokemon class

#ifndef ODDISH_H
#define ODDISH_H
#include <string>
#include <cstdlib> // for randomness
#include "Pokemon.h" // include base class
#include "Moves.h" // to give Pokemon moves

using namespace std;

class Oddish: public Pokemon {
	public:
		Oddish(); // default constructor
		~Oddish(); // non-default constructor

	private:
		// Include sprites here


};

#endif

Oddish::Oddish() { // default constructor
	
	// SET STATS
	
	Pokemon::setnum(14); // set pokemon number	
	Pokemon::setmaxHealth(23); // set max health
	Pokemon::setcurrHealth(23); // set curr to max Health
	Pokemon::setlevel(1);
	Pokemon::setexp(0);
	Pokemon::setmaxAttack(rand() % 7); // random attack stat
	Pokemon::setcurrAttack(Pokemon::getmaxAttack()); // set curr to max
	Pokemon::setmaxDef((rand() % 7) + 1); // random def stat
	Pokemon::setcurrDef(Pokemon::getmaxDef()); // set curr to random
	Pokemon::setmaxSpeed((rand() % 7) + 1); // random speed stat
	Pokemon::setcurrSpeed(Pokemon::getmaxSpeed()); // set curr to max
	Pokemon::setKO(0); // not KO'd
	Pokemon::setmoveLevel(6); // learns new move at lv 6
	Pokemon::settype("grass");
	Pokemon::setname("Oddish");
	Pokemon::setweak("fire");

	// SET MOVES

	Pokemon::setMoves(0, 11, 15, 29, 17); // tackle,razorleaf,vinewhip,swift,confusion
}
