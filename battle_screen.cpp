/* This file provides a graphical interface for the battle scenes in the game
 *
 */

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <ctime>

using namespace std;

const int SCREEN_WIDTH = 768;
const int SCREEN_HEIGHT = 576;

bool init();//initialize display window
bool loadMedia();//load images to be used
void close();//free memory and delete window
SDL_Surface* loadSurface(string path);//optimize loaded images

SDL_Window* gWindow = NULL;//pointer to the images displayed on the screen
SDL_Surface* gScreenSurface = NULL;//pointer to the surface containing the images to be displayed
SDL_Surface* gBackground = NULL;//pointer to the image containing battle backgrounds
SDL_Surface* gBattleMenu = NULL;//pointer to image containing battle menus
SDL_Surface* gPlayerBack = NULL;//pointer to image containing of trainers back

int main()
{
	if(!inti()){//initializes window, if it fails display error message
		cout << "Failed to initialize!" << endl;
	}
	else{//if window is initialized successfully
		if(!loadMedia()){//load the images, if it fails display error message
			cout << "Failed to load media!" << endl;
		}
		else{//if media is successfully loaded begin displaying images
			bool quit = false;//boolean variable for when the user want to quit
			SDL_Event e;//variable for keyboard events entered by user
			
		}
	}
}

bool init()
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
			init imgFlags = IMG_INIT_PNG;
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

bool loadMedia()
{
	//loads images
	bool success = true;

	gBackground = loadSurface("Battle_BackGrounds.png");//load battle screen background image
	if(gBackGround == NULL) success = false;//if not loaded properly return unsuccessful

	
}
