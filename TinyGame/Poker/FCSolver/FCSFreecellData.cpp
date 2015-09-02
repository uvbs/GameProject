////////////////////////////////////////////////
///\file FCSFreecellData.cpp
///\brief This file contains the implementation of the FCSFreecellData class
///\author Michael Mann
///\version 1.0
///\date August 2002
////////////////////////////////////////////////

#include "FCSFreecellData.h"


#include "FCSTalonStateWithLocations.h"

#include <iostream>
using namespace std;

FCSFreecellData::FCSFreecellData()
{
	InitFCSFreecellData();
}

void FCSFreecellData::InitFCSFreecellData()
{
	IsOptimizeClass = false;

	m_StateType = DEFAULT_STATE_TYPE;

	m_StatePacks = NULL;
	m_MaxNumberOfStatePacks = 0;
	m_NumberOfStatePacks = 0;
	m_NumberOfStatesInLastPack = 0;
	m_StatePackLength = 0;

	m_NumberOfCheckedStates = 0;
	m_SolutionStates = NULL;
	m_NumberOfSolutionStates = 0;

	m_SolutionMoves = NULL;
	m_ProtoSolutionMoves = NULL;

	m_NumberOfStatesInCollection = 0;
	m_MaxNumberOfStatesInCollection = -1;

	m_MaxDepth = -1;
	m_MaxNumberOfCheckedStates = -1;

	m_DebugDisplayInfo = NULL;

	m_TestsOrderNumber = FCS_TESTS_NUM;
	for(int a=0;a<FCS_TESTS_NUM;a++)
		m_TestsOrder[a] = a;

	m_StateStorage = NULL;
	m_StackStorage = NULL;

	m_NumberOfFreecells = DEFAULT_NUMBER_OF_FREECELLS;
	m_NumberOfStacks = DEFAULT_NUMBER_OF_STACKS;
	m_NumberOfDecks = DEFAULT_NUMBER_OF_DECKS;

	m_SequencesAreBuiltBy = DEFAULT_BUILD_SEQUENCE_TYPE;
	m_IsUnlimitedSequenceMove = false;
	m_EmptyStacksFill = DEFAULT_EMPTY_STACK_FILL_TYPE;

	m_OptimizeSolutionPath = false;

	//****** I'm not quite sure where to put these, so they're here for now *****
	//****** Since they vary, should they all be pointers and allocated only if needed ?  *****
	m_IndirectCompare = NULL;
	m_IndirectHash = NULL;
}

