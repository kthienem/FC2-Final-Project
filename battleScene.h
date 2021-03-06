/* This file contains functions to display the graphics needed for a battle
 * scene in game.
 *
 */

#ifndef BATTLESCENE_H
#define BATTLESCENE_H

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <ctime>
#include <string>
#include <unistd.h>
#include "Player.h"

using namespace std;

const int SCREEN_WIDTH = 768;
const int SCREEN_HEIGHT = 576;

enum Names{//used to index arrays that contain information about pokemon images
	BLANK,			//0
	RATTATA,		//1
	SNORLAX,		//2
	ZUBAT,			//3
	PIDGEY,			//4
	MACHOP,			//5
	MANKEY,			//6
	PIKACHU,		//7
	ELECTABUZZ,	//8
	SQUIRTLE,		//9
	SLOWPOKE,		//10
	CHARMANDER,	//11
	PONYTA,			//12
	BULBASAUR,	//13
	ODDISH,			//14
	DROWZEE,		//15
	ABRA,				//16
	NAMES
};

enum PokeMoves{
	TACKLE,				//0
	SCRATCH,			//1
	EMBER,				//2
	WATERGUN,			//3
	CUT,					//4
	SHOCK,				//5
	KARATECHOP,		//6
	GUST,					//7
	HEADBUTT,			//8
	BITE,					//9
	BUBBLEBEAM,		//10
	RAZORLEAF,		//11
	WINGATTACK,		//12
	QUICKATTACK,	//13
	FLAMETHROWER,	//14
	VINEWHIP,			//15
	THUNDERBOLT,	//16
	CONFUSION,		//17
	PSYCHIC,			//18
	PSYBEAM,			//19
	LOWKICK,			//20
	SLASH,				//21
	PECK,					//22
	SEISMICTOSS,	//23
	THUNDER,			//24
	HYDROPUMP,		//25
	FIREBLAST,		//26
	SOLARBEAM,		//27
	BODYSLAM,			//28
	SWIFT,				//29
	MOVES
};

enum Health{
	GREEN,		//0
	YELLOW,		//1
	RED,			//2
	COLORS
};

class battleScene{
	public:
		battleScene(Player*, SDL_Window*, int);//non-default constructor
		bool init();//initialize display window
		bool loadMedia();//load images to be used along with SDL_Rects
		void close();//free memory and delete window
		SDL_Surface* loadSurface(string path);//optimize loaded images
		int battle(int);//runs battle scene
		void moveArrow(string);//moves arrow to appropriate position
		int menuOption(string);//directs player to proper screen depending on their menu choice
		void pokemonMenu(int);//function to update screen to list of player's pokemon

	private:
		int return_value;
		int nextPoke;
		int battleType;//the type of battle that the player is entering, wild, trainer, etc.
		Player* myTrainer;//an object of type Player to access the player's pokemon and their stats
		SDL_Window* gWindow;//window that images are updated to
		SDL_Surface* gScreenSurface;//the screen surface that images are blitted to
		SDL_Surface* gBackground;//points to image of battle background
		SDL_Rect gBack;//where field background will go in window
		SDL_Surface* gMenuSheet;//sprite sheet containing menu images
		SDL_Rect gMenuBack;//where the menu background is loacted in sprite sheet
		SDL_Rect gMenuBackWindow;//where menu background will go in window
		SDL_Rect gOptions;//where the options menu is located in sprite sheet
		SDL_Rect gOptionsWindow;//where the options menu will go in window
		SDL_Rect gOpponentLevel;//where the level of the enemy pokemon is located on the sprite sheet
		SDL_Rect gOpponentLevelWindow;//where the level of the enemy pokemon will go in window
		SDL_Rect gPlayerLevel;//where the level of the players pokemon is located on the sprite sheet
		SDL_Rect gPlayerLevelWindow;//where the level of the players pokemon will go in window
		SDL_Rect gSelectionArrow;//the black arrow used to indicate what option the user is pointing at
		SDL_Rect gArrowPosition[8];//the positions of the arrow on the screen
		SDL_Rect gEmptyPokeball;//image of empty pokeball from sprite sheet
		SDL_Rect gLivePokeball;//image of live pokeball from sprite sheet
		SDL_Rect gDeadPokeball;//image of dead pokeball from sprite sheet
		SDL_Rect gPokeballs;//image for available pokeballs
		SDL_Rect gPokeballsWindow;//location for available pokeballs image
		SDL_Rect gMovesMenu;//image for menu containing pokemons moves
		SDL_Rect gMovesMenuPos;//position in window for moves menu
		SDL_Rect gCurrentArrowPos;//current poition of selection arrow
		SDL_Surface* gPokemonMenu;//image containing list of player's pokemon
		SDL_Rect gPokemon;//location on image for nackground to list of pokemon
		SDL_Rect gPokemonWindow;//where background for the list of pokemon will go in window
		SDL_Rect gPokemonListSelect;//image for selected pokemon in list
		SDL_Rect gPokemonListUnselect;//image for non selected pokemon in list
		SDL_Rect gPokemonListWindow[6];//images for pokemon in list
		SDL_Surface* gNames_Moves;//image file with names of pokemon and moves
		SDL_Rect gPokemonNames[NAMES];//rectangles for names of pokemon in image file
		SDL_Rect gMoves[MOVES];//recatngles for names of moves in image file
		SDL_Rect gMovesWindow[4];//where the four pokemon moves go in the window
		SDL_Rect gHealthBar[COLORS];//images for the different colors of the health bar
		SDL_Rect gOpponentHealth;//where the opponents health goes on the screen
		SDL_Rect gPlayerHealth;//where the players health goes on the screen
		SDL_Surface* gBattleSprites;//image with pokemon for battle and selection menu
		SDL_Rect gOpponentBattlePokemon[NAMES];//images for pokemon facing screen during battle
		SDL_Rect gPlayerBattlePokemon[NAMES];//images for pokemon facing away during battle
		SDL_Rect gSelectPokemon[NAMES];//images for small image of pokemon when viewing user's pokemon
		SDL_Rect gCurrentPlayerColor;//current color of health bar for player
		SDL_Rect gCurrentOpponentColor;//current color of health bar for opponent
		SDL_Rect gOpponentName;//name of opponents current pokemon
		SDL_Rect gOpponentPokemon;//current image for opponent pokemon
		SDL_Rect gPlayerName;//name of players current pokemon
		SDL_Rect gPlayerPokemon;//current image for player pokemon
		SDL_Rect gPokemonSelectImage[6];//locations on window for selecting pokemon
		SDL_Rect gPokemonSelectName[6];//locations on window for selecting pokemon names

};

battleScene::battleScene(Player* myPlayer, SDL_Window* myWindow, int type)
{
	gWindow = myWindow;//points to the window that is used for the rest of the games graphics
	battleType = type;//the type of battle that the player is entering into
	myTrainer = myPlayer;//copy of the Player object that is being used for the game
	gScreenSurface = NULL;//the screen that images are blitted to
}

bool battleScene::init()
{
	//creates window
	bool success = true;//lets main know if window has successfully loaded

	if(SDL_Init(SDL_INIT_VIDEO) < 0){//Error if SDL could not initialize
		cout << "SDL could not initialize! SDL Error: " << SDL_GetError() << endl;
		success = false;
	}
	else{
		if(gWindow == NULL){//Error if window could not be 
			cout << "Window could not be created! SDL Error: " << SDL_GetError() << endl;
			success = false;
		}
		else{
			int imgFlags = IMG_INIT_PNG;
			if( !( IMG_Init( imgFlags) & imgFlags ) ){
				cout << "SDL_image could not initialize! SDL_image error: " << IMG_GetError() << endl;
				success = false;
			}
			else{
				gScreenSurface = SDL_GetWindowSurface(gWindow);
			}
		}
	}
	return success;//return whether function was successful or not

}

