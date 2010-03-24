//*****************************************************************************
// FILE NAME:  main.cpp
//
//*****************************************************************************
#include "Game.h"

int main(int argc, char *argv[])
{
	// create game and initialize it
	Game game;
	game.Init();

	// run the game
	while (game.Run())
	{
		
	}

	return 0;
}