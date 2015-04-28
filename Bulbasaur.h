// Specific Pokemon class

#ifndef BULBASAUR_H
#define BULBASAUR_H
#include <string>
#include <cstdlib> // for randomness
#include "Pokemon.h" // include base class
#include "Moves.h" // to give Pokemon moves

using namespace std;

class Bulbasaur: public Pokemon {
	public:
		Bulbasaur(); // default constructor
		~Bulbasaur(); // non-default constructor

	private:
		// Include sprites here


};

#endif

Bulbasaur::Bulbasaur() { // default constructor
	
	// SET STATS
	
	Pokemon::setmaxHealth(35); // set max health
	Pokemon::setcurrHealth(35); // set curr to max Health
	Pokemon::setlevel(1);
	Pokemon::setexp(0);
	Pokemon::setmaxAttack(rand() % 7); // random attack stat
	Pokemon::setcurrAttack(Pokemon::getmaxAttack()); // set curr to max
	Pokemon::setmaxDef(rand() % 7); // random def stat
	Pokemon::setcurrDef(Pokemon::getmaxDef()); // set curr to random
	Pokemon::setmaxSpeed(rand() % 7); // random speed stat
	Pokemon::setcurrSpeed(Pokemon::getmaxSpeed()); // set curr to max
	Pokemon::setKO(0); // not KO'd
	Pokemon::setmoveLevel(6); // learns new move at lv 6
	Pokemon::settype("grass");
	Pokemon::setname("Bulbsaur");
	Pokemon::setweak("fire");

	// SET MOVES

	Pokemon::setMoves(0, 4, 8, 11, 15); // tackle,cut,headbutt,razorleaf,vinewhip
}
