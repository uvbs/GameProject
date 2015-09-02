////////////////////////////////////////////////
///\file MainGameFunctions.h
///\brief These are the main functions to solve any freecell-type game
///\author Michael Mann
///\version 1.0
///\date September 2002
////////////////////////////////////////////////

#include "FCSFreecellSolvingAlgorithm.h"
#include "FCSSimpleSimonSolvingAlgorithm.h"
#include "FCSTalonSolvingAlgorithm.h"
#include "FCCommandLineArguments.h"
#include "FCSOptimizeSolvingAlgorithm.h"
#include "FCPresets.h"


///\brief Creates the correct solving algorithm
void MainCreate(FCSPresetID	GamePreset, FCCommandLineArguments* CommandLine,
				FCSFreecellSolvingAlgorithm** FreecellSolvingAlgorithm,
				FCSSimpleSimonSolvingAlgorithm** SimpleSimonSolvingAlgorithm,
				FCSTalonSolvingAlgorithm** TalonSolvingAlgorithm);

///\brief Solves the game with the correct solving algorithm
FCSStateSolvingReturnCodes MainSolve(FCSPresetID GamePreset, FCSStateWithLocations *InitialState,
				FCSFreecellSolvingAlgorithm* FreecellSolvingAlgorithm,
				FCSSimpleSimonSolvingAlgorithm* SimpleSimonSolvingAlgorithm,
				FCSTalonSolvingAlgorithm* TalonSolvingAlgorithm);

///\brief Resumes the game with the correct solving algorithm
FCSStateSolvingReturnCodes MainResume(FCSPresetID GamePreset,
				FCSFreecellSolvingAlgorithm* FreecellSolvingAlgorithm,
				FCSSimpleSimonSolvingAlgorithm* SimpleSimonSolvingAlgorithm,
				FCSTalonSolvingAlgorithm* TalonSolvingAlgorithm);

///\brief Optimizes the game with the correct solving algorithm
void MainOptimize(FCSPresetID GamePreset, FCSStateWithLocations *InitialState, FCSStateWithLocations *DuplicateInitialState,
				FCSFreecellSolvingAlgorithm* FreecellSolvingAlgorithm,
				FCSSimpleSimonSolvingAlgorithm* SimpleSimonSolvingAlgorithm,
				FCSTalonSolvingAlgorithm* TalonSolvingAlgorithm);

///\brief Shows the solution to the game with the correct solving algorithm
void MainSolution(FCSPresetID GamePreset, FCSStateWithLocations *InitialState, FCSStateWithLocations *DuplicateInitialState,
				FCSFreecellSolvingAlgorithm* FreecellSolvingAlgorithm,
				FCSSimpleSimonSolvingAlgorithm* SimpleSimonSolvingAlgorithm,
				FCSTalonSolvingAlgorithm* TalonSolvingAlgorithm);

///\brief Shows the statistics of the game with the correct solving algorithm
void MainStats(FCSPresetID GamePreset,
				FCSFreecellSolvingAlgorithm* FreecellSolvingAlgorithm,
				FCSSimpleSimonSolvingAlgorithm* SimpleSimonSolvingAlgorithm,
				FCSTalonSolvingAlgorithm* TalonSolvingAlgorithm);