////////////////////////////////////////////////
///\file FCCommandLineArguments.cpp
///\brief This file contains the implementation of the FCCommandLineArguments class
///\author Michael Mann
///\version 1.0
///\date August 2002
////////////////////////////////////////////////

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <iostream>
#include "FCCommandLineArguments.h"
#include "FCPresets.h"
#include "Config.h"

using namespace std;

///\brief Display the help string
void DisplayHelp();

FCCommandLineArguments::FCCommandLineArguments()
{
	m_DisplayHelp = false;
	m_DebugDisplayInfo = new FCSDebugDisplayInfo();

	m_DebugDisplayInfo->m_NumberOfFreecells = m_NumberOfFreecells = DEFAULT_NUMBER_OF_FREECELLS;
	m_DebugDisplayInfo->m_NumberOfStacks = m_NumberOfStacks = DEFAULT_NUMBER_OF_STACKS;
	m_DebugDisplayInfo->m_NumberOfDecks = m_NumberOfDecks = DEFAULT_NUMBER_OF_DECKS;
	m_SequencesBuiltBy[0] = NULL;
	m_SequencesBuiltByType = DEFAULT_BUILD_SEQUENCE_TYPE;
	m_SequenceMoveUnlimited[0] = NULL;
	m_IsSequenceMoveUnlimited = false;
	m_EmptyStacksFilledBy[0] = NULL;
	m_EmptyStacksFilledByType = DEFAULT_EMPTY_STACK_FILL_TYPE;
	m_GameName[0] = NULL;
	m_MaxNumberOfIterations = -1;
	m_MaxDepth = -1;
	m_TempTestOrder[0] = NULL;
	m_NumberOfTests = 0;
	memset(m_TestOrder, NULL, sizeof(int)*FCS_TESTS_NUM);
	m_SolvingMethod[0] = NULL;
	m_SolvingMethodType = DEFAULT_SOLVING_METHOD;
	m_AStarWeights[0] = NULL;
	for (int a = 0;a<FC_NUMBER_OF_ASTAR_WEIGHTS ;a++)
		m_AStarWeightValues[a] = -1;
	m_Seed = 0;
	m_MaxStoredStates = -1;
	m_OptimizeSolution = false;
	m_StateTypeArray[0] = NULL;
	m_StateType = DEFAULT_STATE_TYPE;
	m_StateStorage[0] = NULL;
	m_StackStorage[0] = NULL;
	m_Talon[0] = NULL;
	m_TalonType = FCS_TALON_NONE;
	m_StateStorageType = DEFAULT_STATE_STORAGE_TYPE;
	m_StackStorageType = DEFAULT_STACK_STORAGE_TYPE;
	m_UserState[0] = NULL;
	m_InitialState = NULL;
}

FCCommandLineArguments::~FCCommandLineArguments()
{
}

