////////////////////////////////////////////////
///\file MainCppFreecellSolver.cpp
///\brief This is the main program for the C++ Freecell Solver
///\author Michael Mann
///\version 1.0
///\date August 2002
////////////////////////////////////////////////

#include <iostream>
using namespace std;
#ifdef _DEBUG
#include <afx.h>
#endif

#include "FCSFreecellSolvingAlgorithm.h"
#include "FCSSimpleSimonSolvingAlgorithm.h"
#include "FCCommandLineArguments.h"
#include "FCSOptimizeSolvingAlgorithm.h"
#include "FCPresets.h"
#include "MainGameFunctions.h"


///\brief The C++ Freecell Solver main function
int main(int argc, char **argv)
{

#ifdef _DEBUG
	CMemoryState oldMemState, newMemState, diffMemState;
	oldMemState.Checkpoint();
#endif

	FCSFreecellSolvingAlgorithm* SolvingAlgorithm = NULL;
	FCSSimpleSimonSolvingAlgorithm* SimpleSimonSolvingAlgorithm = NULL;
	FCSTalonSolvingAlgorithm* TalonSolvingAlgorithm = NULL;
	FCCommandLineArguments CommandLineArguments;
	FCSStateWithLocations *InitialState,
						  *DuplicateInitialState;
	FCSStateSolvingReturnCodes SolveValue;
	FCSPresetID	GamePreset;

	if (!CommandLineArguments.Parse(argc, argv))
		exit(-1);
	if (!CommandLineArguments.Verify())
		exit(-1);

	GamePreset = FCSPresetName::GetPresetID(CommandLineArguments.GetGameName());
	MainCreate(GamePreset, &CommandLineArguments, 
			&SolvingAlgorithm, &SimpleSimonSolvingAlgorithm, &TalonSolvingAlgorithm);

	InitialState = CommandLineArguments.GetInitialState();

	DuplicateInitialState = CreateStateWithLocations();
	DuplicateInitialState->Copy(InitialState);

	InitialState->CanonizeState(CommandLineArguments.GetNumberOfFreecells(), 
								CommandLineArguments.GetNumberOfStacks());

	if (InitialState != NULL)
	{
		SolveValue = MainSolve(GamePreset, InitialState, 
						SolvingAlgorithm, SimpleSimonSolvingAlgorithm, TalonSolvingAlgorithm);
	}
	
//**** The only solving algorithm that needs depth is DFS, and it looks like
//**** It will find where it left off by itself, so 0 is fine.
//**** If it isn't, add a depth variable to the DFSSolvingAlgorithm class
/*	while (SolveValue == FCS_STATE_SUSPEND_PROCESS)
	{
		MainResume(GamePreset, SolvingAlgorithm, SimpleSimonSolvingAlgorithm, TalonSolvingAlgorithm);
	}
*/
	if (SolveValue == FCS_STATE_WAS_SOLVED)
	{
		cout << "This game is solveable." << endl;
		if (CommandLineArguments.GetOptimizeSolution())
		{
			cout << "Now trying to optimize solution." << endl;
			MainOptimize(GamePreset, InitialState, DuplicateInitialState, 
				SolvingAlgorithm, SimpleSimonSolvingAlgorithm, TalonSolvingAlgorithm);
		}
		else
		{
			MainSolution(GamePreset, InitialState, DuplicateInitialState, 
				SolvingAlgorithm, SimpleSimonSolvingAlgorithm, TalonSolvingAlgorithm);
		}
	}
	else
	{
		cout << "I could not solve this game." << endl;
	}

	MainStats(GamePreset, SolvingAlgorithm, SimpleSimonSolvingAlgorithm, TalonSolvingAlgorithm);

	DuplicateInitialState->CleanState();
	delete DuplicateInitialState;

	delete InitialState;
	delete SolvingAlgorithm;
	delete SimpleSimonSolvingAlgorithm;
	delete TalonSolvingAlgorithm;

#ifdef _DEBUG
	newMemState.Checkpoint();
	if( diffMemState.Difference( oldMemState, newMemState ) )
	{
		diffMemState.DumpStatistics();
		TRACE( "Memory leaked detected!!!\n" );
		cout << "Memory leaked detected!!!" << endl;
		_CrtDumpMemoryLeaks();
	}
#endif

	return 0;
}

