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
	MOVE_TACKLE,      //0
	MOVE_SCRATCH,     //1
	MOVE_EMBER,       //2
	MOVE_WATERGUN,    //3
	MOVE_CUT,         //4
	MOVE_SHOCK,       //5
	MOVE_DOUBLESLAP,  //6
	MOVE_GUST,        //7
	MOVE_HEADBUTT,    //8
	MOVE_BITE,        //9 
	MOVE_BUBBLEBEAM,  //10
	MOVE_RAZORLEAF,   //11
	MOVE_CONFUSION,   //12
	MOVE_QUICKATTACK, //13
	MOVE_FLAMETHROWER,//14
	MOVE_VINEWHIP     //15
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
		case MOVE_DOUBLESLAP:
			moveName.assign("doubleslap");
			base = 30;
			modify = 2;
			type.assign("normal");
			break;
		case MOVE_GUST:
			moveName.assign("gust");
			base = 40;
			modify = 2;
			type.assign("flying");
			break;
		case MOVE_HEADBUTT:
			moveName.assign("headbutt");
			base = 70;
			modify = 1.4;
			type.assign("normal");
			break;
		case MOVE_BITE:
			moveName.assign("bite");
			base = 60;
			modify = 2;
			type.assign("dark");
			break;
		case MOVE_BUBBLEBEAM:
			moveName.assign("bubblebeam");
			base = 65;
			modify = 1.6;
			type.assign("water");
			break;
		case MOVE_RAZORLEAF:
			moveName.assign("razorleaf");
			base = 55;
			modify = 1.4;
			type.assign("grass");
			break;
		case MOVE_CONFUSION:
			moveName.assign("confusion");
			base = 50;
			modify = 1;
			type.assign("psychic");
			break;
		case MOVE_QUICKATTACK:
			moveName.assign("quick attack");
			base = 40;
			modify = 1.8;
			type.assign("normal");
			break;	
		case MOVE_FLAMETHROWER:
			moveName.assign("flamethrower");
			base = 90;
			modify = 1;
			type.assign("fire");
			break;
		case MOVE_VINEWHIP:
			moveName.assign("vine whip");
			base = 70;
			modify = 1;
			type.assign("grass");
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