bool battleScene::loadMedia()
{
	//loads images
	bool success = true;

	gBackground = loadSurface("Battle_Backgrounds.png");//load battle scene background image
	if(gBackground == NULL) success = false;//if not loaded properly return unsuccessful

	gBack.x = 0;//set top corner of image to top corner of window
	gBack.y = 0;
	gBack.w = SCREEN_WIDTH;//stretch image across entire window
	gBack.h = 404;

	gMenuSheet = loadSurface("HP_Bars_In-battle_Menu.png");//load sprite sheet with menu and HP bars
	if(gMenuSheet == NULL) success = false;
	else{
		SDL_SetColorKey(gMenuSheet, SDL_TRUE, SDL_MapRGB(gMenuSheet->format, 255, 255, 255));//set transparency
	}

	gPokemonMenu = loadSurface("Pokemon_Menu.png");//loads list of available pokemon
	if(gPokemonMenu == NULL) success = false;
	else{
		SDL_SetColorKey(gPokemonMenu, SDL_TRUE, SDL_MapRGB(gPokemonMenu->format, 0, 128, 0));//set transparency
	}

	gBattleSprites = loadSurface("Pokemon.png");//loads image with different pokemon for battle
	if(gBattleSprites == NULL) success - false;
	else{
		SDL_SetColorKey(gBattleSprites, SDL_TRUE, SDL_MapRGB(gBattleSprites->format, 200, 200, 168));//set transparency
	}

	gNames_Moves = loadSurface("Names_Moves.png");//loads image with list of pokemon names and moves
	if(gNames_Moves == NULL) success = false;
	else{
		SDL_SetColorKey(gNames_Moves, SDL_TRUE, SDL_MapRGB(gNames_Moves->format, 255, 255, 255));//set transparency
	}

	//Location of menu background on image
	gMenuBack.x = 297;
	gMenuBack.y = 56;
	gMenuBack.w = 240;
	gMenuBack.h = 48;

	//location of menu background on window
	gMenuBackWindow.x = 0;
	gMenuBackWindow.y = 404;
	gMenuBackWindow.w = SCREEN_WIDTH;
	gMenuBackWindow.h = SCREEN_HEIGHT-404;

	//location of options on sprite sheet
	gOptions.x = 146;
	gOptions.y = 4;
	gOptions.w = 120;
	gOptions.h = 48;

	//location of options menu on window
	gOptionsWindow.x = SCREEN_WIDTH/2;
	gOptionsWindow.y = 404;
	gOptionsWindow.w = SCREEN_WIDTH/2;
	gOptionsWindow.h = SCREEN_HEIGHT-404;

	//location of opponents level and health info on sprite sheet
	gOpponentLevel.x = 3;
	gOpponentLevel.y = 3;
	gOpponentLevel.w = 100;
	gOpponentLevel.h = 29;

	//location of opponents level and health info on window
	gOpponentLevelWindow.x = 50;
	gOpponentLevelWindow.y = 40;
	gOpponentLevelWindow.w = 300;
	gOpponentLevelWindow.h = 87;

	//location of players level and health info on sprite sheet
	gPlayerLevel.x = 3;
	gPlayerLevel.y = 44;
	gPlayerLevel.w = 104;
	gPlayerLevel.h = 37;

	//location of players level and health info on window
	gPlayerLevelWindow.x = (SCREEN_WIDTH/2) + 20;
	gPlayerLevelWindow.y = 275;
	gPlayerLevelWindow.w = 350;
	gPlayerLevelWindow.h = 125;

	//location of selection arrow on sprite sheet
	gSelectionArrow.x = 269;
	gSelectionArrow.y = 4;
	gSelectionArrow.w = 5;
	gSelectionArrow.h = 9;

	//fight option position
	gArrowPosition[0].x = 415;
	gArrowPosition[0].y = 452;
	gArrowPosition[0].w = 14;
	gArrowPosition[0].h = 26;

	//pokemon option position
	gArrowPosition[1].x = 415;
	gArrowPosition[1].y = 509;
	gArrowPosition[1].w = 14;
	gArrowPosition[1].h = 26;

	//bag option position
	gArrowPosition[2].x = 595;
	gArrowPosition[2].y = 452;
	gArrowPosition[2].w = 14;
	gArrowPosition[2].h = 26;

	//run option position
	gArrowPosition[3].x = 595;
	gArrowPosition[3].y = 509;
	gArrowPosition[3].w = 14;
	gArrowPosition[3].h = 26;

	//position of first move
	gArrowPosition[4].x = 22;
	gArrowPosition[4].y = 440;
	gArrowPosition[4].w = 14;
	gArrowPosition[4].h = 26;

	//position of second move
	gArrowPosition[5].x = 22;
	gArrowPosition[5].y = 509;
	gArrowPosition[5].w = 14;
	gArrowPosition[5].h = 26;

	//position of third move
	gArrowPosition[6].x = 270;
	gArrowPosition[6].y = 440;
	gArrowPosition[6].w = 14;
	gArrowPosition[6].h = 26;

	//position of fourth move
	gArrowPosition[7].x = 270;
	gArrowPosition[7].y = 509;
	gArrowPosition[7].w = 14;
	gArrowPosition[7].h = 26;

	//location of empty pokeball on sprite sheet, not implemented in game
	gEmptyPokeball.x = 123;
	gEmptyPokeball.y = 65;
	gEmptyPokeball.w = 7;
	gEmptyPokeball.h = 7;

	//location of live pokeball in sprite sheet, no implemented in game
	gLivePokeball.x = 133;
	gLivePokeball.y = 65;
	gLivePokeball.w = 7;
	gLivePokeball.h = 7;

	//location of dead pokeball in sprite sheet, not implemented in game
	gDeadPokeball.x = 143;
	gDeadPokeball.y = 65;
	gDeadPokeball.w = 7;
	gDeadPokeball.h = 7;

	//location of list of players available pokeballs in sprite sheet, not implemented in game
	gPokeballs.x = 178;
	gPokeballs.y = 65;
	gPokeballs.w = 104;
	gPokeballs.h = 12;
	
	//location of list of players available pokeballs in window, not implemented in game
	gPokeballsWindow.x = (SCREEN_WIDTH/2) + 20;
	gPokeballsWindow.y = 350;
	gPokeballsWindow.w = 300;
	gPokeballsWindow.h = 20;

	//location of moves menu in sprite sheet
	gMovesMenu.x = 297;
	gMovesMenu.y = 4;
	gMovesMenu.w = 239;
	gMovesMenu.h = 47;

	//location of moves menu in window
	gMovesMenuPos.x = 0;
	gMovesMenuPos.y = 404;
	gMovesMenuPos.w = SCREEN_WIDTH;
	gMovesMenuPos.h = SCREEN_HEIGHT - 404;

	//location of pokemon selection menu background in sprite sheet
	gPokemon.x = 5;
	gPokemon.y = 5;
	gPokemon.w = 240;
	gPokemon.h = 160;

	//location of pokemon selection menu background in window
	gPokemonWindow.x = 0;
	gPokemonWindow.y = 0;
	gPokemonWindow.w = SCREEN_WIDTH;
	gPokemonWindow.h = SCREEN_HEIGHT;

	//location of pokemons names in sprite sheet
	gPokemonNames[PIKACHU].x = 0;
	gPokemonNames[PIKACHU].y = 20;
	gPokemonNames[PIKACHU].w = 128;
	gPokemonNames[PIKACHU].h = 20;

	gPokemonNames[CHARMANDER].x = 0;
	gPokemonNames[CHARMANDER].y = 40;
	gPokemonNames[CHARMANDER].w = 128;
	gPokemonNames[CHARMANDER].h = 20;

	gPokemonNames[SQUIRTLE].x = 0;
	gPokemonNames[SQUIRTLE].y = 60;
	gPokemonNames[SQUIRTLE].w = 128;
	gPokemonNames[SQUIRTLE].h = 20;

	gPokemonNames[BULBASAUR].x = 0;
	gPokemonNames[BULBASAUR].y = 0;
	gPokemonNames[BULBASAUR].w = 128;
	gPokemonNames[BULBASAUR].h = 20;

	gPokemonNames[RATTATA].x = 0;
	gPokemonNames[RATTATA].y = 80;
	gPokemonNames[RATTATA].w = 128;
	gPokemonNames[RATTATA].h = 20;

	gPokemonNames[PIDGEY].x = 0;
	gPokemonNames[PIDGEY].y = 100;
	gPokemonNames[PIDGEY].w = 128;
	gPokemonNames[PIDGEY].h = 20;

	gPokemonNames[MACHOP].x = 0;
	gPokemonNames[MACHOP].y = 120;
	gPokemonNames[MACHOP].w = 128;
	gPokemonNames[MACHOP].h = 20;

	gPokemonNames[DROWZEE].x = 0;
	gPokemonNames[DROWZEE].y = 140;
	gPokemonNames[DROWZEE].w = 128;
	gPokemonNames[DROWZEE].h = 20;

	gPokemonNames[SNORLAX].x = 0;
	gPokemonNames[SNORLAX].y = 160;
	gPokemonNames[SNORLAX].w = 128;
	gPokemonNames[SNORLAX].h = 20;

	gPokemonNames[ZUBAT].x = 0;
	gPokemonNames[ZUBAT].y = 180;
	gPokemonNames[ZUBAT].w = 128;
	gPokemonNames[ZUBAT].h = 20;

	gPokemonNames[MANKEY].x = 0;
	gPokemonNames[MANKEY].y = 200;
	gPokemonNames[MANKEY].w = 128;
	gPokemonNames[MANKEY].h = 20;

	gPokemonNames[ELECTABUZZ].x = 0;
	gPokemonNames[ELECTABUZZ].y = 220;
	gPokemonNames[ELECTABUZZ].w = 128;
	gPokemonNames[ELECTABUZZ].h = 20;

	gPokemonNames[SLOWPOKE].x = 0;
	gPokemonNames[SLOWPOKE].y = 240;
	gPokemonNames[SLOWPOKE].w = 128;
	gPokemonNames[SLOWPOKE].h = 20;

	gPokemonNames[PONYTA].x = 0;
	gPokemonNames[PONYTA].y = 260;
	gPokemonNames[PONYTA].w = 128;
	gPokemonNames[PONYTA].h = 20;

	gPokemonNames[ODDISH].x = 0;
	gPokemonNames[ODDISH].y = 280;
	gPokemonNames[ODDISH].w = 128;
	gPokemonNames[ODDISH].h = 20;

	gPokemonNames[ABRA].x = 0;
	gPokemonNames[ABRA].y = 300;
	gPokemonNames[ABRA].w = 128;
	gPokemonNames[ABRA].h = 20;

	//location of moves names in sprite sheet
	gMoves[TACKLE].x = 142;
	gMoves[TACKLE].y = 0;
	gMoves[TACKLE].w = 155;
	gMoves[TACKLE].h = 20;

	gMoves[SCRATCH].x = 142;
	gMoves[SCRATCH].y = 20;
	gMoves[SCRATCH].w = 155;
	gMoves[SCRATCH].h = 20;

	gMoves[EMBER].x = 142;
	gMoves[EMBER].y = 40;
	gMoves[EMBER].w = 155;
	gMoves[EMBER].h = 20;

	gMoves[WATERGUN].x = 142;
	gMoves[WATERGUN].y = 60;
	gMoves[WATERGUN].w = 155;
	gMoves[WATERGUN].h = 20;

	gMoves[CUT].x = 142;
	gMoves[CUT].y = 80;
	gMoves[CUT].w = 155;
	gMoves[CUT].h = 20;

	gMoves[SHOCK].x = 142;
	gMoves[SHOCK].y = 100;
	gMoves[SHOCK].w = 155;
	gMoves[SHOCK].h = 20;

	gMoves[KARATECHOP].x = 0;
	gMoves[KARATECHOP].y = 340;
	gMoves[KARATECHOP].w = 128;
	gMoves[KARATECHOP].h = 20;

	gMoves[GUST].x = 142;
	gMoves[GUST].y = 140;
	gMoves[GUST].w = 155;
	gMoves[GUST].h = 20;

	gMoves[HEADBUTT].x = 142;
	gMoves[HEADBUTT].y = 160;
	gMoves[HEADBUTT].w = 155;
	gMoves[HEADBUTT].h = 20;

	gMoves[BITE].x = 142;
	gMoves[BITE].y = 180;
	gMoves[BITE].w = 155;
	gMoves[BITE].h = 20;

	gMoves[BUBBLEBEAM].x = 142;
	gMoves[BUBBLEBEAM].y = 200;
	gMoves[BUBBLEBEAM].w = 155;
	gMoves[BUBBLEBEAM].h = 20;

	gMoves[RAZORLEAF].x = 142;
	gMoves[RAZORLEAF].y = 220;
	gMoves[RAZORLEAF].w = 155;
	gMoves[RAZORLEAF].h = 20;

	gMoves[WINGATTACK].x = 0;
	gMoves[WINGATTACK].y = 320;
	gMoves[WINGATTACK].w = 128;
	gMoves[WINGATTACK].h = 20;

	gMoves[QUICKATTACK].x = 142;
	gMoves[QUICKATTACK].y = 260;
	gMoves[QUICKATTACK].w = 155;
	gMoves[QUICKATTACK].h = 20;

	gMoves[FLAMETHROWER].x = 142;
	gMoves[FLAMETHROWER].y = 280;
	gMoves[FLAMETHROWER].w = 155;
	gMoves[FLAMETHROWER].h = 20;

	gMoves[VINEWHIP].x = 142;
	gMoves[VINEWHIP].y = 300;
	gMoves[VINEWHIP].w = 155;
	gMoves[VINEWHIP].h = 20;

	gMoves[THUNDERBOLT].x = 142;
	gMoves[THUNDERBOLT].y = 320;
	gMoves[THUNDERBOLT].w = 155;
	gMoves[THUNDERBOLT].h = 20;

	gMoves[PSYCHIC].x = 142;
	gMoves[PSYCHIC].y = 360;
	gMoves[PSYCHIC].w = 155;
	gMoves[PSYCHIC].h = 20;

	gMoves[PSYBEAM].x = 142;
	gMoves[PSYBEAM].y = 380;
	gMoves[PSYBEAM].w = 155;
	gMoves[PSYBEAM].h = 20;

	gMoves[LOWKICK].x = 142;
	gMoves[LOWKICK].y = 400;
	gMoves[LOWKICK].w = 155;
	gMoves[LOWKICK].h = 20;

	gMoves[SLASH].x = 142;
	gMoves[SLASH].y = 420;
	gMoves[SLASH].w = 155;
	gMoves[SLASH].h = 20;

	gMoves[PECK].x = 142;
	gMoves[PECK].y = 440;
	gMoves[PECK].w = 155;
	gMoves[PECK].h = 20;

	gMoves[SEISMICTOSS].x = 142;
	gMoves[SEISMICTOSS].y = 460;
	gMoves[SEISMICTOSS].w = 155;
	gMoves[SEISMICTOSS].h = 20;

	gMoves[THUNDER].x = 142;
	gMoves[THUNDER].y = 480;
	gMoves[THUNDER].w = 155;
	gMoves[THUNDER].h = 20;

	gMoves[HYDROPUMP].x = 142;
	gMoves[HYDROPUMP].y = 500;
	gMoves[HYDROPUMP].w = 155;
	gMoves[HYDROPUMP].h = 20;

	gMoves[FIREBLAST].x = 142;
	gMoves[FIREBLAST].y = 520;
	gMoves[FIREBLAST].w = 155;
	gMoves[FIREBLAST].h = 20;

	gMoves[SOLARBEAM].x = 142;
	gMoves[SOLARBEAM].y = 540;
	gMoves[SOLARBEAM].w = 155;
	gMoves[SOLARBEAM].h = 20;

	gMoves[BODYSLAM].x = 140;
	gMoves[BODYSLAM].y = 560;
	gMoves[BODYSLAM].w = 155;
	gMoves[BODYSLAM].h = 20;

	gMoves[SWIFT].x = 142;
	gMoves[SWIFT].y = 580;
	gMoves[SWIFT].w = 155;
	gMoves[SWIFT].h = 20;

	gMoves[CONFUSION].x = 142;
	gMoves[CONFUSION].y = 240;
	gMoves[CONFUSION].w = 155;
	gMoves[CONFUSION].h = 20;
	
	//Window position for Move 1
	gMovesWindow[0].x = 40;
	gMovesWindow[0].y = 440;
	gMovesWindow[0].w = 150;
	gMovesWindow[0].h = 26;

	//Window position for Move 2
	gMovesWindow[1].x = 40;
	gMovesWindow[1].y = 509;
	gMovesWindow[1].w = 150;
	gMovesWindow[1].h = 26;

	//Window position for Move 3
	gMovesWindow[2].x = 290;
	gMovesWindow[2].y = 440;
	gMovesWindow[2].w = 150;
	gMovesWindow[2].h = 26;

	//Window position for Move 4
	gMovesWindow[3].x = 290;
	gMovesWindow[3].y = 509;
	gMovesWindow[3].w = 150;
	gMovesWindow[3].h = 26;

	//location of green health bar in sprite sheet
	gHealthBar[GREEN].x = 117;
	gHealthBar[GREEN].y = 9;
	gHealthBar[GREEN].w = 1;
	gHealthBar[GREEN].h = 3;

	//location of yellow health bar in sprite sheet
	gHealthBar[YELLOW].x = 117;
	gHealthBar[YELLOW].y = 13;
	gHealthBar[YELLOW].w = 1;
	gHealthBar[YELLOW].h = 3;

	//location of red health bar in sprite sheet
	gHealthBar[RED].x = 117;
	gHealthBar[RED].y = 17;
	gHealthBar[RED].w = 1;
	gHealthBar[RED].h = 3;

	//location of opponents health in window
	gOpponentHealth.x = 168;
	gOpponentHealth.y = 91;
	gOpponentHealth.w = 144;
	gOpponentHealth.h = 10;

	//location of players health in window
	gPlayerHealth.x = 566;
	gPlayerHealth.y = 333;
	gPlayerHealth.w = 162;
	gPlayerHealth.h = 10;

	//location of selected pokemon in sprite sheet for pokemon menu
	gPokemonListSelect.x = 162;
	gPokemonListSelect.y = 203;
	gPokemonListSelect.w = 149;
	gPokemonListSelect.h = 24;

	//location of unselected pokemon in sprite sheet for pokemon menu
	gPokemonListUnselect.x = 162;
	gPokemonListUnselect.y = 179;
	gPokemonListUnselect.w = 149;
	gPokemonListUnselect.h = 21;

	//location of first pokemon in list in window
	gPokemonListWindow[0].x = 75;
	gPokemonListWindow[0].y = 30;
	gPokemonListWindow[0].w = 650;
	gPokemonListWindow[0].h = 75;

	//location of second pokemon in list in window
	gPokemonListWindow[1].x = 75;
	gPokemonListWindow[1].y = 115;
	gPokemonListWindow[1].w = 650;
	gPokemonListWindow[1].h = 75;

	//location of third pokemon in list in window
	gPokemonListWindow[2].x = 75;
	gPokemonListWindow[2].y = 200;
	gPokemonListWindow[2].w = 650;
	gPokemonListWindow[2].h = 75;

	//location of fourth pokemon in list in window
	gPokemonListWindow[3].x = 75;
	gPokemonListWindow[3].y = 285;
	gPokemonListWindow[3].w = 650;
	gPokemonListWindow[3].h = 75;

	//location of fifth pokemon in list in window
	gPokemonListWindow[4].x = 75;
	gPokemonListWindow[4].y = 370;
	gPokemonListWindow[4].w = 650;
	gPokemonListWindow[4].h = 75;

	//location of sixth pokemon in list in window
	gPokemonListWindow[5].x = 75;
	gPokemonListWindow[5].y = 455;
	gPokemonListWindow[5].w = 650;
	gPokemonListWindow[5].h = 75;

	//locations for image of battle pokemon facing user in sprites sheet
	gOpponentBattlePokemon[PIKACHU].x = 496;
	gOpponentBattlePokemon[PIKACHU].y = 203;
	gOpponentBattlePokemon[PIKACHU].w = 42;
	gOpponentBattlePokemon[PIKACHU].h = 50;

	gOpponentBattlePokemon[CHARMANDER].x = 175;
	gOpponentBattlePokemon[CHARMANDER].y = 12;
	gOpponentBattlePokemon[CHARMANDER].w = 36;
	gOpponentBattlePokemon[CHARMANDER].h = 40;

	gOpponentBattlePokemon[BULBASAUR].x = 14;
	gOpponentBattlePokemon[BULBASAUR].y = 15;
	gOpponentBattlePokemon[BULBASAUR].w = 35;
	gOpponentBattlePokemon[BULBASAUR].h = 34;

	gOpponentBattlePokemon[SQUIRTLE].x = 338;
	gOpponentBattlePokemon[SQUIRTLE].y = 14;
	gOpponentBattlePokemon[SQUIRTLE].w = 39;
	gOpponentBattlePokemon[SQUIRTLE].h = 39;

	gOpponentBattlePokemon[RATTATA].x = 172;
	gOpponentBattlePokemon[RATTATA].y = 212;
	gOpponentBattlePokemon[RATTATA].w = 37;
	gOpponentBattlePokemon[RATTATA].h = 36;

	gOpponentBattlePokemon[PIDGEY].x = 14;
	gOpponentBattlePokemon[PIDGEY].y = 211;
	gOpponentBattlePokemon[PIDGEY].w = 37;
	gOpponentBattlePokemon[PIDGEY].h = 38;

	gOpponentBattlePokemon[MACHOP].x = 178;
	gOpponentBattlePokemon[MACHOP].y = 929;
	gOpponentBattlePokemon[MACHOP].w = 32;
	gOpponentBattlePokemon[MACHOP].h = 43;

	gOpponentBattlePokemon[DROWZEE].x = 975;
	gOpponentBattlePokemon[DROWZEE].y = 336;
	gOpponentBattlePokemon[DROWZEE].w = 45;
	gOpponentBattlePokemon[DROWZEE].h = 47;

	gOpponentBattlePokemon[SNORLAX].x = 1128;
	gOpponentBattlePokemon[SNORLAX].y = 858;
	gOpponentBattlePokemon[SNORLAX].w = 64;
	gOpponentBattlePokemon[SNORLAX].h = 55;

	gOpponentBattlePokemon[ZUBAT].x = 494;
	gOpponentBattlePokemon[ZUBAT].y = 470;
	gOpponentBattlePokemon[ZUBAT].w = 44;
	gOpponentBattlePokemon[ZUBAT].h = 41;

	gOpponentBattlePokemon[MANKEY].x = 494;
	gOpponentBattlePokemon[MANKEY].y = 667;
	gOpponentBattlePokemon[MANKEY].w = 45;
	gOpponentBattlePokemon[MANKEY].h = 40;

	gOpponentBattlePokemon[ELECTABUZZ].x = 970;
	gOpponentBattlePokemon[ELECTABUZZ].y = 658;
	gOpponentBattlePokemon[ELECTABUZZ].w = 62;
	gOpponentBattlePokemon[ELECTABUZZ].h = 60;

	gOpponentBattlePokemon[SLOWPOKE].x = 976;
	gOpponentBattlePokemon[SLOWPOKE].y = 12;
	gOpponentBattlePokemon[SLOWPOKE].w = 48;
	gOpponentBattlePokemon[SLOWPOKE].h = 41;

	gOpponentBattlePokemon[PONYTA].x = 813;
	gOpponentBattlePokemon[PONYTA].y = 71;
	gOpponentBattlePokemon[PONYTA].w = 49;
	gOpponentBattlePokemon[PONYTA].h = 53;

	gOpponentBattlePokemon[ODDISH].x = 661;
	gOpponentBattlePokemon[ODDISH].y = 410;
	gOpponentBattlePokemon[ODDISH].w = 33;
	gOpponentBattlePokemon[ODDISH].h = 38;

	gOpponentBattlePokemon[ABRA].x = 11;
	gOpponentBattlePokemon[ABRA].y = 937;
	gOpponentBattlePokemon[ABRA].w = 44;
	gOpponentBattlePokemon[ABRA].h = 38;

	//locations of battle pokemon facing away from the user in sprite sheet
	gPlayerBattlePokemon[PIKACHU].x = 552;
	gPlayerBattlePokemon[PIKACHU].y = 205;
	gPlayerBattlePokemon[PIKACHU].w = 52;
	gPlayerBattlePokemon[PIKACHU].h = 50;

	gPlayerBattlePokemon[CHARMANDER].x = 234;
	gPlayerBattlePokemon[CHARMANDER].y = 11;
	gPlayerBattlePokemon[CHARMANDER].w = 48;
	gPlayerBattlePokemon[CHARMANDER].h = 44;

	gPlayerBattlePokemon[BULBASAUR].x = 74;
	gPlayerBattlePokemon[BULBASAUR].y = 14;
	gPlayerBattlePokemon[BULBASAUR].w = 46;
	gPlayerBattlePokemon[BULBASAUR].h = 35;

	gPlayerBattlePokemon[SQUIRTLE].x = 393;
	gPlayerBattlePokemon[SQUIRTLE].y = 16;
	gPlayerBattlePokemon[SQUIRTLE].w = 46;
	gPlayerBattlePokemon[SQUIRTLE].h = 34;

	gPlayerBattlePokemon[RATTATA].x = 234;
	gPlayerBattlePokemon[RATTATA].y = 210;
	gPlayerBattlePokemon[RATTATA].w = 50;
	gPlayerBattlePokemon[RATTATA].h = 39;

	gPlayerBattlePokemon[PIDGEY].x = 72;
	gPlayerBattlePokemon[PIDGEY].y = 205;
	gPlayerBattlePokemon[PIDGEY].w = 49;
	gPlayerBattlePokemon[PIDGEY].h = 49;

	gPlayerBattlePokemon[MACHOP].x = 234;
	gPlayerBattlePokemon[MACHOP].y = 930;
	gPlayerBattlePokemon[MACHOP].w = 47;
	gPlayerBattlePokemon[MACHOP].h = 41;

	gPlayerBattlePokemon[DROWZEE].x = 1038;
	gPlayerBattlePokemon[DROWZEE].y = 341;
	gPlayerBattlePokemon[DROWZEE].w = 49;
	gPlayerBattlePokemon[DROWZEE].h = 38;

	gPlayerBattlePokemon[SNORLAX].x = 1193;
	gPlayerBattlePokemon[SNORLAX].y = 865;
	gPlayerBattlePokemon[SNORLAX].w = 62;
	gPlayerBattlePokemon[SNORLAX].h = 42;

	gPlayerBattlePokemon[ZUBAT].x = 552;
	gPlayerBattlePokemon[ZUBAT].y = 471;
	gPlayerBattlePokemon[ZUBAT].w = 55;
	gPlayerBattlePokemon[ZUBAT].h = 42;

	gPlayerBattlePokemon[MANKEY].x = 553;
	gPlayerBattlePokemon[MANKEY].y = 667;
	gPlayerBattlePokemon[MANKEY].w = 52;
	gPlayerBattlePokemon[MANKEY].h = 43;

	gPlayerBattlePokemon[ELECTABUZZ].x = 1038;
	gPlayerBattlePokemon[ELECTABUZZ].y = 665;
	gPlayerBattlePokemon[ELECTABUZZ].w = 49;
	gPlayerBattlePokemon[ELECTABUZZ].h = 48;

	gPlayerBattlePokemon[SLOWPOKE].x = 1033;
	gPlayerBattlePokemon[SLOWPOKE].y = 14;
	gPlayerBattlePokemon[SLOWPOKE].w = 59;
	gPlayerBattlePokemon[SLOWPOKE].h = 38;

	gPlayerBattlePokemon[PONYTA].x = 877;
	gPlayerBattlePokemon[PONYTA].y = 75;
	gPlayerBattlePokemon[PONYTA].w = 50;
	gPlayerBattlePokemon[PONYTA].h = 47;

	gPlayerBattlePokemon[ODDISH].x = 722;
	gPlayerBattlePokemon[ODDISH].y = 403;
	gPlayerBattlePokemon[ODDISH].w = 44;
	gPlayerBattlePokemon[ODDISH].h = 49;

	gPlayerBattlePokemon[ABRA].x = 75;
	gPlayerBattlePokemon[ABRA].y = 930;
	gPlayerBattlePokemon[ABRA].w = 41;
	gPlayerBattlePokemon[ABRA].h = 41;

	//locations of images for pokemon used in selection menu in sprite sheet
	gSelectPokemon[PIKACHU].x = 618;
	gSelectPokemon[PIKACHU].y = 208;
	gSelectPokemon[PIKACHU].w = 20;
	gSelectPokemon[PIKACHU].h = 20;

	gSelectPokemon[CHARMANDER].x = 295;
	gSelectPokemon[CHARMANDER].y = 12;
	gSelectPokemon[CHARMANDER].w = 22;
	gSelectPokemon[CHARMANDER].h = 18;

	gSelectPokemon[BULBASAUR].x = 134;
	gSelectPokemon[BULBASAUR].y = 12;
	gSelectPokemon[BULBASAUR].w = 21;
	gSelectPokemon[BULBASAUR].h = 18;

	gSelectPokemon[SQUIRTLE].x = 456;
	gSelectPokemon[SQUIRTLE].y = 12;
	gSelectPokemon[SQUIRTLE].w = 22;
	gSelectPokemon[SQUIRTLE].h = 18;

	gSelectPokemon[RATTATA].x = 296;
	gSelectPokemon[RATTATA].y = 207;
	gSelectPokemon[RATTATA].w = 19;
	gSelectPokemon[RATTATA].h = 22;

	gSelectPokemon[PIDGEY].x = 135;
	gSelectPokemon[PIDGEY].y = 209;
	gSelectPokemon[PIDGEY].w = 19;
	gSelectPokemon[PIDGEY].h = 18;

	gSelectPokemon[MACHOP].x = 297;
	gSelectPokemon[MACHOP].y = 928;
	gSelectPokemon[MACHOP].w = 17;
	gSelectPokemon[MACHOP].h = 21;

	gSelectPokemon[DROWZEE].x = 1099;
	gSelectPokemon[DROWZEE].y = 337;
	gSelectPokemon[DROWZEE].w = 23;
	gSelectPokemon[DROWZEE].h = 21;

	gSelectPokemon[SNORLAX].x = 1260;
	gSelectPokemon[SNORLAX].y = 862;
	gSelectPokemon[SNORLAX].w = 25;
	gSelectPokemon[SNORLAX].h = 22;

	gSelectPokemon[ZUBAT].x = 617;
	gSelectPokemon[ZUBAT].y = 470;
	gSelectPokemon[ZUBAT].w = 21;
	gSelectPokemon[ZUBAT].h = 19;

	gSelectPokemon[MANKEY].x = 615;
	gSelectPokemon[MANKEY].y = 669;
	gSelectPokemon[MANKEY].w = 26;
	gSelectPokemon[MANKEY].h = 17;

	gSelectPokemon[ELECTABUZZ].x = 1100;
	gSelectPokemon[ELECTABUZZ].y = 667;
	gSelectPokemon[ELECTABUZZ].w = 21;
	gSelectPokemon[ELECTABUZZ].h = 20;

	gSelectPokemon[SLOWPOKE].x = 1100;
	gSelectPokemon[SLOWPOKE].y = 10;
	gSelectPokemon[SLOWPOKE].w = 21;
	gSelectPokemon[SLOWPOKE].h = 20;

	gSelectPokemon[PONYTA].x = 938;
	gSelectPokemon[PONYTA].y = 74;
	gSelectPokemon[PONYTA].w = 25;
	gSelectPokemon[PONYTA].h = 23;

	gSelectPokemon[ODDISH].x = 782;
	gSelectPokemon[ODDISH].y = 407;
	gSelectPokemon[ODDISH].w = 14;
	gSelectPokemon[ODDISH].h = 16;

	gSelectPokemon[ABRA].x = 136;
	gSelectPokemon[ABRA].y = 928;
	gSelectPokemon[ABRA].w = 20;
	gSelectPokemon[ABRA].h = 19;

	//location of opponents pokemon on window
	gOpponentPokemon.x = 500;
	gOpponentPokemon.y = 110;
	gOpponentPokemon.w = 125;
	gOpponentPokemon.h = 125;

	//location of opponents pokemon name on window
	gOpponentName.x = 80;
	gOpponentName.y = 50;
	gOpponentName.w = 150;
	gOpponentName.h = 30;

	//location of players pokemon on window
	gPlayerPokemon.x = 100;
	gPlayerPokemon.y = 254;
	gPlayerPokemon.w = 150;
	gPlayerPokemon.h = 150;

	//location of player pokemon name on window
	gPlayerName.x = 460;
	gPlayerName.y = 290;
	gPlayerName.w = 150;
	gPlayerName.h = 30;

	//locations of names in selection menu on window
	gPokemonSelectName[0].x = 250;
	gPokemonSelectName[0].y = 40;
	gPokemonSelectName[0].w = 150;
	gPokemonSelectName[0].h = 30;

	gPokemonSelectName[1].x = 250;
	gPokemonSelectName[1].y = 125;
	gPokemonSelectName[1].w = 150;
	gPokemonSelectName[1].h = 30;

	gPokemonSelectName[2].x = 250;
	gPokemonSelectName[2].y = 210;
	gPokemonSelectName[2].w = 150;
	gPokemonSelectName[2].h = 30;

	gPokemonSelectName[3].x = 250;
	gPokemonSelectName[3].y = 295;
	gPokemonSelectName[3].w = 150;
	gPokemonSelectName[3].h = 30;

	gPokemonSelectName[4].x = 250;
	gPokemonSelectName[4].y = 380;
	gPokemonSelectName[4].w = 150;
	gPokemonSelectName[4].h = 30;

	gPokemonSelectName[5].x = 250;
	gPokemonSelectName[5].y = 465;
	gPokemonSelectName[5].w = 150;
	gPokemonSelectName[5].h = 30;

	//locations of images of pokemon in selection menu on window
	gPokemonSelectImage[0].x = 170;
	gPokemonSelectImage[0].y = 42;
	gPokemonSelectImage[0].w = 50;
	gPokemonSelectImage[0].h = 50;

	gPokemonSelectImage[1].x = 170;
	gPokemonSelectImage[1].y = 127;
	gPokemonSelectImage[1].w = 50;
	gPokemonSelectImage[1].h = 50;

	gPokemonSelectImage[2].x = 170;
	gPokemonSelectImage[2].y = 212;
	gPokemonSelectImage[2].w = 50;
	gPokemonSelectImage[2].h = 50;

	gPokemonSelectImage[3].x = 170;
	gPokemonSelectImage[3].y = 297;
	gPokemonSelectImage[3].w = 50;
	gPokemonSelectImage[3].h = 50;

	gPokemonSelectImage[4].x = 170;
	gPokemonSelectImage[4].y = 382;
	gPokemonSelectImage[4].w = 50;
	gPokemonSelectImage[4].h = 50;

	gPokemonSelectImage[5].x = 170;
	gPokemonSelectImage[5].y = 467;
	gPokemonSelectImage[5].w = 50;
	gPokemonSelectImage[5].h = 50;

	return success;//return in the function successfully loaded all the media
}

