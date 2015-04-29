// Specific Pokemon class

#ifndef PONYTA_H
#define PONYTA_H
#include <string>
#include <cstdlib> // for randomness
#include "Pokemon.h" // include base class
#include "Moves.h" // to give Pokemon moves

using namespace std;

class Ponyta: public Pokemon {
	public:
		Ponyta(); // default constructor
		~Ponyta(); // non-default constructor

	private:
		// Include sprites here


};

#endif

Ponyta::Ponyta() { // default constructor
	
	// SET STATS
	
	Pokemon::setnum(12); // set pokemon number	
	Pokemon::setmaxHealth(25); // set max health
	Pokemon::setcurrHealth(25); // set curr to max Health
	Pokemon::setlevel(1);
	Pokemon::setexp(0);
	Pokemon::setmaxAttack((rand() % 7) + 2); // random attack stat
	Pokemon::setcurrAttack(Pokemon::getmaxAttack()); // set curr to max
	Pokemon::setmaxDef(rand() % 7); // random def stat
	Pokemon::setcurrDef(Pokemon::getmaxDef()); // set curr to random
	Pokemon::setmaxSpeed((rand() % 7) + 1); // random speed stat
	Pokemon::setcurrSpeed(Pokemon::getmaxSpeed()); // set curr to max
	Pokemon::setKO(0); // not KO'd
	Pokemon::setmoveLevel(5); // learns new move at lv 9
	Pokemon::settype("fire");
	Pokemon::setname("Ponyta");
	Pokemon::setweak("water");

	// SET MOVES

	Pokemon::setMoves(0, 2, 8, 13, 29); // tackle,ember,headbutt,quickattack,swift
}
