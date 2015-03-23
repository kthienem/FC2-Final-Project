/* SDL file to test animation using a sprite sheet */

#include <iostream>
#include <SDL2/SDL.h>

using namespace std;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int WALKING_ANIMATION_FRAMES = 4;

bool init();
bool loadMedia();
void close();
SDL_Surface* loadSurface(string path);

SDL_Window* gWindow = NULL;
SDL_Surface* gScreenSurface = NULL;
SDL_Surface* gSpriteSheet = NULL;
SDL_Rect* gCurrentClip = NULL;
SDL_Surface* gBackground = NULL;
SDL_Rect gWalkLeft[WALKING_ANIMATION_FRAMES];
SDL_Rect gWalkRight[WALKING_ANIMATION_FRAMES];
SDL_Rect gWalkUp[WALKING_ANIMATION_FRAMES];
SDL_Rect gWalkDown[WALKING_ANIMATION_FRAMES];

int main()
{
	if(!init()){
		cout << "Failed to initialize!" << endl;
	}
	else{
		if(!loadMedia()){
			cout << "Failed to load media!" << endl;
		}
		else{
			bool quit = false;
			SDL_Event e;
			gCurrentClip = &gWalkDown[0];
			int frame = 0;
			SDL_Rect stretchRect1;
			stretchRect1.x = SCREEN_WIDTH/2;
			stretchRect1.y = SCREEN_HEIGHT/2;
			stretchRect1.w = 30;
			stretchRect1.h = 30;
			SDL_Rect stretchRect2;
			stretchRect2.x = 0;
			stretchRect2.y = 0;
			stretchRect2.w = SCREEN_WIDTH;
			stretchRect2.h = SCREEN_HEIGHT;

			while(!quit){
				while(SDL_PollEvent(&e) != 0){
					if (e.type == SDL_QUIT){
						quit = true;
					}
					else if(e.type == SDL_KEYDOWN){
						switch(e.key.keysym.sym)
						{
							case SDLK_UP:
								gCurrentClip = &gWalkUp[frame/4];
								stretchRect1.y--;
								break;
							
							case SDLK_DOWN:
								gCurrentClip = &gWalkDown[frame/4];
								stretchRect1.y++;
								break;

							case SDLK_LEFT:
								gCurrentClip = &gWalkLeft[frame/4];
								stretchRect1.x--;
								break;

							case SDLK_RIGHT:
								gCurrentClip = &gWalkRight[frame/4];
								stretchRect1.x++;
								break;
				
							default:
								break;
						}
					}
				}
//				SDL_Rect* currentClip = &gSpriteClips[frame/4];

//				SDL_BlitSurface(gSpriteSheet, currentClip, gScreenSurface, NULL);
//				SDL_UpdateWindowSurface(gWindow);

				SDL_BlitScaled(gBackground, NULL, gScreenSurface, &stretchRect2);
				SDL_BlitSurface(gSpriteSheet, gCurrentClip, gScreenSurface, &stretchRect1);

				SDL_UpdateWindowSurface(gWindow);
				frame++;
				if (frame/4 >= WALKING_ANIMATION_FRAMES){
					frame = 0;
				}
			}
		}
	}
}

bool init()
{
	bool success = true;

	if(SDL_Init(SDL_INIT_VIDEO) < 0){
		cout << "SDL could not initialize! SDL Error: " << SDL_GetError() << endl;
		success = false;
	}
	else{
		gWindow = SDL_CreateWindow("Animation", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

		if(gWindow == NULL){
			cout << "Window could not be created! SDL Error: " << SDL_GetError() << endl;
			success = false;
		}
		else{
			gScreenSurface = SDL_GetWindowSurface(gWindow);
		}
	}

	return success;
}

bool loadMedia()
{
	bool success = true;

	gBackground = loadSurface("background.bmp");
	if(gBackground == NULL){
		success = false;
	}

	gSpriteSheet = loadSurface("PokemonPlayer.bmp");
	if(gSpriteSheet == NULL){
		success = false;
	}
	else{
		SDL_SetColorKey(gSpriteSheet, SDL_TRUE, SDL_MapRGB(gSpriteSheet->format, 136, 184, 176));

		//Set Walking down locations
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

		//Set walking up locations
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

		//Set walking left locations
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

		//Set walking right locations
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
//	for(int i = 0; i < WALKING_ANIMATION_FRAMES; i++){
//		SDL_FreeSurface(gSpriteClips[i]);
//		gSpritClips[i] = NULL;
//	}

	SDL_DestroyWindow(gWindow);
	gWindow = NULL;

	SDL_Quit();
}

SDL_Surface* loadSurface(string path)
{
	SDL_Surface* optimizedSurface = NULL;

	SDL_Surface* loadedSurface = SDL_LoadBMP(path.c_str());
	if(loadedSurface == NULL){
		cout << "Unable to load image " << path.c_str() << "! SDL Error: " << SDL_GetError() << endl;
	}
	else{
		optimizedSurface = SDL_ConvertSurface(loadedSurface, gScreenSurface->format, NULL);
		if(optimizedSurface == NULL){
			cout << "Unable to optimize image! SDL Error: " << SDL_GetError() << endl;
		}

		SDL_FreeSurface(loadedSurface);
	}

	return optimizedSurface;
}
