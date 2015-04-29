// Specific Pokemon class

#ifndef MANKEY_H
#define MANKEY_H
#include <string>
#include <cstdlib> // for randomness
#include "Pokemon.h" // include base class
#include "Moves.h" // to give Pokemon moves

using namespace std;

class Mankey: public Pokemon {
	public:
		Mankey(); // default constructor
		~Mankey(); // non-default constructor

	private:
		// Include sprites here


};

#endif

Mankey::Mankey() { // default constructor
	
	// SET STATS
	
	Pokemon::setnum(6); // set pokemon number	
	Pokemon::setmaxHealth(22); // set max health
	Pokemon::setcurrHealth(22); // set curr to max Health
	Pokemon::setlevel(1);
	Pokemon::setexp(0);
	Pokemon::setmaxAttack((rand() % 7) + 4); // random attack stat
	Pokemon::setcurrAttack(Pokemon::getmaxAttack()); // set curr to max
	Pokemon::setmaxDef(rand() % 7); // random def stat
	Pokemon::setcurrDef(Pokemon::getmaxDef()); // set curr to random
	Pokemon::setmaxSpeed((rand() % 7)); // random speed stat
	Pokemon::setcurrSpeed(Pokemon::getmaxSpeed()); // set curr to max
	Pokemon::setKO(0); // not KO'd
	Pokemon::setmoveLevel(7); // learns new move at lv 9
	Pokemon::settype("fighting");
	Pokemon::setname("Mankey");
	Pokemon::setweak("flying");

	// SET MOVES

	Pokemon::setMoves(0, 4, 6, 23, 28); // tackle,cut,karatechop,seismic toss,bodyslam
}
