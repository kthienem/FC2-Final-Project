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
		Pokemon(); //default constructor
//		Pokemon(int,int,int,int,int); //non-default
		int getnum();
		void setnum(int);
		void levelUp();	// level+1, add attack,def,reset exp
		//virtual void attack() = 0;
		void attack(int,Pokemon*); //takes in input for what move to use
		void swapAttack(int);// input will be index of attackName for the attack being added. This will ask the user if they want to swap attackName[x] with attackName[y] and then executes the swap
		void subHealth(int); // decrements health of Pokemon
		void addHealth(int); // increments health of Pokemon
		int getcurrHealth(); // return current health
		int getmaxHealth();
		void setmaxHealth(int);
		void setcurrHealth(int); // sets a new current health
		void inccurrHealth(int);
		void setlevel(int);
		int getlevel();
		void setexp(int);
		int getexp();
		void setmaxAttack(int);
		int getmaxAttack();
		void setcurrAttack(int);
		int getcurrAttack();
		void setmaxDef(int);
		int getmaxDef();
		void setcurrDef(int);
		int getcurrDef();
		void setmaxSpeed(int);
		int getmaxSpeed();
		void setcurrSpeed(int);
		int getcurrSpeed();
		void setKO(int);
		int getKO();
		void heal();
		void settype(string);
		void learnNewMove();
		string gettype();
		string getname();
		string getweak(); 
		void setweak( string );
		void setname(string);
		void setmoveLevel(int);
		int getmoveLevel();
		void setMoves(int,int,int,int,int);
		int incExp(int);
		void setmaxLevelExp(int);
		int getmaxLevelExp();
		string attackname(int);
		int getMoveNum(int);
	private:
		int num; // number of Pokemon
		int maxHealth;
		int currHealth;
		int level;
		int exp;
		int maxLevelExp; // experience needed to level up
		int maxAttack;
		int currAttack;
		int maxDef;
		int currDef;
		int maxSpeed;
		int currSpeed;
		int KO;	// boolean var
		int moveLevel; // level at which Pokemon learns fifth move
		string type; // Pokemon elemental type
		string name; // Pokemon name
		string weak; // type Pokemon is weak to
		vector <string> attackName;//how can we still utilize this given the Moves class?
		vector <Moves> myMoves;// then we can use for example myMoves[2].attack()
};

Pokemon::Pokemon(){
	srand(time(NULL)); // seed rand
	maxLevelExp = 50; //genaric maxlevelexp	
}

void Pokemon::setnum(int n) {
	num = n;
}

int Pokemon::getnum() {
	return num;
}

void Pokemon::heal(){ //pokeCenter and other healing uses
	currHealth = maxHealth; //restore health
	KO = 0;
}

void Pokemon::levelUp() {
	level++; // increment level by 1
	maxHealth+=7; // increment max health
	currHealth+=7; // leveling up heals by 7
	exp = exp - maxLevelExp; // set exp to the overflow 
	maxLevelExp+=30; //increase max exp to next level up
	maxAttack += (rand() % 4) + 1; // add a random attack stat amount
	maxDef += (rand() % 4) + 1; // increment defense by a random stat amount
	maxSpeed += (rand() % 4) + 1; // increment speed

	if(level == moveLevel){ //at level 5 learn new moves	
		cout << "Learned new move: " << myMoves[4].display() << endl << "Replaced move: " << myMoves[0].display() << endl;
		myMoves[0] = myMoves[1]; // first move is removed
		myMoves[1] = myMoves[2];
		myMoves[2] = myMoves[3];
		myMoves[3] = myMoves[4]; // move in slot 5 is added to 4 usable moves
	}

}

int Pokemon::getcurrHealth() {
	return currHealth; // returns current health
}

void Pokemon::setmaxHealth(int h) {
	maxHealth = h;
}

void Pokemon::setcurrHealth(int h) {
	if(h > 0) 
		currHealth = h; // sets new health
	else
		currHealth = 0; // fallback value
}

void Pokemon::inccurrHealth(int add) {
	currHealth += add; //inc health for potion and other increases
}

void Pokemon::attack(int attackNum,Pokemon* poke_damage_ptr){ //attack function that calls certain moveNumber
		int damage = myMoves[attackNum-1].attack(maxAttack); //calculate damage eventually take into account weaknesses and such
		cout << "Attack power: " << damage << endl; //display results
		(*poke_damage_ptr).subHealth(damage); // subtract health based on damage dealt
	}

