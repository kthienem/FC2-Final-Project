// Player.h
// Interface file for Pokemon Player class

#ifndef PLAYER_H
#define PLAYER_H
#include "Pokemon.h"
#include "Pikachu.h"
using namespace std;

class Player {

	public:
		Player(); // constructor
		void Battle(); // initiate battle sequence
		void Menu(); // initiate menu interface
		void Roam(); // initiate field interface (free roam)
		Pokemon* add_pokemon(int);
		void add_wild(Pokemon&);
	private:
		int createPokemon; // number of pokemon owned by player
		Pokemon *myPoke[6]; // array of pointers to pokemon of player's pokemon		
		Pokemon *otherPoke[6];
		//Pokemon Pika(1);
};
Player::Player(){
	createPokemon = 0;
	add_pokemon(0); // add a new pikachu to player pokemon
/*
	myPoke[createPokemon] = new Pokemon(0); //first pokemon pika
	createPokemon++; // new pokemon added
*/
}

void Player::Battle(){
	int pokeNum;
	do{
	cout << "What pokemon would you like to use: ";
	cin >> pokeNum;
	pokeNum--;
	}while(((pokeNum) > createPokemon) && ((pokeNum)<0));
	(*myPoke[pokeNum]).attack(1); // test using attack 1
}

//function for adding a new pokemon into players array
Pokemon* Player::add_pokemon(int PokeNum){
	if(createPokemon < 5){
		//myPoke[createPokemon++] = new Pokemon(PokeNum);
		myPoke[createPokemon++] = new Pikachu;
	}
	else{
		cout << "No more room" << endl;
	}
}

//adds wild pokmeon into player array
void Player::add_wild(Pokemon& wildPoke_ptr){
	if(createPokemon < 5){ //make sure there is room!
		myPoke[createPokemon++] = &wildPoke_ptr;
	}
	else{
		cout << "No more room" << endl;
	}
}

void Player::Roam() {
	cout << "Roam" << endl;
}

#endif