bool FCCommandLineArguments::Parse(int argc, char**argv)
{
	int arg;

	for (arg = 1;arg<argc;arg++)
	{
		/***********************************
		HELP
		************************************/
		if ((!strcmp(argv[arg], "-h")) || (!strcmp(argv[arg], "--help")))
		{
			m_DisplayHelp = true;
			return true;
		}
		/***********************************
		OUTPUT OPTIONS
		************************************/
		else if ((!strcmp(argv[arg], "-p")) || (!strcmp(argv[arg], "--parseable-output")))
		{
			m_DebugDisplayInfo->m_DisplayDebugOptions |= DEBUG_IS_OUTPUT_PARSEABLE;
		}
		else if ((!strcmp(argv[arg], "-t")) || (!strcmp(argv[arg], "--display-10-as-t")))
		{
			m_DebugDisplayInfo->m_DisplayDebugOptions |= DEBUG_DISPLAY_10_AS_T;
		}
		else if ((!strcmp(argv[arg], "-c")) || (!strcmp(argv[arg], "--canonized-order-output")))
		{
			m_DebugDisplayInfo->m_DisplayDebugOptions |= DEBUG_CANONIZED_ORDER_OUTPUT;
		}
		else if ((!strcmp(argv[arg], "-m")) || (!strcmp(argv[arg], "--display-moves")))
		{
			m_DebugDisplayInfo->m_DisplayDebugOptions |= DEBUG_DISPLAY_MOVES;
		}
		else if ((!strcmp(argv[arg], "-sn")) || (!strcmp(argv[arg], "--standard-notation")))
		{
			m_DebugDisplayInfo->m_DisplayDebugOptions |= DEBUG_USE_STANDARD_NOTATION;
		}
		else if ((!strcmp(argv[arg], "-sam")) || (!strcmp(argv[arg], "--display-states-and-moves")))
		{
			m_DebugDisplayInfo->m_DisplayDebugOptions |= (DEBUG_DISPLAY_MOVES | DEBUG_DISPLAY_STATES);
		}
		else if ((!strcmp(argv[arg], "-pi")) || (!strcmp(argv[arg], "--display-parent-iter")))
		{
			m_DebugDisplayInfo->m_DisplayDebugOptions |= DEBUG_DISPLAY_PARENT_ITERATIONS;
		}
		else if ((!strcmp(argv[arg], "-dd")) || (!strcmp(argv[arg], "--display-debug")))
		{
			m_DebugDisplayInfo->m_DisplayDebug = true;
		}

		/***********************************
		GAME VARIANTS OPTIONS
		************************************/
		else if ((!strcmp(argv[arg], "--freecells-num")))
		{
			arg++;
			if (arg != argc)
				m_DebugDisplayInfo->m_NumberOfFreecells = m_NumberOfFreecells = atoi(argv[arg]);
		}
		else if ((!strcmp(argv[arg], "--stacks-num")))
		{
			arg++;
			if (arg != argc)
				m_DebugDisplayInfo->m_NumberOfStacks = m_NumberOfStacks = atoi(argv[arg]);
		}
		else if ((!strcmp(argv[arg], "--decks-num")))
		{
			arg++;
			if (arg != argc)
				m_DebugDisplayInfo->m_NumberOfDecks = m_NumberOfDecks = atoi(argv[arg]);
		}
		else if ((!strcmp(argv[arg], "--sequences-are-built-by")))
		{
			arg++;
			if (arg != argc)
				strcpy(m_SequencesBuiltBy, argv[arg]);
		}
		else if ((!strcmp(argv[arg], "--sequence-move")))
		{
			arg++;
			if (arg != argc)
				strcpy(m_SequenceMoveUnlimited, argv[arg]);
		}
		else if (!strcmp(argv[arg], "--empty-stacks-filled-by"))
		{
			arg++;
			if (arg == argc)
				strcpy(m_EmptyStacksFilledBy, argv[arg]);
		}
		else if (
			(!strcmp(argv[arg], "--game")) ||
			(!strcmp(argv[arg], "--preset")) ||
			(!strcmp(argv[arg], "-g"))
			)
		{
			arg++;
			if (arg != argc)
				strcpy(m_GameName, argv[arg]);
		}
		/***********************************
		SOLVING ALGORITHM OPTIONS
		************************************/
		else if ((!strcmp(argv[arg], "-mi")) || (!strcmp(argv[arg], "--max-iters")))
		{
			arg++;
			if (arg != argc)
				m_MaxNumberOfIterations = atoi(argv[arg]);
		}
		else if ((!strcmp(argv[arg], "-md")) || (!strcmp(argv[arg], "--max-depth")))
		{
			arg++;
			if (arg != argc)
				m_MaxDepth = atoi(argv[arg]);
		}
		else if ((!strcmp(argv[arg], "-to")) || (!strcmp(argv[arg], "--tests-order")))
		{
			arg++;
			if (arg != argc)
				strcpy(m_TempTestOrder, argv[arg]);
		}
		else if ((!strcmp(argv[arg], "-me")) || (!strcmp(argv[arg], "--method")))
		{
			arg++;
			if (arg != argc)
				strcpy(m_SolvingMethod, argv[arg]);
		}
		else if ((!strcmp(argv[arg], "-asw")) || (!strcmp(argv[arg], "--a-star-weights")))
		{
			arg++;
			if (arg != argc)
				strcpy(m_AStarWeights, argv[arg]);
		}
		else if (!strcmp(argv[arg], "--talon"))
		{
			arg++;
			if (arg != argc)
				strcpy(m_Talon, argv[arg]);
		}
		else if (!strcmp(argv[arg], "-seed"))
		{
			arg++;
			if (arg != argc)
				m_Seed = atoi(argv[arg]);
		}
		else if ((!strcmp(argv[arg], "-mss")) || (!strcmp(argv[arg], "--max-stored-states")))
		{
			arg++;
			if (arg != argc)
				m_MaxStoredStates = atoi(argv[arg]);
		}
		else if ((!strcmp(argv[arg], "-opt")) || (!strcmp(argv[arg], "--optimize-solution")))
		{
			m_OptimizeSolution = true;
		}
		else if ((!strcmp(argv[arg], "-st")) || (!strcmp(argv[arg], "--state-type")))
		{
			arg++;
			if (arg != argc)
				strcpy(m_StateTypeArray, argv[arg]);
		}
		else if ((!strcmp(argv[arg], "-stsr")) || (!strcmp(argv[arg], "--state-storage-type")))
		{
			arg++;
			if (arg != argc)
				strcpy(m_StateStorage, argv[arg]);
		}
		else if ((!strcmp(argv[arg], "-sksr")) || (!strcmp(argv[arg], "--stack-storage-type")))
		{
			arg++;
			if (arg != argc)
				strcpy(m_StackStorage, argv[arg]);
		}
		/***********************************
		RUNTIME DISPLAY OPTIONS
		************************************/
		else if ((!strcmp(argv[arg], "-s")) || (!strcmp(argv[arg], "--state-output")))
		{
			m_DebugDisplayInfo->m_DisplayDebugOptions |= DEBUG_ITERATIVE_STATE_OUTPUT;
		}
		else
		{
			break;
		}
 	}

	
	FILE * file;

	if ((arg == argc) || (!strcmp(argv[arg], "-")))
	{
		file = stdin;
	}
	else if (argv[arg][0] == '=')
	{
		cerr << "Error! Unknown option '" << argv[arg] << "'.  "
			 << "Type '" << argv[0] << " --help' for usage information." << endl;
		return false;
	}
	else
	{
		file = fopen(argv[arg], "r");
		if (file == NULL)
		{
			cerr << "Error!  Could not open file '" << argv[arg] << "' for input. "
				 << "Exiting." << endl;
			return false;
		}
	}

	fread(m_UserState, sizeof(char), sizeof(m_UserState)/sizeof(char), file);
	fclose(file);

	return true;
}

