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
      
			while(!quit){
				while(SDL_PollEvent(&e) != 0){
					if(e.type == SDL_QUIT) quit = true;//event entered was x-ing out of window, set quit to true
				}
				SDL_BlitScaled(gBackground, NULL, gScreenSurface, &gBack);//blit background to screen
				SDL_BlitScaled(gMenuSheet, &gMenuBack, gScreenSurface, &gMenuBackWindow);//blit menu background to screen
				//Blit options if it is your turn
				SDL_BlitScaled(gMenuSheet, &gOptions, gScreenSurface, &gOptionsWindow);//blit options menu to screen
				//Blit enemy level if pokemon is out
				SDL_BlitScaled(gMenuSheet, &gOpponentLevel, gScreenSurface, &gOpponentLevelWindow);//blit level and health of enemy pokemon
				//Blit player level if pokemon is out
				SDL_BlitScaled(gMenuSheet, &gPlayerLevel, gScreenSurface, &gPlayerLevelWindow);//blit level and health of player's pokemon
				SDL_UpdateWindowSurface(gWindow);//update window
			}
    }
  }

}
#endif
