// Specific Pokemon class

#ifndef RATTATA_H
#define RATTATA_H
#include <string>
#include <cstdlib> // for randomness
#include "Pokemon.h" // include base class
#include "Moves.h" // to give Pokemon moves

using namespace std;

class Rattata: public Pokemon {
	public:
		Rattata(); // default constructor
		~Rattata(); // non-default constructor

	private:
		// Include sprites here


};

#endif

Rattata::Rattata() { // default constructor
	
	// SET STATS

	Pokemon::setnum(1); // set pokemon number	
	Pokemon::setmaxHealth(24); // set max health
	Pokemon::setcurrHealth(24); // set curr to max Health
	Pokemon::setlevel(1);
	Pokemon::setexp(0);
	Pokemon::setmaxAttack((rand() % 7) + 2); // random attack stat
	Pokemon::setcurrAttack(Pokemon::getmaxAttack()); // set curr to max
	Pokemon::setmaxDef(rand() % 7); // random def stat
	Pokemon::setcurrDef(Pokemon::getmaxDef()); // set curr to random
	Pokemon::setmaxSpeed((rand() % 7) + 1); // random speed stat
	Pokemon::setcurrSpeed(Pokemon::getmaxSpeed()); // set curr to max
	Pokemon::setKO(0); // not KO'd
	Pokemon::setmoveLevel(6); // learns new move at lv 9
	Pokemon::settype("normal");
	Pokemon::setname("Rattata");
	Pokemon::setweak("fighting");

	// SET MOVES

	Pokemon::setMoves(0, 1, 4, 13, 8); // tackle,scratch,cut,quickattack,headbutt
}
