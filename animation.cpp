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
	COLOR_CELL_SIZE
};

double cellColorThreshold[30]={	.55,	// bridge
				.85,	// cave entrance
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
				.9	// center healer
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

void transitionGraphic(SDL_Window *, SDL_Surface *, SDL_Surface*, SDL_Surface*, SDL_Rect*, SDL_Rect,SDL_Rect,SDL_Rect, int);

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
			stretchRect3.x = 1841;			//begin image in middle of map
			stretchRect3.y = 385;
//			stretchRect3.x = 1777;			//begin image in middle of map
//			stretchRect3.y = 625;
//			stretchRect3.x = 705;			//begin image in middle of map
//			stretchRect3.y = 257;
			stretchRect3.w = 384;
			stretchRect3.h = 288;
//PokeCenter Testing...
		stretchRect1.x = 599;			//begin image in middle of map
		stretchRect1.y = 462;
		stretchRect3.x = 1841;			//begin image in middle of map
		stretchRect3.y = 721;

			int caveChoice=0;
			int caveMapX[4]= {	1777,	1761,	1809,	1761};
			int caveMapY[4]= {	609,	721,	545,	561};
			int caveCharX[4]= {	343,	343,	375,	279};
			int caveCharY[4]= {	302,	334,	302,	302};

			int sleeptime=5000;	// sleeptime*16 == time for trainer to take one step
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
			int enteringCenter= 0;
			int exitingCenter= 0;

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
										if( i!= POKEBALL_CELL && i!= WATER_CELL )
											if( cellColorThreshold[i] <= cellComp(trainerCellx, trainerCelly-cellShift, colorCodes[i], gScreenSurface) ) {
												canWalk= 1;
											}
										if( cellColorThreshold[WILD_GRASS_CELL] <= cellComp(trainerCellx, trainerCelly-cellShift, colorCodes[WILD_GRASS_CELL], gScreenSurface) ) steponWildGrass=1;
										if( cellColorThreshold[POKE_CENTER_CELL] <= cellComp(trainerCellx, trainerCelly-cellShift, colorCodes[POKE_CENTER_CELL], gScreenSurface) ) {
											isPokeCenter=1;
											enteringCenter=1;
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
									//	if(isPokeCenter && ! enteringCenter) stretchRect1.y-=2;
										usleep(sleeptime);
										if( ! isCave && ! enteringCenter ) {
											SDL_BlitScaled(gBackground, &stretchRect3, gScreenSurface, &stretchRect2);//put the background image onto gScreenSurface
											SDL_BlitScaled(gSpriteSheet, gCurrentClip, gScreenSurface, &stretchRect1);//put the character image onto gScreenSurface
											SDL_UpdateWindowSurface(gWindow);//update the window with the current surface
										}else{
											transitionGraphic(gWindow, gScreenSurface, gBackground, gSpriteSheet, gCurrentClip, stretchRect1, stretchRect2, stretchRect3, i);
										}
									}
									if(enteringCenter){
										leaveMap1x= stretchRect1.x;
										leaveMap1y= stretchRect1.y;
										leaveMap3x= stretchRect3.x;
										leaveMap3y= stretchRect3.y;
										stretchRect3.x = 0;	//begin image in middle of map
										stretchRect3.y = 0;
										stretchRect1.x = 360;	//place at entrance
										stretchRect1.y = 430;	//place at entrance
										gBackground = gPokeMaps[ POKE_CENTER_MAP ];
										for( int i=16; i<32; i++){//indexed 16-32 in order to keep transition smooth(since we are passing in a counter and the walk up uses 0-15
											if(i%4==0) frame++;
											if(frame>3) frame=0;
											gCurrentClip = &gWalkUp[frame];//set equal to proper frame of walking right sprites
											stretchRect1.y-=2;//move screen
											usleep(sleeptime);
											transitionGraphic(gWindow, gScreenSurface, gBackground, gSpriteSheet, gCurrentClip, stretchRect1, stretchRect2, stretchRect3, i);
										}
									}
									if(isCave) {
										int prevCave= caveChoice;
										do{
											caveChoice= rand()%4;
										}while(prevCave==caveChoice);
										stretchRect1.x= caveCharX[caveChoice];
										stretchRect1.y= caveCharY[caveChoice];
										stretchRect3.x= caveMapX[caveChoice];
										stretchRect3.y= caveMapY[caveChoice];
										for( int i=16; i<32; i++){//indexed 16-32 in order to keep transition smooth(since we are passing in a counter and the walk up uses 0-15
											if(i%4==0) frame++;
											if(frame>3) frame=0;
											gCurrentClip = &gWalkDown[frame];//set equal to proper frame of walking right sprites
											if( canWalk==1 ) {
												if(stretchRect1.y<SCREEN_HEIGHT/2+14 || isOB==1 || isPokeCenter) {
													stretchRect1.y+=2;//move char
													//NOTE: trainer is moved twice as many times as the screen. This is because the character image is shifted n "coordinates" on the window, while the screen is shifted n "pixels" on the background/map image
												} else {
													stretchRect3.y++;//move screen
												}
											}
											usleep(sleeptime);
											transitionGraphic(gWindow, gScreenSurface, gBackground, gSpriteSheet, gCurrentClip, stretchRect1, stretchRect2, stretchRect3, i);
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
										if( i!= POKEBALL_CELL && i!= WATER_CELL && i!= CAVE_ENT_CELL )
											if( cellColorThreshold[i] <= cellComp(trainerCellx, trainerCelly+cellShift, colorCodes[i], gScreenSurface) ) canWalk= 1;
									if( cellColorThreshold[WILD_GRASS_CELL] <= cellComp(trainerCellx, trainerCelly+cellShift, colorCodes[WILD_GRASS_CELL], gScreenSurface) ) steponWildGrass=1;
									if( cellColorThreshold[CENTER_EXIT_CELL] <= cellComp(trainerCellx, trainerCelly+cellShift, colorCodes[CENTER_EXIT_CELL], gScreenSurface) ) exitingCenter=1;
									for( int i=0; i<16; i++){
										if(i%4==0) frame++;
										if(frame>3) frame=0;
										gCurrentClip = &gWalkDown[frame];//set equal to proper frame of walking right sprites
										if( canWalk==1 ) {
											if(isOB==1 || stretchRect1.y<SCREEN_HEIGHT/2+14 || isPokeCenter) {
												stretchRect1.y+=2;//move char
											} else {
												stretchRect3.y++;//move screen
											}
										}
									//	if(isPokeCenter) stretchRect1.y+=2;
										usleep(sleeptime);
										if( ! exitingCenter ) {
											SDL_BlitScaled(gBackground, &stretchRect3, gScreenSurface, &stretchRect2);//put the background image onto gScreenSurface
											SDL_BlitScaled(gSpriteSheet, gCurrentClip, gScreenSurface, &stretchRect1);//put the character image onto gScreenSurface
											SDL_UpdateWindowSurface(gWindow);//update the window with the current surface
										}else{
											transitionGraphic(gWindow, gScreenSurface, gBackground, gSpriteSheet, gCurrentClip, stretchRect1, stretchRect2, stretchRect3, i);
										}
									}
									if(exitingCenter) {
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
											transitionGraphic(gWindow, gScreenSurface, gBackground, gSpriteSheet, gCurrentClip, stretchRect1, stretchRect2, stretchRect3, i);
										}

										SDL_BlitScaled(gBackground, &stretchRect3, gScreenSurface, &stretchRect2);//put the background image onto gScreenSurface
										SDL_BlitScaled(gSpriteSheet, gCurrentClip, gScreenSurface, &stretchRect1);//put the character image onto gScreenSurface
										SDL_UpdateWindowSurface(gWindow);//update the window with the current surface
										isPokeCenter= 0;
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
										if( i!= POKEBALL_CELL && i!= WATER_CELL && i!= CAVE_ENT_CELL )
											if( cellColorThreshold[i] <= cellComp(trainerCellx-cellShift, trainerCelly, colorCodes[i], gScreenSurface) ) canWalk= 1;
									if( cellColorThreshold[WILD_GRASS_CELL] <= cellComp(trainerCellx-cellShift, trainerCelly, colorCodes[WILD_GRASS_CELL], gScreenSurface) ) steponWildGrass=1;
									for( int i=0; i<16; i++){
										if(i%4==0) frame++;
										if(frame>3) frame=0;
										gCurrentClip = &gWalkLeft[frame];//set equal to proper frame of walking right sprites
										if( canWalk==1 ) {
											if(isOB==1 || stretchRect1.x>SCREEN_WIDTH/2-9 || isPokeCenter) {
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
										if( i!= POKEBALL_CELL && i!= WATER_CELL && i!= CAVE_ENT_CELL )
											if( cellColorThreshold[i] <= cellComp(trainerCellx+cellShift, trainerCelly, colorCodes[i], gScreenSurface) ) canWalk= 1;
									if( cellColorThreshold[WILD_GRASS_CELL] <= cellComp(trainerCellx+cellShift, trainerCelly, colorCodes[WILD_GRASS_CELL], gScreenSurface) ) steponWildGrass=1;
									for( int i=0; i<16; i++){
										if(i%4==0) frame++;
										if(frame>3) frame=0;
										gCurrentClip = &gWalkRight[frame];//set equal to proper frame of walking right sprites
										if(canWalk==1){
											if(isOB==1 || stretchRect1.x<SCREEN_WIDTH/2-9 || isPokeCenter) {
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
			//					writeColorCodes(trainerCellx,trainerCelly+cellShift, gScreenSurface);
				cout<<"pokeCenter:		"<<cellComp(trainerCellx, trainerCelly-cellShift, colorCodes[POKE_CENTER_CELL], gScreenSurface)<<endl;
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
	cout<<"bridge:		"<<cellComp(trainerCellx+cellShift, trainerCelly, colorCodes[BRIDGE_CELL], gScreenSurface)<<endl;
	cout<<"cave:		"<<cellComp(trainerCellx+cellShift, trainerCelly, colorCodes[CAVE_ENT_CELL], gScreenSurface)<<endl;
	cout<<"flower:		"<<cellComp(trainerCellx+cellShift, trainerCelly, colorCodes[FLOWER_CELL], gScreenSurface)<<endl;
	cout<<"normal grass:	"<<cellComp(trainerCellx+cellShift, trainerCelly, colorCodes[NORM_GRASS_CELL], gScreenSurface)<<endl;
	cout<<"pathedge:	"<<cellComp(trainerCellx+cellShift, trainerCelly, colorCodes[PATH_EDGE_CELL], gScreenSurface)<<endl;
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

						//This will save all events (to e) that occured while the trainer was taking a step. As long as the event was not a quit, then the event will be arbitrarily stored and then overwritten. This is done so that the user playing may hold down an arrow key, and then the trainer will stop walking as soon as the user releases the key 
						while(SDL_PollEvent(&e) != 0){//while there remains user entered events
							if (e.type == SDL_QUIT){//entered event is equal to pressing red x in top right hand corner of window
								quit = true;//set quit to true to exit the while loop
							}
						}
					}
					if(isPokeCenter)
						gBackground = gPokeMaps[ POKE_CENTER_MAP ];
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
					enteringCenter=0;
					exitingCenter=0;
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
	outFile.open( "foo.txt", ios::out );
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

void transitionGraphic(SDL_Window *window, SDL_Surface *screen, SDL_Surface *gBackground, SDL_Surface *gSpriteSheet, SDL_Rect *gCurrentClip, SDL_Rect stretchRect1, SDL_Rect stretchRect2, SDL_Rect stretchRect3, int counter) {
	SDL_Surface* transsurface;
	for( int i=(16*counter); i<=(16*counter+16); i+=7 ){
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
