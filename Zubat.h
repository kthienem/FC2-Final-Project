// Specific Pokemon class

#ifndef ZUBAT_H
#define ZUBAT_H
#include <string>
#include <cstdlib> // for randomness
#include "Pokemon.h" // include base class
#include "Moves.h" // to give Pokemon moves

using namespace std;

class Zubat: public Pokemon {
	public:
		Zubat(); // default constructor
		~Zubat(); // non-default constructor

	private:
		// Include sprites here


};

#endif

Zubat::Zubat() { // default constructor
	
	// SET STATS
	
	Pokemon::setmaxHealth(26); // set max health
	Pokemon::setcurrHealth(26); // set curr to max Health
	Pokemon::setlevel(1);
	Pokemon::setexp(0);
	Pokemon::setmaxAttack((rand() % 7) + 2); // random attack stat
	Pokemon::setcurrAttack(Pokemon::getmaxAttack()); // set curr to max
	Pokemon::setmaxDef(rand() % 7); // random def stat
	Pokemon::setcurrDef(Pokemon::getmaxDef()); // set curr to random
	Pokemon::setmaxSpeed((rand() % 7) + 2); // random speed stat
	Pokemon::setcurrSpeed(Pokemon::getmaxSpeed()); // set curr to max
	Pokemon::setKO(0); // not KO'd
	Pokemon::setmoveLevel(4); // learns new move at lv 9
	Pokemon::settype("flying");
	Pokemon::setname("Zubat");
	Pokemon::setweak("electric");

	// SET MOVES

	Pokemon::setMoves(1, 29, 7, 12, 17); // scratch,swift,gust,wingattack,confusion
}
