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

	private:
		Pokemon *myPoke[6]; // array of pointers to pokemon of player's pokemon		
		Pokemon Pika;
};
Player::Player() {
//	Pokemon Pika;
	myPoke[0] = &Pika;

}

void Player::Battle() {
//	myPoke[0]->attack(1);
	(*myPoke[0]).attack(1); // test using attack 1
}

void Player::Roam() {
	cout << "Roam" << endl;
}

#endif
