#include "Moves.h"

/*enum moveList{
	MOVE_TACKLE,
	MOVE_SCRATCH,
	MOVE_EMBER,
	MOVE_WATERGUN,
	MOVE_CUT,
	MOVE_SHOCK
};*/

Moves::Moves() {

}

int Moves::attack(int attackStat) {
	/*switch(myAttack) {
		case: MOVE_TACKLE
			tackle(attackStat);
			break;
		case: MOVE_SCRATCH
			scratch(attackStat);
			break;
		case: MOVE_EMBER
			ember(attackStat);
			break;
		case: MOVE_WATERGUN
			watergun(attackStat);
			break;
		case: MOVE_CUT
			cut(attackStat);
			break;
		case: MOVE_SHOCK
			shock(attackStat);
			break;
		default:
			noMove();
			break;
	} */
}

void levelUp() {
	// do something to set myMove = to new value
}

void Moves::noMove(int attackStat){};

int Moves::tackle(int attackStat){};

int Moves::scratch(int attackStat){};

int Moves::ember(int attackStat){};

int Moves::watergun(int attackStat){};

int Moves::cut(int attackStat){};

int Moves::shock(int attackStat){};

