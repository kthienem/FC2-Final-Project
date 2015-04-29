// Specific Pokemon class

#ifndef ABRA_H
#define ABRA_H
#include <string>
#include <cstdlib> // for randomness
#include "Pokemon.h" // include base class
#include "Moves.h" // to give Pokemon moves

using namespace std;

class Abra: public Pokemon {
	public:
		Abra(); // default constructor
		~Abra(); // non-default constructor

	private:
		// Include sprites here


};

#endif

Abra::Abra() { // default constructor
	
	// SET STATS
	
	Pokemon::setnum(16); // set pokemon number	
	Pokemon::setmaxHealth(20); // set max health
	Pokemon::setcurrHealth(20); // set curr to max Health
	Pokemon::setlevel(1);
	Pokemon::setexp(0);
	Pokemon::setmaxAttack((rand() % 7) + 2); // random attack stat
	Pokemon::setcurrAttack(Pokemon::getmaxAttack()); // set curr to max
	Pokemon::setmaxDef(rand() % 7); // random def stat
	Pokemon::setcurrDef(Pokemon::getmaxDef()); // set curr to random
	Pokemon::setmaxSpeed((rand() % 7) + 2); // random speed stat
	Pokemon::setcurrSpeed(Pokemon::getmaxSpeed()); // set curr to max
	Pokemon::setKO(0); // not KO'd
	Pokemon::setmoveLevel(6); // learns new move at lv 9
	Pokemon::settype("psychic");
	Pokemon::setname("Abra");
	Pokemon::setweak("psychic");

	// SET MOVES

	Pokemon::setMoves(17, 19, 29, 13, 18); // confusion,psybeam,swift,quickattack,psychic
}
