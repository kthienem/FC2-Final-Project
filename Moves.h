//Nicholas Lombardo 3_25_15
//moves.h
//this is an implementation file for the moves class
//each object of the move class is it's own move
//for example tackle has its own base damage, modifier and type
//when asked to attack the move caluclates the damage based of these factor
//
//Things to work on, weakness and type advantage maybe in Pokemon or higher levels and more moves, maybe more variation with damage some random bits

#ifndef MOVES_H
#define MOVES_H
#include <string>
#include <vector>
#include <iostream>
//making into genaric class for moves that stores damage and other important info for each attack, so when these are created they are that attack
using namespace std;
enum moveList{
	MOVE_TACKLE,
	MOVE_SCRATCH,
	MOVE_EMBER,
	MOVE_WATERGUN,
	MOVE_CUT,
	MOVE_SHOCK
};

class Moves{
	public:
		Moves(int);
		int attack(int);
		void levelUp();
		string display();
		int noMove();
	private:
		int myMove;
		string type;
		string moveName;
		int base;
		double modify;
};

Moves::Moves(int myMove) { //using a int sets base stats for a move
	switch (myMove) {
		case MOVE_TACKLE: //0
			moveName.assign("tackle");
			base = 20; //base power of tackle
			modify = 2; //divides attackpower of pokemon by 2
			type.assign("normal"); //type of attaack
			break;
		case MOVE_SCRATCH: //1
			moveName.assign("scratch");
			base = 15;
			modify = 2;
			type.assign("normal");
			break;
		case MOVE_EMBER: //2	
			moveName.assign("ember");
			base = 35;
			modify = 2;
			type.assign("fire");
			break;
		case MOVE_WATERGUN:
			moveName.assign("watergun");
			base = 35;
			modify = 3;
			type.assign("water");
			break;
		case MOVE_CUT:
			moveName.assign("cut");
			base = 10;
			modify = 1;
			type.assign("normal");
			break;
		case MOVE_SHOCK:
			moveName.assign("shock");
			base = 35;
			modify = 1.2;
			type.assign("normal");
			break;
		default:
			cout << "default";
			noMove();
			break;
	}
}


void levelUp() {
	// do something to set myMove = to new value
}

int Moves::attack(int attackStat){
	return (base + attackStat/modify); //gernalized attacking function that takes stats from the move and uses them to calculate damage, eventually put in weakness part here too
};

int Moves::noMove(){};

string Moves::display(){ //display fcn that returns string
	return moveName;
};
#endif
