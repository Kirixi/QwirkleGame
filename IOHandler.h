#ifndef IO_HANDLER
#define IO_HANDLER

#include <iostream>

#include "GameManager.h"

class IOHandler {
public:
/**
 *
 * Printing out welcome message
 *
 **/

    static void beginGame();
/**
 *
 * Prints out '>' after every input needed
 *
 **/

    static void prompt();
/**
 *
 * Prints out main menu
 * Asks for user input on what they want to do next
 *
 **/
    static void selection();

/**
 *
 * Starts out new game
 * Asks for user input
 * 1. New game
 * 2. Load game
 * 3. Credits
 * 4. Quit
 *
 **/
    static void newGame();

/**
 *
 * Validates username
 * Checks that all chars in CAPS
 * @param name - the name for player
 *
 **/
    static bool validateName(const string& name);

/**
 *
 * Prints out base gameplay
 * Current player name
 * Scores of player 1 and 2
 * Ask for user input
 *
 **/
    static void playRound();

/**
 *
 * Based on user input will either
 * place tiles , replace tiles or save game
 * @parms operation - place ,replace ,save
 * @parms tile - tile in format of D1
 * @parms keywordAt - needs keyword at to place tile
 * @parms pos - tile position in format A0 - Z25
 *
 **/
    static bool logicHandler(const string& operation, const string& tile, const string& keywordAT, const string& pos);

/**
 *
 * Prints out credits
 *
 **/
    static void credits();

/**
 *
 * Game Loader
 * Loads .save file based on user input
 *
 **/
    static void loadGame();

    static bool is_empty(std::ifstream& file);

/**
 *
 * Checks if tile is valid
 *
 * @parms tile - tile to be checked valid or not
 *
 **/
    static bool checkTile(const string& tile);


/**
 *
 * Checks if tile position  is valid
 *
 * @parms position - position to be checked valid or not
 *
 **/
    static bool checkTilePosition(const string& position);

/**
 *
 * Passes tile and position to GameManager
 *
 * @parms tile - colour, shape -> GameManager
 * @parms position - row,column -> GameManager
 *
 **/
    static void placeTile(const string& tile, const string& position);

/**
 *Seperates the color and shape of the tile and passes into gameManager
 *
 * @parms tile - takes in tile to and is passed to gameManager for replacement
 *
 **/

    static void replaceTile(const string& tile);

/**
 *Prints out messages and notifies the players about their operation
 *
 * @parms message - message from the gameManager to notify the current situation
 * @parms state - state passed from gameManager to check current situation
 *
 **/
    static void notify(const string& message, State state);
/**
 * Will quit the program when called
 **/
    static void quit();

    static bool gameRunning;

    static bool takingInput;
};

#endif // !IO_HANDLER