FCSFreecellData::FCSFreecellData(FCCommandLineArguments* CommandLine)
{
	InitFCSFreecellData();

	//get state type (Debug, Compact, Indirect)
	m_StateType = CommandLine->GetStateType();

	//initialize state packs
	m_MaxNumberOfStatePacks = IA_STATE_PACKS_GROW_BY;
	m_StatePacks = CreateStateWithLocationsMatrix(m_MaxNumberOfStatePacks);
	m_NumberOfStatePacks = 1;
	m_NumberOfStatesInLastPack = 0;
	m_StatePackLength = FC_MAX_STATE_PACK_LENGTH / GetFCSStateWithLocationsClassSize();
	m_StatePacks->CreateArray(0, m_StatePackLength);

	m_MaxNumberOfStatesInCollection = CommandLine->GetMaxStoredStates();
	m_MaxDepth = CommandLine->GetMaxDepth();
	m_MaxNumberOfCheckedStates = CommandLine->GetMaxNumberOfIterations();

	if (CommandLine->GetNumberOfTests() > 0)
	{
		// Initialize the test order
		if (CommandLine->GetTestOrderNumber(0) != NULL)
		{
			int b;
			for (b = 0;CommandLine->GetTestOrderNumber(b) != NULL;b++)
				m_TestsOrder[b] = CommandLine->GetTestOrderNumber(b);

			m_TestsOrderNumber = b;
		}
	}

//*****  The HASH_TABLE_SIZE parameter may be abandoned for a user-controlled value

	//create state and stack storage
	switch(CommandLine->GetStateStorageType())
	{
	case FC_AVL_TREE:
		m_StateStorage = new FCAVLTreeStateStorage(&m_CompareFunction);
		break;
	case FC_AVL_RED_BLACK_TREE:
		m_StateStorage = new FCAVLRedBlackTreeStateStorage(&m_CompareFunction);
		break;
	case FC_RED_BLACK_TREE:
		m_StateStorage = new FCRedBlackTreeStateStorage(&m_CompareFunction);
		break;
	case FC_GLIB_TREE:
		m_StateStorage = new FCGLIBTreeStateStorage(&m_CompareFunction);
		break;
	case FC_GLIB_HASH:
		m_StateStorage = new FCGLIBHashStateStorage(HASH_TABLE_SIZE, &m_CompareFunction, &m_MD5Hash);
		break;
	case FC_INTERNAL_HASH:
		m_StateStorage = new FCInternalHashStateStorage(HASH_TABLE_SIZE, &m_CompareFunction, &m_MD5Hash);
		break;
	case FC_INDIRECT_HASH:
		m_StateStorage = new FCIndirectStateStorage();
			break;
	default:
		m_StateStorage = NULL;
	}

	if ((m_StateType == FC_INDIRECT_STATE) || (m_StateType == FC_TALON_INDIRECT_STATE))
	{
		m_IndirectCompare = new FCSIndirectCardCompareAlgorithm<FCSIndirectCard, void>();
		m_IndirectHash = new MD5HashAlgorithm<FCSIndirectCard>();

		switch(CommandLine->GetStackStorageType())
		{
		case FC_AVL_TREE:
			m_StackStorage = new FCAVLTreeStackStorage(m_IndirectCompare);
			break;
		case FC_AVL_RED_BLACK_TREE:
			m_StackStorage = new FCAVLRedBlackTreeStackStorage(m_IndirectCompare);
			break;
		case FC_RED_BLACK_TREE:
			m_StackStorage = new FCRedBlackTreeStackStorage(m_IndirectCompare);
			break;
		case FC_GLIB_TREE:
			m_StackStorage = new FCGLIBTreeStackStorage(m_IndirectCompare);
			break;
		case FC_GLIB_HASH:
			m_StackStorage = new FCGLIBHashStackStorage(HASH_TABLE_SIZE, m_IndirectCompare, m_IndirectHash);
			break;
		case FC_INTERNAL_HASH:
			m_StackStorage = new FCInternalHashStackStorage(HASH_TABLE_SIZE, m_IndirectCompare, m_IndirectHash);
			break;
		default:
			m_StackStorage = NULL;
		}
	}

	m_DebugDisplayInfo = CommandLine->GetDebugDisplayInfo();
	m_NumberOfFreecells = CommandLine->GetNumberOfFreecells();
	m_NumberOfStacks = CommandLine->GetNumberOfStacks();
	m_NumberOfDecks = CommandLine->GetNumberOfDecks();

	m_SequencesAreBuiltBy = CommandLine->GetSequenceBuildType();

	m_IsUnlimitedSequenceMove = CommandLine->GetIsSequenceMoveUnlimited();
	m_EmptyStacksFill = CommandLine->GetEmptyStacksFill();

	m_OptimizeSolutionPath = CommandLine->GetOptimizeSolution();
}

FCSFreecellData::~FCSFreecellData()
{	
	if (!IsOptimizeClass)
	{
		int p,s;
		for(p=0; p<m_NumberOfStatePacks-1;p++)
			for(s=0 ; s < m_StatePackLength ; s++)
				m_StatePacks->DeleteStateWithLocationsParent(p, s);

		for(s=0; s < m_NumberOfStatesInLastPack ; s++)
			m_StatePacks->DeleteStateWithLocationsParent(p, s);

		// De-allocate the state packs  - fcs_state_ia_finish(instance);
		for(int a=0;a<m_NumberOfStatePacks;a++)
			m_StatePacks->DeleteArray(a);

		delete m_StatePacks;

		delete m_StateStorage;
		delete m_StackStorage;

		if (m_IndirectCompare != NULL)
			delete m_IndirectCompare;

		if (m_IndirectHash != NULL)
			delete m_IndirectHash;

		if (m_DebugDisplayInfo != NULL)
			delete m_DebugDisplayInfo;
	}

	if (m_ProtoSolutionMoves != NULL)
	{
		delete [] m_ProtoSolutionMoves;
		m_ProtoSolutionMoves = NULL;
	}

	if (m_SolutionMoves != NULL)
	{
		delete [] m_SolutionMoves;
		m_SolutionMoves = NULL;
	}

	if (m_SolutionStates != NULL)
	{
		delete [] m_SolutionStates;
		m_SolutionStates = NULL;
	}

}

void FCSFreecellData::InitSolve(FCSStateWithLocations* InitState)
{
	// Allocate the first state and initialize it to init_state
	FCSStateWithLocations *NoUse = NULL;

	//putting in a dummy state
	CheckAndAddState(InitState, &NoUse, 0);
}

