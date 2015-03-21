#include "load_save_test.h"
#include <iostream>
using namespace std;

int main() {
	load_save_test game1;
	game1.loadVars("teststats.txt");
	game1.dispVars();
	game1.levelUp();
	game1.dispVars();
	game1.saveVars();
}
