/* Kristopher Thieneman
 * Lab 7/8
 *
 * This file uses SDL graphics to work on the animation of a character walking
 * across the screen. Two bmp files are loaded in, one containing the images 
 * needed to create the animation and a background image used to test that 
 * unwanted portions of the animation image are made transparent to the 
 * background.
 */

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <unistd.h>
#include <ctime>
using namespace std;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int WALKING_ANIMATION_FRAMES = 4;//number of frames used to create animation

//Map surface constants
enum pokeMaps
{
	POKE_MAP_ROUTE1,	//=0
	POKE_MAP_SIZE	//=number of variables above. used to index gPokeMaps
};

bool init();//initialize the display window
bool loadMedia();//load the images to be used
void close();//free memory and delete the window
SDL_Surface* loadSurface(string path);//optimize the loaded images

SDL_Window* gWindow = NULL;//pointer to the images displayed on the screen
SDL_Surface* gScreenSurface = NULL;//pointer to the surface containing the images to be displayed
SDL_Surface* gSpriteSheet = NULL;//pointer to the image contatining the pieces needed for the animation
SDL_Rect* gCurrentClip = NULL;//pointer to the current portion of gSpriteSheet being dislayed

SDL_Surface* gPokeMaps[ POKE_MAP_SIZE ];
SDL_Surface* gBackground = NULL;//pointer to the image that is used as the background

SDL_Rect gWalkLeft[WALKING_ANIMATION_FRAMES];//series of frames from gSpriteSheet to animate walking left
SDL_Rect gWalkRight[WALKING_ANIMATION_FRAMES];//animate walking right
SDL_Rect gWalkUp[WALKING_ANIMATION_FRAMES];//animate walking up the screen
SDL_Rect gWalkDown[WALKING_ANIMATION_FRAMES];//animate walking down the screen

