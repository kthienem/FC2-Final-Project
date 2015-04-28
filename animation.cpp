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
				.85	// pokeMart door
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
void healMyPokemon(SDL_Window *, SDL_Surface *, SDL_Surface*, SDL_Surface*, SDL_Rect*, SDL_Rect,SDL_Rect,SDL_Rect);
SDL_Rect determineWarpLoc(SDL_Rect);
void talkToPillar(SDL_Rect,int *,int *);
void dispMessage(SDL_Rect, SDL_Rect, int, int);

SDL_Window* gWindow = NULL;		//pointer to the images displayed on the screen
SDL_Surface* gScreenSurface = NULL;	//pointer to the surface containing the images to be displayed
SDL_Surface* gSpriteSheet = NULL;	//pointer to the image contatining the pieces needed for the animation
SDL_Rect* gCurrentClip = NULL;		//pointer to the current portion of gSpriteSheet being dislayed

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
			bool quit = false;		//boolean variable for when the user wants to quit
			SDL_Event e;			//variable for keyboard events entered by user
			gCurrentClip = &gWalkDown[0];	//sets first image to be used for the walking player
			int frame = 0;			//count for the current frame being used for the animation

			SDL_Rect stretchRect1;			//rectangle used for where to place the character
			stretchRect1.x = SCREEN_WIDTH/2 - 9;	//place at center of screen
			stretchRect1.y = SCREEN_HEIGHT/2 + 14;	//place at center of screen
			stretchRect1.w = 50;			//size of portion of sprite sheet taken up by character
			stretchRect1.h = 50;			//size of portion of sprite sheet taken up by character


			SDL_Rect stretchRect2;			//rectangle to stretch the background image to fit to window
			stretchRect2.x = 0;			//begin image at top corner of winow
			stretchRect2.y = 0;
			stretchRect2.w = SCREEN_WIDTH;		//make image take up the entire window
			stretchRect2.h = SCREEN_HEIGHT;

			SDL_Rect stretchRect3;			// rectangle that zooms in on current viewable map
			//starting x,y MUST be x=1+16a, y=1+16b
			stretchRect3.x = 1185;			//begin image in middle of map
			stretchRect3.y = 17;
//			stretchRect3.x = 1777;			//begin image in middle of map
//			stretchRect3.y = 625;
//			stretchRect3.x = 705;			//begin image in middle of map
//			stretchRect3.y = 257;
			stretchRect3.w = 384;
			stretchRect3.h = 288;