void FCSFreecellData::CleanData()
{
	for(int a=0;a<m_NumberOfSolutionStates-1;a++)
	{
		m_SolutionStates->Get(a, 0)->CleanState();
		m_SolutionStates->DeleteArray(a);
		delete m_ProtoSolutionMoves[a];
	}

	delete [] m_SolutionStates;
	m_SolutionStates = NULL;

	delete [] m_ProtoSolutionMoves;
	m_ProtoSolutionMoves = NULL;
}

void FCSFreecellData::ShowSolution(FCSStateWithLocations* InitStateWithLocations, 
								   FCSStateWithLocations* DupStateWithLocations)
{
	//Combine the move stacks of every depth to one big move stack.
	CreateTotalMovesStack();

	for(int a=0;a<m_NumberOfSolutionStates;a++)
	{
		m_SolutionStates->Get(a, 0)->CleanState();
		m_SolutionStates->Delete(a);
	}

	delete [] m_SolutionStates;
	m_SolutionStates = NULL;

	m_SolutionMoves->Normalize(InitStateWithLocations, m_NumberOfFreecells, m_NumberOfStacks, m_NumberOfDecks);

	FCSMove* Move = FCSMove::Create();
	char AsString[STATE_STRING_SIZE];
	int NumberOfMoves = 0;
	
	if (m_DebugDisplayInfo->m_DisplayDebugOptions & DEBUG_DISPLAY_STATES)
	{
		cout << "-=-=-=-=-=-=-=-=-=-=-=-" << endl << endl;
		DupStateWithLocations->StateAsString(AsString, m_DebugDisplayInfo);
		cout << AsString << endl << endl << "====================" << endl << endl;
	}

	while (GetNextMove(DupStateWithLocations, Move) == 0)
	{
		if (m_DebugDisplayInfo->m_DisplayDebugOptions & DEBUG_DISPLAY_MOVES)
		{
			(m_DebugDisplayInfo->m_DisplayDebugOptions & DEBUG_USE_STANDARD_NOTATION) ?
					Move->MoveAsStringStandardNotation(AsString) :
					Move->MoveAsString(AsString);

			if (m_DebugDisplayInfo->m_DisplayDebugOptions & (DEBUG_DISPLAY_STATES | DEBUG_USE_STANDARD_NOTATION))
				cout << "Move: ";

			NumberOfMoves++;

			cout << NumberOfMoves << ":\t" << AsString 
				 << ((m_DebugDisplayInfo->m_DisplayDebugOptions & DEBUG_USE_STANDARD_NOTATION) ? " " : "\n");

			if ((m_DebugDisplayInfo->m_DisplayDebugOptions & DEBUG_USE_STANDARD_NOTATION) &&
				((NumberOfMoves % 10 == 0) || (m_DebugDisplayInfo->m_DisplayDebugOptions & DEBUG_DISPLAY_STATES)))
				cout << '\n';

			if (m_DebugDisplayInfo->m_DisplayDebugOptions & DEBUG_DISPLAY_STATES)
				cout << '\n';
		}

		if (m_DebugDisplayInfo->m_DisplayDebugOptions & DEBUG_DISPLAY_STATES)
		{
			DupStateWithLocations->StateAsString(AsString, m_DebugDisplayInfo);
			cout << AsString << endl;
		}

		if (m_DebugDisplayInfo->m_DisplayDebugOptions & (DEBUG_DISPLAY_STATES | (!DEBUG_USE_STANDARD_NOTATION)))
			cout << '\n' << "====================" << endl << endl;
	}

	if (m_DebugDisplayInfo->m_DisplayDebugOptions & (DEBUG_USE_STANDARD_NOTATION | (!DEBUG_DISPLAY_STATES )))
		cout << '\n' << endl;

	delete Move;
}