bool FCCommandLineArguments::Verify()
{
	bool verify = true;

	if (m_DisplayHelp)
	{
		DisplayHelp();
		return false;
	}

	if (m_NumberOfFreecells > MAX_NUM_FREECELLS)
	{
		cerr << "Error! The freecells\' number exceeds the maximum of " 
			 << MAX_NUM_FREECELLS << endl
			 << "Recompile the program if you wish to have more." << endl << endl;
		verify = false;
	}
	if (m_NumberOfFreecells < 0)
	{
		cerr << "Error!  Must have a positive number of freecells!" << endl << endl;
		verify = false;
	}

	if (m_NumberOfStacks > MAX_NUM_STACKS)
	{
		cerr << "Error! The stacks' number exceeds the maximum of " 
			 << MAX_NUM_STACKS << endl
			 << "Recompile the program if you wish to have more." << endl << endl;
		verify = false;
	}
	if (m_NumberOfStacks < 1)
	{
		cerr << "Error!  Must have at least one stack of cards!" << endl << endl;
		verify = false;
	}

	if (m_NumberOfDecks > MAX_NUM_DECKS)
	{
		cerr << "Error! The decks' number exceeds the maximum of " 
			 << MAX_NUM_DECKS << endl
			 << "Recompile the program if you wish to have more." << endl << endl;
		verify = false;
	}
	if (m_NumberOfDecks < 1)
	{
		cerr << "Error!  Must have at least one deck of cards!" << endl << endl;
		verify = false;
	}

	if (m_SequencesBuiltBy[0] != NULL)
	{
		if (!strcmp(m_SequencesBuiltBy, "suit"))
		{
			m_SequencesBuiltByType = FCS_SEQ_BUILT_BY_SUIT;
		}
		else if (!strcmp(m_SequencesBuiltBy, "rank"))
		{
			m_SequencesBuiltByType = FCS_SEQ_BUILT_BY_RANK;
		}
		else if (!strcmp(m_SequencesBuiltBy, "alternate_color"))
		{
			m_SequencesBuiltByType = FCS_SEQ_BUILT_BY_ALTERNATE_COLOR;
		}
		else
		{
			cerr << "Error! --sequence-are-built-by parameter incorrect" << endl
				 << "Please choose from 'suit', 'alternate_color', 'rank'" << endl << endl;
			verify = false;
		}
	}

	if (m_SequenceMoveUnlimited[0] != NULL)
	{
		if (!strcmp(m_SequenceMoveUnlimited, "unlimited"))
		{
			m_IsSequenceMoveUnlimited = true;
		}
		else if (!strcmp(m_SequenceMoveUnlimited, "limited"))
		{
			m_IsSequenceMoveUnlimited = false;
		}
		else
		{
			cerr << "Error! --sequence-move parameter incorrect" << endl
				 << "Please choose from 'unlimited' or 'limited'" << endl << endl;
			verify = false;
		}
	}

	if (m_EmptyStacksFilledBy[0] != NULL)
	{
		if (!strcmp(m_EmptyStacksFilledBy, "kings"))
		{
			m_EmptyStacksFilledByType  = FCS_ES_FILLED_BY_KINGS_ONLY;
		}
		else if (!strcmp(m_EmptyStacksFilledBy, "none"))
		{
			m_EmptyStacksFilledByType  = FCS_ES_FILLED_BY_NONE;
		}
		else if (!strcmp(m_EmptyStacksFilledBy, "all"))
		{
			m_EmptyStacksFilledByType  = FCS_ES_FILLED_BY_ANY_CARD;
		}
		else
		{
			cerr << "Error! --empty-stacks-filled-by parameter incorrect" << endl
				 << "Please choose from 'kings', 'none', 'all'" << endl << endl;
			verify = false;
		}
	}

	if (m_TempTestOrder[0] != NULL)
	{
		char* ErrorString;
		if (FCSApplyTestOrder(m_TestOrder, m_TempTestOrder, &m_NumberOfTests, m_GameName, &ErrorString) != 0)
		{
			cerr << "Error! " << ErrorString << endl << endl;
			verify = false;
			delete [] ErrorString;
		}
	}

	if (m_GameName[0] != NULL)
	{
		FCSPreset* preset = FCSPreset::GetPresetInfo(m_GameName);
		
		if(preset == NULL)
		{
			cerr << "Error!  Unknown game '" << m_GameName << "'!" << endl << endl;
			verify = false;
		}
		else
		{
			if (preset->m_NumberOfFreecells > MAX_NUM_FREECELLS)
			{
				cerr << "Error! \"" << m_GameName << "\"'s freecells\' number exceeds the maximum of " 
					 << MAX_NUM_FREECELLS << endl
					 << "Recompile the program if you wish to have more." << endl << endl;
				verify = false;
			}

			if (preset->m_NumberOfFreecells < MAX_NUM_FREECELLS)
			{
				cerr << "Warning! \"" << m_GameName << "\" is using less than the maximum "
					 << "number of freecells.  Performance will suffer." << endl
					 << "Recompile the program with the exact number of freecells "
					 << "for optimal performance." << endl << endl;
			}

			m_DebugDisplayInfo->m_NumberOfFreecells = m_NumberOfFreecells = preset->m_NumberOfFreecells;

			if (preset->m_NumberOfStacks > MAX_NUM_STACKS)
			{
				cerr << "Error! \"" << m_GameName << "\"'s stacks' number exceeds the maximum of " 
					 << MAX_NUM_STACKS << endl
					 << "Recompile the program if you wish to have more." << endl << endl;
				verify = false;
			}

			if (preset->m_NumberOfStacks < MAX_NUM_STACKS)
			{
				cerr << "Warning! \"" << m_GameName << "\" is using less than the maximum "
					 << "number of stacks.  Performance will suffer." << endl
					 << "Recompile the program with the exact number of stacks "
					 << "for optimal performance." << endl << endl;
			}

			m_DebugDisplayInfo->m_NumberOfStacks = m_NumberOfStacks = preset->m_NumberOfStacks;

			if (preset->m_NumberOfDecks > MAX_NUM_DECKS)
			{
				cerr << "Error! \"" << m_GameName << "\"'s decks' number exceeds the maximum of " 
					 << MAX_NUM_DECKS << endl
					 << "Recompile the program if you wish to have more." << endl << endl;
				verify = false;
			}

			if (preset->m_NumberOfDecks < MAX_NUM_DECKS)
			{
				cerr << "Warning! \"" << m_GameName << "\" is using less than the maximum "
					 << "number of decks.  Performance will suffer." << endl
					 << "Recompile the program with the exact number of decks "
					 << "for optimal performance." << endl << endl;
			}

			m_DebugDisplayInfo->m_NumberOfDecks = m_NumberOfDecks = preset->m_NumberOfDecks;

			if (m_SequencesBuiltByType != preset->m_SequencesAreBuiltBy)
			{
				cerr << "Warning! \"" << m_GameName << "\" has a different default "
					 << "value for --sequences-are-built-by.  Using value from command line if "
					 << "specified.  Otherwise using value from game preset." << endl << endl;
			}

			m_SequencesBuiltByType = preset->m_SequencesAreBuiltBy;

			if ((m_SequenceMoveUnlimited[0] != NULL) && (m_IsSequenceMoveUnlimited != preset->m_UnlimitedSequenceMove))
			{
				cerr << "Warning! \"" << m_GameName << "\" has a different default "
					 << "value for --sequence-move.  Using value from command line if "
					 << "specified.  Otherwise using value from game preset." << endl << endl;
			}

			m_IsSequenceMoveUnlimited = preset->m_UnlimitedSequenceMove;

			if (m_EmptyStacksFilledByType != preset->m_EmptyStacksFill)
			{
				cerr << "Warning! \"" << m_GameName << "\" has a different default "
					 << "value for --empty-stacks-filled-by.  Using value from command line if "
					 << "specified.  Otherwise using value from game preset." << endl << endl;
			}

			m_EmptyStacksFilledByType = preset->m_EmptyStacksFill;
			m_TalonType = m_DebugDisplayInfo->m_TalonType = preset->m_TalonType;

			char* Dummy;
			//this shouldn't fail but...
			if (FCSApplyTestOrder(m_TestOrder, preset->m_TestOrder, &m_NumberOfTests, m_GameName, &Dummy) != 0)
			{
				cerr << "Error! " << Dummy << endl;
				delete [] Dummy;
				verify = false;
			}

			delete preset;
		}
	}
		
	if (m_MaxNumberOfIterations < 0)
	{
		cerr << "Warning! It is generally a good idea to set --max_iters to "
			 << "a value, so the program will not occupy too much memory." << endl << endl;
	}

	if (m_MaxDepth >= 0)
	{
		cerr << "Warning! It's not a good idea to set --max-depth, because that "
			 << "way several important intermediate states become inaccesible." << endl << endl;
	}

	if (m_SolvingMethod[0] != NULL)
	{
		if (!strcmp(m_SolvingMethod, "dfs"))
		{
			m_SolvingMethodType = FCS_METHOD_HARD_DFS;
		}
		else if (!strcmp(m_SolvingMethod, "soft-dfs"))
		{
			m_SolvingMethodType = FCS_METHOD_SOFT_DFS;
		}
		else if (!strcmp(m_SolvingMethod, "bfs"))
		{
			m_SolvingMethodType = FCS_METHOD_BFS;
		}
		else if (!strcmp(m_SolvingMethod, "a-star"))
		{
			m_SolvingMethodType = FCS_METHOD_A_STAR;
		}
		else if (!strcmp(m_SolvingMethod, "random-dfs"))
		{
			m_SolvingMethodType = FCS_METHOD_RANDOM_DFS;
		}
		else
		{
			cerr << "Error!  Unknown solving method '" << m_SolvingMethod << "'." << endl << endl;
			verify = false;
		}
	}

	if (m_AStarWeights[0] != NULL)
	{
		char *token;
		char CopyAStarWeights[FC_SIZE_OF_ASTAR_WEIGHT_ARRAY];
		int b = 0, counter = 0;
		bool fail = false;

		strcpy(CopyAStarWeights, m_AStarWeights);
		token = strtok(CopyAStarWeights, ",\n\0");
		if (token != NULL)
		{
			//check token for non-digit characters
			while (token[b] != NULL)
			{
				if (((token[b] < '0') || (token[b] < '0')) && ((token[b] != '.')))
				{
					fail = true;
					break;
				}
				b++;
			}
			if (!fail)
				m_AStarWeightValues[counter++] = atof(token);
		}
		else
		{
			cerr << "Error!  Invalid value for --a-star-weights value" << endl << endl;
			fail = true;
		}

		if (!fail)
		{
			do 
			{
				token = strtok(NULL, ",\n\0");
				if (token != NULL)
				{
					b = 0;
					//check token for non-digit characters
					while (token[b] != NULL)
					{
						if (((token[b] < '0') || (token[b] < '0')) && ((token[b] != '.')))
						{
							cerr << "Error!  Invalid value for --a-star-weights value" << endl << endl;
							fail = true;
							break;
						}
						b++;
					}

					if (!fail)
						m_AStarWeightValues[counter++] = atof(token);
				}
			}
			while ((token != NULL) && (counter < FC_NUMBER_OF_ASTAR_WEIGHTS));
		}

		if (counter != 5)
		{
			cerr << "Warning!  Not enough values for --a-star-weights, using default "
				 << "values for remaining parameters."  << endl << endl;
		}

		if (fail)
			verify = false;
	}

	if (m_Talon[0] != NULL)
	{
		if (!strcmp(m_StateTypeArray, "none"))
		{
			m_TalonType = m_DebugDisplayInfo->m_TalonType = FCS_TALON_NONE;
		}
		else if (!strcmp(m_StateTypeArray, "gypsy"))
		{
			m_TalonType = m_DebugDisplayInfo->m_TalonType = FCS_TALON_GYPSY;
		}
		else if (!strcmp(m_StateTypeArray, "klondike"))
		{
			m_TalonType = m_DebugDisplayInfo->m_TalonType = FCS_TALON_KLONDIKE;
		}
		else
		{
			cerr << "Error!  Unknown talon type '" << m_StateTypeArray << "'." << endl << endl;
			verify = false;
		}
	}

	if (m_StateTypeArray[0] != NULL)
	{
		if (!strcmp(m_StateTypeArray, "debug"))
		{
			GlobalStateType = m_StateType = FC_DEBUG_STATE;
		}
		else if (!strcmp(m_StateTypeArray, "compact"))
		{
			GlobalStateType = m_StateType = FC_COMPACT_STATE;
		}
		else if (!strcmp(m_StateTypeArray, "indirect"))
		{
			GlobalStateType = m_StateType = FC_INDIRECT_STATE;
		}
		else
		{
			cerr << "Error!  Unknown state type '" << m_StateTypeArray << "'." << endl << endl;
			verify = false;
		}
	}

	if (m_TalonType != FCS_TALON_NONE)
	{
		switch(m_StateType)
		{
		case FC_DEBUG_STATE:
			GlobalStateType = m_StateType = FC_TALON_DEBUG_STATE;
			break;
		case FC_COMPACT_STATE:
			GlobalStateType = m_StateType = FC_TALON_COMPACT_STATE;
			break;
		case FC_INDIRECT_STATE:
			GlobalStateType = m_StateType = FC_TALON_INDIRECT_STATE;
			break;
		}
	}

	if (m_StateStorage[0] != NULL)
	{
		if (!strcmp(m_StateStorage, "avl"))
		{
			m_StateStorageType = FC_AVL_TREE;
		}
		else if (!strcmp(m_StateStorage, "avl_rb"))
		{
			m_StateStorageType = FC_AVL_RED_BLACK_TREE;
		}
		else if (!strcmp(m_StateStorage, "red_black"))
		{
			m_StateStorageType = FC_RED_BLACK_TREE;
		}
		else if (!strcmp(m_StateStorage, "glib_tree"))
		{
			m_StateStorageType = FC_GLIB_TREE;
		}
		else if (!strcmp(m_StateStorage, "glib_hash"))
		{
			m_StateStorageType = FC_GLIB_HASH;
		}
		else if (!strcmp(m_StateStorage, "internal"))
		{
			m_StateStorageType = FC_INTERNAL_HASH;
		}
		else if (!strcmp(m_StateStorage, "indirect"))
		{
			if ((m_StateType != FC_INDIRECT_STATE) && (m_StateType != FC_TALON_INDIRECT_STATE))
			{
				cerr << "Error!  Indirect storage must use indirect states!" << endl << endl;
				verify = false;
			}
			else
			{
				m_StateStorageType = FC_INDIRECT_HASH;
			}
		}
		else
		{
			cerr << "Error!  Unknown state storage type '" << m_StateTypeArray << "'." << endl << endl;
			verify = false;
		}
	}

	if (m_StackStorage[0] != NULL)
	{
		if (!strcmp(m_StackStorage, "avl"))
		{
			m_StackStorageType = FC_AVL_TREE;
		}
		else if (!strcmp(m_StackStorage, "avl_rb"))
		{
			m_StackStorageType = FC_AVL_RED_BLACK_TREE;
		}
		else if (!strcmp(m_StackStorage, "red_black"))
		{
			m_StackStorageType = FC_RED_BLACK_TREE;
		}
		else if (!strcmp(m_StackStorage, "glib_tree"))
		{
			m_StackStorageType = FC_GLIB_TREE;
		}
		else if (!strcmp(m_StackStorage, "glib_hash"))
		{
			m_StackStorageType = FC_GLIB_HASH;
		}
		else if (!strcmp(m_StackStorage, "internal"))
		{
			m_StackStorageType = FC_INTERNAL_HASH;
		}
		else
		{
			cerr << "Error!  Unknown stack storage type '" << m_StateTypeArray << "'." << endl << endl;
			verify = false;
		}
	}

	if (m_DebugDisplayInfo->m_DisplayDebugOptions & (DEBUG_USE_STANDARD_NOTATION | (!DEBUG_DISPLAY_MOVES)))
	{
		cerr << "Warning! --standard-notation must be used in conjunction with "
			 << "--display-moves.  It will be ignored." << endl << endl;
		m_DebugDisplayInfo->m_DisplayDebugOptions &= !DEBUG_USE_STANDARD_NOTATION;
	}

	if (m_UserState[0] != NULL)
	{
		if (verify)
		{
			FCSCard *MisplacedCard = NULL;

			m_InitialState = CreateInitialState(m_UserState, m_NumberOfFreecells, 
												m_NumberOfStacks, m_NumberOfDecks, m_TalonType);

			int Valid = m_InitialState->CheckStateValidity(m_NumberOfFreecells, m_NumberOfStacks,
													m_NumberOfDecks, &MisplacedCard, m_TalonType);
			if (Valid != 0)
			{
				char CardString[10];

				verify = false;

				MisplacedCard->Perl2User(CardString, (m_DebugDisplayInfo->m_DisplayDebugOptions & DEBUG_DISPLAY_10_AS_T), 
										m_DebugDisplayInfo->m_DisplayDebug);

				delete m_InitialState;
				m_InitialState = NULL;

				switch(Valid)
				{
				case 1:
					cerr << "Error!  There's a missing card: " << CardString << endl << endl;
					break;
				case 2:
					cerr << "Error!  There's an extra card: " << CardString << endl << endl;
					break;
				case 3:
					cerr << "Error!  There's an empty slot in one of the stacks" << endl << endl;
					break;
				}
			}

			if (MisplacedCard != NULL)
				delete MisplacedCard;
		}
	}

	if (!verify)
		delete m_DebugDisplayInfo;

	return verify;

}

