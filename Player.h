// Player.h
// Interface file for Pokemon Player class

#ifndef PLAYER_H
#define PLAYER_H
#include <vector>
#include "Pokemon.h"
#include "Squirtle.h"
#include "Pikachu.h"
#include "Bulbasaur.h"
#include "Charmander.h"
using namespace std;

class Player {
	public:
		Player(); // constructor
		void Battle(); // initiate battle sequence
		void Menu(); // initiate menu interface
		void Roam(); // initiate field interface (free roam)
		void add_pokemon(int, int);
		void add_wild(Pokemon&);
	private:
		int createPokemon; // number of pokemon owned by player
		vector <Pokemon*> myPoke; //array for pointers
		vector <Pokemon*> otherPoke; //array for pointers
		//Pokemon *myPoke[6]; // array of pointers to pokemon of player's pokemon
		////Pokemon *otherPoke[6];
		////Pokemon Pika(1);
};
Player::Player(){
	add_pokemon(1,1); // add a new pikachu to player pokemon
	add_pokemon(1,0); //pikachu to do damage to
/*
//myPoke[createPokemon] = new Pokemon(0); //first pokemon pika
//createPokemon++; // new pokemon added
*/
}
void Player::Battle(){
	(*myPoke[0]).attack(otherPoke[0]); // test using attack 1
	cout << "Their health: " << (*otherPoke[0]).getcurrHealth() << endl;
	cout << "Your health: " << (*myPoke[0]).getcurrHealth() << endl;
}
//function for adding a new pokemon into players array
void Player::add_pokemon(int PokeNum, int person){ //person 1 or 0 for player or npc
	Pokemon *newPoke_ptr;
	switch(PokeNum) {
		case 1:
			newPoke_ptr = new Pikachu;
			break;
		case 2:
			newPoke_ptr = new Bulbasaur;
			break;
		case 3:
			newPoke_ptr = new Squirtle;
			break;
		case 4:
			newPoke_ptr = new Charmander;
			break;
	};
	//myPoke[createPokemon++] = new Pokemon(PokeNum);
	if(person == 1){
		myPoke.push_back(newPoke_ptr);
	}
	else{
		otherPoke.push_back(newPoke_ptr);
	}
}
//adds wild pokmeon into player array
void Player::add_wild(Pokemon& wildPoke_ptr){
	if(createPokemon < 5){ //make sure there is room!
	myPoke.push_back(&wildPoke_ptr);
	}
	else{
		cout << "No more room" << endl;
	}
}

void Player::Roam() {
	cout << "Roam" << endl;
}

#endif
