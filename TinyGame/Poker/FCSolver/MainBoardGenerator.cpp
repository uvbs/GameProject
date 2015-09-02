////////////////////////////////////////////////
///\file MainBoardGenerator.cpp
///\brief This is a program to generate Freecell (and similar solitare games) boards
///\author Michael Mann
///\version 1.0
///\date September 2002
////////////////////////////////////////////////

#include <time.h>
#include <iostream.h>
#include <fstream.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "AFreecellGameBoard.h"

int main(int argc, char **argv)
{
	int Seed = time(NULL);
	char GameName[GAME_NAME_LENGTH];
	char* GameBoardString;
	GameName[0] = NULL;
	ofstream Write;
	bool Display10AsT = false;

	//parse the command line
	for (int arg = 1;arg<argc;arg++)
	{
		if (!strcmp(argv[arg], "-n"))
		{
			arg++;
			if (arg != argc)
				Seed = atoi(argv[arg]);
		}
		else if (!strcmp(argv[arg], "-t"))
		{
			Display10AsT = true;
		}
		else if (!strcmp(argv[arg], "-g"))
		{
			arg++;
			if (arg != argc)
				strcpy(GameName, argv[arg]);
		}
		else
		{
			break;
		}
	}

	AFreecellGameBoard* GameBoard = CreateAFreecellGameBoard(GameName, Display10AsT);
	if (GameBoard == NULL)
	{
		cerr << "Invalid game name!" << endl;
		exit(-1);
	}

	GameBoardString = new char[GameBoard->GetGameBoardSize()+1];
	GameBoardString[0] = NULL;
	GameBoard->Shuffle(Seed);
	GameBoard->Deal(GameBoardString);

	if (arg == argc)
	{
		//use cout
		cout << GameBoardString << endl;
	}
	else
	{
		//use ofstream
		Write.open(argv[arg]);
		Write << GameBoardString << endl;
		Write.close();
	}

	return 0;
}