void battleScene::close()
{
	//Deallocate Surfaces
	SDL_FreeSurface(gBackground);
	SDL_FreeSurface(gScreenSurface);
	SDL_FreeSurface(gMenuSheet);
	SDL_FreeSurface(gPokemonMenu);
	SDL_FreeSurface(gNames_Moves);

	SDL_DestroyWindow(gWindow);//destroys the window
	gWindow = NULL;//points window pointer to NULL just incase

	SDL_Quit();//cleans up initialized subsystems
}

SDL_Surface* battleScene::loadSurface(string path)
{
	//optimizes the loaded image
	SDL_Surface* optimizedSurface = NULL;//pointer to optimized image

	SDL_Surface* loadedSurface = IMG_Load(path.c_str());//loads image
	if(loadedSurface == NULL){//Error if image could not load properly
		cout << "Unable to load image " << path.c_str() << "! SDL Error: " << SDL_GetError() << endl;
	}
	else{
		optimizedSurface = SDL_ConvertSurface(loadedSurface, gScreenSurface->format, NULL);//convert image to optmized form
		if(optimizedSurface == NULL){//Error if issue optimizing
			cout << "Unable to optimize image! SDL Error: " << SDL_GetError() << endl;
		}

		SDL_FreeSurface(loadedSurface);//Free memory that held the initial image
	}
	
	return optimizedSurface;
}

