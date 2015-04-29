##########################################################################################

README file for the CatAndMouse project (2015)
Partners: Yishay Feldman & Sharon Naftaly

##########################################################################################

Our design choices:
	MVP design:
	==========
	In this project we chose to implement the GUI using the MVP design pattern.
	We will now explain how we implemented the MVP:
	First of all, we used the MVP loop, as given to us, and implemented the various functions to work with this
	loop. We added to the loop some minor changes to support error handling when error come from the 
	presenters/views functions.
	
	Each GUI, i.e. each state of the program - the different menus GUIs, the world builder, the error
	message and the the play game, has its view, a presenter, and a start and stop functions.
	
	The Views:
	---------
	The view for each GUI is implemented by a suitable viewTranslateEvent (VTE) functions, which is responsible 
	for getting the events from the the keyboard/mouse and tranlate them into a logical event suitable
	for its specific GUI, which will be handled by the presenter.
	There are several VTE functions that we implemented: 
	- a simple menu VTE - used by the main menu and the choose cat/mouse type GUIs. This VTE handles menu
		GUIs which have a label at the top and regular buttons under it. This makes all the GUIs
		that look this way to be handled by one function.
	- The main menu VTE and the choose animal VTE which calls the simple Menu VTE, with specific data such as 
		the number of buttons in the GUI.
	- a complex menu VTE - used by the mouse/cat skill and edit/load game and save world GUIs. This VTE handles 
		menu GUIs which have a label at the top, a first button which is a value selection button, and all 
		other buttons which are regular. This makes all the GUIs that look this way to be handled by one function.
	- The world builder VTE - used by the world builder GUI.
	- The error message VTE - used by the error message GUI, which has to labels and only one button.
	- The play game VTE - used by the play game GUI.
	
	the viewState struct:
	--------------------
	There is a struct called viewState, which is common to all GUIs, and holds the relevent view data 
	for the current view: 
		- the SDL image used for the gui buttons and labels
		- the SDL image used for the background image (applicable for the menus GUIs)
		- the SDL image used for the grid items images (applicable for the world builder and play game GUI)
		- the buttons array, which holds the buttons of the GUI - for easy access to the buttons properties
			and updating their state (selected/not selected, enabled/disabled).
		- the labels array, which holds the labels of the GUI (applicable only for play game GUI)
			for easy access and change of the labels.
		- the grid items images array, which holds the images of the the grid items (applicable for the 
			world builder and play game GUI), for easily putting the grid item image on the grid.
		- the current button - used for holding which button is currently selected (applicable for the 
			menus GUIs)
		-  the UITree - holds all the widgets of the gui, in a hirarchial form, where the window is at the root.
			We use the UITree for traversing and getting the different widgets in order to update the display,
			and later it is also used for freeing the resources. 
		- the Top Panel Node - the Node in the UITree which is the parent of all the widgets in the top panel,
			used for easy access to the top panel items (applicable only for play gameand world builder GUI)
		- the Side Panel Node - the Node in the UITree which is the parent of all the widgets in the side panel,
			used for easy access to the side panel items (applicable for play game and world builder GUI)
		- the grid panel - used for easy access to the grid panel, for updating the items in it (applicable for 
			play game and world builder GUI)
	 
	The presenters:
	-------------- 
	The preseneter for each GUI are implemented by a suitable presenterHandleEvent (PHE) functions, which gets
	a logical event from the GUIs VTE function, and updates the GUIs model and viewState accordingly.
	There is one PHE function for each GUI. The menus GUIs PHE functions and the error message GUI 
	all call a general menu PHE function, which takes the logical event and returns the relevent stateId. After 
	that each such PHE functions makes additional updates to the model/view (if needed). This reduces code duplication,
	since most of the logic inside the menus PHE function is inside the genral function.
	In addition the world builder and play game have their own whole PHE functions.
	 
	The models structs:
	-----------------------
	There are several structs that hold the model data:
 	- the menuData struct:
 		A common struct for the different menus (and error message) models. It holds data for the selections made
 			regarding the buttons selected, the cat/mouse type and the cat/mouse skills, as well as which GUI (StateId)
 			was previuos to some of the GUIs (for guis which can be reached from several GUIs). 
 		The fact that this gui holds data for many GUIs helps when going back and forth in the differemt menus,
 			for "remembering" which button was selected, what was the player type/skill etc.
 		In addition it holds the game grid data when needed - when going from the play game to reconfiguring the 
 			players or when going from the world builder to the error message and save game menu.
 		Since each transition between GUIs in the program involves going to a menu type of GUI, this data 
 			structure is used as the initData struct, thus making it easy to pass data between most GUIs. 
	- the world builder data struct:
	 	The world builder data struct holds data important for the world builder - the cat/cheese/mouse position on
	 	the grid, the current selected position, which world file is edited, which items are missing, the grid data
	 	and information about the first player and the number of turns.
	- the play gama data struct:
		The play game data struct holds information about the game - such as who is the current player, the players
		types and skills, which game file was loaded, the positions of the cat/chees/mouse in the  grid, the grid data 
		itself and if the game is paused or over.
	comment: all mpdel data structs have information about the next stateId, needed by the stop function,
		In order for it to know if return data should by returned or if we should return NULL. 
	
	The start functions:
	-------------------
	each gui has its own start function. The start function is responsible for setting the GUI's model according to the
	init data, to set up the different view state elements - creating the UITree with all the widgets, setting the
	buttons/labels array etc, and to draw the GUI according to the UITree.
	Fot the menus there is a general start function, which is called by all the menus. There are several common
	start function for similar GUIs (such as the choose cat/mouse, the cat/mouse skill, and the saveworld and 
	edit/load game). In addition there is a start function for the world builder, error message and play game GUIs.
	
	The stop functions:
	-------------------
	The stop functions are responsible for freeing the data (view satet and model) of each gui, and creating 
	the initialization data for the next GUI (if needed). 	  
	There is one stop function for all the menus (and the error message), and additionaly there are stop functions
	for the world builder and the play game GUI, which free their models, and create an initialization data
	in the form of a menuData struct.
	

