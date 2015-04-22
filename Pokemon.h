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
#include <cstdlib>
#include <ctime>
#include "Moves.h"

using namespace std;

class Pokemon{
	public:
		Pokemon(int=0);
		void levelUp();	// level+1, add attack,def,reset exp
		//virtual void attack() = 0;
		void attack(int); //takes in input for what move to use
		void swapAttack(int);// input will be index of attackName for the attack being added. This will ask the user if they want to swap attackName[x] with attackName[y] and then executes the swap
		void subHealth(int); // decrements health of Pokemon
		void addHealth(int); // increments health of Pokemon
		int getcurrHealth(); // return current health
		void setcurrHealth(int); // sets a new current health
		void inccurrHealth(int);
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
Pokemon::Pokemon(int pokeNum){
	//initalize moves
	int mv1, mv2, mv3, mv4;
	switch(pokeNum) {
		case 0: //pika
			mv1 = 0;
			mv2 = 5;
			mv3 = 10;
			mv4 = 7;
			break;
		case 1: //char
			mv1 = 6;
			mv2 = 2;
			mv3 = 0;
			mv4 = 1;
			break;
		case 2: //squirtle
			mv1 = 3;
			mv2 = 6;
			mv3 = 8;
			mv4 = 4;
			break;
	};
//here is where we set for each type of pokemon

// Allow for randomization
srand(time(NULL)); // seed rand
	
//create a move //0 stands for tackle, 1 scratch, 2 ember, 3 watergun

	Moves move1(mv1), move2(mv2), move3(mv3), move4(mv4);
	
	myMoves.push_back(move1); //push move into pokemon myMove vector
	myMoves.push_back(move2);
	myMoves.push_back(move3);
	myMoves.push_back(move4);
	maxAttack = 10; //set maxAttack
	currHealth = 100;
	KO = 0;

}

void Pokemon::levelUp() {
	maxHealth+=10; // increment max health
	currHealth = maxHealth; // leveling up fully restores health
	level++; // increment level by 1
	exp = 0; // set experience back to zero
	maxAttack += rand() % 5; // add a random attack stat amount
	// Don't increment current attack?
	maxDef += rand() % 5; // increment defense by a random stat amount
	// Don't increment current def?
	maxSpeed += rand() % 5; // increment speed
	// Don't increment current Speed?
	// KO does not change
	
	// If necessary, add a new move
/*
	myMoves[0] = myMoves[1];
	myMoves[1] = myMoves[2];
	myMoves[2] = myMoves[3];
	myMoves.pop_back(); // remove move from back
	myMoves.push_back(newmove); // add new move to the back
*/
}

int Pokemon::getcurrHealth() {
	return currHealth; // returns current health
}

void Pokemon::setcurrHealth(int h) {
	if(h > 0) 
		currHealth = h; // sets new health
	else
		currHealth = 0;
}

void Pokemon::inccurrHealth(int add) {
	currHealth += add; //inc health for potion and other increases
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
		subHealth(damage); // subtract health based on damage dealt
		}
	}
}

void Pokemon::subHealth(int dam) {
	cout << "Health was: " << getcurrHealth() << endl;
	int damage = dam - currDef; // calculate damage 
	if(damage > 0) {
		setcurrHealth((getcurrHealth() - damage)); // reduce current health
	}
	else {
		setcurrHealth(getcurrHealth() - 1); // at least one point of health is always lost
	}
	if(getcurrHealth() <= 0) { // check if pokemon is KO'd
		KO = 1; // if health falls below 0, Pokemon becomes KO'd
		cout << "KO" << endl;
	}
	cout << "Health is now: " << getcurrHealth() << endl;
}

void Pokemon::addHealth(int add) {
	if(add >= 0) {
		setcurrHealth((getcurrHealth() + add)); // increments health
	}	
}

#endif
