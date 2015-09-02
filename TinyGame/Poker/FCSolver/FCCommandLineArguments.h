#ifndef MMANN_FCCOMMAND_LINE_ARGUMENTS_H
#define MMANN_FCCOMMAND_LINE_ARGUMENTS_H

////////////////////////////////////////////////
///\file FCCommandLineArguments.h
///\brief This file contains the FCCommandLineArguments class
///\author Michael Mann
///\version 1.0
///\date August 2002
////////////////////////////////////////////////

#include "Config.h"
#include "FCEnums.h"
#include "FCState.h"
#include "FCSDebugDisplayInfo.h"

///Max size of sequence built by string
#define FC_SIZE_OF_SEQUENCE_BUILT_BY		16
///Max size of sequence move string
#define FC_SIZE_OF_SEQUENCE_MOVE			10
///Max size of empty stack by string
#define FC_SIZE_OF_EMPTY_STACK_FILLED_BY	6
///Max size of game name string
#define FC_SIZE_OF_GAME_NAME				20
///Max size of solving method string
#define FC_SIZE_OF_SOLVING_METHOD			9
///Max size of a-star weight array string
#define FC_SIZE_OF_ASTAR_WEIGHT_ARRAY		26
///Max number of a-star weights
#define FC_NUMBER_OF_ASTAR_WEIGHTS			5
///Max size of state type string
#define FC_SIZE_OF_STATE_TYPE_ARRAY			9
///Max size of state storage string
#define FC_SIZE_OF_STATE_STORAGE			10
///Max size of stack storage string
#define FC_SIZE_OF_STACK_STORAGE			10
///Max size of talon type string
#define FC_SIZE_OF_TALON					9
///Max size of a user inputed state string
#define FC_SIZE_OF_INPUT_USER_STATE			1024

///Handles all command line arguments
class FCCommandLineArguments
{
public:
	///Constructor
	FCCommandLineArguments();

	//The initial state will NOT be deleted here if it's valid
	///Destructor
	virtual ~FCCommandLineArguments();

	///\brief Parse the command line
	///
	///\param argc is the number of arguments
	///\param argv is an array of null-terminated strings
	///\return Return true for successful parse, false otherwise
	bool Parse(int argc, char **argv);

	///\brief Verify the validity of the command line arguments
	///
	///\return Return true for successful verification, false otherwise
	bool Verify();

	///\brief Get the max depth parameter
	///
	///\return Max depth
	int GetMaxDepth();

	///\brief Get the max number of iterations parameter
	///
	///\return Max number of interations
	int GetMaxNumberOfIterations();

	///\brief Get the number of tests for the solver
	///
	///\return Number of tests
	int GetNumberOfTests();

	///\brief Get test number
	///
	///\param Number is the order number of the test
	///\return Test number at that order position
	char GetTestOrderNumber(int Number);

	///\brief Get the number of freecells used for the game
	///
	///\return Number of freecells in the games
	int GetNumberOfFreecells();

	///\brief Get the number of stacks used for the game
	///
	///\return Number of stacks in the games
	int GetNumberOfStacks();

	///\brief Get the number of decks used for the game
	///
	///\return Number of decks in the games
	int GetNumberOfDecks();

	///\brief Get the randomize seed initializer (used for random-DFS)
	///
	///\return Seed initializer
	int GetSeed();

	///\brief Get the maximum number of stored states
	///
	///\return Maximum number of stored states
	int GetMaxStoredStates();

	///\brief Get the sequence by which the cards are build for the game
	///
	///\return Build sequence
	BuildSequenceByEnum GetSequenceBuildType();

	///\brief Get flag determining whether sequence moves are unlimited in size
	///
	///\return Unlimited sequence moves flag
	bool GetIsSequenceMoveUnlimited();

	///\brief Get how empty stacks are filled in a game
	///
	///\return Empty stack fill type
	FCEmptyStacksFillType GetEmptyStacksFill();

	///\brief Get the name of the game being played
	///
	///\return A string of the name of the game
	char* GetGameName();

	///\brief Get a string of the a-star weights
	///
	///\return A string of all the a-star weight values
	char* GetAStarWeights();

	///\brief Get a specific a-star weight value
	///
	///\param Number is the weight requested
	///\return Weight value requested
	double GetAStarWeightValues(int Number);

	///\brief Get the state type used in the game
	///
	///\return State type
	FCStateType GetStateType();

