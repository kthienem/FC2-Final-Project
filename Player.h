// Player.h
// Interface file for Pokemon Player class
// creates battles and has all the functions neccessary for displaying output and dealing damage and reporting results

#ifndef PLAYER_H
#define PLAYER_H
#include <vector>
#include <fstream>
#include "Pokemon.h"
#include "Squirtle.h" // include each specific Pokemon class 
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
		void Menu(); // initiate menu interface
		void Roam(); // initiate field interface (free roam)
		void add_pokemon(int, int, int); //add new pokemon, first int is pokemon number and second is player
		int checkValidPoke(int); //checks if user pokmeon is koed or not return 1 if the pokemon is not ko and therefore valid, 0 otherwise 
		int noValid(); //checks for valid (not KOed) user pokemon and prints out what position they are in 
		int noValid_other(); //checks for opponent valid pokmon and subs them out, used for trainer battles
		int switchPoke(int); //takes input and tries to play that pokemon, if no valid pokemon it returns -1 for whited out, -2 for there are valid, but you didn't choose one, and the position of the valid pokemon if one is picked
		int run(); //run sequence 70% chance of running away
		int fight_comp(); //computer fight sequence that picks a random move
		int fight(int); //uses the inputed values as the user's move
		int catchPoke(); //random chance of catching a pokemon based 
		int getmyHealth(); //return health of current pokemon
		int getmyMaxHealth(); //return max health of current pokemon
		int getopHealth(); //get op currhelth
		int getopMaxHealth(); //get op maxhealth
		int mycheckKO(int); //check a certain poke for a ko used to see what switchable pokemon
		int opcheckKO(int); //check if op is koed
		int myCurrentPoke(); //get current pokemon's number
		int opCurrentPoke(); //get current pokemon's number for op
		int myLevel(); //return my level
		int anyLevel(int); //return level of any pokemon
		int opLevel(); //return level of opponent
		int NextOp(); //checks if this opint is less than size to see if it can switch to that pokemon for them
		int whatPokeinParty(int); //get pokemon number for any poke in the party
		int getMoveNum(int); //get move number for move of choice
		int getNumPoke(); //get number of current pokemon
		void wild_battle(int); //load a wild pokemon in to otherPoke
		void fish_battle(int); //load a fish pokemon in to otherPoke
		void pokeCenter(int); //heals all the pokemon with a input for if the player whited out or not, if they white out then it erases current exp
		void player_battle(int,int); //load a player battle
		void create_trainer(int,int); //loads ceratin trainers based of what inputs
		void save_pokemon_stats(); //creates save file
		void load_pokemon_stats(); //loads in data and creates pokemon
		int rand_between(int, int); //creates a random number between first input and second
	private:
		int createPokemon; // number of pokemon owned by player
		vector <Pokemon*> myPoke; //array for pointers
		vector <Pokemon*> otherPoke; //array for pointers
		int cp; //current pokemon
		int op; //op's current pokemon
};
Player::Player(int newGame){
	srand(time(NULL));
	if(newGame){
		add_pokemon(7,1,2); // add a new pikachu to player pokemon
		cp = 0;
	}
	else{
		load_pokemon_stats();
	}
}
//function for adding a new pokemon into players array
void Player::add_pokemon(int PokeNum, int person, int levelDesired){ //person 1 or 0 for player or npc
	Pokemon *newPoke_ptr;
	switch(PokeNum) { // structure to instantiate specific Pokemon class
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
	for(int i = 1; i < levelDesired; i++){
		(*newPoke_ptr).levelUp();
	}
	if(person == 1){ //limit to 6 here and need to in the catch time too
		myPoke.push_back(newPoke_ptr);
	}
	else{
		otherPoke.push_back(newPoke_ptr);
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
			found = 1; // at least one Pokemon is not KO'd
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
			found = 1; // at least one Pokemon is not KO'd
		}
	}
	if(found)
		return 0;
	else
		return 1; //no valid pokemon
}
void Player::wild_battle(int level_enc){ // Wild grass Pokemon battle
	otherPoke.clear();
	add_pokemon((rand()%16)+1,0,level_enc); // comp rand
	op = 0; //opponent poke
	cout << endl << "Wild Battle!" << endl;
	cout << "A Wild, Level " << (*otherPoke[op]).getlevel() << " " << (*otherPoke[op]).getname() << " appeared!" << endl << endl;
}

void Player::fish_battle(int level_enc){ // Water Pokemon battle 
	otherPoke.clear();
	cout << endl << "Fish Battle!" << endl;
	add_pokemon(rand_between(9,10),0,level_enc); // comp rand, ensures Water type Pokemon
	op = 0; //opponent poke
	cout << "A Wild, Level " << (*otherPoke[op]).getlevel() << " " << (*otherPoke[op]).getname() << " was hooked!" << endl << endl;
}

