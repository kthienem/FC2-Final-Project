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

using namespace std;

const int SCREEN_WIDTH = 768;
const int SCREEN_HEIGHT = 576;

class battleScene{
	public:
		battleScene();
		bool init();//initialize display window
		bool loadMedia();//load images to be used
		void close();//free memory and delete window
		SDL_Surface* loadSurface(string path);//optimize loaded images
		void battle();
		void moveArrow(string);

	private:
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
		SDL_Rect gMovesMenu;//image for menu containing pokemons moves
		SDL_Rect gCurrentArrowPos;//current poition of selection arrow

};

battleScene::battleScene()
{
	gWindow = NULL;
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
		gWindow = SDL_CreateWindow("Battle", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);//create window with undefined position and previously given dimensions

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

	gMovesMenu.x = 297;
	gMovesMenu.y = 4;
	gMovesMenu.w = 239;
	gMovesMenu.h = 47;

	return success;
}

void battleScene::close()
{
	//Deallocate Surfaces
	SDL_FreeSurface(gBackground);
	SDL_FreeSurface(gScreenSurface);

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
      
			while(!quit){
				while(SDL_PollEvent(&e) != 0){
					if(e.type == SDL_QUIT) quit = true;//event entered was x-ing out of window, set quit to true
					else if(e.type == SDL_KEYDOWN){//key has been pressed
						if(turn%2 == 0 && inMenu){//player is in option menu and it is their turn
							switch(e.key.keysym.sym){//switch with key type parameter
								case SDLK_UP:
									moveArrow("up");
									break;
								case SDLK_DOWN:
									moveArrow("down");
									break;
								case SDLK_LEFT:
									moveArrow("left");
									break;
								case SDLK_RIGHT:
									moveArrow("right");
									break;
								default:
									break;
							}
						}
					}
				}
				SDL_BlitScaled(gBackground, NULL, gScreenSurface, &gBack);//blit background to screen
				SDL_BlitScaled(gMenuSheet, &gMenuBack, gScreenSurface, &gMenuBackWindow);//blit menu background to screen
				//Blit options if it is your turn
				if (turn%2 == 0){
					SDL_BlitScaled(gMenuSheet, &gOptions, gScreenSurface, &gOptionsWindow);//blit options menu to screen
					SDL_BlitScaled(gMenuSheet, &gSelectionArrow, gScreenSurface, &gCurrentArrowPos);//blit selection arrow onto screen
				}
				//Blit enemy level if pokemon is out
				SDL_BlitScaled(gMenuSheet, &gOpponentLevel, gScreenSurface, &gOpponentLevelWindow);//blit level and health of enemy pokemon
				//Blit player level if pokemon is out
				SDL_BlitScaled(gMenuSheet, &gPlayerLevel, gScreenSurface, &gPlayerLevelWindow);//blit level and health of player's pokemon
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
	}
}
#endif
