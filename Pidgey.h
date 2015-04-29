// Specific Pokemon class

#ifndef PIDGEY_H
#define PIDGEY_H
#include <string>
#include <cstdlib> // for randomness
#include "Pokemon.h" // include base class
#include "Moves.h" // to give Pokemon moves

using namespace std;

class Pidgey: public Pokemon {
	public:
		Pidgey(); // default constructor
		~Pidgey(); // non-default constructor

	private:
		// Include sprites here


};

#endif

Pidgey::Pidgey(){ // default constructor
	
	// SET STATS
	
	Pokemon::setmaxHealth(22); // set max health
	Pokemon::setcurrHealth(22); // set curr to max Health
	Pokemon::setlevel(1);
	Pokemon::setexp(0);
	Pokemon::setmaxAttack((rand() % 7) + 1); // random attack stat
	Pokemon::setcurrAttack(Pokemon::getmaxAttack()); // set curr to max
	Pokemon::setmaxDef(rand() % 7); // random def stat
	Pokemon::setcurrDef(Pokemon::getmaxDef()); // set curr to random
	Pokemon::setmaxSpeed(rand() % 7 + 2); // random speed stat
	Pokemon::setcurrSpeed(Pokemon::getmaxSpeed()); // set curr to max
	Pokemon::setKO(0); // not KO'd
	Pokemon::setmoveLevel(7); // learns new move at lv 7
	Pokemon::settype("flying");
	Pokemon::setname("Pidgey");
	Pokemon::setweak("electric");
	//Pokemon::setmaxLevelExp(20);
	// SET MOVES

	Pokemon::setMoves(0, 7, 13, 22, 12); // tackle,gust,quickattack,peck,wingattack
}
