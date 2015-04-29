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
	MOVE_KARATECHOP,  //6
	MOVE_GUST,        //7
	MOVE_HEADBUTT,    //8
	MOVE_BITE,        //9 
	MOVE_BUBBLEBEAM,  //10
	MOVE_RAZORLEAF,   //11
	MOVE_WINGATTACK,  //12
	MOVE_QUICKATTACK, //13
	MOVE_FLAMETHROWER,//14
	MOVE_VINEWHIP,    //15
	MOVE_THUNDERBOLT, //16
	MOVE_CONFUSION,   //17
	MOVE_PSYCHIC,     //18
	MOVE_PSYBEAM,     //19
	MOVE_LOWKICK,     //20
	MOVE_SLASH,       //21
	MOVE_PECK,        //22
	MOVE_SEISMICTOSS, //23
	MOVE_THUNDER,     //24
	MOVE_HYDROPUMP,   //25
	MOVE_FIREBLAST,   //26
	MOVE_SOLARBEAM,   //27
	MOVE_BODYSLAM,    //28
	MOVE_SWIFT        //29
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
			base = 5; //base power of tackle
			modify = 2; //divides attackpower of pokemon by 2
			type.assign("normal"); //type of attaack
			break;
		case MOVE_SCRATCH: //1
			moveName.assign("scratch");
			base = 8;
			modify = 2;
			type.assign("normal");
			break;
		case MOVE_EMBER: //2	
			moveName.assign("ember");
			base = 12;
			modify = 2;
			type.assign("fire");
			break;
		case MOVE_WATERGUN:
			moveName.assign("watergun");
			base = 9;
			modify = 3;
			type.assign("water");
			break;
		case MOVE_CUT:
			moveName.assign("cut");
			base = 12;
			modify = 1;
			type.assign("normal");
			break;
		case MOVE_SHOCK:
			moveName.assign("shock");
			base = 13;
			modify = 1.2;
			type.assign("electric");
			break;
		case MOVE_KARATECHOP:
			moveName.assign("karate chop");
			base = 14;
			modify = 2;
			type.assign("fighting");
			break;
		case MOVE_GUST:
			moveName.assign("gust");
			base = 14;
			modify = 2;
			type.assign("flying");
			break;
		case MOVE_HEADBUTT:
			moveName.assign("headbutt");
			base = 16;
			modify = 1.4;
			type.assign("normal");
			break;
		case MOVE_BITE:
			moveName.assign("bite");
			base = 18;
			modify = 2;
			type.assign("normal");
			break;
		case MOVE_BUBBLEBEAM:
			moveName.assign("bubblebeam");
			base = 20;
			modify = 1.6;
			type.assign("water");
			break;
		case MOVE_RAZORLEAF:
			moveName.assign("razorleaf");
			base = 20;
			modify = 1.4;
			type.assign("grass");
			break;
		case MOVE_WINGATTACK:
			moveName.assign("wing attack");
			base = 20;
			modify = 1.4;
			type.assign("flying");
			break;
		case MOVE_QUICKATTACK:
			moveName.assign("quick attack");
			base = 18;
			modify = 1.8;
			type.assign("normal");
			break;	
		case MOVE_FLAMETHROWER:
			moveName.assign("flamethrower");
			base = 25;
			modify = 1;
			type.assign("fire");
			break;
		case MOVE_VINEWHIP:
			moveName.assign("vine whip");
			base = 27;
			modify = 1;
			type.assign("grass");
			break;
		case MOVE_THUNDERBOLT:
			moveName.assign("thunderbolt");
			base = 29;
			modify = 1;
			type.assign("electric");
		case MOVE_CONFUSION:
			moveName.assign("thunderbolt");
			base = 15;
			modify = 1;
			type.assign("psychic");
		case MOVE_PSYCHIC:
			moveName.assign("Psychic");
			base = 20;
			modify = 1.3;
			type.assign("psychic");
		case MOVE_PSYBEAM:
			moveName.assign("psybeam");
			base = 15;
			modify = 2;
			type.assign("psychic");
		case MOVE_LOWKICK:
			moveName.assign("low kick");
			base = 18;
			modify = 1.5;
			type.assign("fighting");
		case MOVE_SLASH:
			moveName.assign("slash");
			base = 18;
			modify = 1.3;
			type.assign("normal");
		case MOVE_PECK:
			moveName.assign("peck");
			base = 15;
			modify = 1.6;
			type.assign("flying");
		case MOVE_SEISMICTOSS:
			moveName.assign("seismic toss");
			base = 22;
			modify = 1.2;
			type.assign("fighting");
		case MOVE_THUNDER:
			moveName.assign("thunder");
			base = 25;
			modify = 1;
			type.assign("electric");
		case MOVE_HYDROPUMP:
			moveName.assign("hydropump");
			base = 25;
			modify = 1;
			type.assign("water");
		case MOVE_FIREBLAST:
			moveName.assign("firsblast");
			base = 25;
			modify = 1;
			type.assign("fire");
		case MOVE_SOLARBEAM:
			moveName.assign("solar beam");
			base = 25;
			modify = 1;
			type.assign("grass");
		case MOVE_BODYSLAM:
			moveName.assign("body slam");
			base = 20;
			modify = 1.7;
			type.assign("normal");
		case MOVE_SWIFT:
			moveName.assign("swift");
			base = 16;
			modify = 1.2;
			type.assign("normal");

		default:
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
