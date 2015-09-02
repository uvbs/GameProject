#ifndef MMANN_CONFIG_H
#define MMANN_CONFIG_H

/***********************
These are the presets for freecell.
May decide to get rid of these max presets altogether at some point

#define MAX_NUM_INITIAL_CARDS_IN_A_STACK 7
#define MAX_NUM_STACKS					8
#define MAX_NUM_FREECELLS				4
#define MAX_NUM_DECKS					2
**********************************/

//These max presets handle all games currently solvable by SF's solver
#define MAX_NUM_INITIAL_CARDS_IN_A_STACK 10
#define MAX_NUM_STACKS					13
#define MAX_NUM_FREECELLS				8
#define MAX_NUM_DECKS					2

#if MAX_NUM_INITIAL_CARDS_IN_A_STACK+12>(MAX_NUM_DECKS*52)
#define MAX_NUM_CARDS_IN_A_STACK (MAX_NUM_DECKS*52)
#else
#define MAX_NUM_CARDS_IN_A_STACK (MAX_NUM_INITIAL_CARDS_IN_A_STACK+12)
#endif

//These defaults assume freecell is the default game
#define DEFAULT_NUMBER_OF_FREECELLS		4
#define DEFAULT_NUMBER_OF_STACKS		8
#define DEFAULT_NUMBER_OF_DECKS			1
#define DEFAULT_STATE_TYPE				FC_DEBUG_STATE
#define DEFAULT_STATE_STORAGE_TYPE		FC_AVL_TREE
#define DEFAULT_STACK_STORAGE_TYPE		FC_AVL_TREE
#define DEFAULT_BUILD_SEQUENCE_TYPE		FCS_SEQ_BUILT_BY_ALTERNATE_COLOR
#define DEFAULT_EMPTY_STACK_FILL_TYPE	FCS_ES_FILLED_BY_ANY_CARD
#define DEFAULT_SOLVING_METHOD			FCS_METHOD_HARD_DFS

#define FCS_TESTS_NUM 10

#define FC_MAX_STATE_PACK_LENGTH		0x10000
#define IA_STATE_PACKS_GROW_BY			32
#define INCREASE_DFS_MAX_DEPTH_BY		16
#define STATE_STRING_SIZE				2000

#endif