int main()
{
	if(!init()){//initializes the window, if it fails display error message
		cout << "Failed to initialize!" << endl;
	}
	else{//if window is initialized successfully
		if(!loadMedia()){//load the images needed, if it fails display error message
			cout << "Failed to load media!" << endl;
		}
		else{//if media is successfully loaded begin displaying images
			bool quit = false;//boolean variable for when the user wants to quit
			SDL_Event e;//variable for keyboard events entered by user
			gCurrentClip = &gWalkDown[0];//sets first image to be used for the walking player
			int frame = 0;//count for the current frame being used for the animation
			SDL_Rect stretchRect1;//rectangle used for where to place the character
			stretchRect1.x = SCREEN_WIDTH/2 - 9;//place at center of screen
			stretchRect1.y = SCREEN_HEIGHT/2 + 14;//place at center of screen
			stretchRect1.w = 55;//size of portion of sprite sheet taken up by character
			stretchRect1.h = 55;//size of portion of sprite sheet taken up by character
			SDL_Rect stretchRect2;//rectangle to stretch the background image to fit to window
			stretchRect2.x = 0;//begin image at top corner of winow
			stretchRect2.y = 0;
			stretchRect2.w = SCREEN_WIDTH;//make image take up the entire window
			stretchRect2.h = SCREEN_HEIGHT;
			SDL_Rect stretchRect3;	// rectangle that zooms in on current viewable map
			stretchRect3.x = 1;//begin image at top corner of winow
			stretchRect3.y = 1;
			stretchRect3.w = 384;
			stretchRect3.h = 288;

			while(!quit){//while the user has not entered the event to quit
				while(SDL_PollEvent(&e) != 0){//while there remains user entered events
					if (e.type == SDL_QUIT){//entered event is equal to pressing red x in top right hand corner of window
						quit = true;//set quit to true to exit the while loop
					}
					else if(e.type == SDL_KEYDOWN){//a key has been pressed
				//	gPrevClip = gCurrentClip;
						switch(e.key.keysym.sym)//switch with key type parameter
						{
							case SDLK_UP://up arrow key
								frame=0;
								for( int i=0; i<16; i++){
		//use (16-i)/4 ? so that we end at zero
									if(i%4==0) frame++;
									if(frame>3) frame=0;
									gCurrentClip = &gWalkUp[frame];//set equal to proper frame of walking right sprites
									stretchRect3.y--;
									usleep(30000);
									SDL_BlitScaled(gBackground, &stretchRect3, gScreenSurface, &stretchRect2);//put the background image onto gScreenSurface
									SDL_BlitScaled(gSpriteSheet, gCurrentClip, gScreenSurface, &stretchRect1);//put the character image onto gScreenSurface
									SDL_UpdateWindowSurface(gWindow);//update the window with the current surface
								}
								break;
							
							case SDLK_DOWN://down arrow key
								frame=0;
								for( int i=0; i<16; i++){
									if(i%4==0) frame++;
									if(frame>3) frame=0;
									gCurrentClip = &gWalkDown[frame];//set equal to proper frame of walking right sprites
									stretchRect3.y++;
									usleep(30000);
									SDL_BlitScaled(gBackground, &stretchRect3, gScreenSurface, &stretchRect2);//put the background image onto gScreenSurface
									SDL_BlitScaled(gSpriteSheet, gCurrentClip, gScreenSurface, &stretchRect1);//put the character image onto gScreenSurface
									SDL_UpdateWindowSurface(gWindow);//update the window with the current surface
								}
								break;

							case SDLK_LEFT://left arrow key
								frame=0;
								for( int i=0; i<16; i++){
									if(i%4==0) frame++;
									if(frame>3) frame=0;
									gCurrentClip = &gWalkLeft[frame];//set equal to proper frame of walking right sprites
									stretchRect3.x--;
									usleep(30000);
									SDL_BlitScaled(gBackground, &stretchRect3, gScreenSurface, &stretchRect2);//put the background image onto gScreenSurface
									SDL_BlitScaled(gSpriteSheet, gCurrentClip, gScreenSurface, &stretchRect1);//put the character image onto gScreenSurface
									SDL_UpdateWindowSurface(gWindow);//update the window with the current surface
								}
								break;

							case SDLK_RIGHT://right arrow key
								frame=0;
								for( int i=0; i<16; i++){
									if(i%4==0) frame++;
									if(frame>3) frame=0;
									gCurrentClip = &gWalkRight[frame];//set equal to proper frame of walking right sprites
									stretchRect3.x++;
									usleep(30000);
									SDL_BlitScaled(gBackground, &stretchRect3, gScreenSurface, &stretchRect2);//put the background image onto gScreenSurface
									SDL_BlitScaled(gSpriteSheet, gCurrentClip, gScreenSurface, &stretchRect1);//put the character image onto gScreenSurface
									SDL_UpdateWindowSurface(gWindow);//update the window with the current surface
								}
								//cout<<"x: "<<stretchRect3.x<<"\ny: "<<stretchRect3.y<<endl<<endl;
								break;
				
							default://do nothing when any other keys are pressed
								break;
						}
					}
					gBackground = gPokeMaps[ POKE_MAP_ROUTE1 ];

					SDL_BlitScaled(gBackground, &stretchRect3, gScreenSurface, &stretchRect2);//put the background image onto gScreenSurface
					SDL_BlitScaled(gSpriteSheet, gCurrentClip, gScreenSurface, &stretchRect1);//put the character image onto gScreenSurface

					SDL_UpdateWindowSurface(gWindow);//update the window with the current surface
					frame++;//increment frame
					if (frame == WALKING_ANIMATION_FRAMES){//set frames to zero when number of walking animation frames is reached to cycle through images
						frame = 0;
					}
/*					while( SDL_FIRSTEVENT != SDL_QUIT ) {
						if( SDL_FIRSTEVENT == SDL_LASTEVENT || SDL_FIRSTEVENT == NULL ){
							SDL_FlushEvent(SDL_FIRSTEVENT);
							break;
						}
						SDL_FlushEvent(SDL_FIRSTEVENT);
					}
*/				}
			}
		}
	}
	close();
}


bool init()
{
	//creates window
	bool success = true;//lets main know if window was successfully loaded

	if(SDL_Init(SDL_INIT_VIDEO) < 0){//Error if SDL could not initialize
		cout << "SDL could not initialize! SDL Error: " << SDL_GetError() << endl;
		success = false;//lets main know SDL could not initialize
	}
	else{
		gWindow = SDL_CreateWindow("Animation", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);//create window with undefined position and previously given dimensions

		if(gWindow == NULL){//if window points to NULL error
			cout << "Window could not be created! SDL Error: " << SDL_GetError() << endl;
			success = false;
		}
		else{
			//Initialize Map Loading
			int imgFlags = IMG_INIT_PNG;
			if( !( IMG_Init( imgFlags ) & imgFlags ) )
			{
				cout<<"SDL_image could not initialize! SDL_image error: "<<IMG_GetError()<<endl;
				success = false;
			}
			else
			{
				gScreenSurface = SDL_GetWindowSurface(gWindow);
			}
		}
	}

	return success;//return whether function was successful or not
}

