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
		string gettype();
		string getname();
		void setname(string);
		void setmoveLevel(int);
		int getmoveLevel();
		void setMoves(int,int,int,int,int);
		int incExp(int);
		void setmaxLevelExp(int);
		string attackname(int);
	private:
		int maxHealth;
		int currHealth;
		int level;
		int exp;
		int maxLevelExp;
		int maxAttack;
		int currAttack;
		int maxDef;
		int currDef;
		int maxSpeed;
		int currSpeed;
		int KO;	// boolean var
		int moveLevel; // level at which Pokemon learns fifth move
		string type;
		string name;
		vector <string> attackName;//how can we still utilize this given the Moves class?
		vector <Moves> myMoves;// then we can use for example myMoves[2].attack()
};

Pokemon::Pokemon(int pokeNum){
	srand(time(NULL)); // seed rand
	maxLevelExp = 50; //genaric maxlevelexp	
}

void Pokemon::heal(){ //pokeCenter and other healing uses
	currHealth = maxHealth; //restore health
}

void Pokemon::levelUp() {
	cout << endl << "Level UP!!!" << "Now level: " << level << endl << endl;
	maxHealth+=7; // increment max health
	currHealth+=7; // leveling up heals by 7
	level++; // increment level by 1
	exp = exp - maxLevelExp; // set exp to the overflow 
	maxLevelExp+=30; //increase max exp to next level up
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
	if(level == 5){ //at level 5 learn new moves
		cout << "Learned new move! Placed as move 4 and replaced move 1" << endl;
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
		currHealth = 0;
}

void Pokemon::inccurrHealth(int add) {
	currHealth += add; //inc health for potion and other increases
}
/*void Pokemon::attack(int moveNum){ //attack function that calls certain moveNumber
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
}*/

void Pokemon::attack(int attackNum,Pokemon* poke_damage_ptr){ //attack function that calls certain moveNumber
		//cout << "Move number "<< attackNum <<" is " << myMoves[attackNum-1].display() << endl;
		int damage = myMoves[attackNum-1].attack(maxAttack); //calculate damage eventually take into account weaknesses and such
		cout << "Attack power: " << damage << endl; //display results
		(*poke_damage_ptr).subHealth(damage); // subtract health based on damage dealt
	}

void Pokemon::subHealth(int dam) {
	//cout << "Health was: " << getcurrHealth() << endl;
	int damage = dam - currDef; // calculate damage
	cout << "Resisted " << getcurrDef() << " damage!" << endl; 
	if(damage > 0) {
		cout << "Actual damage: " << (damage) << endl;
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
	//cout << "Health is now: " << getcurrHealth() << endl;
}

void Pokemon::addHealth(int add) {
	if(add >= 0) {
		setcurrHealth((getcurrHealth() + add)); // increments health
	}	
}

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
void Pokemon::setmaxLevelExp(int newMax){
	maxLevelExp = newMax;
}
int Pokemon::getmaxHealth(){
	return maxHealth;
}

string Pokemon::attackname(int moveNum){
	return myMoves[moveNum].display();

}

void Pokemon::setMoves(int mv1, int mv2, int mv3, int mv4, int mv5) {

	Moves move1(mv1), move2(mv2), move3(mv3), move4(mv4), move5(mv5);

	myMoves.push_back(move1); // push moves into myMoves
	myMoves.push_back(move2);
	myMoves.push_back(move3);
	myMoves.push_back(move4);
	myMoves.push_back(move5); // potential move to learn
}

#endif
