#ifndef LOAD_SAVE_TEST_H
#define LOAD_SAVE_TEST_H

#include <fstream>
#include <iostream>
using namespace std;

class load_save_test {
	public:
		load_save_test(int=1,int=1,int=20,int=20,string="Ash");//level,spriteMap,mapCoord[0],mapCoord[1]
		void loadVars(string);
		void saveVars();
		void dispVars();
		void levelUp();
	private:	
		ifstream inFile;
		ofstream outFile;
		int level;
		int spriteMap;
		int mapCoord[2];
		string name;
};

#endif