	///\brief Get the type of storage used for the states in the game
	///
	///\return Storage type for states
	FCStorageType GetStateStorageType();

	///\brief Get the type of storage used for the stacks in the game
	///
	///\return Storage type for stacks
	FCStorageType GetStackStorageType();

	///\brief Get the talon type of the game
	///
	///\return Talon type
	FCSTalonType GetTalonType();

	///\brief Get the solving method used to solve the game
	///
	///\return Solving method
	FCSolvingMethodType GetSolvingMethodType();

	///\brief Get flag determining if a solved solution should be optimized
	///
	///\return Optimization flag
	bool GetOptimizeSolution();

	///\brief Get the initial state of the game to solve
	///
	///\return A pointer to the initial state
	FCSStateWithLocations* GetInitialState();

	///\brief Get the debug display parameters
	///
	///\return A pointer to the debug display parameters
	FCSDebugDisplayInfo* GetDebugDisplayInfo();

protected:

	///Flag determining if command line help should be displayed
	bool m_DisplayHelp;

	///Number of freecells in the game
	int m_NumberOfFreecells;

	///Number of stacks in the game
	int m_NumberOfStacks;

	///Number of decks in the game
	int m_NumberOfDecks;

	///String holding command line parameter determining how sequences are built
	char m_SequencesBuiltBy[FC_SIZE_OF_SEQUENCE_BUILT_BY];

	///Sequence build type for the game
	BuildSequenceByEnum m_SequencesBuiltByType;

	///String holding command line parameter determining if sequence moves are unlimited
	char m_SequenceMoveUnlimited[FC_SIZE_OF_SEQUENCE_MOVE];

	///Flag determining if sequence moves are unlimited
	bool m_IsSequenceMoveUnlimited;

	///String holding command line parameter determining how empty stacks are filled
	char m_EmptyStacksFilledBy[FC_SIZE_OF_EMPTY_STACK_FILLED_BY];

	///How empty stacks are filled in the game	
	FCEmptyStacksFillType m_EmptyStacksFilledByType;

	///Name of the game being played
	char m_GameName[FC_SIZE_OF_GAME_NAME];

	///The maximum number of states that will be checked by the solving algorithm
	int m_MaxNumberOfIterations;

	///Maximum depth the solving algorithm will explore
	int m_MaxDepth;

	///String holding command line parameter determining the test order
	char m_TempTestOrder[FCS_TESTS_NUM*3+1];

	///The order the tests will be performed
	int m_TestOrder[FCS_TESTS_NUM];

	///The number of tests being performed (per state)
	int m_NumberOfTests;

	///String holding command line parameter determining the solving method
	char m_SolvingMethod[FC_SIZE_OF_SOLVING_METHOD];

	///The solving method used
	FCSolvingMethodType m_SolvingMethodType;

	///String holding command line parameter determining the a-star weights when the 
	///a-star solving algorithm is used
	char m_AStarWeights[FC_SIZE_OF_ASTAR_WEIGHT_ARRAY];

	///The values of a-star weights
	double m_AStarWeightValues[FC_NUMBER_OF_ASTAR_WEIGHTS];

	///Randomize seed initializer
	int m_Seed;

	///Maximum number of stored states
	int m_MaxStoredStates;

	///Flag to optimize a solved solution
	bool m_OptimizeSolution;

	///String holding command line parameter determining the state type used
	char m_StateTypeArray[FC_SIZE_OF_STATE_TYPE_ARRAY];

	///State type (debug, compact, indirect)
	FCStateType m_StateType;

	///String holding command line parameter determining the type of storage for states
	char m_StateStorage[FC_SIZE_OF_STATE_STORAGE];

	///String holding command line parameter determining the type of storage for stacks
	char m_StackStorage[FC_SIZE_OF_STACK_STORAGE];

	///String holding command line parameter determining the type of talon
	char m_Talon[FC_SIZE_OF_TALON];

	///Talon type used for the game
	FCSTalonType m_TalonType;

	///How the states are stored
	FCStorageType m_StateStorageType;

	///How the stacks are stored
	FCStorageType m_StackStorageType;

	///String representing a string version of the start state of the game
	char m_UserState[FC_SIZE_OF_INPUT_USER_STATE];

	///The initial state in the game
	FCSStateWithLocations* m_InitialState;

	///Debug display parameters
	FCSDebugDisplayInfo* m_DebugDisplayInfo;
};

#endif