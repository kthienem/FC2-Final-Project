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
		int getmovenum();
		void setmovenum(int);
	private:
		int myMove;
		string type;
		string moveName;
		int movenum;
		int base;
		double modify;
};

Moves::Moves(int myMove) { //using a int sets base stats for a move
	switch (myMove) {
		case MOVE_TACKLE: //0
			moveName = "tackle";
			setmovenum(0);
			base = 5; //base power of tackle
			modify = 2; //divides attackpower of pokemon by 2
			type = "normal"; //type of attaack
			break;
		case MOVE_SCRATCH: //1
			moveName = "scratch";
			setmovenum(1);
			base = 8;
			modify = 2;
			type = "normal";
			break;
		case MOVE_EMBER: //2	
			moveName = "ember";
			setmovenum(2);
			base = 12;
			modify = 2;
			type = "fire";
			break;
		case MOVE_WATERGUN: //3
			moveName = "watergun";
			setmovenum(3);
			base = 9;
			modify = 3;
			type = "water";
			break;
		case MOVE_CUT: //4
			moveName = "cut";
			setmovenum(4);
			base = 12;
			modify = 1;
			type = "normal";
			break;
		case MOVE_SHOCK: //5
			moveName = "shock";
			setmovenum(5);
			base = 13;
			modify = 1.2;
			type = "electric";
			break;
		case MOVE_KARATECHOP: //6
			moveName = "karate chop";
			setmovenum(6);
			base = 14;
			modify = 2;
			type = "fighting";
			break;
		case MOVE_GUST: //7
			moveName = "gust";
			setmovenum(7);
			base = 14;
			modify = 2;
			type = "flying";
			break;
		case MOVE_HEADBUTT:
			moveName = "headbutt";
			setmovenum(8);
			base = 16;
			modify = 1.4;
			type = "normal";
			break;
		case MOVE_BITE:
			moveName = "bite";
			setmovenum(9);
			base = 18;
			modify = 2;
			type = "normal";
			break;
		case MOVE_BUBBLEBEAM:
			moveName = "bubblebeam";
			setmovenum(10);
			base = 20;
			modify = 1.6;
			type = "water";
			break;
		case MOVE_RAZORLEAF:
			moveName = "razorleaf";
			setmovenum(11);
			base = 20;
			modify = 1.4;
			type = "grass";
			break;
		case MOVE_WINGATTACK:
			moveName = "wing attack";
			setmovenum(12);
			base = 20;
			modify = 1.4;
			type = "flying";
			break;
		case MOVE_QUICKATTACK:
			moveName = "quick attack";
			setmovenum(13);
			base = 18;
			modify = 1.8;
			type = "normal";
			break;	
		case MOVE_FLAMETHROWER:
			moveName = "flamethrower";
			setmovenum(14);
			base = 25;
			modify = 1;
			type = "fire";
			break;
		case MOVE_VINEWHIP:
			moveName = "vine whip";
			setmovenum(15);
			base = 27;
			modify = 1;
			type = "grass";
			break;
		case MOVE_THUNDERBOLT:
			moveName = "thunderbolt";
			setmovenum(16);
			base = 29;
			modify = 1;
			type = "electric";
			break;
		case MOVE_CONFUSION:
			moveName = "thunderbolt";
			setmovenum(17);
			base = 15;
			modify = 1;
			type = "psychic";
			break;
		case MOVE_PSYCHIC:
			moveName = "Psychic";
			setmovenum(18);
			base = 20;
			modify = 1.3;
			type = "psychic";
			break;
		case MOVE_PSYBEAM:
			moveName = "psybeam";
			setmovenum(19);
			base = 15;
			modify = 2;
			type = "psychic";
			break;
		case MOVE_LOWKICK:
			moveName = "low kick";
			setmovenum(20);
			base = 18;
			modify = 1.5;
			type = "fighting";
			break;
		case MOVE_SLASH:
			moveName = "slash";
			setmovenum(21);
			base = 18;
			modify = 1.3;
			type = "normal";
			break;
		case MOVE_PECK:
			moveName = "peck";
			setmovenum(22);
			base = 15;
			modify = 1.6;
			type = "flying";
			break;
		case MOVE_SEISMICTOSS:
			moveName = "seismic toss";
			setmovenum(23);
			base = 22;
			modify = 1.2;
			type = "fighting";
			break;
		case MOVE_THUNDER:
			moveName = "thunder";
			setmovenum(24);
			base = 25;
			modify = 1;
			type = "electric";
			break;
		case MOVE_HYDROPUMP:
			moveName = "hydropump";
			setmovenum(25);
			base = 25;
			modify = 1;
			type = "water";
			break;
		case MOVE_FIREBLAST:
			moveName = "fireblast";
			setmovenum(26);
			base = 25;
			modify = 1;
			type = "fire";
			break;
		case MOVE_SOLARBEAM:
			moveName = "solar beam";
			setmovenum(27);
			base = 25;
			modify = 1;
			type = "grass";
			break;
		case MOVE_BODYSLAM:
			moveName = "body slam";
			setmovenum(28);
			base = 20;
			modify = 1.7;
			type = "normal";
			break;
		case MOVE_SWIFT:
			moveName = "swift";
			setmovenum(29);
			base = 16;
			modify = 1.2;
			type = "normal";
			break;
		default:
			noMove();
			break;
	}
}

int Moves::getmovenum() {
	return movenum;
}

void Moves::setmovenum(int m) {
	movenum = m;
}

void Moves::levelUp() {
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
