#ifndef MMANN_FCENUMS_H
#define MMANN_FCENUMS_H

////////////////////////////////////////////////
///\file FCEnums.h
///\brief This file contains the enums needed for freecell solver
///\author Michael Mann
///\version 1.0
///\date August 2002
////////////////////////////////////////////////

///The types of moves that can be made during a game
enum FCSMoveTypes
{
	FCS_MOVE_TYPE_STACK_TO_STACK,
	FCS_MOVE_TYPE_STACK_TO_FREECELL,
	FCS_MOVE_TYPE_FREECELL_TO_STACK,
	FCS_MOVE_TYPE_FREECELL_TO_FREECELL,
	FCS_MOVE_TYPE_STACK_TO_FOUNDATION,
	FCS_MOVE_TYPE_FREECELL_TO_FOUNDATION,
	FCS_MOVE_TYPE_FLIP_CARD,
	FCS_MOVE_TYPE_DEAL_GYPSY_TALON,
	FCS_MOVE_TYPE_KLONDIKE_TALON_TO_STACK,
	FCS_MOVE_TYPE_KLONDIKE_TALON_TO_FOUNDATION,
	FCS_MOVE_TYPE_KLONDIKE_FLIP_TALON,
	FCS_MOVE_TYPE_KLONDIKE_REDEAL_TALON,
	FCS_MOVE_TYPE_SEQ_TO_FOUNDATION,
	FCS_MOVE_TYPE_CANONIZE,
	FCS_MOVE_TYPE_SEPARATOR,
	FCS_MOVE_TYPE_NULL,
};

///Possible ways to fill empty stacks for a game
enum FCEmptyStacksFillType {FCS_ES_FILLED_BY_ANY_CARD, 
							FCS_ES_FILLED_BY_KINGS_ONLY, 
							FCS_ES_FILLED_BY_NONE};

///Solving methods for a game
enum FCSolvingMethodType {FCS_METHOD_NONE  = -1, 
						  FCS_METHOD_HARD_DFS  = 0,
						  FCS_METHOD_SOFT_DFS = 1,
						  FCS_METHOD_BFS = 2,
						  FCS_METHOD_A_STAR = 3,
						  FCS_METHOD_OPTIMIZE = 4,
						  FCS_METHOD_RANDOM_DFS = 5};

///Types of states
enum FCStateType {FC_DEBUG_STATE = 1, 
				  FC_COMPACT_STATE, 
				  FC_INDIRECT_STATE,
				  FC_TALON_DEBUG_STATE,
				  FC_TALON_COMPACT_STATE,
				  FC_TALON_INDIRECT_STATE}; 

///Types of talons
enum FCSTalonType {FCS_TALON_NONE,
				   FCS_TALON_GYPSY,
				   FCS_TALON_KLONDIKE};

///How sequences can be built in a game
enum BuildSequenceByEnum{FCS_SEQ_BUILT_BY_SUIT = 1, 
						 FCS_SEQ_BUILT_BY_ALTERNATE_COLOR, 
						 FCS_SEQ_BUILT_BY_RANK};

///Type of storage for stacks and states
enum FCStorageType {FC_AVL_TREE = 1, 
					FC_AVL_RED_BLACK_TREE,
					FC_RED_BLACK_TREE,
					FC_GLIB_TREE,
					FC_GLIB_HASH,
					FC_INTERNAL_HASH,
					FC_INDIRECT_HASH};

///Solving functions return codes
enum FCSStateSolvingReturnCodes
{
	FCS_STATE_WAS_SOLVED,
	FCS_STATE_IS_NOT_SOLVEABLE,
	FCS_STATE_ALREADY_EXISTS,
	FCS_STATE_EXCEEDS_MAX_NUM_TIMES,
	FCS_STATE_BEGIN_SUSPEND_PROCESS,
	FCS_STATE_SUSPEND_PROCESS,
	FCS_STATE_EXCEEDS_MAX_DEPTH,
	FCS_STATE_ORIGINAL_STATE_IS_NOT_SOLVEABLE,
	FCS_STATE_INVALID_STATE,
	FCS_STATE_NOT_BEGAN_YET,
	FCS_STATE_DOES_NOT_EXIST,
	FCS_STATE_OPTIMIZED,
};

///How the states can be visited
enum FCSVisitEnum 
{
	FCS_VISITED_NOT_VISITED = 0x0,
	FCS_VISITED_VISITED = 0x1, 
	FCS_VISITED_IN_SOLUTION_PATH = 0x2, 
	FCS_VISITED_IN_OPTIMIZED_PATH = 0x4,
};

#endif