void Player::player_battle(int pick, int level_enc){ // Battle against a Pokemon trainer
	otherPoke.clear(); //clear other so that it can be filled with player pokemon	
	cout << endl << "Trainer Battle!" << endl;
	create_trainer(pick,level_enc);
	op = 0;
}

void Player::pokeCenter(int whiteout){
	for(int i = 0; i < myPoke.size(); i++){
		(*myPoke[i]).heal(); // heal all Pokemon
		if(whiteout){
			(*myPoke[i]).setexp(0); //clear current experience on whiteout
		}
	}
}

void Player::create_trainer(int pick, int level_enc){
	otherPoke.clear();	// gives trainers semi-random Pokemon
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
    outFile.open( "savePoke.txt", ios::out ); // specify file
	outFile << myPoke.size() << " "; //how many pokemon
	outFile << cp << endl; //current pokemon
	for(int i = 0; i < myPoke.size(); i++){	// save all Pokemon stats
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
		outFile << endl; //end of one pokemon

	}
    outFile.close();
}

void Player::load_pokemon_stats(){
    ifstream inFile;
	string inStr;
	int inInt, numPoke;
    inFile.open( "savePoke.txt",ios::in); // specify file
	inFile >> numPoke;
	inFile >> cp;
	for(int i = 0; i < numPoke; i++){
		inFile >> inInt;
		add_pokemon(inInt,1,0); //adds certain pokemon in then loads the stats so the moves are saved!
		inFile >> inInt; // set all Pokemon stats
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
    inFile.close();
}
void Player::learnNewMove(int i){
	(*myPoke[i]).learnNewMove(); //teach pokemon i the new move
}
int Player::rand_between(int a, int b){ // used for semi-randomness
	return((rand() % (b - a + 1)) + a);
}
int Player::getNumPoke(){
	return myPoke.size();
}
int Player::fight(int userMove){
	cout << "Level " << (*myPoke[cp]).getlevel() << " " <<(*myPoke[cp]).getname() << " attacked with " << (*myPoke[cp]).attackname(userMove-1) << endl;
	(*myPoke[cp]).attack(userMove,otherPoke[op]); // test using attack 1
	//check their ko and give exp
	if((*otherPoke[op]).getKO()){
		(*myPoke[cp]).incExp((*otherPoke[op]).getlevel()*7 + 10); //+25 exp
		cout << "Gained " << ((*otherPoke[op]).getlevel()*7 + 10) << " exp!" << endl;
		return 1; //return 1 for a KO
	}
	return 0;
}
int Player::catchPoke(){
	if(myPoke.size()!=6){ // have room for more Pokemon
		int value = rand()%100; // value to compare health fraction to
		double max = (*otherPoke[op]).getmaxHealth();
		double curr = (*otherPoke[op]).getcurrHealth();
		if(value > ((curr/max)*100)){ // catch Pokemon
    		cout << "Caught" << endl;
    	    myPoke.push_back(otherPoke[op]);
       	    otherPoke.clear(); //get rid of him
	   		return 1; //did catch
    	}
		else{
			cout << "Shoot, the wild pokemon escaped the Pokeball... should'a used a master ball or bought a falcon!" << endl << endl;
			return 0; //did not catch
		}
	}
	else{
		return 0;
	}
}
int Player::fight_comp(){
		int compMove = (rand() % 4) + 1;
        cout << "Wild, Level " << (*otherPoke[op]).getlevel() << " " << (*otherPoke[op]).getname() << " attacked with " << (*otherPoke[op]).attackname(compMove-1) << endl;
        (*otherPoke[op]).attack(compMove,myPoke[cp]); // comp move random 1-4
		if((*myPoke[cp]).getKO()){	
			return 1; //return 1 for a KO
		}
		return 0;
}
int Player::run(){
	int runChance = rand()%10;
		if(runChance > 2){ //70percent maybe later based of speed	
			cout << endl << "Ran away!" << endl << endl;
			return 1; //end encouter
		}
		return 0; //did not run away
}
int Player::switchPoke(int newPoke){
	cout << endl;
	if(noValid()){
		cout << "No useable pokemon! You whited out!" << endl << endl;
		return(-1);
	}
	else{
		if(checkValidPoke(newPoke)){
			cp = newPoke;
    	    cout << "I choose you " << (*myPoke[cp]).getname() << " !" << endl <<endl;
			return(newPoke);
		}
		else{
 	         cout << "Please pick a valid pokemon!" << endl << endl;
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
int Player::NextOp(){
	if(op < (otherPoke.size()-1)){
		op++;
		return 1; //yes he has another pokemon
	}
	else{
		return 0; //no more pokemon reached max
	}
}
#endif
