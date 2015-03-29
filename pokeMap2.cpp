/*This source code copyrighted by Lazy Foo' Productions (2004-2015)
and may not be redistributed without written permission.*/

//Using SDL, standard IO, and strings
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
//#include <stdio.h>
//#include <string>
using namespace std;

//Screen dimension constants
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

//Map surface constants
enum pokeSprites
{
	POKE_SPRITE_ROUTE1,	//=0
//	POKE_SPRITE_PALLET,	//=1
	POKE_SPRITE_SIZE	//=number of variables above. used to index gPokeSprites
};

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//Loads individual image
SDL_Surface* loadSurface( string path );

//The window we'll be rendering to
SDL_Window* gWindow = NULL;
	
//The surface contained by the window
SDL_Surface* gScreenSurface = NULL;

//Array of pointers to SDL surfaces called pokeSprites that contains all maps we are using
SDL_Surface* gPokeSprites[ POKE_SPRITE_SIZE ];

//Current displayed image
SDL_Surface* gStretchedSurface = NULL;

bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Create window
		gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Initialize map loading
			int imgFlags = IMG_INIT_PNG;
			if( !( IMG_Init( imgFlags ) & imgFlags ) )
			{
				cout << "SDL_image could not initialize! SDL_image error: " << IMG_GetError()<<endl;
				success = false;
			}
			else
			{
				//Get window surface
				gScreenSurface = SDL_GetWindowSurface( gWindow );
			}
		}
	}

	return success;
}

bool loadMedia()
{
	//Loading success flag
	bool success = true;

	//Load stretching surface
	gPokeSprites[ POKE_SPRITE_ROUTE1 ] = loadSurface( "pokeMap2.png" );
	if( gPokeSprites[ POKE_SPRITE_ROUTE1 ] == NULL )
	{
		printf( "Failed to load Route1 image!\n" );
		success = false;
	}
/*
	gPokeSprites[ POKE_SPRITE_PALLET ] = loadSurface( "PalletTown.bmp" );
	if( gPokeSprites[ POKE_SPRITE_PALLET ] == NULL )
	{
		printf( "Failed to load Pallet image!\n" );
		success = false;
	}
*/
	return success;
}

void close()
{
	//Deallocate surfaces
	for (int i=0; i<POKE_SPRITE_SIZE; i++)
	{
		SDL_FreeSurface( gPokeSprites[ i ] );
		gPokeSprites[ i ] = NULL;
	}

	//Destroy window
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;

	//Quit SDL subsystems
	SDL_Quit();
}

// USE A FXN LIKE THIS TO UPLOAD SURFACES SO THEN THE SURFACES CAN BE OPTIMIZED. 
// we might need different ones for full screen vs not then
SDL_Surface* loadSurface( string path )
{
	//The final optimized image
	SDL_Surface* optimizedSurface = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
	if( loadedSurface == NULL )
	{
		printf( "Unable to load image %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
	}
	else
	{
		//Convert surface to screen format
		optimizedSurface = SDL_ConvertSurface( loadedSurface, gScreenSurface->format, 0 );
		if( optimizedSurface == NULL )
		{
			printf( "Unable to optimize image %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
		}

		//Get rid of old loaded surface
		SDL_FreeSurface( loadedSurface );
	}

	return optimizedSurface;
}

int main( int argc, char* args[] )
{
	int x=0, y=0;
	//Start up SDL and create window
	if( !init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
		//Load media
		if( !loadMedia() )
		{
			printf( "Failed to load media!\n" );
		}
		else
		{	
			//Main loop flag
			bool quit = false;

			//Event handler
			SDL_Event e;

			//While application is running
			while( !quit )
			{
				//Handle events on queue
				while( SDL_PollEvent( &e ) != 0 )
				{
					//User requests quit
					if( e.type == SDL_QUIT )
					{
						quit = true;
					}else if( e.type == SDL_KEYDOWN )
					{
						switch( e.key.keysym.sym )
						{
							case SDLK_UP:
								y-=15;
								break;
							case SDLK_DOWN:
								y+=15;
								break;
							case SDLK_LEFT:
								x-=15;
								break;
							case SDLK_RIGHT:
								x+=15;
								break;
						}
					}
				}

//				if(x>75 && x<105 && y<15) gStretchedSurface=gPokeSprites[ POKE_SPRITE_PALLET ];
				gStretchedSurface=gPokeSprites[ POKE_SPRITE_ROUTE1 ];
//cout << "x: " << x << endl << "y: " << y << endl;

				//Apply the image stretched
				SDL_Rect stretchRect, stretchRect2;
				stretchRect.x = 0;
				stretchRect.y = 0;
				stretchRect.w = SCREEN_WIDTH;
				stretchRect.h = SCREEN_HEIGHT;
				stretchRect2.x = x;
				stretchRect2.y = y;
				stretchRect2.w = 300;
				stretchRect2.h = 300;
//WE SHOULD BE ABLE TO USE THE SECOND ARGUMENT OF SDL_BLITSCALED() TO MAKE A SCROLLING MAP
				SDL_BlitScaled( gStretchedSurface, &stretchRect2, gScreenSurface, &stretchRect );
			
				//Update the surface
				SDL_UpdateWindowSurface( gWindow );
			}
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}
