//Nicholas Lombardo
//3_25_15
//pokemon.h
//Implementation file for Pokemon class, this class has moves as objects through composition
//Constructor right now gives four basic moves to the pokemon
//
//Eventually you can set up what pokemon gets what moves through this class, to do this maybe take in an int and it will set the rest up.

#ifndef POKEMON_H
#define POKEMON_H

#include <string>
#include <vector>
#include <iostream>
#include "Moves.h"
using namespace std;

class Pokemon{
	public:
		Pokemon();
		void levelUp();	// level+1, add attack,def,reset exp
		//virtual void attack() = 0;
		void attack(int); //takes in input for what move to use
		void swapAttack(int);// input will be index of attackName for the attack being added. This will ask the user if they want to swap attackName[x] with attackName[y] and then executes the swap
		void changeHealth();//edits current health in battle mode
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

/*
void pickPoke(string whichPoke) {
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
*/
//void pikachu() {
	//set level,exp,health,att,def,etc and moves too
//}
Pokemon::Pokemon(){
	//initalize moves
	Moves move1(0), move2(1), move3(2), move4(3);
//create a move //0 stands for tackle, 1 scratch, 2 ember, 3 watergun
	myMoves.push_back(move1); //push move into pokemon myMove vector
	myMoves.push_back(move2);
	myMoves.push_back(move3);
	myMoves.push_back(move4);
	maxAttack = 10; //set maxAttack
}

void Pokemon::attack(int moveNum){ //attack function that calls certain moveNumber
	int userNum; //input by user, can be removed
	while (userNum != -1){ //just a way to check each attack
		cout << "What attack would you like to use (1-4): "; //1-2 right now
		cin >> userNum; //user enters attack
	if(userNum > 0 && userNum < 5){ //makes sure attack is in vector and 1 - 4
		cout << "Move number "<< userNum <<" is " << myMoves[userNum-1].display() << endl;
		int damage = myMoves[userNum-1].attack(maxAttack); //calculate damage eventually take into account weaknesses and such
		cout << "The attack did " << damage << " damage!" << endl; //display results
		}
	}
}

#endif