int FCCommandLineArguments::GetMaxDepth()
{
	return m_MaxDepth;
}

int FCCommandLineArguments::GetMaxNumberOfIterations()
{
	return m_MaxNumberOfIterations;
}

int FCCommandLineArguments::GetNumberOfTests()
{
	return m_NumberOfTests;
}

char FCCommandLineArguments::GetTestOrderNumber(int number)
{
	return m_TestOrder[number];
}

int FCCommandLineArguments::GetNumberOfFreecells()
{
	return m_NumberOfFreecells;
}

int FCCommandLineArguments::GetNumberOfStacks()
{
	return m_NumberOfStacks;
}

int FCCommandLineArguments::GetNumberOfDecks()
{
	return m_NumberOfDecks;
}

int FCCommandLineArguments::GetMaxStoredStates()
{
	return m_MaxStoredStates;
}

int FCCommandLineArguments::GetSeed()
{
	return m_Seed;
}

BuildSequenceByEnum FCCommandLineArguments::GetSequenceBuildType()
{
	return m_SequencesBuiltByType;
}

bool FCCommandLineArguments::GetIsSequenceMoveUnlimited()
{
	return m_IsSequenceMoveUnlimited;
}

FCEmptyStacksFillType FCCommandLineArguments::GetEmptyStacksFill()
{
	return m_EmptyStacksFilledByType;
}

