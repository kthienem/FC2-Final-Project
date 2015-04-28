// Specific Pokemon class

#ifndef PIKACHU_H
#define PIKACHU_H
#include <string>
#include <cstdlib> // for randomness
#include "Pokemon.h" // include base class
#include "Moves.h" // to give Pokemon moves

using namespace std;

class Pikachu: public Pokemon {
	public:
		Pikachu(); // default constructor
		~Pikachu(); // non-default constructor

	private:
		// Include sprites here


};

#endif

Pikachu::Pikachu(){ // default constructor
	
	// SET STATS
	
	Pokemon::setmaxHealth(32); // set max health
	Pokemon::setcurrHealth(32); // set curr to max Health
	Pokemon::setlevel(1);
	Pokemon::setexp(0);
	Pokemon::setmaxAttack(rand() % 7); // random attack stat
	Pokemon::setcurrAttack(Pokemon::getmaxAttack()); // set curr to max
	Pokemon::setmaxDef(rand() % 7); // random def stat
	Pokemon::setcurrDef(Pokemon::getmaxDef()); // set curr to random
	Pokemon::setmaxSpeed(rand() % 7 + 3); // random speed stat
	Pokemon::setcurrSpeed(Pokemon::getmaxSpeed()); // set curr to max
	Pokemon::setKO(0); // not KO'd
	Pokemon::setmoveLevel(7); // learns new move at lv 7
	Pokemon::settype("electric");
	Pokemon::setname("Pikachu");
	//Pokemon::setmaxLevelExp(20);
	// SET MOVES

	Pokemon::setMoves(0, 5, 6, 13, 8); // tackle, shock, doubleslap, quickattack, headbutt
}
