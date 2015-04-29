// Specific Pokemon class

#ifndef SLOWPOKE_H
#define SLOWPOKE_H
#include <string>
#include <cstdlib> // for randomness
#include "Pokemon.h" // include base class
#include "Moves.h" // to give Pokemon moves

using namespace std;

class Slowpoke: public Pokemon {
	public:
		Slowpoke(); // default constructor
		~Slowpoke(); // non-default constructor

	private:
		// Include sprites here


};

#endif

Slowpoke::Slowpoke() { // default constructor
	
	// SET STATS
	
	Pokemon::setnum(10); // set pokemon number	
	Pokemon::setmaxHealth(28); // set max health
	Pokemon::setcurrHealth(28); // set curr to max Health
	Pokemon::setlevel(1);
	Pokemon::setexp(0);
	Pokemon::setmaxAttack((rand() % 7) + 1); // random attack stat
	Pokemon::setcurrAttack(Pokemon::getmaxAttack()); // set curr to max
	Pokemon::setmaxDef((rand() % 7) + 3); // random def stat
	Pokemon::setcurrDef(Pokemon::getmaxDef()); // set curr to random
	Pokemon::setmaxSpeed((rand() % 5)); // random speed stat
	Pokemon::setcurrSpeed(Pokemon::getmaxSpeed()); // set curr to max
	Pokemon::setKO(0); // not KO'd
	Pokemon::setmoveLevel(6); // learns new move at lv 9
	Pokemon::settype("water");
	Pokemon::setname("Slowpoke");
	Pokemon::setweak("electric");

	// SET MOVES

	Pokemon::setMoves(0, 3, 10, 8, 19); // tackle,watergun,bubblebeam,headbutt,psybeam
}
