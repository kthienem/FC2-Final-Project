// Player.h
// Interface file for Pokemon Player class

#ifndef PLAYER_H
#define PLAYER_H
#include "Pokemon.h"
using namespace std;

class Player {

	public:
		Player(); // constructor
		void Battle(); // initiate battle sequence
		void Menu(); // initiate menu interface
		void Roam(); // initiate field interface (free roam)
		void add_pokemon(int);
		void add_wild(Pokemon&);
	private:
		int createPokemon;
		Pokemon *myPoke[6]; // array of pointers to pokemon of player's pokemon		
		//Pokemon Pika(1);
};
Player::Player(){
	createPokemon = 0;
	myPoke[createPokemon] = new Pokemon(0); //first pokemon pika
}

void Player::Battle(){
	int pokeNum;
	do{
	cout << "What pokemon would you like to use: ";
	cin >> pokeNum;
	pokeNum--;
	}while(((pokeNum) < createPokemon) && ((pokeNum)>=0)); ??
	(*myPoke[pokeNum]).attack(1); // test using attack 1
}

//function for adding a new pokemon into players array
void Player::add_pokemon(int PokeNum){
	if(createPokemon < 5){
		myPoke[createPokemon++] = new Pokemon(PokeNum);
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