int battleScene::battle(int wildLevel)
{
	int turn = 0;//used to determine if it is the players turn or not
	//boolean variables to determine what state the screen should be in
	bool inMenu = true;
	bool inMoves = false;
	bool inPokemon = false;
	int selected = 0;//determines which of the pokemon is currently selected in pokemon selection menu
	//set the color of health bars to green to begin
	gCurrentPlayerColor = gHealthBar[GREEN];
	gCurrentOpponentColor = gHealthBar[GREEN];

	if(!init()){//initializes window, if it fails display error message
    cout << "Failed to initialize!" << endl;
  }
  else{//if window is initialized successfully
    if(!loadMedia()){//load the images, if it fails display error message
      cout << "Failed to load media!" << endl;
    }
    else{//if media is successfully loaded begin displaying images
      bool quit = false;//boolean variable for when the user want to quit
			SDL_Event e;//variable for keyboard events entered by user
			gCurrentArrowPos = gArrowPosition[0];
			if(battleType == 0){
     				(*myTrainer).wild_battle(wildLevel); //load wild pokemon to fight
			}
			else if(battleType == -1){
				(*myTrainer).fish_battle(wildLevel);
			}
			else{
				(*myTrainer).player_battle(battleType,wildLevel);
			}
			while(!quit){//while the player has not clicked the red x
				while(SDL_PollEvent(&e) != 0){
					if(e.type == SDL_QUIT) quit = true;//event entered was x-ing out of window, set quit to true
					else if(e.type == SDL_KEYDOWN){//key has been pressed
						if(turn%2 == 0 && inMenu){//player is in option menu and it is their turn
							switch(e.key.keysym.sym){//switch with key type parameter
								case SDLK_UP:
								{
									moveArrow("up");
									break;
								}
								case SDLK_DOWN:
								{
									moveArrow("down");
									break;
								}
								case SDLK_LEFT:
								{
									moveArrow("left");
									break;
								}
								case SDLK_RIGHT:
								{
									moveArrow("right");
									break;
								}
								case SDLK_SPACE:
								{
									int temp1 = menuOption("inMenu");//returns an integer based on where the arrow currently is
									if (temp1 == 5){//if user has chosen to fight
										inMenu = false;
										inMoves = true;
									}
									else if(temp1 == 6){
										quit = (*myTrainer).run();
										if(quit){
											return_value = 4;
										}
										else{
											int theyKoUs = (*myTrainer).fight_comp();
											if(theyKoUs){
												inMenu = false;
												inPokemon = true;
												inMoves = false;
												if((*myTrainer).switchPoke(0) == -1){
													quit = 1;	
													(*myTrainer).pokeCenter(1);
													return_value = 1;
												}
												continue;
											}
										}
									}
									else if(temp1 == 7){
										inMenu = false;
										inPokemon = true;
									}
									else if(temp1 == 8){
										if(battleType <= 0){
											quit = (*myTrainer).catchPoke();
											if(quit){
												return_value = 3; //caught
											}
											else if((*myTrainer).getNumPoke()==6){
												continue;
												//do nothing continue
											}
											else{
												int usko = (*myTrainer).fight_comp();
												if(usko){
													inMenu = false;
													inPokemon = true;
													inMoves = false;
													if((*myTrainer).switchPoke(0) == -1){
														quit = 1;	
														(*myTrainer).pokeCenter(1);
														return_value = 1;
													}
													continue;
												}
											}
										}
									}
									break;
								}
								default:
								{
									break;
								}
							}
						}
						else if(turn%2 == 0 && inMoves){//player is in moves menu and it is their turn
							switch(e.key.keysym.sym){//switch with key type parameter
								case SDLK_UP:
								{
									moveArrow("up");
									break;
								}
								case SDLK_DOWN:
								{
									moveArrow("down");
									break;
								}
								case SDLK_LEFT:
								{
									moveArrow("left");
									break;
								}
								case SDLK_RIGHT:
								{
									moveArrow("right");
									break;
								}
								case SDLK_SPACE:
								{
									int temp2 = menuOption("inMoves");
									inMenu = true;
									inMoves = false;
									if(battleType <= 0){	
										quit = (*myTrainer).fight(temp2);
										if(!quit){
											int ko2_us = (*myTrainer).fight_comp(); //have computer attack	
											if(ko2_us){
												inMenu = false;
												inMoves = false;
												inPokemon = true;
												if((*myTrainer).switchPoke(0) == -1){
													quit = 1;	
													(*myTrainer).pokeCenter(1); //1 for whiteout so erase all current exp
													return_value = 1;
												}
												gCurrentArrowPos = gArrowPosition[0];
											}
										}
										else{ //quit = 1
											return_value = 2; //we koed them
										}
									}
									else{ //trainer case where we need to check ko's
										int ko_them = (*myTrainer).fight(temp2);
										quit = 0;
										if(ko_them){
											if(!((*myTrainer).NextOp())){
												return_value = 2; //defeated enemy trainer
												quit = 1;
											}
										}
										if(!quit){
											int ko_us = (*myTrainer).fight_comp();
											if(ko_us){
												inMenu = false;
												inMoves = false;
												inPokemon = true; 
												if((*myTrainer).switchPoke(0) == -1){
													quit = 1;	
													(*myTrainer).pokeCenter(1); //whiteout
													return_value = 1;
												}
											}
										}
									}
									break;
								}
								default:
								{
									break;
								}
								
							}
						}
						else if(turn%2 == 0 && inPokemon){//player is in moves menu and it is their turn
							switch(e.key.keysym.sym){//switch with key type parameter
								case SDLK_UP:
								{
									if(selected > 0) selected--;//move arrow up if not at top item
									break;
								}
								case SDLK_DOWN:
								{
									if(selected < (*myTrainer).getNumPoke()-1) selected++;//move arrow down if not at bottom item
									break;
								}
								case SDLK_SPACE:
								{//return to options menu when a pokemon is selected
									nextPoke = selected;
									int temp2 = menuOption("inPokemon");
									int anyPoke = (*myTrainer).switchPoke(nextPoke);
									if(anyPoke==-1){
										return_value = 1; //whited out
										(*myTrainer).pokeCenter(1);
										quit = 1;
									}
									else if(anyPoke==-2){
										inMenu = false;
										inMoves = false;
										inPokemon = true; 
									}
									else{
										inMoves = false;	
										inMenu = true;
										inPokemon = false;
									}
									gCurrentArrowPos = gArrowPosition[0];
									break;
								}
								default:
								{
									break;
								}
								
							}
						}
					}
				}
				if(inMenu || inMoves){
					SDL_BlitScaled(gBackground, NULL, gScreenSurface, &gBack);//blit background to screen
					SDL_BlitScaled(gMenuSheet, &gMenuBack, gScreenSurface, &gMenuBackWindow);//blit menu background to screen
					//Blit options if it is your turn
					SDL_BlitScaled(gMenuSheet, &gOptions, gScreenSurface, &gOptionsWindow);//blit options menu to screen
					if (turn%2 == 0 && inMenu){
						SDL_BlitScaled(gMenuSheet, &gOptions, gScreenSurface, &gOptionsWindow);//blit options menu to screen
						SDL_BlitScaled(gMenuSheet, &gSelectionArrow, gScreenSurface, &gCurrentArrowPos);//blit selection arrow onto screen
					}
					else if(turn%2 == 0 && inMoves){
						SDL_BlitScaled(gMenuSheet, &gMovesMenu, gScreenSurface, &gMovesMenuPos);//blit moves menu to screen
						SDL_BlitScaled(gMenuSheet, &gSelectionArrow, gScreenSurface, &gCurrentArrowPos);//blit selection arrow onto screen
						//blit available moves to screen
						SDL_BlitScaled(gNames_Moves, &gMoves[(*myTrainer).getMoveNum(0)], gScreenSurface, &gMovesWindow[0]);
						SDL_BlitScaled(gNames_Moves, &gMoves[(*myTrainer).getMoveNum(1)], gScreenSurface, &gMovesWindow[1]);
						SDL_BlitScaled(gNames_Moves, &gMoves[(*myTrainer).getMoveNum(2)], gScreenSurface, &gMovesWindow[2]);
						SDL_BlitScaled(gNames_Moves, &gMoves[(*myTrainer).getMoveNum(3)], gScreenSurface, &gMovesWindow[3]);
					}
					//Blit enemy level if pokemon is out
					SDL_BlitScaled(gMenuSheet, &gOpponentLevel, gScreenSurface, &gOpponentLevelWindow);//blit level and health of enemy pokemon
					SDL_BlitScaled(gBattleSprites, &gOpponentBattlePokemon[(*myTrainer).opCurrentPoke()], gScreenSurface, &gOpponentPokemon);//blit image of opponents pokemon to screen
					SDL_BlitScaled(gNames_Moves, &gPokemonNames[(*myTrainer).opCurrentPoke()], gScreenSurface, &gOpponentName);//blits name of opponents pokemon to screen
					SDL_BlitScaled(gMenuSheet, &gCurrentOpponentColor, gScreenSurface, &gOpponentHealth);//blit opponents health to screem
					//Blit player level if pokemon is out
					SDL_BlitScaled(gMenuSheet, &gPlayerLevel, gScreenSurface, &gPlayerLevelWindow);//blit level and health of player's pokemon
					SDL_BlitScaled(gBattleSprites, &gPlayerBattlePokemon[(*myTrainer).myCurrentPoke()], gScreenSurface, &gPlayerPokemon);//blit image of opponents pokemon to screen
					SDL_BlitScaled(gNames_Moves, &gPokemonNames[(*myTrainer).myCurrentPoke()], gScreenSurface, &gPlayerName);//blits name of players pokemon to screen
					SDL_BlitScaled(gMenuSheet, &gCurrentPlayerColor, gScreenSurface, &gPlayerHealth);//blit players healt to screen
				}
				else if(inPokemon){//if in pokemon menu run function to blit proper images to screen
					pokemonMenu(selected);
				}
				gPlayerHealth.w = 162*(*myTrainer).getmyHealth()/(*myTrainer).getmyMaxHealth();//scale the width of health bar to match the ratio of current health to max health
				if((*myTrainer).getmyHealth() > (*myTrainer).getmyMaxHealth()/2){//if greater than half max health color is green
					gCurrentPlayerColor = gHealthBar[GREEN];
				}
				else if((*myTrainer).getmyHealth() > (*myTrainer).getmyMaxHealth()/4){//if greater than 1/4 and less than 1/2 max health color is yellow
					gCurrentPlayerColor = gHealthBar[YELLOW];
				}
				else{//if less than 1/4 max health color is red
					gCurrentPlayerColor = gHealthBar[RED];
				}
				gOpponentHealth.w = 144*(*myTrainer).getopHealth()/(*myTrainer).getopMaxHealth();//scale width of health bar to match the ratio of current health to max health
				if((*myTrainer).getopHealth() > (*myTrainer).getopMaxHealth()/2){
					gCurrentOpponentColor = gHealthBar[GREEN];
				}
				else if((*myTrainer).getopHealth() > (*myTrainer).getopMaxHealth()/4){
					gCurrentOpponentColor = gHealthBar[YELLOW];
				}
				else{
					gCurrentOpponentColor = gHealthBar[RED];
				}
				SDL_UpdateWindowSurface(gWindow);//update window
			}
    }
  }
	return(return_value);
}

