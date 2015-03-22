#ifndef POKEMON_H
#define POKEMON_H

#include "Moves.h"
using namespace std;

class Pokemon{
	public:
		Pokemon();
		levelUp();	// level+1, add attack,def,reset exp
		virtual void attack() = 0;
		swapAttack(int);// input will be index of attackName for the attack being added. This will ask the user if they want to swap attackName[x] with attackName[y] and then executes the swap
		changeHealth();//edits current health in battle mode
	private:
		int maxHealth;
		int currHealth;
		int level;
		int exp;
		int maxAttack;
		int currAttack;
		int maxDef;
		int currDef;
		int maxSpeed;
		int currSpeed;
		int KO;	// boolean var
		string type;
		vector <string> attackName;//how can we still utilize this given the Moves class?
		vector <Moves> myMoves;// then we can use for example myMoves[2].attack()
};

#endif

pickPoke() {
	switch(whichPoke) {
		case: POKE_PIKACHU
			pikachu();
			break;
		case: POKE_BULBASAUR
			bulbasaur();
			break;
		case: POKE_CHARMANDER
			charmander();
			break;
		case: POKE_SQUIRTLE
			squirtle();
			break;
	};
}

pikachu() {
	//set level,exp,health,att,def,etc
}