void FCSFreecellData::TraceSolution()
{
	//  Trace the solution.
	FCSStateWithLocations* State = m_FinalState;

	// The depth of the last state reached is the number of them
	m_NumberOfSolutionStates = State->m_Depth+1;

	if (m_SolutionStates != NULL)
		delete [] m_SolutionStates;

	if (m_ProtoSolutionMoves != NULL)
		delete [] m_ProtoSolutionMoves;

	// Allocate space for the solution stacks
	m_SolutionStates = CreateStateWithLocationsMatrix(m_NumberOfSolutionStates);
	m_ProtoSolutionMoves = new FCSMoveStack*[m_NumberOfSolutionStates-1];

	// Retrace the step from the current state to its parents
	while (State->m_Parent != NULL)
	{
		// Mark the state as part of the non-optimized solution
		State->m_Visited |= FCS_VISITED_IN_SOLUTION_PATH;

		// Duplicate the move stack
		m_ProtoSolutionMoves[State->m_Depth-1] = State->m_MovesToParent->Copy();

		// Duplicate the state to a freshly malloced memory
		m_SolutionStates->Create(State->m_Depth);
		m_SolutionStates->Get(State->m_Depth, 0)->Copy(State);

		// Move to the parent state
		State = State->m_Parent;
	}

	// There's one more state than there are move stacks
	State->m_Visited |= FCS_VISITED_IN_SOLUTION_PATH;
	m_SolutionStates->Create(0);
	m_SolutionStates->Get(0, 0)->Copy(State);
}

int FCSFreecellData::GetNumberOfCheckedStates()
{
	return m_NumberOfCheckedStates;
}

int FCSFreecellData::GetNumberOfStatesInCollection()
{
	return m_NumberOfStatesInCollection;
}

void FCSFreecellData::IncreaseMaxNumberOfCheckedStates()
{
	m_MaxNumberOfCheckedStates += 10;
}

int FCSFreecellData::GetNextMove(FCSStateWithLocations* StateWithLocations, FCSMove* Move)
{
	int ReturnValue = m_SolutionMoves->Pop(&Move);

	if (ReturnValue == 0)
	{
		ApplyMove(StateWithLocations, Move, m_NumberOfFreecells, 
					m_NumberOfStacks, m_NumberOfDecks); 
	}

	return ReturnValue;
}

void FCSFreecellData::CreateTotalMovesStack()
{
	int Depth;
	m_SolutionMoves = CreateMoveStack();

	// The moves are inserted from the highest depth to depth 0 in order
	// to preserve their order stack-wise 

	if (m_DebugDisplayInfo->m_DisplayDebug)
	{
		int Sum = 0;
		for(Depth=0 ; Depth <= m_NumberOfSolutionStates-2 ; Depth++)
		{
			Sum += m_ProtoSolutionMoves[Depth]->GetNumberOfMoves();
			cout << "Depth " << Depth << " : " << Sum << endl;
		}
	}

	for(Depth=m_NumberOfSolutionStates-2;Depth>=0;Depth--)
		m_SolutionMoves->SwallowStack(m_ProtoSolutionMoves[Depth]);

	delete [] m_ProtoSolutionMoves;
	m_ProtoSolutionMoves = NULL;
}

//fcs_state_ia_alloc
FCSStateWithLocations* FCSFreecellData::StatePackAlloc()
{
	if (m_NumberOfStatesInLastPack == m_StatePackLength)
	{
		if (m_NumberOfStatePacks == m_MaxNumberOfStatePacks)
		{
			m_MaxNumberOfStatePacks += IA_STATE_PACKS_GROW_BY;
			m_StatePacks->ReallocArray(m_NumberOfStatePacks, m_MaxNumberOfStatePacks);
		}
		m_StatePacks->CreateArray(m_NumberOfStatePacks, m_StatePackLength);
		m_NumberOfStatePacks++;
		m_NumberOfStatesInLastPack = 0;
	}

	return m_StatePacks->Get(m_NumberOfStatePacks-1, m_NumberOfStatesInLastPack++);
}

//fcs_state_ia_release
void FCSFreecellData::StatePackRelease()
{
	m_NumberOfStatesInLastPack--;
}

void FCSFreecellData::CheckStateBegin(FCSStateWithLocations** NewStateWithLocations, FCSStateWithLocations* StateWithLocations,
										  FCSMoveStack *Move)
{
	(*NewStateWithLocations) = StatePackAlloc();
	(*NewStateWithLocations)->Copy(StateWithLocations);
	(*NewStateWithLocations)->m_Parent = StateWithLocations;
	(*NewStateWithLocations)->m_MovesToParent = Move;
	// Make sure depth is consistent with the game graph.
	// I.e: the depth of every newly discovered state is derived from
	// the state from which it was discovered.
	(*NewStateWithLocations)->m_Depth = StateWithLocations->m_Depth + 1;
	(*NewStateWithLocations)->m_Visited = FCS_VISITED_NOT_VISITED; 
	Move->Reset();
}

