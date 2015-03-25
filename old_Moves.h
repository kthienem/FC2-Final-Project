#ifndef MOVES_H
#define MOVES_H

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

		int noMove();
		int tackle(int);	// all moves will return the "net damage" dealt
		int scratch(int);
		int ember(int);
		int watergun(int);
		int cut(int);
		int shock(int);
	private:
		int myMove;
		string moveName;
};

Moves::Moves(int myMove) {
	switch (myMove) {
		case MOVE_TACKLE:
			tackle(attackStat);
			break;
		case MOVE_SCRATCH:
			scratch(attackStat);
			break;
		case MOVE_EMBER:
			ember(attackStat);
			break;
		case MOVE_WATERGUN:
			watergun(attackStat);
			break;
		case MOVE_CUT:
			cut(attackStat);
			break;
		case MOVE_SHOCK:
			shock(attackStat);
			break;
		default:
			noMove();
			break;
	}
}

int Moves::attack(int attackStat) {
	
}

void levelUp() {
	// do something to set myMove = to new value
}

int Moves::tackle(int attackStat){
	return (20 + attackStat/2); 
};

int Moves::scratch(int attackStat){};

int Moves::ember(int attackStat){};

int Moves::watergun(int attackStat){};

int Moves::cut(int attackStat){};

int Moves::shock(int attackStat){};

int Moves::noMove(){};

string Moves::display(){
	return moveName;
};
#endif
