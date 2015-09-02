////////////////////////////////////////////////
///\file MainGameFunctions.cpp
///\brief These are the main functions to solve any freecell-type game
///\author Michael Mann
///\version 1.0
///\date September 2002
////////////////////////////////////////////////

#include "MainGameFunctions.h"

#include <iostream>
using namespace std;

void MainCreate(FCSPresetID	GamePreset, FCCommandLineArguments* CommandLine,
				FCSFreecellSolvingAlgorithm** FreecellSolvingAlgorithm,
				FCSSimpleSimonSolvingAlgorithm** SimpleSimonSolvingAlgorithm,
				FCSTalonSolvingAlgorithm** TalonSolvingAlgorithm)
{
	switch(GamePreset)
	{
	case FCS_PRESET_NONE:
	case FCS_PRESET_BAKERS_DOZEN:
	case FCS_PRESET_BAKERS_GAME:
	case FCS_PRESET_CRUEL:
	case FCS_PRESET_DER_KATZENSCHWANZ:
	case FCS_PRESET_DIE_SCHLANGE:
	case FCS_PRESET_EIGHT_OFF:
	case FCS_PRESET_FORECELL: 
	case FCS_PRESET_FREECELL:
	case FCS_PRESET_GOOD_MEASURE:
	case FCS_PRESET_KINGS_ONLY_BAKERS_GAME:
	case FCS_PRESET_RELAXED_FREECELL:
	case FCS_PRESET_RELAXED_SEAHAVEN_TOWERS:
	case FCS_PRESET_SEAHAVEN_TOWERS:
		*FreecellSolvingAlgorithm = FCSFreecellSolvingAlgorithm::Create(CommandLine);
		break;
	case FCS_PRESET_SIMPLE_SIMON:
		*SimpleSimonSolvingAlgorithm = FCSSimpleSimonSolvingAlgorithm::Create(CommandLine);
		break;
	case FCS_PRESET_KLONDIKE:
		*TalonSolvingAlgorithm = FCSTalonSolvingAlgorithm::Create(CommandLine);
		break;
	case FCS_PRESET_YUKON:
	case FCS_PRESET_BELEAGUERED_CASTLE:
	default:
		break;
	}
}

FCSStateSolvingReturnCodes MainSolve(FCSPresetID GamePreset, FCSStateWithLocations *InitialState,
				FCSFreecellSolvingAlgorithm* FreecellSolvingAlgorithm,
				FCSSimpleSimonSolvingAlgorithm* SimpleSimonSolvingAlgorithm,
				FCSTalonSolvingAlgorithm* TalonSolvingAlgorithm)
{
	switch(GamePreset)
	{
	case FCS_PRESET_NONE:
	case FCS_PRESET_BAKERS_DOZEN:
	case FCS_PRESET_BAKERS_GAME:
	case FCS_PRESET_CRUEL:
	case FCS_PRESET_DER_KATZENSCHWANZ:
	case FCS_PRESET_DIE_SCHLANGE:
	case FCS_PRESET_EIGHT_OFF:
	case FCS_PRESET_FORECELL: 
	case FCS_PRESET_FREECELL:
	case FCS_PRESET_GOOD_MEASURE:
	case FCS_PRESET_KINGS_ONLY_BAKERS_GAME:
	case FCS_PRESET_RELAXED_FREECELL:
	case FCS_PRESET_RELAXED_SEAHAVEN_TOWERS:
	case FCS_PRESET_SEAHAVEN_TOWERS:
		return FreecellSolvingAlgorithm->Solve(InitialState, 0);
	case FCS_PRESET_SIMPLE_SIMON:
		return SimpleSimonSolvingAlgorithm->Solve(InitialState, 0);
	case FCS_PRESET_KLONDIKE:
		return TalonSolvingAlgorithm->Solve(InitialState, 0);
	case FCS_PRESET_YUKON:
	case FCS_PRESET_BELEAGUERED_CASTLE:
	default:
		return FCS_STATE_DOES_NOT_EXIST;
	}

	return FCS_STATE_DOES_NOT_EXIST;
}

