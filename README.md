# FC2-Final-Project
Repository containing all files for Fund Comp II final project.

SETUP INSTRUCTIONS:

	Here is a link to our GitHub repository containing the code for our project:

	https://github.com/kthienem/FC2-Final-Project

	NOTE: Make sure that the computer you are compiling on has SDL2

	Once you have the code, enter the command make. This will create an executable called
	animation

	Run animation to play the game. 

GAME MANUAL:

	NOTE: If this is your first time playing the game, please do not choose the 'LOAD GAME'
	option on the home screen. Our game includes a save feature so that you can load a 
	previously saved game; however, it causes bugs if you try to load a game but have not 
	saved a game yet. Please choose the 'NEW GAME' option the first time you play.

	This is a game of collecting and battling Pokemon; creatures who inhabit our 
	mysterious World. You start out with one such Pokemon, Pikachu, to help in your 
	adventure. Walk around but tread carefully, for you may startle a wild Pokemon who
	inhabit the wild grass. Use your Pokemon to battle your way through wild Pokemon and
	other trainers like you to get stronger. Grow your team of Pokemon to eventually 
	defeat the Pokemon Gym leader in the final town to truly test your skills. If you 
	struggle along the way, always remember that you can go to the red-roofed Pokecenter
	to heal your team and continue your adventure. Simply talk to the lovely nurse behind
	the desk. If your entire team becomes unable to fight, you will find yourself at the 
	Pokecenter with a fully healed team, but watch out because all of the experience your
	Pokemon have earned will be lost! Also, do not forget to take a nice break to go 
	fishing by approaching the water and pressing space bar.

CONTROLS:

	Move around the map and menus with the arrow keys.
	Enter the in-game menu by pressing 'p'.
	While in the in-game menu, you can view your party of Pokemon, save your game, exit 
	the in-game menu, and quit the game.
	Enter all player choices by clicking the SPACEBAR. This includes both choosing menu
	options, battle scenes, and also in-game character options as well. 

BATTLE SEQUENCE:

	When you enter a battle, your first Pokemon will fight for you.
	You and your opponent take turns choosing attacks to fight each other.
	If either Pokemon has its health decreased to 0, then it is knocked out.
	If you wish to leave a battle, you may chosse the 'run' option to escape. Be careful,
	because this might not always work and you leave yourself open to an opponent attack.
	You may also choose to switch your current fighting Pokemon for another Pokemon in 
	your party by choosing the 'Pokemon' option.
	You may expand your party to include up to 6 Pokemon. When you are fighting a Pokemon,
	you may choose the 'bag' option to attempt to catch a Pokemon and add it to your 
	party. This works better the lower the health of the Pokemon is you are trying to 
	catch.
	Whichever trainer or wild Pokemon which defeats all of the Pokemon belonging to the 
	other party will win the battle, ending the bettle sequence. 
	When your Pokemon defeats another Pokemon, it will gain experience relative to how
	high of a level the defeated Pokemon has. If you gain enough experience, your Pokemon
	will level up and maybe even learn a new move! 
	NOTE: The battle information is outputted to the terminal, so make sure you are 
	checking information about the battle there!

KNOWN BUGS:

	If the user attempts to load a game but there is no save data, there may be a 
	seg fault and core dump. 

	In the party menu, health bars do not accurately depict the current health of Pokemon.
	Be concientious! These Pokemon are your friends!

	In the battle scene, the Pokemon levels are not displayed.