void Pokemon::subHealth(int dam) {
	int damage = dam - currDef; // calculate damage
	cout << "Resisted " << getcurrDef() << " damage!" << endl; // account for defense
	if(damage > 0) {
		cout << "Actual damage: " << (damage) << endl; // display damage
		setcurrHealth((getcurrHealth() - damage)); // reduce current health
	}
	else {
		setcurrHealth(getcurrHealth() - 1); // at least one point of health is always lost
	}
	if(getcurrHealth() <= 0) { // check if pokemon is KO'd
		setcurrHealth(0); //set curr health to 0
		KO = 1; // if health falls below 0, Pokemon becomes KO'd
		cout << "KO" << endl;
	}
	cout << endl;
}

void Pokemon::addHealth(int add) {
	if(add >= 0) {
		setcurrHealth((getcurrHealth() + add)); // increments health
	}	
}

/* Getters and setters for Pokemon data members */

void Pokemon::setlevel(int lv) {
	level = lv;
}

int Pokemon::getlevel() {
	return level;
}

void Pokemon::setexp(int e) {
	exp = e;
}

int Pokemon::incExp(int e) {
	exp+=e; //add e to exp
	if(exp >= maxLevelExp){
		levelUp();
		cout << "Level Up! Now Level: " << level << endl << endl;
	}
	return exp;
}

int Pokemon::getexp() {
	return exp;
}

void Pokemon::setmaxAttack(int a) {
	maxAttack = a;
}

int Pokemon::getmaxAttack() {
	return maxAttack;
}

void Pokemon::setcurrAttack(int a) {
	currAttack = a;
}

int Pokemon::getcurrAttack() {
	return currAttack;
}

void Pokemon::setmaxDef(int d) {
	maxDef = d;
}

int Pokemon::getmaxDef() {
	return maxDef;
}

void Pokemon::setcurrDef(int d) {
	currDef = d;
}

int Pokemon::getcurrDef() {
	return currDef;
}

void Pokemon::setmaxSpeed(int s) {
	maxSpeed = s;
}

int Pokemon::getmaxSpeed() {
	return maxSpeed;
}

void Pokemon::setcurrSpeed(int s) {
	currSpeed = s;
}

int Pokemon::getcurrSpeed() {
	return currSpeed;
}

void Pokemon::setKO(int k) {
	KO = k;
}

int Pokemon::getKO() {
	return KO;
}

void Pokemon::setmoveLevel(int l) {
	moveLevel = l;
}

int Pokemon::getmoveLevel() {
	return moveLevel;
}

void Pokemon::settype(string t) {
	type = t;
}

string Pokemon::gettype() {
	return type;
}

string Pokemon::getname() {
	return name;
}
void Pokemon::setname(string a) {
	name = a;
}
void Pokemon::setweak(string w) {
	weak = w;
}
string Pokemon::getweak() {
	return weak;
}
void Pokemon::setmaxLevelExp(int newMax){
	maxLevelExp = newMax;
}
int Pokemon::getmaxLevelExp(){
	return maxLevelExp;
}
int Pokemon::getmaxHealth(){
	return maxHealth;
}

string Pokemon::attackname(int moveNum){
	return myMoves[moveNum].display();

}

void Pokemon::learnNewMove(){
		myMoves[0] = myMoves[1]; // first move is replaced
        myMoves[1] = myMoves[2]; // move other moves back one usable spot
        myMoves[2] = myMoves[3];
        myMoves[3] = myMoves[4]; // move in fifth slot moved to usable fourth slot	
}

void Pokemon::setMoves(int mv1, int mv2, int mv3, int mv4, int mv5) {

	Moves move1(mv1), move2(mv2), move3(mv3), move4(mv4), move5(mv5); // assign moves based on enum values

	myMoves.push_back(move1); // push moves into myMoves
	myMoves.push_back(move2);
	myMoves.push_back(move3);
	myMoves.push_back(move4); // last usable move slot
	myMoves.push_back(move5); // potential move to learn
}

int Pokemon::getMoveNum(int move) { // returns enum values of specified move number
	switch (move) {
		case 0:
			return myMoves[0].getmovenum();
			break;
		
		case 1:
			return myMoves[1].getmovenum();
			break;

		case 2:
			return myMoves[2].getmovenum();
			break;
		
		case 3:
			return myMoves[3].getmovenum();
			break;

		default:
			break; 
	}
}

#endif