FCSStateSolvingReturnCodes MainResume(FCSPresetID GamePreset,
				FCSFreecellSolvingAlgorithm* FreecellSolvingAlgorithm,
				FCSSimpleSimonSolvingAlgorithm* SimpleSimonSolvingAlgorithm,
				FCSTalonSolvingAlgorithm* TalonSolvingAlgorithm)
{
	switch(GamePreset)
	{
	case FCS_PRESET_NONE:
	case FCS_PRESET_BAKERS_DOZEN:
	case FCS_PRESET_BAKERS_GAME:
	case FCS_PRESET_CRUEL:
	case FCS_PRESET_DER_KATZENSCHWANZ:
	case FCS_PRESET_DIE_SCHLANGE:
	case FCS_PRESET_EIGHT_OFF:
	case FCS_PRESET_FORECELL: 
	case FCS_PRESET_FREECELL:
	case FCS_PRESET_GOOD_MEASURE:
	case FCS_PRESET_KINGS_ONLY_BAKERS_GAME:
	case FCS_PRESET_RELAXED_FREECELL:
	case FCS_PRESET_RELAXED_SEAHAVEN_TOWERS:
	case FCS_PRESET_SEAHAVEN_TOWERS:
		FreecellSolvingAlgorithm->IncreaseMaxNumberOfCheckedStates();
		return FreecellSolvingAlgorithm->Resume(0);
	case FCS_PRESET_SIMPLE_SIMON:
		SimpleSimonSolvingAlgorithm->IncreaseMaxNumberOfCheckedStates();
		return SimpleSimonSolvingAlgorithm->Resume(0);
	case FCS_PRESET_KLONDIKE:
		TalonSolvingAlgorithm->IncreaseMaxNumberOfCheckedStates();
		return TalonSolvingAlgorithm->Resume(0);
	case FCS_PRESET_YUKON:
	case FCS_PRESET_BELEAGUERED_CASTLE:
	default:
		return FCS_STATE_DOES_NOT_EXIST;
	}

	return FCS_STATE_DOES_NOT_EXIST;

}


void MainOptimize(FCSPresetID GamePreset, FCSStateWithLocations *InitialState, FCSStateWithLocations *DuplicateInitialState,
				FCSFreecellSolvingAlgorithm* FreecellSolvingAlgorithm,
				FCSSimpleSimonSolvingAlgorithm* SimpleSimonSolvingAlgorithm,
				FCSTalonSolvingAlgorithm* TalonSolvingAlgorithm)
{
	FCSOptimizeSolvingAlgorithm<FCSFreecellSolvingAlgorithm>* FreecellOptimize;
	FCSOptimizeSolvingAlgorithm<FCSSimpleSimonSolvingAlgorithm>* SimpleSimonOptimize;
	FCSOptimizeSolvingAlgorithm<FCSTalonSolvingAlgorithm>* TalonOptimize;

	switch(GamePreset)
	{
	case FCS_PRESET_NONE:
	case FCS_PRESET_BAKERS_DOZEN:
	case FCS_PRESET_BAKERS_GAME:
	case FCS_PRESET_CRUEL:
	case FCS_PRESET_DER_KATZENSCHWANZ:
	case FCS_PRESET_DIE_SCHLANGE:
	case FCS_PRESET_EIGHT_OFF:
	case FCS_PRESET_FORECELL: 
	case FCS_PRESET_FREECELL:
	case FCS_PRESET_GOOD_MEASURE:
	case FCS_PRESET_KINGS_ONLY_BAKERS_GAME:
	case FCS_PRESET_RELAXED_FREECELL:
	case FCS_PRESET_RELAXED_SEAHAVEN_TOWERS:
	case FCS_PRESET_SEAHAVEN_TOWERS:
		FreecellSolvingAlgorithm->CleanData();
		FreecellOptimize = new FCSOptimizeSolvingAlgorithm<FCSFreecellSolvingAlgorithm>(FreecellSolvingAlgorithm);
		FreecellOptimize->Solve(InitialState, 0);
		FreecellOptimize->TraceSolution();
		FreecellOptimize->ShowSolution(InitialState, DuplicateInitialState);
		delete FreecellOptimize;
		break;
	case FCS_PRESET_SIMPLE_SIMON:
		SimpleSimonSolvingAlgorithm->CleanData();
		SimpleSimonOptimize = new FCSOptimizeSolvingAlgorithm<FCSSimpleSimonSolvingAlgorithm>(SimpleSimonSolvingAlgorithm);
		SimpleSimonOptimize->Solve(InitialState, 0);
		SimpleSimonOptimize->TraceSolution();
		SimpleSimonOptimize->ShowSolution(InitialState, DuplicateInitialState);
		delete SimpleSimonOptimize;
		break;
	case FCS_PRESET_KLONDIKE:
		TalonSolvingAlgorithm->CleanData();
		TalonOptimize = new FCSOptimizeSolvingAlgorithm<FCSTalonSolvingAlgorithm>(TalonSolvingAlgorithm);
		TalonOptimize->Solve(InitialState, 0);
		TalonOptimize->TraceSolution();
		TalonOptimize->ShowSolution(InitialState, DuplicateInitialState);
		delete TalonOptimize;
		break;
	case FCS_PRESET_YUKON:
	case FCS_PRESET_BELEAGUERED_CASTLE:
	default:
		break;
	}
}