//PokeCenter Testing...
//		stretchRect1.x = 599;			//begin image in middle of map
//		stretchRect1.y = 462;
//		stretchRect3.x = 1841;			//begin image in middle of map
//		stretchRect3.y = 721;

			int caveChoice=0;
			int caveMapX[4]= {	1777,	1761,	1809,	1761};
			int caveMapY[4]= {	609,	721,	545,	561};
			int caveCharX[4]= {	343,	343,	375,	279};
			int caveCharY[4]= {	302,	334,	302,	302};

			int sleeptime=5000;	// sleeptime*16 == time for trainer to take one step
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

			int trainerCellx= stretchRect1.x+7;
			int trainerCelly= stretchRect1.y+16;
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
								} else {	// else, make him continue walking up
									if( stretchRect3.y-15 < topEdge ) isOB=1;
									if( isOB==0 ){
										for( int i=stretchRect3.x; i<stretchRect3.x+stretchRect3.w; i++) {
											if(white==getpixel(i,stretchRect3.y-15,gBackground)) whiteCount++;
											if( whiteCount>10 ) isOB=1;
										}
									}
									for( int i=0; i<COLOR_CELL_SIZE; i++ ){
										if( i!= POKEBALL_CELL && i!= WATER_CELL && i!= CENTER_HEALER_CELL && i!= GYM_PILLAR_CELL){
											if( i== DOOR_BLUE_N_CELL || i== DOOR_BLUE_SE_CELL || i== DOOR_GREEN_S_CELL || i== DOOR_RED_NW_CELL || i== DOOR_RED_S_CELL || i== DOOR_WOOD_CELL || i== DOOR_MART_CELL){
												if( cellColorThreshold[i] <= cellComp(trainerCellx, trainerCelly-cellShift, colorCodes[i], gScreenSurface)) {
													if( i==DOOR_MART_CELL )
														dispMessage(stretchRect1, stretchRect3, isCave, 1);
													else
														dispMessage(stretchRect1, stretchRect3, isCave, 0);
												}
											}else if( cellColorThreshold[i] <= cellComp(trainerCellx, trainerCelly-cellShift, colorCodes[i], gScreenSurface) ) {
												canWalk= 1;
											}
										}
										if( cellColorThreshold[WILD_GRASS_CELL] <= cellComp(trainerCellx, trainerCelly-cellShift, colorCodes[WILD_GRASS_CELL], gScreenSurface) ) steponWildGrass=1;
										if( cellColorThreshold[GYM_FLOOR_WARP_CELL] <= cellComp(trainerCellx, trainerCelly-cellShift, colorCodes[GYM_FLOOR_WARP_CELL], gScreenSurface) ) isWarpTile=1;
										if( cellColorThreshold[POKE_CENTER_CELL] <= cellComp(trainerCellx, trainerCelly-cellShift, colorCodes[POKE_CENTER_CELL], gScreenSurface) ) {
											isPokeCenter=1;
											enteringBuilding=1;
										}
										if( cellColorThreshold[GYM_DOOR_CELL] <= cellComp(trainerCellx, trainerCelly-cellShift, colorCodes[GYM_DOOR_CELL], gScreenSurface) ) {
											isGym=1;
											enteringBuilding=1;
										}
									}
									if( cellColorThreshold[CAVE_ENT_CELL] <= cellComp(trainerCellx, trainerCelly-cellShift, colorCodes[CAVE_ENT_CELL], gScreenSurface) ) isCave= 1;
									for( int i=0; i<16; i++){
		//use (16-i)/4 ? so that we end at zero
										if(i%4==0) frame++;
										if(frame>3) frame=0;
										gCurrentClip = &gWalkUp[frame];//set equal to proper frame of walking right sprites
										if( canWalk==1 ) {
											if(stretchRect1.y>SCREEN_HEIGHT/2+14 || isOB==1 || isPokeCenter) {
												stretchRect1.y-=2;//move char
												//NOTE: trainer is moved twice as many times as the screen. This is because the character image is shifted n "coordinates" on the window, while the screen is shifted n "pixels" on the background/map image
											} else {
												stretchRect3.y--;//move screen
											}
										}
									//	if(isPokeCenter && ! enteringBuilding) stretchRect1.y-=2;
										usleep(sleeptime);
										if( ! isCave && ! enteringBuilding ) {
											SDL_BlitScaled(gBackground, &stretchRect3, gScreenSurface, &stretchRect2);//put the background image onto gScreenSurface
											SDL_BlitScaled(gSpriteSheet, gCurrentClip, gScreenSurface, &stretchRect1);//put the character image onto gScreenSurface
											SDL_UpdateWindowSurface(gWindow);//update the window with the current surface
										}else{
											transitionGraphic(gWindow, gScreenSurface, gBackground, gSpriteSheet, gCurrentClip, stretchRect1, stretchRect2, stretchRect3, i, transitionSpeed);
										}
									}
									if(enteringBuilding){
										leaveMap1x= stretchRect1.x;
										leaveMap1y= stretchRect1.y;
										leaveMap3x= stretchRect3.x;
										leaveMap3y= stretchRect3.y;
										if(isPokeCenter) {
											stretchRect3.x = 0;	//begin image in middle of map
											stretchRect3.y = 0;
											stretchRect1.x = 360;	//place at entrance
											stretchRect1.y = 430;	//place at entrance
											gBackground = gPokeMaps[ POKE_CENTER_MAP ];
										}else if(isGym) {
											stretchRect3.x = 0;	//begin image in middle of map
											stretchRect3.y = 0;
											stretchRect1.x = 343;	//place at entrance
											stretchRect1.y = 560;	//place at entrance
											gBackground = gPokeMaps[ POKE_GYM_MAP ];
										}
										for( int i=16; i<32; i++){//indexed 16-32 in order to keep transition smooth(since we are passing in a counter and the walk up uses 0-15
											if(i%4==0) frame++;
											if(frame>3) frame=0;
											gCurrentClip = &gWalkUp[frame];//set equal to proper frame of walking right sprites
											stretchRect1.y-=2;//move screen
											usleep(sleeptime);
											transitionGraphic(gWindow, gScreenSurface, gBackground, gSpriteSheet, gCurrentClip, stretchRect1, stretchRect2, stretchRect3, i, transitionSpeed);
										}
									}
									if(isCave) {
										if(stretchRect3.x<=833) {
											dispMessage(stretchRect1, stretchRect3, isCave, 0);
											SDL_Delay(3000);
										}else{
											int prevCave= caveChoice;
											do{
												caveChoice= rand()%4;
											}while(prevCave==caveChoice);
											stretchRect1.x= caveCharX[caveChoice];
											stretchRect1.y= caveCharY[caveChoice];
											stretchRect3.x= caveMapX[caveChoice];
											stretchRect3.y= caveMapY[caveChoice];
										} 
										for( int i=16; i<32; i++){//indexed 16-32 in order to keep transition smooth(since we are passing in a counter and the walk up uses 0-15
											if(i%4==0) frame++;
											if(frame>3) frame=0;
											gCurrentClip = &gWalkDown[frame];//set equal to proper frame of walking right sprites
											if( canWalk==1 ) {
												if(stretchRect1.y<SCREEN_HEIGHT/2+14 || isOB==1 || isGym || isPokeCenter) {
													stretchRect1.y+=2;//move char
													//NOTE: trainer is moved twice as many times as the screen. This is because the character image is shifted n "coordinates" on the window, while the screen is shifted n "pixels" on the background/map image
												} else {
													stretchRect3.y++;//move screen
												}
											}
											usleep(sleeptime);
											transitionGraphic(gWindow, gScreenSurface, gBackground, gSpriteSheet, gCurrentClip, stretchRect1, stretchRect2, stretchRect3, i, transitionSpeed);
										}
										SDL_BlitScaled(gBackground, &stretchRect3, gScreenSurface, &stretchRect2);//put the background image onto gScreenSurface
										SDL_BlitScaled(gSpriteSheet, gCurrentClip, gScreenSurface, &stretchRect1);//put the character image onto gScreenSurface
										SDL_UpdateWindowSurface(gWindow);//update the window with the current surface

									}
								}
								break;
							
							case SDLK_DOWN://down arrow key
								if( gCurrentClip!=&gWalkDown[0] ) {	// ie. if he is NOT already walking down, change the direction he is facing to down
									gCurrentClip= &gWalkDown[0];
								} else {	// else, make him continue walking down
									if( stretchRect3.y+stretchRect3.h+15 > botEdge ) isOB=1;
									if( isOB==0 ){
										for( int i=stretchRect3.x; i<stretchRect3.x+stretchRect3.w; i++) {
											if(white==getpixel(i,stretchRect3.y+stretchRect3.h+15,gBackground)) whiteCount++;
											if( whiteCount>10 ) isOB=1;
										}
									}
									for( int i=0; i<COLOR_CELL_SIZE; i++ )
										if( i!= POKEBALL_CELL && i!= WATER_CELL && i!= CAVE_ENT_CELL && i!=GYM_PILLAR_CELL)
											if( cellColorThreshold[i] <= cellComp(trainerCellx, trainerCelly+cellShift, colorCodes[i], gScreenSurface) ) canWalk= 1;
									if( cellColorThreshold[WILD_GRASS_CELL] <= cellComp(trainerCellx, trainerCelly+cellShift, colorCodes[WILD_GRASS_CELL], gScreenSurface) ) steponWildGrass=1;
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
											if(isOB==1 || stretchRect1.y<SCREEN_HEIGHT/2+14 || isGym || isPokeCenter) {
												stretchRect1.y+=2;//move char
											} else {
												stretchRect3.y++;//move screen
											}
										}
									//	if(isPokeCenter) stretchRect1.y+=2;
										usleep(sleeptime);
										if( ! exitingBuilding ) {
											SDL_BlitScaled(gBackground, &stretchRect3, gScreenSurface, &stretchRect2);//put the background image onto gScreenSurface
											SDL_BlitScaled(gSpriteSheet, gCurrentClip, gScreenSurface, &stretchRect1);//put the character image onto gScreenSurface
											SDL_UpdateWindowSurface(gWindow);//update the window with the current surface
										}else{
											transitionGraphic(gWindow, gScreenSurface, gBackground, gSpriteSheet, gCurrentClip, stretchRect1, stretchRect2, stretchRect3, i, transitionSpeed);
										}
									}
									if(exitingBuilding) {
										gBackground = gPokeMaps[ POKE_MAP_ROUTE1 ];
										stretchRect1.x= leaveMap1x;
										stretchRect1.y= leaveMap1y;
										stretchRect3.x= leaveMap3x;
										stretchRect3.y= leaveMap3y;
										for( int i=16; i<32; i++){//indexed 16-32 in order to keep transition smooth(since we are passing in a counter and the walk up uses 0-15
											if(i%4==0) frame++;
											if(frame>3) frame=0;
											gCurrentClip = &gWalkDown[frame];//set equal to proper frame of walking right sprites
											stretchRect1.y+=2;//move char
											usleep(sleeptime);
											transitionGraphic(gWindow, gScreenSurface, gBackground, gSpriteSheet, gCurrentClip, stretchRect1, stretchRect2, stretchRect3, i, transitionSpeed);
										}

										SDL_BlitScaled(gBackground, &stretchRect3, gScreenSurface, &stretchRect2);//put the background image onto gScreenSurface
										SDL_BlitScaled(gSpriteSheet, gCurrentClip, gScreenSurface, &stretchRect1);//put the character image onto gScreenSurface
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
									if( stretchRect3.x-15 < leftEdge ) isOB=1;
									if( isOB==0 ) {
										for( int j=stretchRect3.y; j<stretchRect3.y+stretchRect3.h; j++) {
											if(white==getpixel(stretchRect3.x-15,j,gBackground)) whiteCount++;
											if( whiteCount>10 ) isOB=1;
										}
									}
									for( int i=0; i<COLOR_CELL_SIZE; i++ )
										if( i!= POKEBALL_CELL && i!= WATER_CELL && i!= CAVE_ENT_CELL && i!=GYM_PILLAR_CELL)
											if( cellColorThreshold[i] <= cellComp(trainerCellx-cellShift, trainerCelly, colorCodes[i], gScreenSurface) ) canWalk= 1;
									if( cellColorThreshold[WILD_GRASS_CELL] <= cellComp(trainerCellx-cellShift, trainerCelly, colorCodes[WILD_GRASS_CELL], gScreenSurface) ) steponWildGrass=1;
									if( cellColorThreshold[GYM_FLOOR_WARP_CELL] <= cellComp(trainerCellx-cellShift, trainerCelly, colorCodes[GYM_FLOOR_WARP_CELL], gScreenSurface) ) isWarpTile=1;
									for( int i=0; i<16; i++){
										if(i%4==0) frame++;
										if(frame>3) frame=0;
										gCurrentClip = &gWalkLeft[frame];//set equal to proper frame of walking right sprites
										if( canWalk==1 ) {
											if(isOB==1 || stretchRect1.x>SCREEN_WIDTH/2-9 || isGym || isPokeCenter) {
												stretchRect1.x-=2;//move char
											} else { 
												stretchRect3.x--;//move screen
											}
										}
									//	if(isPokeCenter) stretchRect1.x-=2;
										usleep(sleeptime);
										SDL_BlitScaled(gBackground, &stretchRect3, gScreenSurface, &stretchRect2);//put the background image onto gScreenSurface
										SDL_BlitScaled(gSpriteSheet, gCurrentClip, gScreenSurface, &stretchRect1);//put the character image onto gScreenSurface
										SDL_UpdateWindowSurface(gWindow);//update the window with the current surface
									}
								}
								break;

							case SDLK_RIGHT://right arrow key
								if( gCurrentClip!=&gWalkRight[0] ) {	// ie. if he is NOT already walking right, change the direction he is facing to right
									gCurrentClip= &gWalkRight[0];
								} else {	// else, make him continue walking right
									if( stretchRect3.x+stretchRect3.w+15 > rightEdge ) isOB=1;
									if( isOB==0 ) {
										for( int j=stretchRect3.y; j<stretchRect3.y+stretchRect3.h; j++) {
											if(white==getpixel(stretchRect3.x+stretchRect3.w+15,j,gBackground)) whiteCount++;
											if( whiteCount>10 ) isOB=1;
										}
									}
									for( int i=0; i<COLOR_CELL_SIZE; i++ )
										if( i!= POKEBALL_CELL && i!= WATER_CELL && i!= CAVE_ENT_CELL && i!= GYM_PILLAR_CELL)
											if( cellColorThreshold[i] <= cellComp(trainerCellx+cellShift, trainerCelly, colorCodes[i], gScreenSurface) ) canWalk= 1;
									if( cellColorThreshold[WILD_GRASS_CELL] <= cellComp(trainerCellx+cellShift, trainerCelly, colorCodes[WILD_GRASS_CELL], gScreenSurface) ) steponWildGrass=1;
									if( cellColorThreshold[GYM_FLOOR_WARP_CELL] <= cellComp(trainerCellx+cellShift, trainerCelly, colorCodes[GYM_FLOOR_WARP_CELL], gScreenSurface) ) isWarpTile=1;
									for( int i=0; i<16; i++){
										if(i%4==0) frame++;
										if(frame>3) frame=0;
										gCurrentClip = &gWalkRight[frame];//set equal to proper frame of walking right sprites
										if(canWalk==1){
											if(isOB==1 || stretchRect1.x<SCREEN_WIDTH/2-9 || isGym || isPokeCenter) {
												stretchRect1.x+=2;
											} else {
												stretchRect3.x++;//move screen
											}
										}
									//	if(isPokeCenter) stretchRect1.x+=2;
										usleep(sleeptime);
										SDL_BlitScaled(gBackground, &stretchRect3, gScreenSurface, &stretchRect2);//put the background image onto gScreenSurface
										SDL_BlitScaled(gSpriteSheet, gCurrentClip, gScreenSurface, &stretchRect1);//put the character image onto gScreenSurface
										SDL_UpdateWindowSurface(gWindow);//update the window with the current surface
									}
								}
								break;

							case SDLK_SPACE://space bar
			//					writeColorCodes(trainerCellx,trainerCelly-cellShift, gScreenSurface);
								//if hes facing up and the cell he is facing is the center healer
								if( gCurrentClip==&gWalkUp[0] && cellColorThreshold[CENTER_HEALER_CELL] <= cellComp(trainerCellx, trainerCelly-cellShift, colorCodes[CENTER_HEALER_CELL], gScreenSurface) )
									healMyPokemon(gWindow, gScreenSurface, gBackground, gSpriteSheet, gCurrentClip, stretchRect1, stretchRect2, stretchRect3);

								if( gCurrentClip==&gWalkUp[0] && cellColorThreshold[GYM_PILLAR_CELL] <= cellComp(trainerCellx, trainerCelly-cellShift, colorCodes[GYM_PILLAR_CELL], gScreenSurface) )
									talkToPillar(stretchRect1, &gymAnswers, &firstPillarx);

				cout<<"MART:		"<<cellComp(trainerCellx, trainerCelly-cellShift, colorCodes[DOOR_MART_CELL], gScreenSurface)<<endl;
								break;
				
							default://do nothing when any other keys are pressed
								break;
						}
	cout<<"Map:\n";
	cout<<"x: "<<stretchRect3.x<<endl;
	cout<<"y: "<<stretchRect3.y<<endl;
	cout<<"Trainer:\n";
	cout<<"x: "<<stretchRect1.x<<endl;
	cout<<"y: "<<stretchRect1.y<<endl;

						trainerCellx= stretchRect1.x+7;
						trainerCelly= stretchRect1.y+16;

