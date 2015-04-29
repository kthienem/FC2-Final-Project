// Player.h
// Interface file for Pokemon Player class

#ifndef PLAYER_H
#define PLAYER_H
#include <vector>
#include <fstream>
#include "Pokemon.h"
#include "Squirtle.h"
#include "Pikachu.h"
#include "Bulbasaur.h"
#include "Charmander.h"
#include "Rattata.h"
#include "Snorlax.h"
#include "Pidgey.h"
#include "Zubat.h"
#include "Machop.h"
#include "Mankey.h"
#include "Electabuzz.h"
#include "Slowpoke.h"
#include "Ponyta.h"
#include "Oddish.h"
#include "Drowzee.h"
#include "Abra.h"
using namespace std;

class Player {
	public:
		Player(int=1); // constructor
		void learnNewMove(int);
		void Battle(); // initiate battle sequence
		void Menu(); // initiate menu interface
		void Roam(); // initiate field interface (free roam)
		void add_pokemon(int, int, int); //add new pokemon, first int is pokemon number and second is player
		void add_wild(Pokemon&); //
		int checkValidPoke(int);
		int noValid();
		int noValid_other();
		int whatToDo();
		int switchPoke(int);
		int run();
		int fight_comp();
		int fight(int);
		int catchPoke(int, int);
		int getmyHealth();
		int getmyMaxHealth();
		int getopHealth();
		int getopMaxHealth();
		int mycheckKO(int); //check a certain poke for a ko used to see what switchable pokemon
		int opcheckKO(int);
		int myCurrentPoke(); //get current pokemon's number
		int opCurrentPoke(); //get current pokemon's number for op
		int myLevel();
		int anyLevel(int);
		int opLevel();
		int NextOp(int); //checks if this opint is less than size to see if it can switch to that pokemon for them
		int whatPokeinParty(int);
		int getMoveNum(int);
		int getNumPoke();
		void wild_battle(int);
		void fish_battle(int);
		void pokeCenter();
		void player_battle(int,int);
		void create_trainer(int,int);
		void save_pokemon_stats();
		void load_pokemon_stats();
		void push_poke(string); //myPoke.push_back(new Pokemon);
		int rand_between(int, int);
	private:
		int createPokemon; // number of pokemon owned by player
		vector <Pokemon*> myPoke; //array for pointers
		vector <Pokemon*> otherPoke; //array for pointers
		int cp; //current pokemon
		int op;
		//Pokemon *myPoke[6]; // array of pointers to pokemon of player's pokemon
		////Pokemon *otherPoke[6];
		////Pokemon Pika(1);
};
Player::Player(int newGame){
	if(newGame){
		add_pokemon(7,1,0); // add a new pikachu to player pokemon
		cp = 0;
	}
	else{
		load_pokemon_stats();
	}
}
void Player::Battle(){
//	wild_battle();
//	pokeCenter();
//	player_battle();	
}
//function for adding a new pokemon into players array
void Player::add_pokemon(int PokeNum, int person, int levelDesired){ //person 1 or 0 for player or npc
	Pokemon *newPoke_ptr;
	switch(PokeNum) {
		case 1:
			newPoke_ptr = new Rattata;
			break;
		case 2:
			newPoke_ptr = new Snorlax;
			break;
		case 3:
			newPoke_ptr = new Pidgey;
			break;
		case 4:
			newPoke_ptr = new Zubat;
			break;
		case 5:
			newPoke_ptr = new Machop;
			break;
		case 6:
			newPoke_ptr = new Mankey;
			break;
		case 7:
			newPoke_ptr = new Pikachu;
			break;
		case 8:
			newPoke_ptr = new Electabuzz;
			break;
		case 9:
			newPoke_ptr = new Squirtle;
			break;
		case 10:
			newPoke_ptr = new Slowpoke;
			break;
		case 11:
			newPoke_ptr = new Charmander;
			break;
		case 12:
			newPoke_ptr = new Ponyta;
			break;
		case 13:
			newPoke_ptr = new Bulbasaur;
			break;
		case 14:
			newPoke_ptr = new Oddish;
			break;
		case 15:
			newPoke_ptr = new Drowzee;
			break;
		case 16:
			newPoke_ptr = new Abra;
			break;
	};
	for(int i = 0; i < levelDesired; i++){
		(*newPoke_ptr).levelUp();
	}
	//myPoke[createPokemon++] = new Pokemon(PokeNum);
	if(person == 1){ //limit to 6 here and need to in the catch time too
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
void Player::wild_battle(int level_enc){	
	otherPoke.clear();
	add_pokemon((rand()%16)+1,0,level_enc); // comp rand
	op = 0; //opponent poke
	cout << "Wild Battle!" << endl;
//	int choice;
//	int KOed_poke, KOed_yourpoke;	
//	choice = whatToDo();
//	int newPoke = 0;
//	int move = 0;	
	/*switch(choice){
		case 1: //fight can check speed to decide who first
			if(fight(move)){
				return; //end encounter killed wild poke
			}
			if(fight_comp()){ //computer turn
				cp = switchPoke(newPoke);
				if(cp == -1){
					return; //exit battle no more useable pokemon
				}	
			}
			break;
		case 2: //catch
			if(catchPoke(cp,op)){ //end encouter
				return;
			}
			break;
		case 3: //run
			if(run()){
				return;
			}
			break;
		case 4: //switch poke
			cp = switchPoke(newPoke); //set current poke to who we switch
			if(cp == -1){
				return;
			}
			else if(cp == -2){
				while(1){
					cp = switchPoke(newPoke);
					if(cp != -2){
						break;
					}
				}
			}
			break;

	}*/	

	/*while(battleOn){
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
	}*/
}
void Player::fish_battle(int level_enc){	
	otherPoke.clear();
	add_pokemon(rand_between(9,10),0,level_enc); // comp rand
	op = 0; //opponent poke
/*	int	battleOn = 1;
	int userMove;
	int compMove;
	cout << "Caught a Fish!" << endl;
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
			cout << "Wild fish has fainted" << endl;
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
	}*/
}
void Player::player_battle(int pick, int level_enc){
	otherPoke.clear(); //clear other so that it can be filled with player pokemon	
	create_trainer(pick,level_enc);
	op = 0;
/*
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
		
	}*/
}

void Player::pokeCenter(){
	for(int i = 0; i < myPoke.size(); i++){
		(*myPoke[i]).heal();
	}
}

void Player::create_trainer(int pick, int level_enc){
	otherPoke.clear();	
	switch(pick){
		case 1: //chick fire and norm
			add_pokemon(rand_between(11,12),0,level_enc); //fire
			add_pokemon(rand_between(1,2),0,level_enc); //norm
			break;
		case 2: //fisherman water and norm
			add_pokemon(rand_between(9,10),0,level_enc);
			add_pokemon(rand_between(1,2),0,level_enc);
			break;
		case 3: //ranger grass and fly
			add_pokemon(rand_between(13,14),0,level_enc);
			add_pokemon(rand_between(3,4),0,level_enc);
			break;
		case 4: //safari grass and water
			add_pokemon(rand_between(13,14),0,level_enc);
			add_pokemon(rand_between(9,10),0,level_enc);
			add_pokemon(rand_between(13,14),0,level_enc);
			break;	
		case 5: //yellow electric fight
			add_pokemon(rand_between(7,8),0,level_enc);
			add_pokemon(rand_between(5,6),0,level_enc+1);
			add_pokemon(rand_between(5,6),0,level_enc);
			add_pokemon(rand_between(7,8),0,level_enc);
			break;
		case 6: //gym leader psychic
			add_pokemon(rand_between(15,16),0,8);
			add_pokemon(rand_between(1,16),0,10);
			add_pokemon(rand_between(15,16),0,10);
			add_pokemon(rand_between(1,16),0,9);
			add_pokemon(rand_between(15,16),0,10);
		default:
			add_pokemon(rand_between(1,16),0,5);
			add_pokemon(rand_between(1,16),0,5);
			break;
	}
}

void Player::save_pokemon_stats(){
	ofstream outFile;
    outFile.open( "savePoke.txt", ios::out );
	outFile << myPoke.size() << " "; //how many pokemon
	outFile << cp << endl; //current pokemon
	for(int i = 0; i < myPoke.size(); i++){	
		outFile << (*myPoke[i]).getnum() << " ";
		outFile << (*myPoke[i]).getmaxHealth() << " ";
    	outFile << (*myPoke[i]).getcurrHealth() << " ";
    	outFile << (*myPoke[i]).getlevel() << " ";
    	outFile << (*myPoke[i]).getexp() << " ";
    	outFile << (*myPoke[i]).getmaxLevelExp() << " ";
    	outFile << (*myPoke[i]).getmaxAttack() << " ";
    	outFile << (*myPoke[i]).getcurrAttack() << " ";
		outFile << (*myPoke[i]).getmaxDef() << " ";
		outFile << (*myPoke[i]).getcurrDef() << " ";
		outFile << (*myPoke[i]).getmaxSpeed() << " ";
		outFile << (*myPoke[i]).getcurrSpeed() << " ";
		outFile << (*myPoke[i]).getKO() << " ";
		outFile << (*myPoke[i]).getmoveLevel() << " ";
		//outFile << (*myPoke[i]).getMoveNums << " "; //help with this one to save moves! need to make edits in Moves.h
		outFile << endl; //end of one pokemon

	}
    outFile.close();
}

void Player::load_pokemon_stats(){
    ifstream inFile;
	string inStr;
	int inInt, numPoke;
    inFile.open( "savePoke.txt",ios::in);
	inFile >> numPoke;
	inFile >> cp;
	for(int i = 0; i < numPoke; i++){
		inFile >> inInt;
		add_pokemon(inInt,1,0); //adds certain pokemon in then loads the stats so the moves are saved!
		inFile >> inInt;
		(*myPoke[i]).setmaxHealth(inInt);
		inFile >> inInt;
		(*myPoke[i]).setcurrHealth(inInt);
		inFile >> inInt;
    	(*myPoke[i]).setlevel(inInt);
		inFile >> inInt;
    	(*myPoke[i]).setexp(inInt);
		inFile >> inInt;
    	(*myPoke[i]).setmaxLevelExp(inInt);
		inFile >> inInt;
    	(*myPoke[i]).setmaxAttack(inInt);
		inFile >> inInt;
    	(*myPoke[i]).setcurrAttack(inInt);
		inFile >> inInt;
		(*myPoke[i]).setmaxDef(inInt);
		inFile >> inInt;
		(*myPoke[i]).setcurrDef(inInt);
		inFile >> inInt;
		(*myPoke[i]).setmaxSpeed(inInt);
		inFile >> inInt;
		(*myPoke[i]).setcurrSpeed(inInt);
		inFile >> inInt;
		(*myPoke[i]).setKO(inInt);
		inFile >> inInt;
		(*myPoke[i]).setmoveLevel(inInt);
		if((*myPoke[i]).getlevel() >= (*myPoke[i]).getmoveLevel()){
			learnNewMove(i);
			//learn new move
		}
	}
		//(*myPoke[i]).setMoveNums(); //help with this one to save moves! need to make edits in Moves.h
    inFile.close();
}
void Player::learnNewMove(int i){
	(*myPoke[i]).learnNewMove(); //teach pokemon i the new move
}
int Player::rand_between(int a, int b){
	return((rand() % (b - a + 1)) + 1);
}
int Player::getNumPoke(){
	return myPoke.size();
}
int Player::whatToDo(){
	int choice;
	cout << "What would you like to do: 1 fight, 2 catch, 3 run";
	cin >> choice;	
}
int Player::fight(int userMove){
//	int choice;
//	cout << "Choose what move to use: 1,2,3 or 4";
//	cin >> choice;
	cout << (*myPoke[cp]).getname() << " attacked with " << (*myPoke[cp]).attackname(userMove-1) << endl;
	(*myPoke[cp]).attack(userMove,otherPoke[op]); // test using attack 1
	//check their ko and give exp
	if((*otherPoke[op]).getKO()){
		(*myPoke[cp]).incExp(25); //+25 exp
		return 1; //return 1 for a KO
	}
	cout << "Their health: " << (*otherPoke[op]).getcurrHealth() << endl;
	cout << "Your health: " << (*myPoke[cp]).getcurrHealth() << endl;
	return 0;
}
int Player::catchPoke(int cp, int op){
	int value = rand()%100;
    cout << "Rand value: " << value << " Your chance was: " << (((*otherPoke[op]).getmaxHealth()/(*otherPoke[op]).getcurrHealth())*7) << endl;
    if(value < (((*otherPoke[op]).getmaxHealth()/(*otherPoke[op]).getcurrHealth())*10)){
    	cout << "Caught" << endl;
        myPoke.push_back(otherPoke[op]);
        otherPoke.clear(); //get rid of him
		return 1; //did catch
    }
	else{
		return 0; //did not catch
	}
}
int Player::fight_comp(){
		int compMove = (rand() % 4) + 1;
        cout << "Wild "<< (*otherPoke[op]).getname() << " attacked with " << (*otherPoke[op]).attackname(compMove-1) << endl;
        (*otherPoke[op]).attack(compMove,myPoke[cp]); // comp move random 1-4
		if((*myPoke[cp]).getKO()){	
			return 1; //return 1 for a KO
		}
        cout << "Your health: " << (*myPoke[cp]).getcurrHealth() << endl;
        cout << "Their health: " << (*otherPoke[op]).getcurrHealth() << endl;
		return 0;
}
int Player::run(){
	int runChance = rand()%10;
		if(runChance > 4){ //50percent maybe later based of speed	
			return 1; //end encouter
		}
		return 0;
}
int Player::switchPoke(int newPoke){
	if(noValid()){
		cout << "No useable pokemon, seg fault!" << endl;
		return(-1);
	}
	else{
		if(checkValidPoke(newPoke)){
    	    cout << "I choose you " << (*myPoke[cp]).getname() << " !" << endl;
			return(newPoke);
		}
		else{
 	         cout << "Please pick a valid pokemon!" << endl;
			return(-2); //there are valid pokemon, but this isn't one
		}
	}
}
int Player::getMoveNum(int move){
	return ((*myPoke[cp]).getMoveNum(move)); //returns enum value of the move
}
int Player::getmyHealth(){
	return (*myPoke[cp]).getcurrHealth();
}
int Player::getmyMaxHealth(){
	return (*myPoke[cp]).getmaxHealth();
}
int Player::getopHealth(){
	return (*otherPoke[op]).getcurrHealth();
}
int Player::getopMaxHealth(){
	return (*otherPoke[op]).getmaxHealth();
}
int Player::mycheckKO(int poke){
	return (*myPoke[poke]).getKO();
}
int Player::opcheckKO(int poke){
	return (*otherPoke[poke]).getKO();
}
int Player::myCurrentPoke(){
	return ((*myPoke[cp]).getnum());
}
int Player::opCurrentPoke(){
	return ((*otherPoke[op]).getnum());
}
int Player::myLevel(){
	return ((*myPoke[cp]).getlevel());
}
int Player::anyLevel(int poke){
	return ((*myPoke[poke]).getlevel());
}
int Player::opLevel(){
	return ((*otherPoke[op]).getlevel());
}
int Player::whatPokeinParty(int poke){
	return ((*myPoke[poke]).getnum());
}
int Player::NextOp(int poke){
	if(poke < otherPoke.size()){
		return 1; //yes he has another pokemon
	}
	else{
		return 0; //no more pokemon reached max
	}
}
#endif