char* FCCommandLineArguments::GetGameName()
{
	return (char*)&m_GameName;
}

char* FCCommandLineArguments::GetAStarWeights()
{
	return m_AStarWeights;
}

double FCCommandLineArguments::GetAStarWeightValues(int number)
{
	return m_AStarWeightValues[number];
}

FCSolvingMethodType FCCommandLineArguments::GetSolvingMethodType()
{
	return m_SolvingMethodType;
}

FCStateType FCCommandLineArguments::GetStateType()
{
	return m_StateType;
}

FCStorageType FCCommandLineArguments::GetStateStorageType()
{
	return m_StateStorageType;
}

FCStorageType FCCommandLineArguments::GetStackStorageType()
{
	return m_StackStorageType;
}

FCSTalonType FCCommandLineArguments::GetTalonType()
{
	return m_TalonType;
}

bool FCCommandLineArguments::GetOptimizeSolution()
{
	return m_OptimizeSolution;
}

FCSStateWithLocations* FCCommandLineArguments::GetInitialState()
{
	return m_InitialState;
}

FCSDebugDisplayInfo* FCCommandLineArguments::GetDebugDisplayInfo()
{
	return m_DebugDisplayInfo;
}

/*
Notes:

I took out --iter-output becuase it wasn't being used.  (I was used in the original
freecell solver for command handling, which I'm not doing at the moment (if ever)

Made --state-output useful for all FCSStateWithLocations types

Took out signals

Added the following parameters:
	-dd --display-debug
	-st --state-type
	-stsr --state-storage-type
	-sksr --stack-storage-type
*/