//  write values for block to right
	cout<<"floor1:		"<<cellComp(trainerCellx+cellShift, trainerCelly, colorCodes[GYM_FLOOR1_CELL], gScreenSurface)<<endl;
	cout<<"shadow:		"<<cellComp(trainerCellx+cellShift, trainerCelly, colorCodes[GYM_FLOOR_SHADOW_CELL], gScreenSurface)<<endl;
	cout<<"warp:		"<<cellComp(trainerCellx, trainerCelly+cellShift, colorCodes[GYM_FLOOR_WARP_CELL], gScreenSurface)<<endl;
	cout<<"pillar:	"<<cellComp(trainerCellx+cellShift, trainerCelly, colorCodes[GYM_PILLAR_CELL], gScreenSurface)<<endl;
	cout<<"exit:	"<<cellComp(trainerCellx+cellShift, trainerCelly, colorCodes[GYM_FLOOR_EXIT_CELL], gScreenSurface)<<endl;
	cout<<"path:		"<<cellComp(trainerCellx+cellShift, trainerCelly, colorCodes[PATH_CELL], gScreenSurface)<<endl;
	cout<<"plateau:	"<<cellComp(trainerCellx+cellShift, trainerCelly, colorCodes[PLATEAU_CELL], gScreenSurface)<<endl;
	cout<<"platform:	"<<cellComp(trainerCellx+cellShift, trainerCelly, colorCodes[PLATFORM_CELL], gScreenSurface)<<endl;
	cout<<"pokeball:	"<<cellComp(trainerCellx+cellShift, trainerCelly, colorCodes[POKEBALL_CELL], gScreenSurface)<<endl;
	cout<<"skinnypath:	"<<cellComp(trainerCellx+cellShift, trainerCelly, colorCodes[SKINNYPATH_CELL], gScreenSurface)<<endl;
	cout<<"stairs:		"<<cellComp(trainerCellx+cellShift, trainerCelly, colorCodes[STAIRS_CELL], gScreenSurface)<<endl;
	cout<<"water:		"<<cellComp(trainerCellx+cellShift, trainerCelly, colorCodes[WATER_CELL], gScreenSurface)<<endl;
	cout<<"wild grass2:	"<<cellComp(trainerCellx+cellShift, trainerCelly, colorCodes[WILD_GRASS_2_CELL], gScreenSurface)<<endl;
	cout<<"wild grass:	"<<cellComp(trainerCellx+cellShift, trainerCelly, colorCodes[WILD_GRASS_CELL], gScreenSurface)<<endl;

	if(steponWildGrass){
		cout<<"WATCHOUT!\n";
		if(rand()%100 < 15) cout<<"Pokemon Encounter!"<<endl;
//transparency
//		transitionGraphic(gWindow, gScreenSurface, gBackground, gSpriteSheet, gCurrentClip, stretchRect1, stretchRect2, stretchRect3);
	}

	if(isWarpTile==1) {
		frame= 0;
		for( int i=0; i<32; i++){//indexed 16-32 in order to keep transition smooth(since we are passing in a counter and the walk up uses 0-15
			if(i%4==0) gCurrentClip = &gWalkLeft[frame];//set equal to proper frame of walking right sprites
			if(i%4==1) gCurrentClip = &gWalkUp[frame];//set equal to proper frame of walking right sprites
			if(i%4==2) gCurrentClip = &gWalkRight[frame];//set equal to proper frame of walking right sprites
			if(i%4==3) gCurrentClip = &gWalkDown[frame];//set equal to proper frame of walking right sprites
			usleep(warptime);
			transitionGraphic(gWindow, gScreenSurface, gBackground, gSpriteSheet, gCurrentClip, stretchRect1, stretchRect2, stretchRect3, i, transitionSpeed);

			if(i==15) {
				stretchRect1= determineWarpLoc(stretchRect1);
			}
		}
	}

						//This will save all events (to e) that occured while the trainer was taking a step. As long as the event was not a quit, then the event will be arbitrarily stored and then overwritten. This is done so that the user playing may hold down an arrow key, and then the trainer will stop walking as soon as the user releases the key 
						while(SDL_PollEvent(&e) != 0){//while there remains user entered events
							if (e.type == SDL_QUIT){//entered event is equal to pressing red x in top right hand corner of window
								quit = true;//set quit to true to exit the while loop
							}
						}
					}
					if(isPokeCenter)
						gBackground = gPokeMaps[ POKE_CENTER_MAP ];
					else if(isGym)
						if(gymAnswers==2)
							gBackground = gPokeMaps[ POKE_GYM_ANSWERS_MAP ];
						else
							gBackground = gPokeMaps[ POKE_GYM_MAP ];
					else
						gBackground = gPokeMaps[ POKE_MAP_ROUTE1 ];

					SDL_BlitScaled(gBackground, &stretchRect3, gScreenSurface, &stretchRect2);//put the background image onto gScreenSurface
					SDL_BlitScaled(gSpriteSheet, gCurrentClip, gScreenSurface, &stretchRect1);//put the character image onto gScreenSurface