void battleScene::moveArrow(string dir)
{//checks which the direction the user wants to move the arrow then checks where it is at an moves it accordingly
	if(dir == "up"){
		if(gCurrentArrowPos.x == gArrowPosition[0].x && gCurrentArrowPos.y == gArrowPosition[0].y){//arrow is in top left of corner of options menu so can't go up, leave the arrow where it is
			gCurrentArrowPos = gArrowPosition[0];
		}
		else if(gCurrentArrowPos.x == gArrowPosition[1].x && gCurrentArrowPos.y == gArrowPosition[1].y){
			gCurrentArrowPos = gArrowPosition[0];
		}
		else if(gCurrentArrowPos.x == gArrowPosition[2].x && gCurrentArrowPos.y == gArrowPosition[2].y){
			gCurrentArrowPos = gArrowPosition[2];
		}
		else if(gCurrentArrowPos.x == gArrowPosition[3].x && gCurrentArrowPos.y == gArrowPosition[3].y){
			gCurrentArrowPos = gArrowPosition[2];
		}
		else if(gCurrentArrowPos.x == gArrowPosition[4].x && gCurrentArrowPos.y == gArrowPosition[4].y){
			gCurrentArrowPos = gArrowPosition[4];
		}
		else if(gCurrentArrowPos.x == gArrowPosition[5].x && gCurrentArrowPos.y == gArrowPosition[5].y){
			gCurrentArrowPos = gArrowPosition[4];
		}
		else if(gCurrentArrowPos.x == gArrowPosition[6].x && gCurrentArrowPos.y == gArrowPosition[6].y){
			gCurrentArrowPos = gArrowPosition[6];
		}
		else if(gCurrentArrowPos.x == gArrowPosition[7].x && gCurrentArrowPos.y == gArrowPosition[7].y){
			gCurrentArrowPos = gArrowPosition[6];
		}
	}
	else if(dir == "down"){
		if(gCurrentArrowPos.x == gArrowPosition[0].x && gCurrentArrowPos.y == gArrowPosition[0].y){
			gCurrentArrowPos = gArrowPosition[1];
		}
		else if(gCurrentArrowPos.x == gArrowPosition[1].x && gCurrentArrowPos.y == gArrowPosition[1].y){
			gCurrentArrowPos = gArrowPosition[1];
		}
		else if(gCurrentArrowPos.x == gArrowPosition[2].x && gCurrentArrowPos.y == gArrowPosition[2].y){
			gCurrentArrowPos = gArrowPosition[3];
		}
		else if(gCurrentArrowPos.x == gArrowPosition[3].x && gCurrentArrowPos.y == gArrowPosition[3].y){
			gCurrentArrowPos = gArrowPosition[3];
		}
		else if(gCurrentArrowPos.x == gArrowPosition[4].x && gCurrentArrowPos.y == gArrowPosition[4].y){
			gCurrentArrowPos = gArrowPosition[5];
		}
		else if(gCurrentArrowPos.x == gArrowPosition[5].x && gCurrentArrowPos.y == gArrowPosition[5].y){
			gCurrentArrowPos = gArrowPosition[5];
		}
		else if(gCurrentArrowPos.x == gArrowPosition[6].x && gCurrentArrowPos.y == gArrowPosition[6].y){
			gCurrentArrowPos = gArrowPosition[7];
		}
		else if(gCurrentArrowPos.x == gArrowPosition[7].x && gCurrentArrowPos.y == gArrowPosition[7].y){
			gCurrentArrowPos = gArrowPosition[7];
		}
	}
	else if(dir == "left"){
		if(gCurrentArrowPos.x == gArrowPosition[0].x && gCurrentArrowPos.y == gArrowPosition[0].y){
			gCurrentArrowPos = gArrowPosition[0];
		}
		else if(gCurrentArrowPos.x == gArrowPosition[1].x && gCurrentArrowPos.y == gArrowPosition[1].y){
			gCurrentArrowPos = gArrowPosition[1];
		}
		else if(gCurrentArrowPos.x == gArrowPosition[2].x && gCurrentArrowPos.y == gArrowPosition[2].y){
			gCurrentArrowPos = gArrowPosition[0];
		}
		else if(gCurrentArrowPos.x == gArrowPosition[3].x && gCurrentArrowPos.y == gArrowPosition[3].y){
			gCurrentArrowPos = gArrowPosition[1];
		}
		else if(gCurrentArrowPos.x == gArrowPosition[4].x && gCurrentArrowPos.y == gArrowPosition[4].y){
			gCurrentArrowPos = gArrowPosition[4];
		}
		else if(gCurrentArrowPos.x == gArrowPosition[5].x && gCurrentArrowPos.y == gArrowPosition[5].y){
			gCurrentArrowPos = gArrowPosition[5];
		}
		else if(gCurrentArrowPos.x == gArrowPosition[6].x && gCurrentArrowPos.y == gArrowPosition[6].y){
			gCurrentArrowPos = gArrowPosition[4];
		}
		else if(gCurrentArrowPos.x == gArrowPosition[7].x && gCurrentArrowPos.y == gArrowPosition[7].y){
			gCurrentArrowPos = gArrowPosition[5];
		}
	}
	else if(dir == "right"){
		if(gCurrentArrowPos.x == gArrowPosition[0].x && gCurrentArrowPos.y == gArrowPosition[0].y){
			gCurrentArrowPos = gArrowPosition[2];
		}
		else if(gCurrentArrowPos.x == gArrowPosition[1].x && gCurrentArrowPos.y == gArrowPosition[1].y){
			gCurrentArrowPos = gArrowPosition[3];
		}
		else if(gCurrentArrowPos.x == gArrowPosition[2].x && gCurrentArrowPos.y == gArrowPosition[2].y){
			gCurrentArrowPos = gArrowPosition[2];
		}
		else if(gCurrentArrowPos.x == gArrowPosition[3].x && gCurrentArrowPos.y == gArrowPosition[3].y){
			gCurrentArrowPos = gArrowPosition[3];
		}
		else if(gCurrentArrowPos.x == gArrowPosition[4].x && gCurrentArrowPos.y == gArrowPosition[4].y){
			gCurrentArrowPos = gArrowPosition[6];
		}
		else if(gCurrentArrowPos.x == gArrowPosition[5].x && gCurrentArrowPos.y == gArrowPosition[5].y){
			gCurrentArrowPos = gArrowPosition[7];
		}
		else if(gCurrentArrowPos.x == gArrowPosition[6].x && gCurrentArrowPos.y == gArrowPosition[6].y){
			gCurrentArrowPos = gArrowPosition[6];
		}
		else if(gCurrentArrowPos.x == gArrowPosition[7].x && gCurrentArrowPos.y == gArrowPosition[7].y){
			gCurrentArrowPos = gArrowPosition[7];
		}
	}
}

