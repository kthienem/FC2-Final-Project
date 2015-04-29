// Specific Pokemon class

#ifndef ELECTABUZZ_H
#define ELECTABUZZ_H
#include <string>
#include <cstdlib> // for randomness
#include "Pokemon.h" // include base class
#include "Moves.h" // to give Pokemon moves

using namespace std;

class Electabuzz: public Pokemon {
	public:
		Electabuzz(); // default constructor
		~Electabuzz(); // non-default constructor

	private:
		// Include sprites here


};

#endif

Electabuzz::Electabuzz() { // default constructor
	
	// SET STATS
	
	Pokemon::setnum(8); // set pokemon number	
	Pokemon::setmaxHealth(30); // set max health
	Pokemon::setcurrHealth(30); // set curr to max Health
	Pokemon::setlevel(1);
	Pokemon::setexp(0);
	Pokemon::setmaxAttack((rand() % 7) + 3); // random attack stat
	Pokemon::setcurrAttack(Pokemon::getmaxAttack()); // set curr to max
	Pokemon::setmaxDef((rand() % 7) + 2); // random def stat
	Pokemon::setcurrDef(Pokemon::getmaxDef()); // set curr to random
	Pokemon::setmaxSpeed((rand() % 7)); // random speed stat
	Pokemon::setcurrSpeed(Pokemon::getmaxSpeed()); // set curr to max
	Pokemon::setKO(0); // not KO'd
	Pokemon::setmoveLevel(8); // learns new move at lv 9
	Pokemon::settype("electric");
	Pokemon::setname("Electabuzz");
	Pokemon::setweak("grass");

	// SET MOVES

	Pokemon::setMoves(0, 5, 6, 29, 16); // tackle,shock,karate chop,swift,thuderbolt
}