/*	for( int i=0; i<32; i++ ) putpixel( 2*cellShift+trainerCellx+i,trainerCelly,0xff0000 );	
	for( int i=0; i<32; i++ ) putpixel( 2*cellShift+trainerCellx,trainerCelly+i,0xff0000 );	
	for( int i=0; i<32; i++ ) putpixel( 2*cellShift+trainerCellx+i,trainerCelly+32,0xff0000 );	
	for( int i=0; i<32; i++ ) putpixel( 2*cellShift+trainerCellx+32,trainerCelly+i,0xff0000 );	
*/

					SDL_UpdateWindowSurface(gWindow);//update the window with the current surface


					frame=0;//reset frame for updated animation
					isOB=0;
					whiteCount=0;
					steponWildGrass=0;
					canWalk=0;
					isCave=0;
					//isPokeCenter=0;
					enteringBuilding=0;
					exitingBuilding=0;
					isWarpTile=0;
				}
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
	outFile.open( "doorMart.txt", ios::out );
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

void transitionGraphic(SDL_Window *window, SDL_Surface *screen, SDL_Surface *gBackground, SDL_Surface *gSpriteSheet, SDL_Rect *gCurrentClip, SDL_Rect stretchRect1, SDL_Rect stretchRect2, SDL_Rect stretchRect3, int counter, int counterJump) {
	SDL_Surface* transsurface;
	for( int i=(16*counter); i<=(16*counter+16); i+=counterJump ){
		#if SDL_BYTEORDER == SDL_BIG_ENDIAN
			transsurface = SDL_CreateRGBSurface(SDL_SWSURFACE,stretchRect2.w,stretchRect2.h,32, 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF);
		#else
			transsurface = SDL_CreateRGBSurface(SDL_SWSURFACE,stretchRect2.w,stretchRect2.h,32, 0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000);
		#endif
		SDL_FillRect(transsurface, NULL, SDL_MapRGBA(transsurface->format, 0, 0, 0, 255-abs(255-i) ) );
		SDL_BlitScaled(gBackground, &stretchRect3, gScreenSurface, &stretchRect2);//put the background image onto gScreenSurface
		SDL_BlitScaled(gSpriteSheet, gCurrentClip, gScreenSurface, &stretchRect1);//put the character image onto gScreenSurface
		SDL_BlitSurface(transsurface,NULL,screen,&stretchRect2);
		SDL_UpdateWindowSurface(window);//update the window with the current surface
		if(255-abs(255-i) > 200) SDL_Delay(1);	// extend pause when screen is at its darkest
	}
}

