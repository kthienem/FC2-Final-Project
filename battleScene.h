/* This file contains functions to display the graphics needed for a battle scene in game.
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

enum Names{
	PIKACHU,
	CHARMANDER,
	BULBASAUR,
	SQUIRTLE,
	RATTATA,
	PIDGEY,
	MACHOP,
	DROWZEE,
	SNORLAX,
	ZUBAT,
	MANKEY,
	ELECTABUZZ,
	SLOWPOKE,
	PONYTA,
	ODDISH,
	ABRA,
	NAMES
};

enum PokeMoves{
	TACKLE,
	SCRATCH,
	EMBER,
	WATERGUN,
	CUT,
	SHOCK,
	KARATECHOP,
	GUST,
	HEADBUTT,
	BITE,
	BUBBLEBEAM,
	RAZORLEAF,
	WINGATTACK,
	QUICKATTACK,
	FLAMETHROWER,
	VINEWHIP,
	THUNDERBOLT,
	CONFUSION,
	PSYCHIC,
	PSYBEAM,
	LOWKICK,
	SLASH,
	PECK,
	SEISMICTOSS,
	THUNDER,
	HYDROPUMP,
	FIREBLAST,
	SOLARBEAM,
	BODYSLAM,
	SWIFT,
	MOVES
};

enum Health{
	GREEN,
	YELLOW,
	RED,
	COLORS
};

class battleScene{
	public:
		battleScene(Player*, SDL_Window*, int);
		bool init();//initialize display window
		bool loadMedia();//load images to be used
		void close();//free memory and delete window
		SDL_Surface* loadSurface(string path);//optimize loaded images
		void battle();//runs battle scene
		void moveArrow(string);//moves arrow to appropriate position
		int menuOption(string);//directs player to proper screen depending on their menu choice

	private:
		int battleType;
		Player* myTrainer;
		SDL_Window* gWindow;
		SDL_Surface* gScreenSurface;
		SDL_Surface* gBackground;
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
		SDL_Surface* gPokemonMenu;
		SDL_Rect gPokemon;//location on image for list of pokemon
		SDL_Rect gPokemonWindow;//where list of pokemon will go in window
		SDL_Rect gPokemonListSelect;//image for selected pokemon in list
		SDL_Rect gPokemonListUnselect;//image for no selected pokemon in list
		SDL_Rect gPokemonListWindow[6];//images for pokemon list
		SDL_Surface* gNames_Moves;//image file with names of pokemon and moves
		SDL_Rect gPokemonNames[NAMES];
		SDL_Rect gMoves[MOVES];
		SDL_Rect gMovesWindow[4];
		SDL_Rect gHealthBar[COLORS];
		SDL_Rect gOpponentHealth;
		SDL_Rect gPlayerHealth;
		SDL_Surface* gBattleSprites;//image with pokemon for battle and selection menu
		SDL_Rect gOpponentBattlePokemon[NAMES];
		SDL_Rect gPlayerBattlePokemon[NAMES];
		SDL_Rect gSelectPokemon[NAMES];
		SDL_Rect gOpponentPokemon;
		SDL_Rect gPlayerPokemon;

};

battleScene::battleScene(Player* myPlayer, SDL_Window* myWindow, int type)
{
	gWindow = myWindow;
	battleType = type;
	myTrainer = myPlayer;
	gScreenSurface = NULL;
	gBackground = NULL;
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
	//	gWindow = SDL_CreateWindow("Battle", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);//create window with undefined position and previously given dimensions

		if(gWindow == NULL){
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
		SDL_SetColorKey(gMenuSheet, SDL_TRUE, SDL_MapRGB(gMenuSheet->format, 255, 255, 255));
	}

	gPokemonMenu = loadSurface("Pokemon_Menu.png");//loads list of available pokemon
	if(gPokemonMenu == NULL) success = false;
	else{
		SDL_SetColorKey(gPokemonMenu, SDL_TRUE, SDL_MapRGB(gPokemonMenu->format, 0, 128, 0));
	}

	gBattleSprites = loadSurface("Pokemon.png");//loads image with different pokemon for battle
	if(gBattleSprites == NULL) success - false;
	else{
		SDL_SetColorKey(gBattleSprites, SDL_TRUE, SDL_MapRGB(gBattleSprites->format, 200, 200, 168));
		SDL_SetColorKey(gBattleSprites, SDL_TRUE, SDL_MapRGB(gBattleSprites->format, 216, 216, 216));
		SDL_SetColorKey(gBattleSprites, SDL_TRUE, SDL_MapRGB(gBattleSprites->format, 208, 208, 184));
		SDL_SetColorKey(gBattleSprites, SDL_TRUE, SDL_MapRGB(gBattleSprites->format, 96, 152, 128));
	}

	gNames_Moves = loadSurface("Names_Moves.png");//loads image with list of pokemon names and moves
	if(gNames_Moves == NULL) success = false;

	gMenuBack.x = 297;
	gMenuBack.y = 56;
	gMenuBack.w = 240;
	gMenuBack.h = 48;

	gMenuBackWindow.x = 0;
	gMenuBackWindow.y = 404;
	gMenuBackWindow.w = SCREEN_WIDTH;
	gMenuBackWindow.h = SCREEN_HEIGHT-404;

	gOptions.x = 146;
	gOptions.y = 4;
	gOptions.w = 120;
	gOptions.h = 48;

	gOptionsWindow.x = SCREEN_WIDTH/2;
	gOptionsWindow.y = 404;
	gOptionsWindow.w = SCREEN_WIDTH/2;
	gOptionsWindow.h = SCREEN_HEIGHT-404;

	gOpponentLevel.x = 3;
	gOpponentLevel.y = 3;
	gOpponentLevel.w = 100;
	gOpponentLevel.h = 29;

	//Scaled by factor of 2.175, use to scale health bar
	gOpponentLevelWindow.x = 50;
	gOpponentLevelWindow.y = 40;
	gOpponentLevelWindow.w = 300;
	gOpponentLevelWindow.h = 87;

	gPlayerLevel.x = 3;
	gPlayerLevel.y = 44;
	gPlayerLevel.w = 104;
	gPlayerLevel.h = 37;

	gPlayerLevelWindow.x = (SCREEN_WIDTH/2) + 20;
	gPlayerLevelWindow.y = 275;
	gPlayerLevelWindow.w = 350;
	gPlayerLevelWindow.h = 125;

	gSelectionArrow.x = 269;
	gSelectionArrow.y = 4;
	gSelectionArrow.w = 5;
	gSelectionArrow.h = 9;

	//fight position
	gArrowPosition[0].x = 415;
	gArrowPosition[0].y = 452;
	gArrowPosition[0].w = 14;
	gArrowPosition[0].h = 26;

	//pokemon position
	gArrowPosition[1].x = 415;
	gArrowPosition[1].y = 509;
	gArrowPosition[1].w = 14;
	gArrowPosition[1].h = 26;

	//bag position
	gArrowPosition[2].x = 595;
	gArrowPosition[2].y = 452;
	gArrowPosition[2].w = 14;
	gArrowPosition[2].h = 26;

	//run position
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

	gEmptyPokeball.x = 123;
	gEmptyPokeball.y = 65;
	gEmptyPokeball.w = 7;
	gEmptyPokeball.h = 7;

	gLivePokeball.x = 133;
	gLivePokeball.y = 65;
	gLivePokeball.w = 7;
	gLivePokeball.h = 7;

	gDeadPokeball.x = 143;
	gDeadPokeball.y = 65;
	gDeadPokeball.w = 7;
	gDeadPokeball.h = 7;

	gPokeballs.x = 178;
	gPokeballs.y = 65;
	gPokeballs.w = 104;
	gPokeballs.h = 12;
	
	gPokeballsWindow.x = (SCREEN_WIDTH/2) + 20;
	gPokeballsWindow.y = 350;
	gPokeballsWindow.w = 300;
	gPokeballsWindow.h = 20;

	gMovesMenu.x = 297;
	gMovesMenu.y = 4;
	gMovesMenu.w = 239;
	gMovesMenu.h = 47;

	gMovesMenuPos.x = 0;
	gMovesMenuPos.y = 404;
	gMovesMenuPos.w = SCREEN_WIDTH;
	gMovesMenuPos.h = SCREEN_HEIGHT - 404;

	gPokemon.x = 5;
	gPokemon.y = 5;
	gPokemon.w = 240;
	gPokemon.h = 160;

	gPokemonWindow.x = 0;
	gPokemonWindow.y = 0;
	gPokemonWindow.w = SCREEN_WIDTH;
	gPokemonWindow.h = SCREEN_HEIGHT;

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

	gMoves[KARATECHOP].x = 142;
	gMoves[KARATECHOP].y = 120;
	gMoves[KARATECHOP].w = 155;
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

	gMoves[WINGATTACK].x = 142;
	gMoves[WINGATTACK].y = 240;
	gMoves[WINGATTACK].w = 155;
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

	gHealthBar[GREEN].x = 117;
	gHealthBar[GREEN].y = 9;
	gHealthBar[GREEN].w = 1;
	gHealthBar[GREEN].h = 3;

	gHealthBar[YELLOW].x = 117;
	gHealthBar[YELLOW].y = 13;
	gHealthBar[YELLOW].w = 1;
	gHealthBar[YELLOW].h = 3;

	gHealthBar[RED].x = 117;
	gHealthBar[RED].y = 17;
	gHealthBar[RED].w = 1;
	gHealthBar[RED].h = 3;

	gOpponentHealth.x = 168;
	gOpponentHealth.y = 91;
	gOpponentHealth.w = 144;
	gOpponentHealth.h = 10;

	gPlayerHealth.x = 566;
	gPlayerHealth.y = 333;
	gPlayerHealth.w = 162;
	gPlayerHealth.h = 10;

	gPokemonListSelect.x = 162;
	gPokemonListSelect.y = 203;
	gPokemonListSelect.w = 149;
	gPokemonListSelect.h = 24;

	gPokemonListUnselect.x = 162;
	gPokemonListUnselect.y = 179;
	gPokemonListUnselect.w = 149;
	gPokemonListUnselect.h = 21;

	gPokemonListWindow[0].x = 75;
	gPokemonListWindow[0].y = 30;
	gPokemonListWindow[0].w = 650;
	gPokemonListWindow[0].h = 75;

	gPokemonListWindow[1].x = 75;
	gPokemonListWindow[1].y = 115;
	gPokemonListWindow[1].w = 650;
	gPokemonListWindow[1].h = 75;

	gPokemonListWindow[2].x = 75;
	gPokemonListWindow[2].y = 200;
	gPokemonListWindow[2].w = 650;
	gPokemonListWindow[2].h = 75;

	gPokemonListWindow[3].x = 75;
	gPokemonListWindow[3].y = 285;
	gPokemonListWindow[3].w = 650;
	gPokemonListWindow[3].h = 75;

	gPokemonListWindow[4].x = 75;
	gPokemonListWindow[4].y = 370;
	gPokemonListWindow[4].w = 650;
	gPokemonListWindow[4].h = 75;

	gPokemonListWindow[5].x = 75;
	gPokemonListWindow[5].y = 455;
	gPokemonListWindow[5].w = 650;
	gPokemonListWindow[5].h = 75;

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

	return success;
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

void battleScene::battle()
{
	int turn = 0;
	bool inMenu = true;
	bool inMoves = false;
	bool inPokemon = false;
	int selected = 0;

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
     				(*myTrainer).wild_battle(); //load wild pokemon to fight
			}
			while(!quit){
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
									int temp1 = menuOption("inMenu");
									if (temp1 == 5){
										inMenu = false;
										inMoves = true;
									}
									else if(temp1 == 6) quit = true;
									else if(temp1 == 7){
										inMenu = false;
										inPokemon = true;
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
									quit = (*myTrainer).fight(temp2);
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
									if(selected > 0) selected--;
									break;
								}
								case SDLK_DOWN:
								{
									if(selected < 5) selected++;
									break;
								}
								case SDLK_SPACE:
								{
									int temp2 = menuOption("inPokemon");
									inMenu = true;
									inPokemon = false;
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
					SDL_BlitScaled(gMenuSheet, &gHealthBar[GREEN], gScreenSurface, &gOpponentHealth);//blit opponents health to screem
					//Blit player level if pokemon is out
					SDL_BlitScaled(gMenuSheet, &gPlayerLevel, gScreenSurface, &gPlayerLevelWindow);//blit level and health of player's pokemon
					SDL_BlitScaled(gMenuSheet, &gHealthBar[GREEN], gScreenSurface, &gPlayerHealth);//blit players healt to screen
				}
				else if(inPokemon){
					SDL_BlitScaled(gPokemonMenu, &gPokemon, gScreenSurface, &gPokemonWindow);//blit background of pokemon list
					for(int i = 0; i < 6; i++){
						if(i == selected){
							SDL_BlitScaled(gPokemonMenu, &gPokemonListSelect, gScreenSurface, &gPokemonListWindow[i]);//blit first pokemon in list
						}
						else{
							SDL_BlitScaled(gPokemonMenu, &gPokemonListUnselect, gScreenSurface, &gPokemonListWindow[i]);//blit first pokemon in list
							
						}
					}
				}
				gPlayerHealth.w = 162*(*myTrainer).getmyHealth()/(*myTrainer).getmyMaxHealth();
				gOpponentHealth.w = 144*(*myTrainer).getopHealth()/(*myTrainer).getopMaxHealth();
//cout << (*myTrainer).getmyHealth() << endl;
				SDL_UpdateWindowSurface(gWindow);//update window
			}
    }
  }

}

void battleScene::moveArrow(string dir)
{
	if(dir == "up"){
		if(gCurrentArrowPos.x == gArrowPosition[0].x && gCurrentArrowPos.y == gArrowPosition[0].y){
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
{
	if(gCurrentArrowPos.x == gArrowPosition[0].x && gCurrentArrowPos.y == gArrowPosition[0].y){
		gCurrentArrowPos = gArrowPosition[4];
		return 5;
	}
	else if(gCurrentArrowPos.x == gArrowPosition[1].x && gCurrentArrowPos.y == gArrowPosition[1].y){
		return 7;
	}
	else if(gCurrentArrowPos.x == gArrowPosition[2].x && gCurrentArrowPos.y == gArrowPosition[2].y){
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
#endif