void DisplayHelp()
{
	cout << "CppFreecellSolver [options] board_file" << endl << endl
		 << "If board_file is - or unspecified reads standard input" << endl << endl
		 << "Available Options:" << endl
		 << "-h   --help" << endl
		 << "\tdisplay this help screen" << endl
		 << "-s   --state-output" << endl
		 << "\talso output the state in every state that is checked" << endl
		 << "-p   --parseable-output" << endl
		 << "\tOutput the states in a format that is friendly to perl, grep and friends." << endl
		 << "-c   --canonized-order-output" << endl
		 << "\tOutput the stacks and freecells according to their canonic order." << endl
		 << "\t(That means that stacks and freecells won't retain their place.)" << endl
		 << "-t   --display-10-as-t" << endl
		 << "\tDisplay the card 10 as a capital T instead of \"10\"." << endl
		 << "-m   --display-moves" << endl
		 << "\tDisplay the moves instead of the intermediate states." << endl
		 << "-sam --display-states-and-moves" << endl
		 << "\tDisplay both intermediate states and moves." << endl
		 << "-sn  --standard-notation" << endl
		 << "\tDisplay the moves in standard (non-verbose) notation."
		 << "\t(Applicable only if -m was specified)" << endl
		 << "-pi  --display-parent-iter" << endl
		 << "\tDisplay the index of the parent iteration of each state in the run-time dump" << endl << endl
		 << "-dd  --display-debug" << endl
		 << "\tDisplays all debug information" << endl
		 << "--freecells-num [Freecells\' Number]" << endl
		 << "\tThe number of freecells present in the board." << endl
		 << "--stacks-num [Stacks\' Number]" << endl
		 << "\tThe number of stacks present in the board." << endl
		 << "--decks-num [Decks\' Number]" << endl
		 << "\tThe number of decks in the board." << endl
		 << "--sequences-are-built-by {suit|alternate_color|rank}" << endl
		 << "\tSpecifies the type of sequence" << endl
		 << "--sequence-move {limited|unlimited}" << endl
		 << "\tSpecifies whether the sequence move is limited by the number of" << endl
		 << "\tfreecells or vacant stacks or not." << endl
		 << "--empty-stacks-filled-by {kings|none|all}" << endl
		 << "\tSpecifies which cards can fill empty stacks." << endl
		 << "--game [game]   --preset [game]  -g [game]" << endl
		 << "\tSpecifies the type of game. (Implies several of the game settings options above.)  Available presets:" << endl
		 << "\tbakers_dozen       - Baker\'s Dozen" << endl
		 << "\tbakers_game        - Baker\'s Game" << endl
		 << "\tbeleaguered_castle - Beleaguered Castle" << endl
		 << "\tcitadel            - Citadel" << endl
		 << "\tcruel              - Cruel" << endl
		 << "\tder_katz           - Der Katzenschwanz" << endl
		 << "\tdie_schlange       - Die Schlange" << endl
		 << "\teight_off          - Eight Off" << endl
		 << "\tforecell           - Forecell" << endl
		 << "\tfreecell           - Freecell" << endl
		 << "\tgood_measure       - Good Measure" << endl
		 << "\tko_bakers_game     - Kings\' Only Baker\'s Game" << endl
		 << "\trelaxed_freecell   - Relaxed Freecell" << endl
		 << "\trelaxed_seahaven   - Relaxed Seahaven Towers" << endl
		 << "\tseahaven           - Seahaven Towers" << endl
		 << "\tsimple_simon       - Simple Simon" << endl
		 << "\tstreets_and_alleys - Streets and Alleys" << endl << endl
		 << "-md [depth]       --max-depth [depth]" << endl
		 << "\tSpecify a maximal search depth for the solution process." << endl
		 << "-mi [iter_num]    --max-iters [iter_num]" << endl
		 << "\tSpecify a maximal number of iterations number." << endl
		 << "-mss [states_num] --max-stored-states [states_num]" << endl
		 << "\tSpecify the maximal number of states stored in memory." << endl
		 << "-to [tests_order]   --tests-order  [tests_order]" << endl
		 << "\tSpecify a test order string. Each test is represented by one character." << endl
		 << "\tValid tests:" << endl
		 << "\t\tFreecell Tests:" << endl << endl
		 << "\t\t'0' - put top stack cards in the foundations." << endl
		 << "\t\t'1' - put freecell cards in the foundations." << endl
		 << "\t\t'2' - put freecell cards on top of stacks." << endl
		 << "\t\t'3' - put non-top stack cards in the foundations." << endl
		 << "\t\t'4' - move stack cards to different stacks." << endl
		 << "\t\t'5' - move stack cards to a parent card on the same stack." << endl
		 << "\t\t'6' - move sequences of cards onto free stacks." << endl
		 << "\t\t'7' - put freecell cards on empty stacks." << endl
		 << "\t\t'8' - move cards to a different parent." << endl
		 << "\t\t'9' - empty an entire stack into the freecells." << endl << endl
		 << "\tSimple Simon Tests (used only if simple_simon game is selected):" << endl
		 << "\t\t'0' - move a full sequence to the foundations." << endl
		 << "\t\t'1' - move a sequence to a true parent of his." << endl
		 << "\t\t'2' - move a whole stack sequence to a false parent (in order to clear the stack)" << endl
		 << "\t\t'3' - move a sequence to a true parent that has some cards above it." << endl
		 << "\t\t'4' - move a sequence with some cards above it to a true parent." << endl
		 << "\t\t'5' - move a sequence with a junk sequence above it to a true parent" << endl
		 << "\t\tthat has some cards above it." << endl
		 << "\t\t'6' - move a whole stack sequence to a false parent which has some cards above it." << endl
		 << "\t\t'7' - move a sequence to a parent on the same stack." << endl << endl
		 << "\t\tTests are grouped with parenthesis or square brackets. Each group" << endl
		 << "\t\twill be randomized as a whole by the random-dfs scan." << endl << endl
		 << "-me [solving_method]   --method [solving_method]" << endl
		 << "\tSpecify a solving method. Available methods are:" << endl
		 << "\t\t\"a-star\" - A*" << endl
		 << "\t\t\"bfs\" - Breadth-First Search" << endl
		 << "\t\t\"dfs\" - Depth-First Search (default)" << endl
		 << "\t\t\"random-dfs\" - A randomized DFS" << endl
		 << "\t\t\"soft-dfs\" - \"Soft\" DFS" << endl << endl
		 << "-asw [A* Weights]   --a-star-weight [A* Weights]" << endl
		 << "\tSpecify weights for the A* scan, assuming it is used. The parameter" << endl
		 << "\tshould be a comma-separated list of numbers, each one is proportional" << endl
		 << "\tto the weight of its corresponding test." << endl << endl
		 << "\tThe numbers are, in order:" << endl
		 << "\t1. The number of cards out." << endl
		 << "\t2. The maximal sequence move." << endl
		 << "\t3. The number of cards under sequences." << endl
		 << "\t4. The length of the sequences which are found over renegade cards."
		 << "\t5. The depth of the board in the solution." << endl << endl
		 << "-seed [seed_number]" << endl
		 << "\tSet the seed for the random number generator used by the \"random-dfs\" scan." << endl
		 << "-opt  --optimize-solution" << endl
		 << "\tTry and optimize the solution for a small number of moves." << endl
		 << "-st   --state-type {debug|compact|indirect}" << endl
		 << "\tType of state used to solve board." << endl
		 << "-stsr --state-storage-type" << endl
		 << "\tSpecify a storage method for the states.  Available methods are:" << endl
		 << "\t\t\"avl\" - an avl tree" << endl
		 << "\t\t\"avl_rb\" - a red-black avl tree" << endl
		 << "\t\t\"red_black\" - a red-black tree" << endl
		 << "\t\t\"glib_tree\" - a glib tree" << endl
		 << "\t\t\"glib_hash\" - a glib hash" << endl
		 << "\t\t\"internal\" - internal storage" << endl << endl
		 << "-sksr --stack-storage-type" << endl
		 << "\tSpecify a storage method for the stacks.  Available methods are:" << endl
		 << "\t\t\"avl\" - an avl tree" << endl
		 << "\t\t\"avl_rb\" - a red-black avl tree" << endl
		 << "\t\t\"red_black\" - a red-black tree" << endl
		 << "\t\t\"glib_tree\" - a glib tree" << endl
		 << "\t\t\"glib_hash\" - a glib hash" << endl
		 << "\t\t\"internal\" - internal storage" << endl
		 << endl << endl << endl
		 << "C++ Freecell Solver was written by Michael Mann" << endl
		 << "Send comments and suggestions to mmann78@adelphia.net" << endl
		 << endl;
}
