// Specific Pokemon class

#ifndef SNORLAX_H
#define SNORLAX_H
#include <string>
#include <cstdlib> // for randomness
#include "Pokemon.h" // include base class
#include "Moves.h" // to give Pokemon moves

using namespace std;

class Snorlax: public Pokemon {
	public:
		Snorlax(); // default constructor
		~Snorlax(); // non-default constructor

	private:
		// Include sprites here


};

#endif

Snorlax::Snorlax() { // default constructor
	
	// SET STATS
	
	Pokemon::setnum(2); // set pokemon number	
	Pokemon::setmaxHealth(40); // set max health
	Pokemon::setcurrHealth(40); // set curr to max Health
	Pokemon::setlevel(1);
	Pokemon::setexp(0);
	Pokemon::setmaxAttack((rand() % 5)); // random attack stat
	Pokemon::setcurrAttack(Pokemon::getmaxAttack()); // set curr to max
	Pokemon::setmaxDef((rand() % 7) + 4); // random def stat
	Pokemon::setcurrDef(Pokemon::getmaxDef()); // set curr to random
	Pokemon::setmaxSpeed((rand() % 5)); // random speed stat
	Pokemon::setcurrSpeed(Pokemon::getmaxSpeed()); // set curr to max
	Pokemon::setKO(0); // not KO'd
	Pokemon::setmoveLevel(6); // learns new move at lv 9
	Pokemon::settype("normal");
	Pokemon::setname("Snorlax");
	Pokemon::setweak("fighting");

	// SET MOVES

	Pokemon::setMoves(0, 8, 9, 21, 28); // tackle,headbutt,bite,slash,bodyslam
}
