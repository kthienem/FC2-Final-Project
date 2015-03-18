#include "load_save_test.h"
#include <fstream>
#include <iostream>
using namespace std;

load_save_test::load_save_test(int mylevel,int myspriteMap,int myx,int myy,string myname) {
	level = mylevel;
	spriteMap = myspriteMap;
	mapCoord[0] = myx;
	mapCoord[1] = myy;
	name = myname;
}

void load_save_test::loadVars(string myinFile) {
	inFile.open(myinFile.c_str(), ios::in);

	inFile >> level;
	inFile >> spriteMap;
	inFile >> mapCoord[0];
	inFile >> mapCoord[1];
	inFile >> name;

	inFile.close();
}

void load_save_test::saveVars() {
	outFile.open("newFile.txt", ios::out);

	outFile << level << endl;
	outFile << spriteMap << endl;
	outFile << mapCoord[0] << endl;
	outFile << mapCoord[1] << endl;
	outFile << name << endl;

	outFile.close();
}

void load_save_test::dispVars() {
	cout << "L: " << level << endl;
	cout << "M: " << spriteMap << endl;
	cout << "C1: " << mapCoord[0] << endl;
	cout << "C2: " << mapCoord[1] << endl;
	cout << "N: " << name << endl;
}

void load_save_test::levelUp() {
	level++;
}
