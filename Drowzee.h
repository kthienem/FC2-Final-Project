// Specific Pokemon class

#ifndef DROWZEE_H
#define DROWZEE_H
#include <string>
#include <cstdlib> // for randomness
#include "Pokemon.h" // include base class
#include "Moves.h" // to give Pokemon moves

using namespace std;

class Drowzee: public Pokemon {
	public:
		Drowzee(); // default constructor
		~Drowzee(); // non-default constructor

	private:
		// Include sprites here


};

#endif

Drowzee::Drowzee() { // default constructor
	
	// SET STATS
	
	Pokemon::setmaxHealth(24); // set max health
	Pokemon::setcurrHealth(24); // set curr to max Health
	Pokemon::setlevel(1);
	Pokemon::setexp(0);
	Pokemon::setmaxAttack((rand() % 7) + 1); // random attack stat
	Pokemon::setcurrAttack(Pokemon::getmaxAttack()); // set curr to max
	Pokemon::setmaxDef((rand() % 7) + 3); // random def stat
	Pokemon::setcurrDef(Pokemon::getmaxDef()); // set curr to random
	Pokemon::setmaxSpeed((rand() % 7)); // random speed stat
	Pokemon::setcurrSpeed(Pokemon::getmaxSpeed()); // set curr to max
	Pokemon::setKO(0); // not KO'd
	Pokemon::setmoveLevel(6); // learns new move at lv 9
	Pokemon::settype("psychic");
	Pokemon::setname("Drowzee");
	Pokemon::setweak("psychic");

	// SET MOVES

	Pokemon::setMoves(0, 17, 28, 19, 18); // tackle,confusion,bodyslam,psybeam,psychic
}
