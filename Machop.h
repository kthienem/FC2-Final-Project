// Specific Pokemon class

#ifndef MACHOP_H
#define MACHOP_H
#include <string>
#include <cstdlib> // for randomness
#include "Pokemon.h" // include base class
#include "Moves.h" // to give Pokemon moves

using namespace std;

class Machop: public Pokemon {
	public:
		Machop(); // default constructor
		~Machop(); // non-default constructor

	private:
		// Include sprites here


};

#endif

Machop::Machop() { // default constructor
	
	// SET STATS
	
	Pokemon::setmaxHealth(28); // set max health
	Pokemon::setcurrHealth(28); // set curr to max Health
	Pokemon::setlevel(1);
	Pokemon::setexp(0);
	Pokemon::setmaxAttack((rand() % 7) + 2); // random attack stat
	Pokemon::setcurrAttack(Pokemon::getmaxAttack()); // set curr to max
	Pokemon::setmaxDef(rand() % 7); // random def stat
	Pokemon::setcurrDef(Pokemon::getmaxDef() + 1); // set curr to random
	Pokemon::setmaxSpeed((rand() % 7)); // random speed stat
	Pokemon::setcurrSpeed(Pokemon::getmaxSpeed()); // set curr to max
	Pokemon::setKO(0); // not KO'd
	Pokemon::setmoveLevel(6); // learns new move at lv 9
	Pokemon::settype("fighting");
	Pokemon::setname("Machop");
	Pokemon::setweak("flying");

	// SET MOVES

	Pokemon::setMoves(0, 6, 8, 20, 23); // tackle,karatechop,headbutt
										// low kick, seismic toss
}