##########################################################################################

List of modules:
	main - The main module contains the following parts:
			- the CatAndMouse c file - contains the CatAndMouse main function which responsible for starting the 
				console mode or the GUI mode. 
				In the GUI mode - all the different GUIs are initialized first and then the game starts by starting the
				main menu GUI. After that a while loop is implemented to get the event from the mouse/keyboard, 
				and update the GUI accordingly.
			- the cat and mouse header file - holds general data regarding the game, such as the constants used, the 
				names of the world files, different enums used in the entire game (such as the different stateIds), 
				and the GUI struct used by the game. This header file is reachble from all the parts of the game that
				are not in the main module.  
			- the ListUtils c file - conatins functions implementation for all the functions found in ListUtils 
				and ListUtils extended header files. 
			- the ListUtils header file - contains functions declarations for List implementation, as
				given to us by the instructors.
			- the ListUtils exteneded header file - contains function declarations to support Tree implementation
				and to free one node in a list. In addition contains the List struct definition.
			- the MiniMax c file - contains the implementaion of the MiniMax function (with alpha beta pruning)
			- the MiniMax header file - conatins the function declaration for the original MiniMax function,  as
				given to us by the instructors, the MiniMax result struct and evaluation max and min defined values.
			- the MiniMax extended header file - contains the function declaration for the MiniMax (with pruning)
				function, and a defines value for the the evaluation function error (the value the evaluation
				function should return in case of an error).
			- ListUtilsDemo, MiniMaxDemo - contain the main functions for the demos,  as given to us by 
				the instructors.
	presenters - holds a c file for all the menus presenters, and different c files for the world builder,
					play game and error message presenters. 
				 Additionally there are three presenters header file: 
				 	- one for the menus and error message, which contain the menuData struct 
				 	  and the PHE functions declarations for all menus and the error message.
				 	  We put the menus and error message in the same header file, since they all use the
				 	  same struct for the model data.  
				 	- one for the world builder, which contains the WBdata structs and the worldBuilderPHE 
				 	  function decleration.
				 	- one for the play game , which contains the PGdata structs and the PlayGamePHE 
				 	  function decleration.
				 
	views - holds a c file for all the menus views, and different c files for the world builder,
				play game and error message views. 
			Additionally there is the views header file which contain all the viewstate data structs
				 and the different VTE functions declarations. 
	factories - There are two factories we use:
				- the widgets factory - containing all the functions for creating the different widgets, including
					a general function for creating the basis of each widget. 
					This factory has a header file which contains the struct of the widget and the functions declerations.
					The Widget struct is common to all widgets - it contains fields that are common to all Widgets 
					and fields that are specific to certain Widgets. This means that not all widgets used the same fields.
				- the GUIs factory - - consstains the functions for creating the different GUIs, and assigninf the 
					correct VTE, PHE, start and stop fucntions.
					The header file contains only the function declarataion.
	services - This module contains different services used by the program:
				- CatAndMouseUtils - different functions used by the program, such as the console mode fucntion, 
					the evaluation functions, different functuins related to a machine player, and to working 
					with the world files.
					The header file of this groups of services contains the gameState struct, which is used by the
					evaluations function, and contains data about the cat/mouse/chees positions, the current player
					identity and the number of turns left, as well as the grid data itself. 
					Additionaly, this header file contains the functions declarations.
				- ErrorHandling - this module contains different function for error handling, which display information
					about different errors that occured, and changes the isError flag to 1. There is a function that
					uses perror to print errors from system funtions, a function that uses SDL_error to print errors
					from SDL, and a function to print all other error. This file contains the isError global variable
					The header file of this group of services contains the isError varible externlization and the function
					declerations, and is reachble from all c files in the project.
				- GridDataUtils - contain different functions for working with the grid data - such as initGrid, 
					checking the character in a grid position, check if two grid poisitions are the adjacent etc.
					The header file of this group of services contains the gridItem enum, the gridItemposition struct
					and the functions declerations.
				- GUIUtils - contains different functions for displaying the GUI and updating the widgets absolute location
					and blitting the items to each other while trversing the UITree. In addition, there are different functions 
					used for getting the different items positions and sizes in each GUI. 
					The Header file include defined constants regarind the sizes of parts in the GUI, and the function 
					declerations.
				- logical event Items - a header file used by the MVPUtils and the presenters views ans stateTransition functions
					for the commom items that are used by them - the logical event type enum and the logical event struct. 
				- MVPUtils - this group of services include functions used by different parts of the MVP. These function
					are called by the the start, stop, VTE and PHE functions of the different GUIs. These include data 
					initialization functions, data freeing functions, functions that are used for setting up the GUIs 
					layout etc. 
					The header file conatins the functions declerations.
				- WidgetUtils - this group of services is used for manipulating the widgets - setting buttons selected/enabled,
					clearing a panel and setting an image tran[arent.
					the header file conatain the function decleartions.
	stateTransition - This module containing the start and stop functions of the different GUIs (as explained above).
						The module header file includes all the functions declerations.

##########################################################################################

Utility evaluation function:
	high level description - 
	reasoning -
	running time - O(n^2)

##########################################################################################
