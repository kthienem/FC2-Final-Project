/* Patrick Drumm, Kristopher Thieneman
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
#include <vector>
#include <fstream>
#include <map>
#include <cmath>

#include "Player.h"

using namespace std;

//screen is exactly twice the stretching size of the zoomed in map so that we can match coordinates of screen with pixels of surface
const int SCREEN_WIDTH = 768;
const int SCREEN_HEIGHT = 576;

const int WALKING_ANIMATION_FRAMES = 4;//number of frames used to create animation

//Map surface constants
enum pokeMaps
{
	POKE_MAP_ROUTE1,	//=0
	POKE_CENTER_MAP,
	POKE_CENTER_YES_MAP,
	POKE_CENTER_NO_MAP,
	POKE_CENTER_HEALING_MAP,
	POKE_GYM_MAP,
	POKE_GYM_ANSWERS_MAP,
	POKE_MESSAGE,
	POKE_MESSAGE_1,
	POKE_MESSAGE_2,
	POKE_MESSAGE_3,
	POKE_MESSAGE_4,
	POKE_MESSAGE_5,
	POKE_MESSAGE_6a,
	POKE_MESSAGE_6b,
	POKE_MESSAGE_CAVE,
	POKE_MESSAGE_MART,
	POKE_MESSAGE_START,
	POKE_MESSAGE_CANT_PAUSE,
	POKE_MENU_MAIN,
	POKE_MAP_SIZE	//=number of variables above. used to index gPokeMaps
};
//Color code contents
enum cellColorCode
{
	BRIDGE_CELL,
	CAVE_ENT_CELL,
	FLOWER_CELL,
	NORM_GRASS_CELL,
	PATH_EDGE_CELL,
	PATH_CELL,
	PLATEAU_CELL,
	PLATFORM_CELL,
	POKEBALL_CELL,
	POKE_CENTER_CELL,
	SKINNYPATH_CELL,
	STAIRS_CELL,
	WATER_CELL,
	WILD_GRASS_2_CELL,
	WILD_GRASS_CELL,
	CENTER_BOX_CELL,
	CENTER_CUSHION1_CELL,
	CENTER_CUSHION2_CELL,
	CENTER_DOOR2_CELL,
	CENTER_DOOR_CELL,
	CENTER_EXIT_CELL,
	CENTER_FLOOR1_CELL,
	CENTER_FLOOR2_CELL,
	CENTER_FLOOR3_CELL,
	CENTER_HEALER_CELL,
	GYM_DOOR_CELL,
	GYM_FLOOR1_CELL,
	GYM_FLOOR_EXIT_CELL,
	GYM_FLOOR_SHADOW_CELL,
	GYM_FLOOR_WARP_CELL,
	GYM_LEADER_CELL,
	GYM_PILLAR_CELL,
	DOOR_BLUE_N_CELL,
	DOOR_BLUE_SE_CELL,
	DOOR_GREEN_S_CELL,
	DOOR_RED_NW_CELL,
	DOOR_RED_S_CELL,
	DOOR_WOOD_CELL,
	DOOR_MART_CELL,
	TRAINER_BRUNETTE_CELL,
	TRAINER_BRUNETTE2_CELL,
	TRAINER_FISHERMAN_CELL,
	TRAINER_RANGER_CELL,
	TRAINER_SAFARI_CELL,
	TRAINER_YELLOW_CELL,
	COLOR_CELL_SIZE
};

double cellColorThreshold[50]={	.55,	// bridge
				.63,	// cave entrance
				.55,	// flower
				.62,	// normal grass
				.40,	// path edge
				.50,	// path
				.49,	// plateau
				.62,	// platform
				.22,	// pokeball
				.87,	// poke center
				.39,	// skinnypath
				.90,	// stairs
				.25,	// water
				.60,	// wild grass 2
				.60,	// wild grass
				.8,	// center box
				.8,	// center cushion1
				.8,	// center cushion2
				.8,	// center door2
				.8,	// center door
				.9,	// center exit
				.6,	// center floor1
				.6,	// center floor2
				.6,	// center floor3
				.9,	// center healer
				.99,	// gym door
				.70,	// gym floor 1
				.95,	// gym exit
				.60,	// gym floor shadow(with wall sticking out)
				.70,	// gym warp
				.99,	// gym leader
				.90,	// gym pillar
				.90,	// door blue N
				.90,	// door blue SE
				.90,	// door green S
				.90,	// door red NW
				.80,	// door red S
				.90,	// door wood
				.85,	// pokeMart door
				.90,	// trainer brunette
				.70,	// trainer brunette2
				.85,	// trainer fisherman
				.79,	// trainer ranger
				.64,	// trainer safari hat
				.63	// trainer yellow cap
};

bool init();		//initialize the display window
bool loadMedia();	//load the images to be used
void close();		//free memory and delete the window
SDL_Surface* loadSurface(string path);//optimize the loaded images

void putpixel( int, int, int );			// inputs: x,y,color
uint32_t getpixel( int, int, SDL_Surface*);	// output: color; inputs: x,y,surface
double cellComp( int,int,map<int,int>,SDL_Surface* );// output: %match; inputs: x1,y1,cellColormap,surface
void writeColorCodes(int,int,SDL_Surface*);
vector<map<int,int> > readColorCodes();

void transitionGraphic(SDL_Window *, SDL_Surface *, SDL_Surface*, SDL_Surface*, SDL_Rect*, SDL_Rect,SDL_Rect,SDL_Rect, int, int);
void healMyPokemon(SDL_Window *, SDL_Surface *, SDL_Surface*, SDL_Surface*, SDL_Rect*, SDL_Rect,SDL_Rect,SDL_Rect,Player*);
SDL_Rect determineWarpLoc(SDL_Rect);
void talkToPillar(SDL_Rect,int *,int *);
void dispMessage(SDL_Rect, SDL_Rect, int, int, int, int);
bool pauseMenu(SDL_Rect, SDL_Rect, SDL_Rect);
void saveGame(SDL_Rect, SDL_Rect);
void loadGame(SDL_Rect**, SDL_Rect*, SDL_Rect*);
void battleCutScene(SDL_Rect, SDL_Rect, SDL_Rect);
void introSequence(SDL_Rect);
//int goFish();

SDL_Window* gWindow = NULL;		//pointer to the images displayed on the screen
SDL_Surface* gScreenSurface = NULL;	//pointer to the surface containing the images to be displayed
SDL_Surface* gSpriteSheet = NULL;	//pointer to the image contatining the pieces needed for the animation
SDL_Rect* gCurrentClip = NULL;		//pointer to the current portion of gSpriteSheet being dislayed

//Intro stuff
SDL_Surface* gIntroSprites = NULL;	//pointer to the image contatining the pieces needed for the animation
SDL_Rect gTorchic[6];
SDL_Surface* gIntroBackground = NULL;
SDL_Rect gIntroTrees;
SDL_Surface* gIntroTitleSprites = NULL;	//pointer to the image contatining the pieces needed for the animation
SDL_Rect gPokeTitle[6];


SDL_Surface* gPokeMaps[ POKE_MAP_SIZE ];
SDL_Surface* gBackground = NULL;	//pointer to the image that is used as the background

SDL_Rect gWalkLeft[WALKING_ANIMATION_FRAMES];	//series of frames from gSpriteSheet to animate walking left
SDL_Rect gWalkRight[WALKING_ANIMATION_FRAMES];	//animate walking right
SDL_Rect gWalkUp[WALKING_ANIMATION_FRAMES];	//animate walking up the screen
SDL_Rect gWalkDown[WALKING_ANIMATION_FRAMES];	//animate walking down the screen

int main()
{
	srand(time(NULL));
	if(!init()){//initializes the window, if it fails display error message
		cout << "Failed to initialize!" << endl;
	}
	else{//if window is initialized successfully
		if(!loadMedia()){//load the images needed, if it fails display error message
			cout << "Failed to load media!" << endl;
		}
		else{//if media is successfully loaded begin displaying images
//LOADINGSEQUENCE HERE - RETURNS NEW GAME
			int newGame=0;
			Player Nick;



			bool quit = false;		//boolean variable for when the user wants to quit
			SDL_Event e;			//variable for keyboard events entered by user
			gCurrentClip = &gWalkDown[0];	//sets first image to be used for the walking player
			int frame = 0;			//count for the current frame being used for the animation

			SDL_Rect characterRect;			//rectangle used for where to place the character
			characterRect.x = SCREEN_WIDTH/2 - 9;	//place at center of screen
			characterRect.y = SCREEN_HEIGHT/2 + 14;	//place at center of screen
			characterRect.w = 50;			//size of portion of sprite sheet taken up by character
			characterRect.h = 50;			//size of portion of sprite sheet taken up by character


			SDL_Rect stretch2windowRect;			//rectangle to stretch the background image to fit to window
			stretch2windowRect.x = 0;			//begin image at top corner of winow
			stretch2windowRect.y = 0;
			stretch2windowRect.w = SCREEN_WIDTH;		//make image take up the entire window
			stretch2windowRect.h = SCREEN_HEIGHT;

			SDL_Rect mapZoomRect;			// rectangle that zooms in on current viewable map
		// x,y must always be x=1+16a, y=1+16b in order to stay alligned with map cells
//			mapZoomRect.x = 1185;			//begin image in middle of map
//			mapZoomRect.y = 17;
//			mapZoomRect.x = 1777;			//begin image in middle of map
//			mapZoomRect.y = 625;
			mapZoomRect.x = 705;			//begin image in middle of map
			mapZoomRect.y = 257;
			mapZoomRect.w = 384;
			mapZoomRect.h = 288;
//PokeCenter Testing...
		characterRect.x = 599;			//begin image in middle of map
		characterRect.y = 462;
		mapZoomRect.x = 1841;			//begin image in middle of map
		mapZoomRect.y = 721;

			int caveChoice=0;
			int caveMapX[4]= {	1777,	1761,	1809,	1761};
			int caveMapY[4]= {	609,	721,	545,	561};
			int caveCharX[4]= {	343,	343,	375,	279};
			int caveCharY[4]= {	302,	334,	302,	302};

			int sleeptime=500;	// sleeptime*16 == time for trainer to take one step
			int warptime=15000;	// sleeptime*16 == time for trainer to take one step
			int canWalk=1;		// boolean - false if trainer attempts to walk into a tree, wall, etc
			int white= 16777215;	// return of getpixel for white background
			int whiteCount=0;	// counter
			int isOB=0;		// boolean - true if white space or edge of map image is just outside of window border
			// keys for edge pixels of map
			int leftEdge= 1;
			int topEdge= 1;
			int rightEdge= 2240;
			int botEdge= 1168;

			int cellShift= 32;
			int leaveMap1x= 0;
			int leaveMap1y= 0;
			int leaveMap3x= 0;
			int leaveMap3y= 0;

			int steponWildGrass= 0;
			int isCave= 0;
			int isPokeCenter= 0;
			int enteringBuilding= 0;
			int exitingBuilding= 0;
			int isHealer= 0;
			int isGym= 0;
			int isWarpTile= 0;

			int gymAnswers=0;
			int firstPillarx=0;
			

			int transitionSpeed= 7;

			vector<map<int,int> > colorCodes;

			colorCodes= readColorCodes();


			if(!newGame){
				loadGame(&gCurrentClip, &characterRect, &mapZoomRect);
			}

			int trainerCellx= characterRect.x+7;
			int trainerCelly= characterRect.y+16;

	//		introSequence(stretch2windowRect);

			while(!quit){//while the user has not entered the event to quit
				while(SDL_PollEvent(&e) != 0){//while there remains user entered events
					if (e.type == SDL_QUIT){//entered event is equal to pressing red x in top right hand corner of window
						quit = true;//set quit to true to exit the while loop
					}
					else if(e.type == SDL_KEYDOWN){//a key has been pressed
				//	gPrevClip = gCurrentClip;
						switch(e.key.keysym.sym)//switch with key type parameter
						{
							//frame is reset to zero after the switch. Thus each for loop will loop through the stages of walking sprites as gWalkx[1,2,3,0] in that order. Since gCurrentClip will end in gWalkx[0], we can check the direction the trainer is facing by seeing if the gCurrentClip == gWalkx[0]
							case SDLK_UP://up arrow key
								if( gCurrentClip!=&gWalkUp[0] ) {	// ie. if he is NOT already walking up, change the direction he is facing to up
									gCurrentClip= &gWalkUp[0];
								}else if(mapZoomRect.x==1 && mapZoomRect.y==1 && characterRect.y==14 && (characterRect.x==247 || characterRect.x==279)){
														dispMessage(characterRect, mapZoomRect, isCave, 0, 1, 0);
								} else {	// else, make him continue walking up
									if( mapZoomRect.y-15 < topEdge ) isOB=1;
									if( isOB==0 ){
										for( int i=mapZoomRect.x; i<mapZoomRect.x+mapZoomRect.w; i++) {
											if(white==getpixel(i,mapZoomRect.y-15,gBackground)) whiteCount++;
											if( whiteCount>10 ) isOB=1;
										}
									}
									for( int i=0; i<COLOR_CELL_SIZE; i++ ){
										if( i!= POKEBALL_CELL && i!= WATER_CELL && i!= CENTER_HEALER_CELL && i!= GYM_PILLAR_CELL && i!= GYM_LEADER_CELL){
											if( i== DOOR_BLUE_N_CELL || i== DOOR_BLUE_SE_CELL || i== DOOR_GREEN_S_CELL || i== DOOR_RED_NW_CELL || i== DOOR_RED_S_CELL || i== DOOR_WOOD_CELL || i== DOOR_MART_CELL){
												if( cellColorThreshold[i] <= cellComp(trainerCellx, trainerCelly-cellShift, colorCodes[i], gScreenSurface)) {
													if( i==DOOR_MART_CELL )
														dispMessage(characterRect, mapZoomRect, isCave, 1, 0, 0);
													else
														dispMessage(characterRect, mapZoomRect, isCave, 0, 0, 0);
												}
												
											}else if( cellColorThreshold[i] <= cellComp(trainerCellx, trainerCelly-cellShift, colorCodes[i], gScreenSurface) ) {
												canWalk= 1;
											}
										}
									}
							// Check for cell cases
								// Wild grass
									if( cellColorThreshold[WILD_GRASS_CELL] <= cellComp(trainerCellx, trainerCelly-cellShift, colorCodes[WILD_GRASS_CELL], gScreenSurface) || cellColorThreshold[WILD_GRASS_2_CELL] <= cellComp(trainerCellx, trainerCelly-cellShift, colorCodes[WILD_GRASS_2_CELL], gScreenSurface) ) steponWildGrass=1;
								// Warp pad(gym)
									if( cellColorThreshold[GYM_FLOOR_WARP_CELL] <= cellComp(trainerCellx, trainerCelly-cellShift, colorCodes[GYM_FLOOR_WARP_CELL], gScreenSurface) ) isWarpTile=1;
								// PokeCenter entrance
									if( cellColorThreshold[POKE_CENTER_CELL] <= cellComp(trainerCellx, trainerCelly-cellShift, colorCodes[POKE_CENTER_CELL], gScreenSurface) ) {
										isPokeCenter=1;
										enteringBuilding=1;
									}
								// Gym entrance
									if( cellColorThreshold[GYM_DOOR_CELL] <= cellComp(trainerCellx, trainerCelly-cellShift, colorCodes[GYM_DOOR_CELL], gScreenSurface) ) {
										isGym=1;
										enteringBuilding=1;
									}
								// Cave entrance
									if( cellColorThreshold[CAVE_ENT_CELL] <= cellComp(trainerCellx, trainerCelly-cellShift, colorCodes[CAVE_ENT_CELL], gScreenSurface) ) isCave= 1;

									for( int i=0; i<16; i++){
		//use (16-i)/4 ? so that we end at zero
										if(i%4==0) frame++;
										if(frame>3) frame=0;
										gCurrentClip = &gWalkUp[frame];//set equal to proper frame of walking right sprites
										if( canWalk==1 ) {
											if(characterRect.y>SCREEN_HEIGHT/2+14 || isOB==1 || isPokeCenter) {
												characterRect.y-=2;//move char
												//NOTE: trainer is moved twice as many times as the screen. This is because the character image is shifted n "coordinates" on the window, while the screen is shifted n "pixels" on the background/map image
											} else {
												mapZoomRect.y--;//move screen
											}
										}
									//	if(isPokeCenter && ! enteringBuilding) characterRect.y-=2;
										usleep(sleeptime);
										if( ! isCave && ! enteringBuilding ) {
											SDL_BlitScaled(gBackground, &mapZoomRect, gScreenSurface, &stretch2windowRect);//put the background image onto gScreenSurface
											SDL_BlitScaled(gSpriteSheet, gCurrentClip, gScreenSurface, &characterRect);//put the character image onto gScreenSurface
											SDL_UpdateWindowSurface(gWindow);//update the window with the current surface
										}else{
											transitionGraphic(gWindow, gScreenSurface, gBackground, gSpriteSheet, gCurrentClip, characterRect, stretch2windowRect, mapZoomRect, i, transitionSpeed);
										}
									}
									if(enteringBuilding){
										leaveMap1x= characterRect.x;
										leaveMap1y= characterRect.y;
										leaveMap3x= mapZoomRect.x;
										leaveMap3y= mapZoomRect.y;
										if(isPokeCenter) {
											mapZoomRect.x = 0;	//begin image in middle of map
											mapZoomRect.y = 0;
											characterRect.x = 360;	//place at entrance
											characterRect.y = 430;	//place at entrance
											gBackground = gPokeMaps[ POKE_CENTER_MAP ];
										}else if(isGym) {
											mapZoomRect.x = 0;	//begin image in middle of map
											mapZoomRect.y = 0;
											characterRect.x = 343;	//place at entrance
											characterRect.y = 560;	//place at entrance
											gBackground = gPokeMaps[ POKE_GYM_MAP ];
										}
										for( int i=16; i<32; i++){//indexed 16-32 in order to keep transition smooth(since we are passing in a counter and the walk up uses 0-15
											if(i%4==0) frame++;
											if(frame>3) frame=0;
											gCurrentClip = &gWalkUp[frame];//set equal to proper frame of walking right sprites
											characterRect.y-=2;//move screen
											usleep(sleeptime);
											transitionGraphic(gWindow, gScreenSurface, gBackground, gSpriteSheet, gCurrentClip, characterRect, stretch2windowRect, mapZoomRect, i, transitionSpeed);
										}
									}
									if(isCave) {
										if(mapZoomRect.x<=833) {
											dispMessage(characterRect, mapZoomRect, isCave, 0, 0, 0);
											SDL_Delay(3000);
										}else{
											int prevCave= caveChoice;
											do{
												caveChoice= rand()%4;
											}while(prevCave==caveChoice);
											characterRect.x= caveCharX[caveChoice];
											characterRect.y= caveCharY[caveChoice];
											mapZoomRect.x= caveMapX[caveChoice];
											mapZoomRect.y= caveMapY[caveChoice];
										} 
										for( int i=16; i<32; i++){//indexed 16-32 in order to keep transition smooth(since we are passing in a counter and the walk up uses 0-15
											if(i%4==0) frame++;
											if(frame>3) frame=0;
											gCurrentClip = &gWalkDown[frame];//set equal to proper frame of walking right sprites
											if( canWalk==1 ) {
												if(characterRect.y<SCREEN_HEIGHT/2+14 || isOB==1 || isGym || isPokeCenter) {
													characterRect.y+=2;//move char
													//NOTE: trainer is moved twice as many times as the screen. This is because the character image is shifted n "coordinates" on the window, while the screen is shifted n "pixels" on the background/map image
												} else {
													mapZoomRect.y++;//move screen
												}
											}
											usleep(sleeptime);
											transitionGraphic(gWindow, gScreenSurface, gBackground, gSpriteSheet, gCurrentClip, characterRect, stretch2windowRect, mapZoomRect, i, transitionSpeed);
										}
										SDL_BlitScaled(gBackground, &mapZoomRect, gScreenSurface, &stretch2windowRect);//put the background image onto gScreenSurface
										SDL_BlitScaled(gSpriteSheet, gCurrentClip, gScreenSurface, &characterRect);//put the character image onto gScreenSurface
										SDL_UpdateWindowSurface(gWindow);//update the window with the current surface

									}
								}
								break;
							
							case SDLK_DOWN://down arrow key
								if( gCurrentClip!=&gWalkDown[0] ) {	// ie. if he is NOT already walking down, change the direction he is facing to down
									gCurrentClip= &gWalkDown[0];
								} else {	// else, make him continue walking down
									if( mapZoomRect.y+mapZoomRect.h+15 > botEdge ) isOB=1;
									if( isOB==0 ){
										for( int i=mapZoomRect.x; i<mapZoomRect.x+mapZoomRect.w; i++) {
											if(white==getpixel(i,mapZoomRect.y+mapZoomRect.h+15,gBackground)) whiteCount++;
											if( whiteCount>10 ) isOB=1;
										}
									}
									for( int i=0; i<COLOR_CELL_SIZE; i++ )
										if( i!= POKEBALL_CELL && i!= WATER_CELL && i!= CAVE_ENT_CELL && i!=GYM_PILLAR_CELL)
											if( cellColorThreshold[i] <= cellComp(trainerCellx, trainerCelly+cellShift, colorCodes[i], gScreenSurface) ) canWalk= 1;
									if( cellColorThreshold[WILD_GRASS_CELL] <= cellComp(trainerCellx, trainerCelly+cellShift, colorCodes[WILD_GRASS_CELL], gScreenSurface) || cellColorThreshold[WILD_GRASS_2_CELL] <= cellComp(trainerCellx, trainerCelly+cellShift, colorCodes[WILD_GRASS_2_CELL], gScreenSurface) ) steponWildGrass=1;
									if( cellColorThreshold[GYM_FLOOR_WARP_CELL] <= cellComp(trainerCellx, trainerCelly+cellShift, colorCodes[GYM_FLOOR_WARP_CELL], gScreenSurface) ) {
										isWarpTile=1;
									}
									if( cellColorThreshold[CENTER_EXIT_CELL] <= cellComp(trainerCellx, trainerCelly+cellShift, colorCodes[CENTER_EXIT_CELL], gScreenSurface) ) exitingBuilding=1;
									if( cellColorThreshold[GYM_FLOOR_EXIT_CELL] <= cellComp(trainerCellx, trainerCelly+cellShift, colorCodes[GYM_FLOOR_EXIT_CELL], gScreenSurface) ) exitingBuilding=1;
									for( int i=0; i<16; i++){
										if(i%4==0) frame++;
										if(frame>3) frame=0;
										gCurrentClip = &gWalkDown[frame];//set equal to proper frame of walking right sprites
										if( canWalk==1 ) {
											if(isOB==1 || characterRect.y<SCREEN_HEIGHT/2+14 || isGym || isPokeCenter) {
												characterRect.y+=2;//move char
											} else {
												mapZoomRect.y++;//move screen
											}
										}
									//	if(isPokeCenter) characterRect.y+=2;
										usleep(sleeptime);
										if( ! exitingBuilding ) {
											SDL_BlitScaled(gBackground, &mapZoomRect, gScreenSurface, &stretch2windowRect);//put the background image onto gScreenSurface
											SDL_BlitScaled(gSpriteSheet, gCurrentClip, gScreenSurface, &characterRect);//put the character image onto gScreenSurface
											SDL_UpdateWindowSurface(gWindow);//update the window with the current surface
										}else{
											transitionGraphic(gWindow, gScreenSurface, gBackground, gSpriteSheet, gCurrentClip, characterRect, stretch2windowRect, mapZoomRect, i, transitionSpeed);
										}
									}
									if(exitingBuilding) {
										gBackground = gPokeMaps[ POKE_MAP_ROUTE1 ];
										characterRect.x= leaveMap1x;
										characterRect.y= leaveMap1y;
										mapZoomRect.x= leaveMap3x;
										mapZoomRect.y= leaveMap3y;
										for( int i=16; i<32; i++){//indexed 16-32 in order to keep transition smooth(since we are passing in a counter and the walk up uses 0-15
											if(i%4==0) frame++;
											if(frame>3) frame=0;
											gCurrentClip = &gWalkDown[frame];//set equal to proper frame of walking right sprites
											characterRect.y+=2;//move char
											usleep(sleeptime);
											transitionGraphic(gWindow, gScreenSurface, gBackground, gSpriteSheet, gCurrentClip, characterRect, stretch2windowRect, mapZoomRect, i, transitionSpeed);
										}

										SDL_BlitScaled(gBackground, &mapZoomRect, gScreenSurface, &stretch2windowRect);//put the background image onto gScreenSurface
										SDL_BlitScaled(gSpriteSheet, gCurrentClip, gScreenSurface, &characterRect);//put the character image onto gScreenSurface
										SDL_UpdateWindowSurface(gWindow);//update the window with the current surface
										isPokeCenter= 0;
										isGym= 0;
										gymAnswers= 0;
									}
								}
								break;

							case SDLK_LEFT://left arrow key
								if( gCurrentClip!=&gWalkLeft[0] ) {	// ie. if he is NOT already walking left, change the direction he is facing to left
									gCurrentClip= &gWalkLeft[0];
								} else {	// else, make him continue walking left
									if( mapZoomRect.x-15 < leftEdge ) isOB=1;
									if( isOB==0 ) {
										for( int j=mapZoomRect.y; j<mapZoomRect.y+mapZoomRect.h; j++) {
											if(white==getpixel(mapZoomRect.x-15,j,gBackground)) whiteCount++;
											if( whiteCount>10 ) isOB=1;
										}
									}
									for( int i=0; i<COLOR_CELL_SIZE; i++ )
										if( i!= POKEBALL_CELL && i!= WATER_CELL && i!= CAVE_ENT_CELL && i!=GYM_PILLAR_CELL)
											if( cellColorThreshold[i] <= cellComp(trainerCellx-cellShift, trainerCelly, colorCodes[i], gScreenSurface) ) canWalk= 1;
									if( cellColorThreshold[WILD_GRASS_CELL] <= cellComp(trainerCellx-cellShift, trainerCelly, colorCodes[WILD_GRASS_CELL], gScreenSurface) || cellColorThreshold[WILD_GRASS_2_CELL] <= cellComp(trainerCellx-cellShift, trainerCelly, colorCodes[WILD_GRASS_2_CELL], gScreenSurface) ) steponWildGrass=1;
									if( cellColorThreshold[GYM_FLOOR_WARP_CELL] <= cellComp(trainerCellx-cellShift, trainerCelly, colorCodes[GYM_FLOOR_WARP_CELL], gScreenSurface) ) isWarpTile=1;
									for( int i=0; i<16; i++){
										if(i%4==0) frame++;
										if(frame>3) frame=0;
										gCurrentClip = &gWalkLeft[frame];//set equal to proper frame of walking right sprites
										if( canWalk==1 ) {
											if(isOB==1 || characterRect.x>SCREEN_WIDTH/2-9 || isGym || isPokeCenter) {
												characterRect.x-=2;//move char
											} else { 
												mapZoomRect.x--;//move screen
											}
										}
									//	if(isPokeCenter) characterRect.x-=2;
										usleep(sleeptime);
										SDL_BlitScaled(gBackground, &mapZoomRect, gScreenSurface, &stretch2windowRect);//put the background image onto gScreenSurface
										SDL_BlitScaled(gSpriteSheet, gCurrentClip, gScreenSurface, &characterRect);//put the character image onto gScreenSurface
										SDL_UpdateWindowSurface(gWindow);//update the window with the current surface
									}
								}
								break;

							case SDLK_RIGHT://right arrow key
								if( gCurrentClip!=&gWalkRight[0] ) {	// ie. if he is NOT already walking right, change the direction he is facing to right
									gCurrentClip= &gWalkRight[0];
								} else {	// else, make him continue walking right
									if( mapZoomRect.x+mapZoomRect.w+15 > rightEdge ) isOB=1;
									if( isOB==0 ) {
										for( int j=mapZoomRect.y; j<mapZoomRect.y+mapZoomRect.h; j++) {
											if(white==getpixel(mapZoomRect.x+mapZoomRect.w+15,j,gBackground)) whiteCount++;
											if( whiteCount>10 ) isOB=1;
										}
									}
									for( int i=0; i<COLOR_CELL_SIZE; i++ )
										if( i!= POKEBALL_CELL && i!= WATER_CELL && i!= CAVE_ENT_CELL && i!= GYM_PILLAR_CELL){
											if( cellColorThreshold[i] <= cellComp(trainerCellx+cellShift, trainerCelly, colorCodes[i], gScreenSurface) ) canWalk= 1;
										}
									if( cellColorThreshold[WILD_GRASS_CELL] <= cellComp(trainerCellx+cellShift, trainerCelly, colorCodes[WILD_GRASS_CELL], gScreenSurface) || cellColorThreshold[WILD_GRASS_2_CELL] <= cellComp(trainerCellx+cellShift, trainerCelly, colorCodes[WILD_GRASS_2_CELL], gScreenSurface) ) steponWildGrass=1;
									if( cellColorThreshold[GYM_FLOOR_WARP_CELL] <= cellComp(trainerCellx+cellShift, trainerCelly, colorCodes[GYM_FLOOR_WARP_CELL], gScreenSurface) ) isWarpTile=1;
									for( int i=0; i<16; i++){
										if(i%4==0) frame++;
										if(frame>3) frame=0;
										gCurrentClip = &gWalkRight[frame];//set equal to proper frame of walking right sprites
										if(canWalk==1){
											if(isOB==1 || characterRect.x<SCREEN_WIDTH/2-9 || isGym || isPokeCenter) {
												characterRect.x+=2;
											} else {
												mapZoomRect.x++;//move screen
											}
										}
									//	if(isPokeCenter) characterRect.x+=2;
										usleep(sleeptime);
										SDL_BlitScaled(gBackground, &mapZoomRect, gScreenSurface, &stretch2windowRect);//put the background image onto gScreenSurface
										SDL_BlitScaled(gSpriteSheet, gCurrentClip, gScreenSurface, &characterRect);//put the character image onto gScreenSurface
										SDL_UpdateWindowSurface(gWindow);//update the window with the current surface
									}
								}
								break;

							case SDLK_SPACE://space bar
				//				writeColorCodes(trainerCellx+cellShift,trainerCelly, gScreenSurface);
						// Check for cell cases
							// PokeCenter healer
								if( gCurrentClip==&gWalkUp[0] ){
									if( cellColorThreshold[CENTER_HEALER_CELL] <= cellComp(trainerCellx, trainerCelly-cellShift, colorCodes[CENTER_HEALER_CELL], gScreenSurface) )
										healMyPokemon(gWindow, gScreenSurface, gBackground, gSpriteSheet, gCurrentClip, characterRect, stretch2windowRect, mapZoomRect, &Nick);
								// Gym Pillar/Statue
									if( cellColorThreshold[GYM_PILLAR_CELL] <= cellComp(trainerCellx, trainerCelly-cellShift, colorCodes[GYM_PILLAR_CELL], gScreenSurface) )
										talkToPillar(characterRect, &gymAnswers, &firstPillarx);
								// Gym leader
									if( cellColorThreshold[GYM_LEADER_CELL] <= cellComp(trainerCellx, trainerCelly-cellShift, colorCodes[GYM_LEADER_CELL], gScreenSurface) ){
										battleCutScene(characterRect, stretch2windowRect, mapZoomRect);
										cout<<"BATTLETIME\n";
									}
								// Trainers
									for(int i=TRAINER_BRUNETTE_CELL; i<=TRAINER_YELLOW_CELL; i++)
										if( cellColorThreshold[i] <= cellComp(trainerCellx, trainerCelly-cellShift, colorCodes[i], gScreenSurface) ){
											battleCutScene(characterRect, stretch2windowRect, mapZoomRect);
											if(i==TRAINER_BRUNETTE_CELL) i++;	// ensures that brunette 1 and 2 are always read into battle as same user. If brunette 1 is matched, this will skip brunette 2 which works to our advantage because then we do not need to worry about a single brunette trainer matching twice
											cout<<"littlebattle\n";
										}
								}else if( gCurrentClip==&gWalkDown[0] ){
								// Trainers
									for(int i=TRAINER_BRUNETTE_CELL; i<=TRAINER_YELLOW_CELL; i++)
										if( cellColorThreshold[i] <= cellComp(trainerCellx, trainerCelly+cellShift, colorCodes[i], gScreenSurface) ){
											battleCutScene(characterRect, stretch2windowRect, mapZoomRect);
											if(i==TRAINER_BRUNETTE_CELL) i++;	// ensures that brunette 1 and 2 are always read into battle as same user. If brunette 1 is matched, this will skip brunette 2 which works to our advantage because then we do not need to worry about a single brunette trainer matching twice
											cout<<"littlebattle\n";
										}
								}else if( gCurrentClip==&gWalkLeft[0] ){
								// Trainers
									for(int i=TRAINER_BRUNETTE_CELL; i<=TRAINER_YELLOW_CELL; i++)
										if( cellColorThreshold[i] <= cellComp(trainerCellx-cellShift, trainerCelly, colorCodes[i], gScreenSurface) ){
											battleCutScene(characterRect, stretch2windowRect, mapZoomRect);
											if(i==TRAINER_BRUNETTE_CELL) i++;	// ensures that brunette 1 and 2 are always read into battle as same user. If brunette 1 is matched, this will skip brunette 2 which works to our advantage because then we do not need to worry about a single brunette trainer matching twice
											cout<<"littlebattle\n";
										}
								}else if( gCurrentClip==&gWalkRight[0] ){
								// Trainers
									for(int i=TRAINER_BRUNETTE_CELL; i<=TRAINER_YELLOW_CELL; i++)
										if( cellColorThreshold[i] <= cellComp(trainerCellx+cellShift, trainerCelly, colorCodes[i], gScreenSurface) ){
											battleCutScene(characterRect, stretch2windowRect, mapZoomRect);
											if(i==TRAINER_BRUNETTE_CELL) i++;	// ensures that brunette 1 and 2 are always read into battle as same user. If brunette 1 is matched, this will skip brunette 2 which works to our advantage because then we do not need to worry about a single brunette trainer matching twice
											cout<<"littlebattle\n";
										}
								}
								break;
				
							case SDLK_p:	// Pause Menu
								if(isPokeCenter || isGym)
									dispMessage(characterRect, mapZoomRect, isCave, 0, 0, 1);	// If inside, don't let the user pause!
								else
									quit=pauseMenu(characterRect, stretch2windowRect, mapZoomRect);	// Outside, so let the user pause
								break;

							default://do nothing when any other keys are pressed
								break;
						}


						if(steponWildGrass){
							if(rand()%100 < 15){
								battleCutScene(characterRect, stretch2windowRect, mapZoomRect);
								Nick.wild_battle();
					//		transitionGraphic(gWindow, gScreenSurface, gBackground, gSpriteSheet, gCurrentClip, characterRect, stretch2windowRect, mapZoomRect);
							}
						}
				
						if(isWarpTile==1) {
							frame= 0;
						// Spin character around, and then warp him to another warp pad in the gym
							for( int i=0; i<32; i++){
								if(i%4==0) gCurrentClip = &gWalkLeft[frame];//set equal to proper frame of walking right sprites
								if(i%4==1) gCurrentClip = &gWalkUp[frame];//set equal to proper frame of walking right sprites
								if(i%4==2) gCurrentClip = &gWalkRight[frame];//set equal to proper frame of walking right sprites
								if(i%4==3) gCurrentClip = &gWalkDown[frame];//set equal to proper frame of walking right sprites
								usleep(warptime);
								transitionGraphic(gWindow, gScreenSurface, gBackground, gSpriteSheet, gCurrentClip, characterRect, stretch2windowRect, mapZoomRect, i, transitionSpeed);
							// Halfway through spinning motion, once the screen has blacked out, change the character location on the map
								if(i==15) {
									characterRect= determineWarpLoc(characterRect);
								}
							}
						}

						//This will save all events (to e) that occured while the trainer was taking a step. As long as the event was not a quit, then the event will be arbitrarily stored and then overwritten. This is done so that the user playing may hold down an arrow key, and then the trainer will stop walking as soon as the user releases the key 
						while(SDL_PollEvent(&e) != 0){//while there remains user entered events
							if (e.type == SDL_QUIT){//entered event is equal to pressing red x in top right hand corner of window
								quit = true;//set quit to true to exit the while loop
							}
						}
					}//end KEYDOWN elseif

					if(isPokeCenter)
						gBackground = gPokeMaps[ POKE_CENTER_MAP ];			// PokeCenter
					else if(isGym)
						if(gymAnswers==2)
							gBackground = gPokeMaps[ POKE_GYM_ANSWERS_MAP ];	// Gym with answers
						else
							gBackground = gPokeMaps[ POKE_GYM_MAP ];		// Gym without answers
					else
						gBackground = gPokeMaps[ POKE_MAP_ROUTE1 ];			// Main route map

				// Blit and update window
					SDL_BlitScaled(gBackground, &mapZoomRect, gScreenSurface, &stretch2windowRect);//put the background image onto gScreenSurface
					SDL_BlitScaled(gSpriteSheet, gCurrentClip, gScreenSurface, &characterRect);//put the character image onto gScreenSurface
					SDL_UpdateWindowSurface(gWindow);//update the window with the current surface

				// store new trainer location(for cellComp use)
					trainerCellx= characterRect.x+7;
					trainerCelly= characterRect.y+16;

					frame=0;		//reset frame for updated animation
					whiteCount=0;		// reset white counter
				// Reset each of the following so that the value can be re-evaluated on the next step
					isOB=0;			// out of bounds
					steponWildGrass=0;
					canWalk=0;
					isCave=0;
					enteringBuilding=0;
					exitingBuilding=0;
					isWarpTile=0;
				}
			}//end game loop
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

	gPokeMaps[ POKE_MAP_ROUTE1 ] = loadSurface("pokeMap3.png");//load background image
	if(gPokeMaps[ POKE_MAP_ROUTE1 ] == NULL){//if not loaded properly return unsuccessful 
		success = false;
	}

	gPokeMaps[ POKE_CENTER_MAP ] = loadSurface("pokeCenter.png");//load background image
	if(gPokeMaps[ POKE_CENTER_MAP ] == NULL){//if not loaded properly return unsuccessful 
		success = false;
	}

	gPokeMaps[ POKE_CENTER_YES_MAP ] = loadSurface("pokeCenteryes.png");//load background image
	if(gPokeMaps[ POKE_CENTER_YES_MAP ] == NULL){//if not loaded properly return unsuccessful 
		success = false;
	}

	gPokeMaps[ POKE_CENTER_NO_MAP ] = loadSurface("pokeCenterno.png");//load background image
	if(gPokeMaps[ POKE_CENTER_NO_MAP ] == NULL){//if not loaded properly return unsuccessful 
		success = false;
	}

	gPokeMaps[ POKE_CENTER_HEALING_MAP ] = loadSurface("pokeCenterhealing.png");//load background image
	if(gPokeMaps[ POKE_CENTER_HEALING_MAP ] == NULL){//if not loaded properly return unsuccessful 
		success = false;
	}

	gPokeMaps[ POKE_GYM_MAP ] = loadSurface("gymBase2.png");//load background image
	if(gPokeMaps[ POKE_GYM_MAP ] == NULL){//if not loaded properly return unsuccessful 
		success = false;
	}

	gPokeMaps[ POKE_GYM_ANSWERS_MAP ] = loadSurface("gymBase.png");//load background image
	if(gPokeMaps[ POKE_GYM_ANSWERS_MAP ] == NULL){//if not loaded properly return unsuccessful 
		success = false;
	}

	gPokeMaps[ POKE_MESSAGE ] = loadSurface("pokeFrameNarrow.png");//load background image
	if(gPokeMaps[ POKE_MESSAGE ] == NULL){//if not loaded properly return unsuccessful 
		success = false;
	}

	gPokeMaps[ POKE_MESSAGE_1 ] = loadSurface("pokeFrameNotAvailable.png");//load background image
	if(gPokeMaps[ POKE_MESSAGE_1 ] == NULL){//if not loaded properly return unsuccessful 
		success = false;
	}

	gPokeMaps[ POKE_MESSAGE_2 ] = loadSurface("pokeFrameNotAvailable2.png");//load background image
	if(gPokeMaps[ POKE_MESSAGE_2 ] == NULL){//if not loaded properly return unsuccessful 
		success = false;
	}

	gPokeMaps[ POKE_MESSAGE_3 ] = loadSurface("pokeFrameNotAvailable3.png");//load background image
	if(gPokeMaps[ POKE_MESSAGE_3 ] == NULL){//if not loaded properly return unsuccessful 
		success = false;
	}

	gPokeMaps[ POKE_MESSAGE_4 ] = loadSurface("pokeFrameNotAvailable4.png");//load background image
	if(gPokeMaps[ POKE_MESSAGE_4 ] == NULL){//if not loaded properly return unsuccessful 
		success = false;
	}

	gPokeMaps[ POKE_MESSAGE_5 ] = loadSurface("pokeFrameNotAvailable5.png");//load background image
	if(gPokeMaps[ POKE_MESSAGE_5 ] == NULL){//if not loaded properly return unsuccessful 
		success = false;
	}

	gPokeMaps[ POKE_MESSAGE_6a ] = loadSurface("pokeFrameNotAvailable6.png");//load background image
	if(gPokeMaps[ POKE_MESSAGE_6a ] == NULL){//if not loaded properly return unsuccessful 
		success = false;
	}

	gPokeMaps[ POKE_MESSAGE_6b ] = loadSurface("pokeFrameNotAvailable6b.png");//load background image
	if(gPokeMaps[ POKE_MESSAGE_6b ] == NULL){//if not loaded properly return unsuccessful 
		success = false;
	}

	gPokeMaps[ POKE_MESSAGE_CAVE ] = loadSurface("pokeFrameCave.png");//load background image
	if(gPokeMaps[ POKE_MESSAGE_CAVE ] == NULL){//if not loaded properly return unsuccessful 
		success = false;
	}

	gPokeMaps[ POKE_MESSAGE_MART ] = loadSurface("pokeFrameMart.png");//load background image
	if(gPokeMaps[ POKE_MESSAGE_MART ] == NULL){//if not loaded properly return unsuccessful 
		success = false;
	}

	gPokeMaps[ POKE_MESSAGE_START ] = loadSurface("pokeFrameStart.png");//load background image
	if(gPokeMaps[ POKE_MESSAGE_START ] == NULL){//if not loaded properly return unsuccessful 
		success = false;
	}

	gPokeMaps[ POKE_MESSAGE_CANT_PAUSE ] = loadSurface("pokeFrameCantPause.png");//load background image
	if(gPokeMaps[ POKE_MESSAGE_CANT_PAUSE ] == NULL){//if not loaded properly return unsuccessful 
		success = false;
	}

	gPokeMaps[ POKE_MENU_MAIN ] = loadSurface("pokeFrameTall.png");//load background image
	if(gPokeMaps[ POKE_MENU_MAIN ] == NULL){//if not loaded properly return unsuccessful 
		success = false;
	}

	gIntroBackground = loadSurface("IntroBackground.png");//load sprite sheet containing animation images
	if(gIntroBackground == NULL){//if not loaded properly return unsuccessful
		success = false;
	}
	else{
		//Set Walking down clips
		gIntroTrees.x = 128;
		gIntroTrees.y = 0;
		gIntroTrees.w = 240;
		gIntroTrees.h = 160;
	}

	gIntroTitleSprites = loadSurface("PokeTitle.png");//load sprite sheet containing animation images
	if(gIntroTitleSprites == NULL){//if not loaded properly return unsuccessful
		success = false;
	}
	else{
		SDL_SetColorKey(gIntroTitleSprites, SDL_TRUE, SDL_MapRGB(gIntroTitleSprites->format, 255, 255, 255));//the color given in SDL_MapRGB one the sprite sheet will be made transparent to background image
		gPokeTitle[0].x = 5;
		gPokeTitle[0].y = 6;
		gPokeTitle[0].w = 175;
		gPokeTitle[0].h = 57;

		gPokeTitle[1].x = 178;
		gPokeTitle[1].y = 6;
		gPokeTitle[1].w = 175;
		gPokeTitle[1].h = 57;

		gPokeTitle[2].x = 357;
		gPokeTitle[2].y = 7;
		gPokeTitle[2].w = 175;
		gPokeTitle[2].h = 57;

		gPokeTitle[3].x = 2;
		gPokeTitle[3].y = 72;
		gPokeTitle[3].w = 175;
		gPokeTitle[3].h = 57;

		gPokeTitle[4].x = 181;
		gPokeTitle[4].y = 71;
		gPokeTitle[4].w = 175;
		gPokeTitle[4].h = 57;

		gPokeTitle[5].x = 366;
		gPokeTitle[5].y = 71;
		gPokeTitle[5].w = 175;
		gPokeTitle[5].h = 57;
	}

	gIntroSprites = loadSurface("JumpFall.png");//load sprite sheet containing animation images
	if(gIntroSprites == NULL){//if not loaded properly return unsuccessful
		success = false;
	}
	else{
		//Set Walking down clips
		gTorchic[0].x = 0;
		gTorchic[0].y = 54;
		gTorchic[0].w = 24;
		gTorchic[0].h = 24;

		gTorchic[1].x = 21;
		gTorchic[1].y = 54;
		gTorchic[1].w = 24;
		gTorchic[1].h = 24;

		gTorchic[2].x = 41;
		gTorchic[2].y = 54;
		gTorchic[2].w = 24;
		gTorchic[2].h = 24;

		gTorchic[3].x = 63;
		gTorchic[3].y = 54;
		gTorchic[3].w = 24;
		gTorchic[3].h = 24;

		gTorchic[4].x = 90;
		gTorchic[4].y = 50;
		gTorchic[4].w = 24;
		gTorchic[4].h = 24;

		gTorchic[5].x = 119;
		gTorchic[5].y = 48;
		gTorchic[5].w = 24;
		gTorchic[5].h = 24;

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

void putpixel( int x, int y, int color ) {
	unsigned int *ptr = (unsigned int*)gScreenSurface->pixels;
	int lineoffset = y * (gScreenSurface->pitch / 4);
	ptr[ lineoffset + x ] = color;
}

uint32_t getpixel( int x, int y, SDL_Surface *surface ) {
	unsigned int *ptr = (unsigned int*)surface->pixels;
	int lineoffset = y * (surface->pitch / 4);
	return ( ptr[ lineoffset + x ] );
}

double cellComp( int x1, int y1, map<int,int> cellCode, SDL_Surface *surface ){
	double match= 0;
	map<int,int> localColors;

	for( int i=0; i<32; i++ ) {	
		for( int j=0; j<32; j++ ) {	
			localColors[ getpixel( x1+i, y1+j, gScreenSurface ) ]++;
		}
	}
	for( map<int,int>::const_iterator i=localColors.begin(); i!=localColors.end(); i++){
		match+= (localColors[i->first] < cellCode[i->first]) ? localColors[i->first] : cellCode[i->first];
	}

	return (match/1024);
}

void writeColorCodes(int x, int y, SDL_Surface *surface){

	ofstream outFile;
	outFile.open( "trainerBrunette2.txt", ios::out );
	map<int,int> colors;

	for( int i=0; i<32; i++ ) {	
		for( int j=0; j<32; j++ ) {	
			colors[ getpixel( x+i, y+j, gScreenSurface ) ]++;
		}
	}

	for( map<int,int>::const_iterator i=colors.begin(); i!=colors.end(); i++){
		outFile<< i->first << " " << i->second <<" ";
	}
	outFile << -1 << " " << -1 << endl;

	outFile.close();
}

vector<map<int,int> > readColorCodes(){
	ifstream inFile;
	inFile.open( "cellPixelColors.txt",ios::in );
	vector<map<int,int> > colorCodes;
	map<int,int> tempMap;
	int tempKey, tempVal, i=0;

	while(inFile>>tempKey && inFile>>tempVal){
		if( tempKey!= -1){
			tempMap.insert(map<int,int>::value_type(tempKey, tempVal) );
		} else {
			colorCodes.push_back(tempMap);
			tempMap.clear();
		}
	}

	inFile.close();

	return colorCodes;
}

void transitionGraphic(SDL_Window *window, SDL_Surface *screen, SDL_Surface *gBackground, SDL_Surface *gSpriteSheet, SDL_Rect *gCurrentClip, SDL_Rect characterRect, SDL_Rect stretch2windowRect, SDL_Rect mapZoomRect, int counter, int counterJump) {
	SDL_Surface* transsurface;
	for( int i=(16*counter); i<=(16*counter+16); i+=counterJump ){
		#if SDL_BYTEORDER == SDL_BIG_ENDIAN
			transsurface = SDL_CreateRGBSurface(SDL_SWSURFACE,stretch2windowRect.w,stretch2windowRect.h,32, 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF);
		#else
			transsurface = SDL_CreateRGBSurface(SDL_SWSURFACE,stretch2windowRect.w,stretch2windowRect.h,32, 0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000);
		#endif
		SDL_FillRect(transsurface, NULL, SDL_MapRGBA(transsurface->format, 0, 0, 0, 255-abs(255-i) ) );
		SDL_BlitScaled(gBackground, &mapZoomRect, gScreenSurface, &stretch2windowRect);//put the background image onto gScreenSurface
		SDL_BlitScaled(gSpriteSheet, gCurrentClip, gScreenSurface, &characterRect);//put the character image onto gScreenSurface
		SDL_BlitSurface(transsurface,NULL,screen,&stretch2windowRect);
		SDL_UpdateWindowSurface(window);//update the window with the current surface
		if(255-abs(255-i) > 200) SDL_Delay(1);	// extend pause when screen is at its darkest
	}
}

void healMyPokemon(SDL_Window *window, SDL_Surface *screen, SDL_Surface *gBackground, SDL_Surface *gSpriteSheet, SDL_Rect *gCurrentClip, SDL_Rect characterRect, SDL_Rect stretch2windowRect, SDL_Rect mapZoomRect, Player *Nick) {
	
			gBackground = gPokeMaps[ POKE_CENTER_YES_MAP ];
			SDL_BlitScaled(gBackground, &mapZoomRect, gScreenSurface, &stretch2windowRect);//put the background image onto gScreenSurface
			SDL_BlitScaled(gSpriteSheet, gCurrentClip, gScreenSurface, &characterRect);//put the character image onto gScreenSurface
			SDL_UpdateWindowSurface(window);//update the window with the current surface

			bool quit = false;		//boolean variable for when the user wants to quit
			SDL_Event e;			//variable for keyboard events entered by user

			while(!quit){//while the user has not entered the event to quit
				while(SDL_PollEvent(&e) != 0){//while there remains user entered events
					if (e.type == SDL_QUIT){//entered event is equal to pressing red x in top right hand corner of window
						quit = true;//set quit to true to exit the while loop
					}
					else if(e.type == SDL_KEYDOWN){//a key has been pressed
						switch(e.key.keysym.sym)//switch with key type parameter
						{
							//frame is reset to zero after the switch. Thus each for loop will loop through the stages of walking sprites as gWalkx[1,2,3,0] in that order. Since gCurrentClip will end in gWalkx[0], we can check the direction the trainer is facing by seeing if the gCurrentClip == gWalkx[0]
							case SDLK_RIGHT://right arrow key
								gBackground = gPokeMaps[ POKE_CENTER_NO_MAP ];
								SDL_BlitScaled(gBackground, &mapZoomRect, gScreenSurface, &stretch2windowRect);//put the background image onto gScreenSurface
								SDL_BlitScaled(gSpriteSheet, gCurrentClip, gScreenSurface, &characterRect);//put the character image onto gScreenSurface
								SDL_UpdateWindowSurface(window);//update the window with the current surface
								break;
							case SDLK_LEFT://left arrow key
								gBackground = gPokeMaps[ POKE_CENTER_YES_MAP ];
								SDL_BlitScaled(gBackground, &mapZoomRect, gScreenSurface, &stretch2windowRect);//put the background image onto gScreenSurface
								SDL_BlitScaled(gSpriteSheet, gCurrentClip, gScreenSurface, &characterRect);//put the character image onto gScreenSurface
								SDL_UpdateWindowSurface(window);//update the window with the current surface
								break;
							case SDLK_SPACE://space bar
								if( gBackground == gPokeMaps[ POKE_CENTER_YES_MAP ] ) {
									(*Nick).pokeCenter();//heals all pokemon
									gBackground = gPokeMaps[ POKE_CENTER_HEALING_MAP ];
									SDL_BlitScaled(gBackground, &mapZoomRect, gScreenSurface, &stretch2windowRect);//put the background image onto gScreenSurface
									SDL_BlitScaled(gSpriteSheet, gCurrentClip, gScreenSurface, &characterRect);//put the character image onto gScreenSurface
									SDL_UpdateWindowSurface(window);//update the window with the current surface
									SDL_Delay(2000);
									for(int i=0; i<32; i++)
										transitionGraphic(gWindow, gScreenSurface, gBackground, gSpriteSheet, gCurrentClip, characterRect, stretch2windowRect, mapZoomRect, i, 3);
								}
								quit = true;
								break;
						}
						while(SDL_PollEvent(&e) != 0){//while there remains user entered events
							if (e.type == SDL_QUIT){//entered event is equal to pressing red x in top right hand corner of window
								quit = true;//set quit to true to exit the while loop
							}
						}
					}
				}
			}
}

SDL_Rect determineWarpLoc( SDL_Rect characterRect ) {
	int gymWarpRow[6]={	80,	144,	272,	336,	464,	528};		// row holds Y coords
	int gymWarpCol[7]={	23,	183,	279,	407,	471,	535,	695};	// col holds X coords

	if(characterRect.y==gymWarpRow[0]){
		if(characterRect.x==gymWarpCol[0]){
			characterRect.y= gymWarpRow[1];
			characterRect.x= gymWarpCol[5];
		}
		else if(characterRect.x==gymWarpCol[1]){
			characterRect.y= gymWarpRow[0];
			characterRect.x= gymWarpCol[3];
		}
		else if(characterRect.x==gymWarpCol[2]){
			characterRect.y= gymWarpRow[3];
			characterRect.x= gymWarpCol[5];
		}
		else if(characterRect.x==gymWarpCol[3]){
			characterRect.y= gymWarpRow[0];
			characterRect.x= gymWarpCol[1];
		}
		else if(characterRect.x==gymWarpCol[5]){
			characterRect.y= gymWarpRow[2];
			characterRect.x= gymWarpCol[5];
		}
		else if(characterRect.x==gymWarpCol[6]){
			characterRect.y= gymWarpRow[4];
			characterRect.x= gymWarpCol[5];
		}
	}else if(characterRect.y==gymWarpRow[1]){
		if(characterRect.x==gymWarpCol[0]){
			characterRect.y= gymWarpRow[3];
			characterRect.x= gymWarpCol[4];
		}
		else if(characterRect.x==gymWarpCol[1]){
			characterRect.y= gymWarpRow[3];
			characterRect.x= gymWarpCol[0];
		}
		else if(characterRect.x==gymWarpCol[2]){
			characterRect.y= gymWarpRow[2];
			characterRect.x= gymWarpCol[1];
		}
		else if(characterRect.x==gymWarpCol[3]){
			characterRect.y= gymWarpRow[5];
			characterRect.x= gymWarpCol[0];
		}
		else if(characterRect.x==gymWarpCol[5]){
			characterRect.y= gymWarpRow[0];
			characterRect.x= gymWarpCol[0];
		}
		else if(characterRect.x==gymWarpCol[6]){
			characterRect.y= gymWarpRow[4];
			characterRect.x= gymWarpCol[0];
		}
	}else if(characterRect.y==gymWarpRow[2]){
		if(characterRect.x==gymWarpCol[0]){
			characterRect.y= gymWarpRow[2];
			characterRect.x= gymWarpCol[7];
		}
		else if(characterRect.x==gymWarpCol[1]){
			characterRect.y= gymWarpRow[1];
			characterRect.x= gymWarpCol[2];
		}
		else if(characterRect.x==gymWarpCol[5]){
			characterRect.y= gymWarpRow[0];
			characterRect.x= gymWarpCol[5];
		}
		else if(characterRect.x==gymWarpCol[6]){
			characterRect.y= gymWarpRow[4];
			characterRect.x= gymWarpCol[6];
		}
	}else if(characterRect.y==gymWarpRow[3]){
		if(characterRect.x==gymWarpCol[0]){
			characterRect.y= gymWarpRow[1];
			characterRect.x= gymWarpCol[1];
		}
		else if(characterRect.x==gymWarpCol[1]){
			characterRect.y= gymWarpRow[5];
			characterRect.x= gymWarpCol[1];
		}
		else if(characterRect.x==gymWarpCol[4]){	// Make warp tile from leader go back to entrance warp
			characterRect.y= gymWarpRow[4];
			characterRect.x= gymWarpCol[4];
		}
		else if(characterRect.x==gymWarpCol[5]){
			characterRect.y= gymWarpRow[0];
			characterRect.x= gymWarpCol[2];
		}
		else if(characterRect.x==gymWarpCol[6]){
			characterRect.y= gymWarpRow[2];
			characterRect.x= gymWarpCol[0];
		}
	}else if(characterRect.y==gymWarpRow[4]){
		if(characterRect.x==gymWarpCol[0]){
			characterRect.y= gymWarpRow[1];
			characterRect.x= gymWarpCol[6];
		}
		else if(characterRect.x==gymWarpCol[1]){
			characterRect.y= gymWarpRow[5];
			characterRect.x= gymWarpCol[5];
		}
		else if(characterRect.x==gymWarpCol[4]){
			characterRect.y= gymWarpRow[5];
			characterRect.x= gymWarpCol[6];
		}
		else if(characterRect.x==gymWarpCol[5]){
			characterRect.y= gymWarpRow[0];
			characterRect.x= gymWarpCol[6];
		}
		else if(characterRect.x==gymWarpCol[6]){
			characterRect.y= gymWarpRow[2];
			characterRect.x= gymWarpCol[6];
		}
	}else if(characterRect.y==gymWarpRow[5]){
		if(characterRect.x==gymWarpCol[0]){
			characterRect.y= gymWarpRow[1];
			characterRect.x= gymWarpCol[3];
		}
		else if(characterRect.x==gymWarpCol[1]){
			characterRect.y= gymWarpRow[3];
			characterRect.x= gymWarpCol[1];
		}
		else if(characterRect.x==gymWarpCol[5]){
			characterRect.y= gymWarpRow[4];
			characterRect.x= gymWarpCol[1];
		}
		else if(characterRect.x==gymWarpCol[6]){
			characterRect.y= gymWarpRow[4];
			characterRect.x= gymWarpCol[4];
		}
	}

	return(characterRect);

}

void talkToPillar(SDL_Rect characterRect, int *gymAnswers, int *firstPillarx ) {
	if(*gymAnswers==0){
		*firstPillarx= characterRect.x;
		(*gymAnswers)++;
	}else if(*gymAnswers==1) {
		if( characterRect.x!=*firstPillarx ){
			(*gymAnswers)++;
		}
	}
}

void dispMessage(SDL_Rect characterRect, SDL_Rect mapZoomRect, int isCave, int isMart, int isStart, int isInside) {

	SDL_Rect stretchRect4;			//rectangle used for
	stretchRect4.w = (SCREEN_WIDTH/6)*4;			//size of portion of sprite sheet taken up by character
	stretchRect4.h = (SCREEN_HEIGHT/4);			//size of portion of sprite sheet taken up by character
	stretchRect4.x = SCREEN_WIDTH/6;	//place at center of screen
	if(characterRect.y>SCREEN_WIDTH/2)
		stretchRect4.y = SCREEN_HEIGHT/15;	//place at center of screen
	else
		stretchRect4.y = 2*(SCREEN_HEIGHT/3);	//place at center of screen
		
	if(isCave)
		SDL_BlitScaled(gPokeMaps[ POKE_MESSAGE_CAVE ], NULL, gScreenSurface, &stretchRect4);
	else if(isStart)
		SDL_BlitScaled(gPokeMaps[ POKE_MESSAGE_START ], NULL, gScreenSurface, &stretchRect4);
	else if(isMart)
		SDL_BlitScaled(gPokeMaps[ POKE_MESSAGE_MART ], NULL, gScreenSurface, &stretchRect4);
	else if(isInside)
		SDL_BlitScaled(gPokeMaps[ POKE_MESSAGE_CANT_PAUSE ], NULL, gScreenSurface, &stretchRect4);
	else if(mapZoomRect.x<529)
		SDL_BlitScaled(gPokeMaps[ POKE_MESSAGE_1 ], NULL, gScreenSurface, &stretchRect4);
	else if(characterRect.x>535){
		SDL_BlitScaled(gPokeMaps[ POKE_MESSAGE_6a ], NULL, gScreenSurface, &stretchRect4);
		SDL_UpdateWindowSurface(gWindow);//update the window with the current surface
		SDL_Delay(2000);
		SDL_BlitScaled(gPokeMaps[ POKE_MESSAGE_6b ], NULL, gScreenSurface, &stretchRect4);
	}else{
		int message= (rand()%((POKE_MESSAGE_5-POKE_MESSAGE_2)+1)) + POKE_MESSAGE_2;
		SDL_BlitScaled(gPokeMaps[ message ], NULL, gScreenSurface, &stretchRect4);
	}
	SDL_UpdateWindowSurface(gWindow);//update the window with the current surface
	SDL_Delay(4000);
}

bool pauseMenu(SDL_Rect characterRect, SDL_Rect stretch2windowRect, SDL_Rect mapZoomRect) {
	SDL_Rect stretchRect4;			//rectangle used for
	stretchRect4.w = SCREEN_WIDTH/4;			//size of portion of sprite sheet taken up by character
	stretchRect4.h = (SCREEN_HEIGHT/2);			//size of portion of sprite sheet taken up by character
	stretchRect4.x = 2*(SCREEN_WIDTH/3);	//place at center of screen
	stretchRect4.y = SCREEN_HEIGHT/15;	//place at center of screen

	SDL_Rect stretchRect5;			//rectangle used for
	stretchRect5.w = stretchRect4.w-30;			//size of portion of sprite sheet taken up by character
	stretchRect5.h = (stretchRect4.h-30)/4;			//size of portion of sprite sheet taken up by character
	stretchRect5.x = stretchRect4.x+15;	//place at center of screen
	stretchRect5.y = stretchRect4.y+16;	//place at center of screen

	SDL_Surface* transsurface;
	#if SDL_BYTEORDER == SDL_BIG_ENDIAN
		transsurface = SDL_CreateRGBSurface(SDL_SWSURFACE,stretch2windowRect.w,stretch2windowRect.h,32, 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF);
	#else
		transsurface = SDL_CreateRGBSurface(SDL_SWSURFACE,stretch2windowRect.w,stretch2windowRect.h,32, 0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000);
	#endif
	SDL_FillRect(transsurface, &stretchRect5, SDL_MapRGBA(transsurface->format, 143, 200, 255, 120));
//		SDL_BlitScaled(gBackground, &mapZoomRect, gScreenSurface, &stretch2windowRect);//put the background image onto gScreenSurface
//		SDL_BlitScaled(gSpriteSheet, gCurrentClip, gScreenSurface, &characterRect);//put the character image onto gScreenSurface

	SDL_BlitScaled(gPokeMaps[ POKE_MENU_MAIN ], NULL, gScreenSurface, &stretchRect4);
	SDL_BlitSurface(transsurface,NULL,gScreenSurface,&stretch2windowRect);
	SDL_UpdateWindowSurface(gWindow);//update the window with the current surface

	bool quitMenu = false;		// true to quit pause menu
	bool quitGame = false;		// true to quit game
	SDL_Event e;			//variable for keyboard events entered by user

	while(!quitMenu && !quitGame){//while the user has not entered the event to quit
		while(SDL_PollEvent(&e) != 0){//while there remains user entered events
			if (e.type == SDL_QUIT){//entered event is equal to pressing red x in top right hand corner of window
				quitGame = true;//set quit to true to exit the while loop
			}
			else if(e.type == SDL_KEYDOWN){//a key has been pressed
				switch(e.key.keysym.sym)//switch with key type parameter
				{
					//frame is reset to zero after the switch. Thus each for loop will loop through the stages of walking sprites as gWalkx[1,2,3,0] in that order. Since gCurrentClip will end in gWalkx[0], we can check the direction the trainer is facing by seeing if the gCurrentClip == gWalkx[0]
					case SDLK_UP://right arrow key
						if(stretchRect5.y>stretchRect4.y+16){
							SDL_FillRect(transsurface, &stretchRect5, SDL_MapRGBA(transsurface->format, 0, 0, 0, 0));
							stretchRect5.y -= stretchRect5.h;	//place at center of screen
							SDL_FillRect(transsurface, &stretchRect5, SDL_MapRGBA(transsurface->format, 143, 200, 255, 120));
							SDL_BlitScaled(gPokeMaps[ POKE_MENU_MAIN ], NULL, gScreenSurface, &stretchRect4);
							SDL_BlitSurface(transsurface,NULL,gScreenSurface,&stretch2windowRect);
							SDL_UpdateWindowSurface(gWindow);//update the window with the current surface
						}
						break;
					case SDLK_DOWN://left arrow key
						if(stretchRect5.y<(stretchRect4.y+16)+3*stretchRect5.h){
							SDL_FillRect(transsurface, &stretchRect5, SDL_MapRGBA(transsurface->format, 0, 0, 0, 0));
							stretchRect5.y += stretchRect5.h;	//place at center of screen
							SDL_FillRect(transsurface, &stretchRect5, SDL_MapRGBA(transsurface->format, 143, 200, 255, 120));
							SDL_BlitScaled(gPokeMaps[ POKE_MENU_MAIN ], NULL, gScreenSurface, &stretchRect4);
							SDL_BlitSurface(transsurface,NULL,gScreenSurface,&stretch2windowRect);
							SDL_UpdateWindowSurface(gWindow);//update the window with the current surface
						}
						break;
					case SDLK_SPACE://space bar
					//Pokemon
						if(stretchRect5.y==(stretchRect4.y+16)+0*stretchRect5.h){

					//Save
						}else if(stretchRect5.y==(stretchRect4.y+16)+1*stretchRect5.h){
							saveGame(characterRect, mapZoomRect);
					//Quit Game
						}else if(stretchRect5.y==(stretchRect4.y+16)+2*stretchRect5.h){
							quitGame = true;
					//Exit Menu
						}else if(stretchRect5.y==(stretchRect4.y+16)+3*stretchRect5.h){
							quitMenu = true;
						}
						break;
				}
				while(SDL_PollEvent(&e) != 0){		//while there remains user entered events
					if (e.type == SDL_QUIT){	//entered event is equal to pressing red x in top right hand corner of window
						quitGame = true;	//set quit to true to exit the while loop
					}
				}
			}
		}
	}
	return quitGame;
}

void saveGame(SDL_Rect characterRect, SDL_Rect mapZoomRect) {

	ofstream outFile;
	outFile.open( "savedGame.txt", ios::out );

	for(int i=0; i<4; i++){
		if(&gWalkLeft[i]==gCurrentClip){
			outFile << "Left" << endl;
			outFile << i << endl;
		}else if(&gWalkRight[i]==gCurrentClip){
			outFile << "Right" << endl;
			outFile << i << endl;
		}else if(&gWalkUp[i]==gCurrentClip){
			outFile << "Up" << endl;
			outFile << i << endl;
		}else if(&gWalkDown[i]==gCurrentClip){
			outFile << "Down" << endl;
			outFile << i << endl;
		}
	}

	outFile << characterRect.x << endl;
	outFile << characterRect.y << endl;
	outFile << mapZoomRect.x << endl;
	outFile << mapZoomRect.y << endl;

	outFile.close();
}

void loadGame(SDL_Rect **gCurrentClip, SDL_Rect *characterRect, SDL_Rect *mapZoomRect) {

	ifstream inFile;
	inFile.open( "savedGame.txt",ios::in );

	string temp1;
	int temp2;
	inFile>>temp1;
	inFile>>temp2;
	if(temp1=="Right") *gCurrentClip= &gWalkRight[temp2];
	if(temp1=="Left") *gCurrentClip= &gWalkLeft[temp2];
	if(temp1=="Up") *gCurrentClip= &gWalkUp[temp2];
	if(temp1=="Down") *gCurrentClip= &gWalkDown[temp2];

	int temp;
	inFile>>temp;
	(*characterRect).x= temp;
	inFile>>temp;
	(*characterRect).y= temp;
	inFile>>temp;
	(*mapZoomRect).x= temp;
	inFile>>temp;
	(*mapZoomRect).y= temp;

	inFile.close();

}

void battleCutScene(SDL_Rect characterRect, SDL_Rect stretch2windowRect, SDL_Rect mapZoomRect){
	SDL_Delay(500);

	for(int i=0; i<8; i++){
		if(i%2 == 0){
			SDL_FillRect( gScreenSurface, NULL, SDL_MapRGB( gScreenSurface->format, 0x00, 0x00, 0x00 ) );
		}else{
			SDL_BlitScaled(gBackground, &mapZoomRect, gScreenSurface, &stretch2windowRect);//put the background image onto gScreenSurface
			SDL_BlitScaled(gSpriteSheet, gCurrentClip, gScreenSurface, &characterRect);//put the character image onto gScreenSurface
		}
		SDL_UpdateWindowSurface(gWindow);//update the window with the current surface
		SDL_Delay(150);
	}

}

void introSequence(SDL_Rect stretch2windowRect){
	SDL_Rect torchicRect;			//rectangle used for where to place the character
	torchicRect.x = SCREEN_WIDTH;	//place at center of screen
	torchicRect.y = 2*(SCREEN_HEIGHT/3)+10;	//place at center of screen
	torchicRect.w = 50;			//size of portion of sprite sheet taken up by character
	torchicRect.h = 50;			//size of portion of sprite sheet taken up by character

	SDL_Rect stretchRect4;			//rectangle used for
	stretchRect4.w = (SCREEN_WIDTH/6)*4+20;			//size of portion of sprite sheet taken up by character
	stretchRect4.h = (SCREEN_HEIGHT/4)+10;			//size of portion of sprite sheet taken up by character
	stretchRect4.x = SCREEN_WIDTH/6;	//place at center of screen
	stretchRect4.y = SCREEN_HEIGHT/5+15;	//place at center of screen


//torchic running
for(int j=0; j<9; j++)
	for(int i=0; i<4; i++){
		SDL_BlitScaled(gIntroBackground, &gIntroTrees, gScreenSurface, &stretch2windowRect);//put the background image onto gScreenSurface
			gIntroTrees.x-=2;
			if(gIntroTrees.x<=64) gIntroTrees.x=128;
		torchicRect.x-=15;
		SDL_BlitScaled(gIntroSprites, &gTorchic[i], gScreenSurface, &torchicRect);//put the character image onto gScreenSurface
		SDL_UpdateWindowSurface(gWindow);//update the window with the current surface
		if(i==0) SDL_Delay(50);
		SDL_Delay(150);
	}
//torchic falling
for(int j=10; j>0; j-=5)
	for(int i=4; i<6; i++){
		SDL_BlitScaled(gIntroBackground, &gIntroTrees, gScreenSurface, &stretch2windowRect);//put the background image onto gScreenSurface
		gIntroTrees.x-=1;
		if(gIntroTrees.x<=64) gIntroTrees.x=128;
		torchicRect.x-=j;
		SDL_BlitScaled(gIntroSprites, &gTorchic[i], gScreenSurface, &torchicRect);//put the character image onto gScreenSurface
		SDL_UpdateWindowSurface(gWindow);//update the window with the current surface
		SDL_Delay(150);
	}
		SDL_Delay(550);
//pokemon title
for(int i=0; i<6*6; i++){
		SDL_BlitScaled(gIntroBackground, &gIntroTrees, gScreenSurface, &stretch2windowRect);//put the background image onto gScreenSurface
		gIntroTrees.x-=1;
		if(gIntroTrees.x<=64) gIntroTrees.x=128;
		SDL_BlitScaled(gIntroSprites, &gTorchic[5], gScreenSurface, &torchicRect);//put the character image onto gScreenSurface
		torchicRect.x+=3;
		SDL_BlitScaled(gIntroTitleSprites, &gPokeTitle[i%6], gScreenSurface, &stretchRect4);//put the background image onto gScreenSurface
		SDL_UpdateWindowSurface(gWindow);//update the window with the current surface
		SDL_Delay(150);

}

}

//int goFish() {
//	return
//}