bool loadMedia()
{
	//loads images and creates walking animation clips
	bool success = true;//function was successful

	gPokeMaps[ POKE_MAP_ROUTE1 ] = loadSurface("pokeMap2.png");//load background image
	if(gPokeMaps[ POKE_MAP_ROUTE1 ] == NULL){//if not loaded properly return unsuccessful 
		success = false;
	}

	gSpriteSheet = loadSurface("PokemonPlayer.bmp");//load sprite sheet containing animation images
	if(gSpriteSheet == NULL){//if not loaded properly return unsuccessful
		success = false;
	}
	else{
		SDL_SetColorKey(gSpriteSheet, SDL_TRUE, SDL_MapRGB(gSpriteSheet->format, 136, 184, 176));//the color given in SDL_MapRGB one the sprite sheet will be made transparent to background image

		//Set Walking down clips
		gWalkDown[0].x = 2;
		gWalkDown[0].y = 2;
		gWalkDown[0].w = 32;
		gWalkDown[0].h = 32;

		gWalkDown[1].x = 36;
		gWalkDown[1].y = 2;
		gWalkDown[1].w = 32;
		gWalkDown[1].h = 32;

		gWalkDown[2].x = 70;
		gWalkDown[2].y = 2;
		gWalkDown[2].w = 32;
		gWalkDown[2].h = 32;

		gWalkDown[3].x = 104;
		gWalkDown[3].y = 2;
		gWalkDown[3].w = 32;
		gWalkDown[3].h = 32;

		//Set walking up clips
		gWalkUp[0].x = 2;
		gWalkUp[0].y = 36;
		gWalkUp[0].w = 32;
		gWalkUp[0].h = 32;
		
		gWalkUp[1].x = 36;
		gWalkUp[1].y = 36;
		gWalkUp[1].w = 32;
		gWalkUp[1].h = 32;
	
		gWalkUp[2].x = 70;
		gWalkUp[2].y = 36;
		gWalkUp[2].w = 32;
		gWalkUp[2].h = 32;
	
		gWalkUp[3].x = 104;
		gWalkUp[3].y = 36;
		gWalkUp[3].w = 32;
		gWalkUp[3].h = 32;

		//Set walking left clips
		gWalkLeft[0].x = 2;
		gWalkLeft[0].y = 70;
		gWalkLeft[0].w = 32;
		gWalkLeft[0].h = 32;

		gWalkLeft[1].x = 36;
		gWalkLeft[1].y = 70;
		gWalkLeft[1].w = 32;
		gWalkLeft[1].h = 32;

		gWalkLeft[2].x = 70;
		gWalkLeft[2].y = 70;
		gWalkLeft[2].w = 32;
		gWalkLeft[2].h = 32;

		gWalkLeft[3].x = 104;
		gWalkLeft[3].y = 70;
		gWalkLeft[3].w = 32;
		gWalkLeft[3].h = 32;

		//Set walking right clips
		gWalkRight[0].x = 2;
		gWalkRight[0].y = 104;
		gWalkRight[0].w = 32;
		gWalkRight[0].h = 32;

		gWalkRight[1].x = 36;
		gWalkRight[1].y = 104;
		gWalkRight[1].w = 32;
		gWalkRight[1].h = 32;

		gWalkRight[2].x = 70;
		gWalkRight[2].y = 104;
		gWalkRight[2].w = 32;
		gWalkRight[2].h = 32;

		gWalkRight[3].x = 104;
		gWalkRight[3].y = 104;
		gWalkRight[3].w = 32;
		gWalkRight[3].h = 32;
	}

	return success;
}

void close()
{
	//Deallocate Surfaces
	for (int i=0; i<POKE_MAP_SIZE; i++) {
		SDL_FreeSurface( gPokeMaps[ i ] );
		gPokeMaps[ i ]=NULL;
	}

	SDL_DestroyWindow(gWindow);//destroys the window
	gWindow = NULL;//points window pointer to NULL just incase

	SDL_Quit();//cleans up initialized subsystems
}

SDL_Surface* loadSurface(string path)
{
	//optimizes the loaded image
	SDL_Surface* optimizedSurface = NULL;//pointer to optimized image

	SDL_Surface* loadedSurface = IMG_Load(path.c_str());//loads image
	if(loadedSurface == NULL){//Error if image could not load properly
		cout << "Unable to load image " << path.c_str() << "! SDL Error: " << SDL_GetError() << endl;
	}
	else{
		optimizedSurface = SDL_ConvertSurface(loadedSurface, gScreenSurface->format, NULL);//convert image to optimized form
		if(optimizedSurface == NULL){//Error if issue optimizing
			cout << "Unable to optimize image! SDL Error: " << SDL_GetError() << endl;
		}

		SDL_FreeSurface(loadedSurface);//Free the memory that held the initil image
	}

	return optimizedSurface;//returns the optimized surface
}
