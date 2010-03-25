//*****************************************************************************
// FILE NAME:  main.cpp
//
//*****************************************************************************
#include "Game.h"

int main(int argc, char *argv[])
{
	// create and initialize game
	Game game;
	game.Init();

	// run the game
	game.Run();

	return 0;
}