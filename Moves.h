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
		Moves();

		int attack(int);
		void levelUp();

		void noMove(void);
		int tackle(int);	// all moves will return the "net damage" dealt
		int scratch(int);
		int ember(int);
		int watergun(int);
		int cut(int);
		int shock(int);
	private:
		int myMove;
};

#endif