bool FCSFreecellData::CheckStateEnd(FCSStateWithLocations** NewStateWithLocations, FCSStateWithLocations* StateWithLocations,
										FCSDerivedStatesList** DerivedStateList, FCSMoveStack** Move, FCSMove** TempMove, int depth, 
										FCSStateSolvingReturnCodes* ReturnCode)
{
	FCSStateWithLocations* ExistingState;
	FCSStateSolvingReturnCodes Check;

// The last move in a move stack should be FCS_MOVE_TYPE_CANONIZE
// because it indicates that the order of the stacks and freecells
// need to be recalculated 

	(*TempMove)->SetType(FCS_MOVE_TYPE_CANONIZE);
	(*Move)->Push(*TempMove);

	Check = CheckAndAddState(*NewStateWithLocations, &ExistingState, depth);
		
	if ((Check == FCS_STATE_BEGIN_SUSPEND_PROCESS) ||
		(Check == FCS_STATE_SUSPEND_PROCESS))
	{
		// This state is not going to be used, so
		// let's clean it.
		(*NewStateWithLocations)->CleanState();
		*ReturnCode = Check;
		return true;
	}
	else if (Check == FCS_STATE_ALREADY_EXISTS)
	{
		StatePackRelease();
		(*DerivedStateList)->AddState(ExistingState);
	}
	else
	{   
		(*DerivedStateList)->AddState(*NewStateWithLocations);

		// See above.
		(*Move) = CreateMoveStack();
	}

	return false;
}

FCSStateSolvingReturnCodes FCSFreecellData::CheckAndAddState(FCSStateWithLocations* NewState,
																 FCSStateWithLocations** ExistingState,
																 int Depth)
{
	if (((m_MaxNumberOfCheckedStates >= 0) &&
		(m_NumberOfCheckedStates >= m_MaxNumberOfCheckedStates)) ||
		((m_MaxNumberOfStatesInCollection >= 0) &&
		(m_NumberOfStatesInCollection >= m_MaxNumberOfStatesInCollection)))
	{
		return FCS_STATE_BEGIN_SUSPEND_PROCESS;
	}

	if ((m_MaxDepth >= 0) &&
		(m_MaxDepth <= Depth))
	{
		return FCS_STATE_EXCEEDS_MAX_DEPTH;
	}

	NewState->CanonizeState(m_NumberOfFreecells, m_NumberOfStacks);
	NewState->CacheStacks(m_StackStorage, m_NumberOfStacks);

	if (m_StateStorage->CheckAndInsert(ExistingState, NewState))
	{
		// The new state was not found in the cache, and it was already inserted
		m_NumberOfStatesInCollection++;
		return FCS_STATE_DOES_NOT_EXIST;
	}

	return FCS_STATE_ALREADY_EXISTS;
}

bool FCSFreecellData::IsParentCard(FCSCard* Child, FCSCard* Parent)
{
	return ((Child->GetCardNumber()+1 == Parent->GetCardNumber()) && 
			((m_SequencesAreBuiltBy == FCS_SEQ_BUILT_BY_RANK) ?
				1 :
				((m_SequencesAreBuiltBy == FCS_SEQ_BUILT_BY_SUIT) ?
				   (Child->GetSuit() == Parent->GetSuit()) : 
					((Child->GetSuit() & 0x1) != (Parent->GetSuit()&0x1))
			))
			);
}

int FCSFreecellData::CalculateMaxSequenceMoves(int FreecellNumber, int FreeStackNumber)
{
	return ( m_IsUnlimitedSequenceMove ? INT_MAX :
			  (
			  (m_EmptyStacksFill == FCS_ES_FILLED_BY_ANY_CARD) ? 
					((FreecellNumber+1)<<(FreeStackNumber)) :
					(FreecellNumber+1)
			  )
		   );
}

void FCSFreecellData::MoveSequence(FCSStateWithLocations* NewStateWithLocations, FCSCard* Card, FCSMoveStack *MoveStack, FCSMove** TempMove, 
					int DestStack, int SourceStack, int Start, int End)
{
	int Loop;

	for (Loop = Start; Loop <= End ; Loop++)
		NewStateWithLocations->PushStackCardIntoStack(DestStack, SourceStack, Loop);

	for (Loop = Start; Loop <= End ; Loop++)
		NewStateWithLocations->PopStackCard(SourceStack, Card);

	(*TempMove)->SetType(FCS_MOVE_TYPE_STACK_TO_STACK);
	(*TempMove)->SetSourceStack(SourceStack);
	(*TempMove)->SetDestStack(DestStack);
	(*TempMove)->SetNumberOfCardsInSequence(End-Start+1);

	MoveStack->Push(*TempMove);
}