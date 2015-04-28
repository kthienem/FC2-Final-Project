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
		void add_pokemon(int, int); //add new pokemon, first int is pokemon number and second is player
		void add_wild(Pokemon&); //
		int checkValidPoke(int);
		int noValid();
		int noValid_other();
		void wild_battle();
		void pokeCenter();
		void player_battle(int);
		void create_trainer(int);
	private:
		int createPokemon; // number of pokemon owned by player
		vector <Pokemon*> myPoke; //array for pointers
		vector <Pokemon*> otherPoke; //array for pointers
		int cp; //current pokemon
		//Pokemon *myPoke[6]; // array of pointers to pokemon of player's pokemon
		////Pokemon *otherPoke[6];
		////Pokemon Pika(1);
};
Player::Player(){
	add_pokemon(1,1); // add a new pikachu to player pokemon
	add_pokemon(2,1);
	add_pokemon(3,1); // add a new squirtle to player pokemon
	add_pokemon(4,1);
	cp = 0;
}
void Player::Battle(){
//	wild_battle();
//	pokeCenter();
//	player_battle();	
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
int Player::checkValidPoke(int pokeCheck){
	if(pokeCheck > (myPoke.size()-1)) //make sure it is a valid location
		return 0; //pokemon is not valid
	else if((*myPoke[pokeCheck]).getKO() == 0) //if it is KOed return 0
		return 1;
	else
		return 0; //poke is not valid
}

int Player::noValid(){
	int found = 0;
	for(int i = 0; i < myPoke.size(); i++){
		if((*myPoke[i]).getKO() == 0){
			found = 1;
			cout << (*myPoke[i]).getname() << " in position " << i << " is ready to fight" << endl;
		}
	}
	if(found)
		return 0;
	else
		return 1; //no valid pokemon
}
int Player::noValid_other(){
	int found = 0;
	for(int i = 0; i < otherPoke.size(); i++){
		if((*otherPoke[i]).getKO() == 0){
			found = 1;
		}
	}
	if(found)
		return 0;
	else
		return 1; //no valid pokemon
}
void Player::wild_battle(){	
	otherPoke.clear();
	add_pokemon((rand()%4)+1,0); // comp rand
	int op = 0; //opponent poke
	int	battleOn = 1;
	int userMove;
	int compMove;
	cout << "Wild Battle!" << endl;
	while(battleOn){
		if((*myPoke[cp]).getKO()){ //make sure current poke isn't KO'ed
			if(noValid()){
				cout << "No useable pokemon, seg fault!" << endl;
				break;
			}
			else{
				while(1){
					cout << "Choose next Poke: ";
					cin >> cp;
					if(checkValidPoke(cp)){ //if there exists another pokemon not ko'ed
						cout << "I choose you " << (*myPoke[cp]).getname() << " !" << endl;
						break;
					}
					cout << "Please pick a valid pokemon!" << endl;
					if(cp == -1) break; //break case if entered value is -1
				}
			}
		}
		cout << endl << "-----------------------" << endl;
		cout << "What move would you like to use: ";
		cin >> userMove;
		
		while((userMove > 5) || (userMove < 1)){
			cout << "Please choose a valid move (1-4): ";
			cin >> userMove;
		}
		if(userMove == 5){ //catch, problem with if you choose an invalid number then it doesn't load the catch function, should use sub functions to change that
			int value = rand()%100;
			cout << "Rand value: " << value << " Your chance was: " << (((*otherPoke[op]).getmaxHealth()/(*otherPoke[op]).getcurrHealth())*7) << endl;
			if(value < (((*otherPoke[op]).getmaxHealth()/(*otherPoke[op]).getcurrHealth())*7)){
				cout << "Caught" << endl;
				myPoke.push_back(otherPoke[op]);
				otherPoke.clear(); //get rid of him
				break;
			}
		}
		else if(userMove != 5){ //skip attack phase if used
			cout << (*myPoke[cp]).getname() << " attacked with " << (*myPoke[cp]).attackname(userMove-1) << endl;
			(*myPoke[cp]).attack(userMove,otherPoke[op]); // test using attack 1
			cout << "Their health: " << (*otherPoke[op]).getcurrHealth() << endl;
			cout << "Your health: " << (*myPoke[cp]).getcurrHealth() << endl;
		}
		if((*otherPoke[op]).getKO()){
			cout << "Wild pokemon has fainted" << endl;
			(*myPoke[cp]).incExp(25); //+25 exp
			break;	
		}	
		cout << endl << "-----------------------" << endl;
		compMove = (rand() % 4) + 1;
		cout << "Wild "<< (*otherPoke[op]).getname() << " attacked with " << (*otherPoke[op]).attackname(compMove-1) << endl;
		(*otherPoke[op]).attack(compMove,myPoke[cp]); // comp move random 1-4
		cout << "Your health: " << (*myPoke[cp]).getcurrHealth() << endl;
		cout << "Their health: " << (*otherPoke[op]).getcurrHealth() << endl;
		if((*myPoke[cp]).getKO()){ //make sure current poke isn't KO'ed
			if(noValid()){
				cout << "No useable pokemon, seg fault!" << endl;
				break;
			}
			else{
				while(1){
					cout << "Choose next Poke: ";
					cin >> cp;
					if(checkValidPoke(cp)){ //if there exists another pokemon not ko'ed
						cout << "I choose you " << (*myPoke[cp]).getname() << " !" << endl;
						break;
					}
					cout << "Please pick a valid pokemon!" << endl;
					if(cp == -1) break; //break case if entered value is -1
				}
			}
		}
	}
}
void Player::player_battle(int pick){
	otherPoke.clear(); //clear other so that it can be filled with player pokemon	
	create_trainer(pick);
	int op = 0; //opponent poke
	int	battleOn = 1;
	int userMove;
	int compMove;
	cout << "Player Battle! vs Gary!" << endl;
	while(battleOn){
		if((*myPoke[cp]).getKO()){ //make sure current poke isn't KO'ed
			if(noValid()){
				cout << "No useable pokemon, seg fault!" << endl;
				break;
			}
			else{
				while(1){
					cout << "Choose next Poke: ";
					cin >> cp;
					if(checkValidPoke(cp)){ //if there exists another pokemon not ko'ed
						cout << "I choose you " << (*myPoke[cp]).getname() << " !" << endl;
						break;
					}
					cout << "Please pick a valid pokemon!" << endl;
					if(cp == -1) break; //break case if entered value is -1
				}
			}
		}
		cout << endl << "-----------------------" << endl;
		cout << "What move would you like to use: ";
		cin >> userMove;
		while((userMove > 4) || userMove < 1){
			cout << "Please choose a valid move (1-4):";
			cin >> userMove;
		}
		cout << (*myPoke[cp]).getname() << " attacked with " << (*myPoke[cp]).attackname(userMove-1) << endl;
		(*myPoke[cp]).attack(userMove,otherPoke[op]); //attack their pokemon with move
		cout << "Their health: " << (*otherPoke[op]).getcurrHealth() << endl;
		cout << "Your health: " << (*myPoke[cp]).getcurrHealth() << endl;
		if((*otherPoke[op]).getKO()){
			cout << "Pokemon has fainted" << endl;
			(*myPoke[cp]).incExp(25); //+25 exp
			if(noValid_other()){
				cout << "You have defeated Gary!" << endl;
				break;
			}
			else{
				op++; //next user pokemon
			}
		}	
		cout << endl << "-----------------------" << endl;
		cout << "Gary's " << (*otherPoke[op]).getname() << " attacked!" << endl;
		compMove = (rand() % 4) + 1;
		(*otherPoke[op]).attack(compMove,myPoke[cp]); // comp move random 1-4
		cout << "Your health: " << (*myPoke[cp]).getcurrHealth() << endl;
		cout << "Their health: " << (*otherPoke[op]).getcurrHealth() << endl;
		if((*myPoke[cp]).getKO()){ //make sure current poke isn't KO'ed
			if(noValid()){
				cout << "No useable pokemon, seg fault!" << endl;
				break;
			}
			else{
				while(1){
					cout << "Choose next Poke: ";
					cin >> cp;
					if(checkValidPoke(cp)){ //if there exists another pokemon not ko'ed
						cout << "I choose you " << (*myPoke[cp]).getname() << " !" << endl;
						break;
					}
					cout << "Please pick a valid pokemon!" << endl;
					if(cp == -1) break; //break case if entered value is -1
				}
			}
		}
		
	}
}

void Player::pokeCenter(){
	for(int i = 0; i < myPoke.size(); i++){
		(*myPoke[i]).heal();
	}
}

void Player::create_trainer(int pick){
	switch(pick){
		case 1: //chick fire and norm
			add_pokemon(1,0); 
			add_pokemon(3,0); 
			break;
		case 2: //yellow electric fight
			add_pokemon(3,0);
			add_pokemon(4,0);
			add_pokemon(1,0); 
			add_pokemon(1,0);
			break;
		case 3: //ranger grass and fly
			add_pokemon(1,0);
			add_pokemon(2,0); 
			break;
		case 4: //safari grass and water
			add_pokemon(3,0);
			add_pokemon(3,0);
			add_pokemon(1,0); 
			break;
		case 5: //fisherman water and norm
			add_pokemon(2,0);
			add_pokemon(2,0);
			break;
		case 6: //gym leader psychic
			add_pokemon(2,0);
		default:
			add_pokemon(1,0);
			add_pokemon(4,0);
			break;
	}
}
#endif