void MainSolution(FCSPresetID GamePreset, FCSStateWithLocations *InitialState, FCSStateWithLocations *DuplicateInitialState,
				FCSFreecellSolvingAlgorithm* FreecellSolvingAlgorithm,
				FCSSimpleSimonSolvingAlgorithm* SimpleSimonSolvingAlgorithm,
				FCSTalonSolvingAlgorithm* TalonSolvingAlgorithm)
{
	switch(GamePreset)
	{
	case FCS_PRESET_NONE:
	case FCS_PRESET_BAKERS_DOZEN:
	case FCS_PRESET_BAKERS_GAME:
	case FCS_PRESET_CRUEL:
	case FCS_PRESET_DER_KATZENSCHWANZ:
	case FCS_PRESET_DIE_SCHLANGE:
	case FCS_PRESET_EIGHT_OFF:
	case FCS_PRESET_FORECELL: 
	case FCS_PRESET_FREECELL:
	case FCS_PRESET_GOOD_MEASURE:
	case FCS_PRESET_KINGS_ONLY_BAKERS_GAME:
	case FCS_PRESET_RELAXED_FREECELL:
	case FCS_PRESET_RELAXED_SEAHAVEN_TOWERS:
	case FCS_PRESET_SEAHAVEN_TOWERS:
		FreecellSolvingAlgorithm->TraceSolution();
		FreecellSolvingAlgorithm->ShowSolution(InitialState, DuplicateInitialState);
		break;
	case FCS_PRESET_SIMPLE_SIMON:
		SimpleSimonSolvingAlgorithm->TraceSolution();
		SimpleSimonSolvingAlgorithm->ShowSolution(InitialState, DuplicateInitialState);
		break;
	case FCS_PRESET_KLONDIKE:
		TalonSolvingAlgorithm->TraceSolution();
		TalonSolvingAlgorithm->ShowSolution(InitialState, DuplicateInitialState);
		break;
	case FCS_PRESET_YUKON:
	case FCS_PRESET_BELEAGUERED_CASTLE:
	default:
		break;
	}
}


void MainStats(FCSPresetID GamePreset,
				FCSFreecellSolvingAlgorithm* FreecellSolvingAlgorithm,
				FCSSimpleSimonSolvingAlgorithm* SimpleSimonSolvingAlgorithm,
				FCSTalonSolvingAlgorithm* TalonSolvingAlgorithm)
{
	switch(GamePreset)
	{
	case FCS_PRESET_NONE:
	case FCS_PRESET_BAKERS_DOZEN:
	case FCS_PRESET_BAKERS_GAME:
	case FCS_PRESET_CRUEL:
	case FCS_PRESET_DER_KATZENSCHWANZ:
	case FCS_PRESET_DIE_SCHLANGE:
	case FCS_PRESET_EIGHT_OFF:
	case FCS_PRESET_FORECELL: 
	case FCS_PRESET_FREECELL:
	case FCS_PRESET_GOOD_MEASURE:
	case FCS_PRESET_KINGS_ONLY_BAKERS_GAME:
	case FCS_PRESET_RELAXED_FREECELL:
	case FCS_PRESET_RELAXED_SEAHAVEN_TOWERS:
	case FCS_PRESET_SEAHAVEN_TOWERS:
		cout << "Total number of states checked is " << FreecellSolvingAlgorithm->GetNumberOfCheckedStates() << "." << endl;
		cout << "This scan generated " << FreecellSolvingAlgorithm->GetNumberOfStatesInCollection() <<  " states." << endl;
		break;
	case FCS_PRESET_SIMPLE_SIMON:
		cout << "Total number of states checked is " << SimpleSimonSolvingAlgorithm->GetNumberOfCheckedStates() << "." << endl;
		cout << "This scan generated " << SimpleSimonSolvingAlgorithm->GetNumberOfStatesInCollection() <<  " states." << endl;
		break;
	case FCS_PRESET_KLONDIKE:
		cout << "Total number of states checked is " << TalonSolvingAlgorithm->GetNumberOfCheckedStates() << "." << endl;
		cout << "This scan generated " << TalonSolvingAlgorithm->GetNumberOfStatesInCollection() <<  " states." << endl;
		break;
	case FCS_PRESET_YUKON:
	case FCS_PRESET_BELEAGUERED_CASTLE:
	default:
		break;
	}
}