int battleScene::menuOption(string state)
{//checks where the arrow currently is and returns a value that lets the battle function know what to do
	if(gCurrentArrowPos.x == gArrowPosition[0].x && gCurrentArrowPos.y == gArrowPosition[0].y){
		gCurrentArrowPos = gArrowPosition[4];
		return 5;
	}
	else if(gCurrentArrowPos.x == gArrowPosition[1].x && gCurrentArrowPos.y == gArrowPosition[1].y){
		return 7;
	}
	else if(gCurrentArrowPos.x == gArrowPosition[2].x && gCurrentArrowPos.y == gArrowPosition[2].y){
		return 8;
	}
	else if(gCurrentArrowPos.x == gArrowPosition[3].x && gCurrentArrowPos.y == gArrowPosition[3].y){
		return 6;
	}
	else if(gCurrentArrowPos.x == gArrowPosition[4].x && gCurrentArrowPos.y == gArrowPosition[4].y){
		gCurrentArrowPos = gArrowPosition[0];
		return 1;
	}
	else if(gCurrentArrowPos.x == gArrowPosition[5].x && gCurrentArrowPos.y == gArrowPosition[5].y){
		gCurrentArrowPos = gArrowPosition[0];
		return 2;
	}
	else if(gCurrentArrowPos.x == gArrowPosition[6].x && gCurrentArrowPos.y == gArrowPosition[6].y){
		gCurrentArrowPos = gArrowPosition[0];
		return 3;
	}
	else if(gCurrentArrowPos.x == gArrowPosition[7].x && gCurrentArrowPos.y == gArrowPosition[7].y){
		gCurrentArrowPos = gArrowPosition[0];
		return 4;
	}
}

void battleScene::pokemonMenu(int selected)
{
	SDL_BlitScaled(gPokemonMenu, &gPokemon, gScreenSurface, &gPokemonWindow);//blit background of pokemon list
	for(int i = 0; i < (*myTrainer).getNumPoke(); i++){//loop through all of the trainers pokemon
		if(i == selected){
			SDL_BlitScaled(gPokemonMenu, &gPokemonListSelect, gScreenSurface, &gPokemonListWindow[i]);//blit the selected image to screen if that pokemon is currently selected
		}
		else{
			SDL_BlitScaled(gPokemonMenu, &gPokemonListUnselect, gScreenSurface, &gPokemonListWindow[i]);//blit fthe unselected image to screen if that pokemon is not selected
		}
		SDL_BlitScaled(gNames_Moves, &gPokemonNames[(*myTrainer).whatPokeinParty(i)], gScreenSurface, &gPokemonSelectName[i]);//blit the name of the pokemon
		SDL_BlitScaled(gBattleSprites, &gSelectPokemon[(*myTrainer).whatPokeinParty(i)], gScreenSurface, &gPokemonSelectImage[i]);//blit the image of the pokemon
	}

}
#endif