void healMyPokemon(SDL_Window *window, SDL_Surface *screen, SDL_Surface *gBackground, SDL_Surface *gSpriteSheet, SDL_Rect *gCurrentClip, SDL_Rect stretchRect1, SDL_Rect stretchRect2, SDL_Rect stretchRect3) {
	
			gBackground = gPokeMaps[ POKE_CENTER_YES_MAP ];
			SDL_BlitScaled(gBackground, &stretchRect3, gScreenSurface, &stretchRect2);//put the background image onto gScreenSurface
			SDL_BlitScaled(gSpriteSheet, gCurrentClip, gScreenSurface, &stretchRect1);//put the character image onto gScreenSurface
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
								SDL_BlitScaled(gBackground, &stretchRect3, gScreenSurface, &stretchRect2);//put the background image onto gScreenSurface
								SDL_BlitScaled(gSpriteSheet, gCurrentClip, gScreenSurface, &stretchRect1);//put the character image onto gScreenSurface
								SDL_UpdateWindowSurface(window);//update the window with the current surface
								break;
							case SDLK_LEFT://left arrow key
								gBackground = gPokeMaps[ POKE_CENTER_YES_MAP ];
								SDL_BlitScaled(gBackground, &stretchRect3, gScreenSurface, &stretchRect2);//put the background image onto gScreenSurface
								SDL_BlitScaled(gSpriteSheet, gCurrentClip, gScreenSurface, &stretchRect1);//put the character image onto gScreenSurface
								SDL_UpdateWindowSurface(window);//update the window with the current surface
								break;
							case SDLK_SPACE://space bar
								if( gBackground == gPokeMaps[ POKE_CENTER_YES_MAP ] ) {
									gBackground = gPokeMaps[ POKE_CENTER_HEALING_MAP ];
									SDL_BlitScaled(gBackground, &stretchRect3, gScreenSurface, &stretchRect2);//put the background image onto gScreenSurface
									SDL_BlitScaled(gSpriteSheet, gCurrentClip, gScreenSurface, &stretchRect1);//put the character image onto gScreenSurface
									SDL_UpdateWindowSurface(window);//update the window with the current surface
									SDL_Delay(2000);
									for(int i=0; i<32; i++)
										transitionGraphic(gWindow, gScreenSurface, gBackground, gSpriteSheet, gCurrentClip, stretchRect1, stretchRect2, stretchRect3, i, 3);
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

SDL_Rect determineWarpLoc( SDL_Rect stretchRect1 ) {
	int gymWarpRow[6]={	80,	144,	272,	336,	464,	528};		// row holds Y coords
	int gymWarpCol[7]={	23,	183,	279,	407,	471,	535,	695};	// col holds X coords

	if(stretchRect1.y==gymWarpRow[0]){
		if(stretchRect1.x==gymWarpCol[0]){
			stretchRect1.y= gymWarpRow[1];
			stretchRect1.x= gymWarpCol[5];
		}
		else if(stretchRect1.x==gymWarpCol[1]){
			stretchRect1.y= gymWarpRow[0];
			stretchRect1.x= gymWarpCol[3];
		}
		else if(stretchRect1.x==gymWarpCol[2]){
			stretchRect1.y= gymWarpRow[3];
			stretchRect1.x= gymWarpCol[5];
		}
		else if(stretchRect1.x==gymWarpCol[3]){
			stretchRect1.y= gymWarpRow[0];
			stretchRect1.x= gymWarpCol[1];
		}
		else if(stretchRect1.x==gymWarpCol[5]){
			stretchRect1.y= gymWarpRow[2];
			stretchRect1.x= gymWarpCol[5];
		}
		else if(stretchRect1.x==gymWarpCol[6]){
			stretchRect1.y= gymWarpRow[4];
			stretchRect1.x= gymWarpCol[5];
		}
	}else if(stretchRect1.y==gymWarpRow[1]){
		if(stretchRect1.x==gymWarpCol[0]){
			stretchRect1.y= gymWarpRow[3];
			stretchRect1.x= gymWarpCol[4];
		}
		else if(stretchRect1.x==gymWarpCol[1]){
			stretchRect1.y= gymWarpRow[3];
			stretchRect1.x= gymWarpCol[0];
		}
		else if(stretchRect1.x==gymWarpCol[2]){
			stretchRect1.y= gymWarpRow[2];
			stretchRect1.x= gymWarpCol[1];
		}
		else if(stretchRect1.x==gymWarpCol[3]){
			stretchRect1.y= gymWarpRow[5];
			stretchRect1.x= gymWarpCol[0];
		}
		else if(stretchRect1.x==gymWarpCol[5]){
			stretchRect1.y= gymWarpRow[0];
			stretchRect1.x= gymWarpCol[0];
		}
		else if(stretchRect1.x==gymWarpCol[6]){
			stretchRect1.y= gymWarpRow[4];
			stretchRect1.x= gymWarpCol[0];
		}
	}else if(stretchRect1.y==gymWarpRow[2]){
		if(stretchRect1.x==gymWarpCol[0]){
			stretchRect1.y= gymWarpRow[2];
			stretchRect1.x= gymWarpCol[7];
		}
		else if(stretchRect1.x==gymWarpCol[1]){
			stretchRect1.y= gymWarpRow[1];
			stretchRect1.x= gymWarpCol[2];
		}
		else if(stretchRect1.x==gymWarpCol[5]){
			stretchRect1.y= gymWarpRow[0];
			stretchRect1.x= gymWarpCol[5];
		}
		else if(stretchRect1.x==gymWarpCol[6]){
			stretchRect1.y= gymWarpRow[4];
			stretchRect1.x= gymWarpCol[6];
		}
	}else if(stretchRect1.y==gymWarpRow[3]){
		if(stretchRect1.x==gymWarpCol[0]){
			stretchRect1.y= gymWarpRow[1];
			stretchRect1.x= gymWarpCol[1];
		}
		else if(stretchRect1.x==gymWarpCol[1]){
			stretchRect1.y= gymWarpRow[5];
			stretchRect1.x= gymWarpCol[1];
		}
		else if(stretchRect1.x==gymWarpCol[4]){	// Make warp tile from leader go back to entrance warp
			stretchRect1.y= gymWarpRow[4];
			stretchRect1.x= gymWarpCol[4];
		}
		else if(stretchRect1.x==gymWarpCol[5]){
			stretchRect1.y= gymWarpRow[0];
			stretchRect1.x= gymWarpCol[2];
		}
		else if(stretchRect1.x==gymWarpCol[6]){
			stretchRect1.y= gymWarpRow[2];
			stretchRect1.x= gymWarpCol[0];
		}
	}else if(stretchRect1.y==gymWarpRow[4]){
		if(stretchRect1.x==gymWarpCol[0]){
			stretchRect1.y= gymWarpRow[1];
			stretchRect1.x= gymWarpCol[6];
		}
		else if(stretchRect1.x==gymWarpCol[1]){
			stretchRect1.y= gymWarpRow[5];
			stretchRect1.x= gymWarpCol[5];
		}
		else if(stretchRect1.x==gymWarpCol[4]){
cout<<"halp"<<endl;
			stretchRect1.y= gymWarpRow[5];
			stretchRect1.x= gymWarpCol[6];
		}
		else if(stretchRect1.x==gymWarpCol[5]){
			stretchRect1.y= gymWarpRow[0];
			stretchRect1.x= gymWarpCol[6];
		}
		else if(stretchRect1.x==gymWarpCol[6]){
			stretchRect1.y= gymWarpRow[2];
			stretchRect1.x= gymWarpCol[6];
		}
	}else if(stretchRect1.y==gymWarpRow[5]){
		if(stretchRect1.x==gymWarpCol[0]){
			stretchRect1.y= gymWarpRow[1];
			stretchRect1.x= gymWarpCol[3];
		}
		else if(stretchRect1.x==gymWarpCol[1]){
			stretchRect1.y= gymWarpRow[3];
			stretchRect1.x= gymWarpCol[1];
		}
		else if(stretchRect1.x==gymWarpCol[5]){
			stretchRect1.y= gymWarpRow[4];
			stretchRect1.x= gymWarpCol[1];
		}
		else if(stretchRect1.x==gymWarpCol[6]){
			stretchRect1.y= gymWarpRow[4];
			stretchRect1.x= gymWarpCol[4];
		}
	}

	return(stretchRect1);

}

void talkToPillar(SDL_Rect stretchRect1, int *gymAnswers, int *firstPillarx ) {
	if(*gymAnswers==0){
		*firstPillarx= stretchRect1.x;
		(*gymAnswers)++;
	}else if(*gymAnswers==1) {
		if( stretchRect1.x!=*firstPillarx ){
			(*gymAnswers)++;
		}
	}
}

void dispMessage(SDL_Rect stretchRect1, SDL_Rect stretchRect3, int isCave, int isMart) {

	SDL_Rect stretchRect4;			//rectangle used for
	stretchRect4.w = (SCREEN_WIDTH/6)*4;			//size of portion of sprite sheet taken up by character
	stretchRect4.h = (SCREEN_HEIGHT/4);			//size of portion of sprite sheet taken up by character
	stretchRect4.x = SCREEN_WIDTH/6;	//place at center of screen
	if(stretchRect1.y>SCREEN_WIDTH/2)
		stretchRect4.y = SCREEN_HEIGHT/15;	//place at center of screen
	else
		stretchRect4.y = 2*(SCREEN_HEIGHT/3);	//place at center of screen
		
	if(isCave)
		SDL_BlitScaled(gPokeMaps[ POKE_MESSAGE_CAVE ], NULL, gScreenSurface, &stretchRect4);
	else if(isMart)
		SDL_BlitScaled(gPokeMaps[ POKE_MESSAGE_MART ], NULL, gScreenSurface, &stretchRect4);
	else if(stretchRect3.x<529)
		SDL_BlitScaled(gPokeMaps[ POKE_MESSAGE_1 ], NULL, gScreenSurface, &stretchRect4);
	else if(stretchRect1.x>535){
		SDL_BlitScaled(gPokeMaps[ POKE_MESSAGE_6a ], NULL, gScreenSurface, &stretchRect4);
		SDL_UpdateWindowSurface(gWindow);//update the window with the current surface
		SDL_Delay(2000);
		SDL_BlitScaled(gPokeMaps[ POKE_MESSAGE_6b ], NULL, gScreenSurface, &stretchRect4);
	}else{
		int message= (rand()%((POKE_MESSAGE_5-POKE_MESSAGE_2)+1)) + POKE_MESSAGE_2;
		SDL_BlitScaled(gPokeMaps[ message ], NULL, gScreenSurface, &stretchRect4);
	}
	SDL_UpdateWindowSurface(gWindow);//update the window with the current surface
	SDL_Delay(3000);
}
