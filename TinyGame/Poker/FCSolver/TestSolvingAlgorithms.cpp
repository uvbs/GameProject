#include "FCCommandLineArguments.h"
#include "FCSFreecellSolvingAlgorithm.h"
#include "FCSOptimizeSolvingAlgorithm.h"
#include "TestSolvingAlgorithms.h"
#include "MainGameFunctions.h"
#include "AFreecellGameBoard.h"

#include <iostream>
using std::endl;
using std::cout;
using std::cerr;

////////////////////////////////////////////////
///\file TestSolvingAlgorithms.cpp
///\brief This file contains the implementation of functions to test the freecell solving algorithms
///\author Michael Mann
///\version 1.0
///\date August 2002
////////////////////////////////////////////////

/*************************
Is there a way that I don't have to cast all this structures all of the time?
**************************/

/************************
* FREECELL TESTS
*************************/
/*
// run through each solving algorithm type in debug state type, 
// no optimization, avl-tree storage and stack type, on the board in firsttest.txt
static const char * const Test1[] = { "", "-sam", "-s", "-dd", "-stsr", "avl", "-sksr", "avl", "-st", "debug", "-me", "dfs", "-g", "freecell", "firsttest.txt", NULL};
static const char * const Test2[] = { "", "-sam", "-s", "-dd", "-stsr", "avl", "-sksr", "avl", "-st", "debug", "-me", "soft-dfs", "-g", "freecell", "firsttest.txt", NULL};
static const char * const Test3[] = { "", "-sam", "-s", "-dd", "-stsr", "avl", "-sksr", "avl", "-st", "debug", "-me", "random-dfs", "-g", "freecell", "firsttest.txt", NULL};
static const char * const Test4[] = { "", "-sam", "-s", "-dd", "-stsr", "avl", "-sksr", "avl", "-st", "debug", "-me", "a-star", "-g", "freecell", "firsttest.txt", NULL};
static const char * const Test5[] = { "", "-sam", "-s", "-dd", "-stsr", "avl", "-sksr", "avl", "-st", "debug", "-me", "bfs", "-g", "freecell", "firsttest.txt", NULL};

// run through each solving algorithm type in debug state type, 
// optimization on, avl-tree storage and stack type, on the board in firsttest.txt
static const char * const Test6[] = { "", "-sam", "-s", "-dd", "-stsr", "avl", "-sksr", "avl", "-st", "debug", "-opt", "-me", "dfs", "-g", "freecell", "firsttest.txt", NULL};
static const char * const Test7[] = { "", "-sam", "-s", "-dd", "-stsr", "avl", "-sksr", "avl", "-st", "debug", "-opt", "-me", "soft-dfs", "-g", "freecell", "firsttest.txt", NULL};
static const char * const Test8[] = { "", "-sam", "-s", "-dd", "-stsr", "avl", "-sksr", "avl", "-st", "debug", "-opt", "-me", "random-dfs", "-g", "freecell", "firsttest.txt", NULL};
static const char * const Test9[] = { "", "-sam", "-s", "-dd", "-stsr", "avl", "-sksr", "avl", "-st", "debug", "-opt", "-me", "a-star", "-g", "freecell", "firsttest.txt", NULL};
static const char * const Test10[] = { "", "-sam", "-s", "-dd", "-stsr", "avl", "-sksr", "avl", "-st", "debug", "-opt", "-me", "bfs", "-g", "freecell", "firsttest.txt", NULL};

// run through each solving algorithm type in debug state type, 
// no optimization, avl red-black tree storage and stack type, on the board in firsttest.txt
static const char * const Test11[] = { "", "-sam", "-s", "-dd", "-stsr", "avl_rb", "-sksr", "avl_rb", "-st", "debug", "-me", "dfs", "-g", "freecell", "firsttest.txt", NULL};
static const char * const Test12[] = { "", "-sam", "-s", "-dd", "-stsr", "avl_rb", "-sksr", "avl_rb", "-st", "debug", "-me", "soft-dfs", "-g", "freecell", "firsttest.txt", NULL};
static const char * const Test13[] = { "", "-sam", "-s", "-dd", "-stsr", "avl_rb", "-sksr", "avl_rb", "-st", "debug", "-me", "random-dfs", "-g", "freecell", "firsttest.txt", NULL};
static const char * const Test14[] = { "", "-sam", "-s", "-dd", "-stsr", "avl_rb", "-sksr", "avl_rb", "-st", "debug", "-me", "a-star", "-g", "freecell", "firsttest.txt", NULL};
static const char * const Test15[] = { "", "-sam", "-s", "-dd", "-stsr", "avl_rb", "-sksr", "avl_rb", "-st", "debug", "-me", "bfs", "-g", "freecell", "firsttest.txt", NULL};

// run through each solving algorithm type in debug state type, 
// optimization on, avl red-black tree storage and stack type, on the board in firsttest.txt
static const char * const Test16[] = { "", "-sam", "-s", "-dd", "-stsr", "avl_rb", "-sksr", "avl_rb", "-st", "debug", "-opt", "-me", "dfs", "-g", "freecell", "firsttest.txt", NULL};
static const char * const Test17[] = { "", "-sam", "-s", "-dd", "-stsr", "avl_rb", "-sksr", "avl_rb", "-st", "debug", "-opt", "-me", "soft-dfs", "-g", "freecell", "firsttest.txt", NULL};
static const char * const Test18[] = { "", "-sam", "-s", "-dd", "-stsr", "avl_rb", "-sksr", "avl_rb", "-st", "debug", "-opt", "-me", "random-dfs", "-g", "freecell", "firsttest.txt", NULL};
static const char * const Test19[] = { "", "-sam", "-s", "-dd", "-stsr", "avl_rb", "-sksr", "avl_rb", "-st", "debug", "-opt", "-me", "a-star", "-g", "freecell", "firsttest.txt", NULL};
static const char * const Test20[] = { "", "-sam", "-s", "-dd", "-stsr", "avl_rb", "-sksr", "avl_rb", "-st", "debug", "-opt", "-me", "bfs", "-g", "freecell", "firsttest.txt", NULL};

// run through each solving algorithm type in debug state type, 
// no optimization, red-black tree storage and stack type, on the board in firsttest.txt
static const char * const Test21[] = { "", "-sam", "-s", "-dd", "-stsr", "red_black", "-sksr", "red_black", "-st", "debug", "-me", "dfs", "-g", "freecell", "firsttest.txt", NULL};
static const char * const Test22[] = { "", "-sam", "-s", "-dd", "-stsr", "red_black", "-sksr", "red_black", "-st", "debug", "-me", "soft-dfs", "-g", "freecell", "firsttest.txt", NULL};
static const char * const Test23[] = { "", "-sam", "-s", "-dd", "-stsr", "red_black", "-sksr", "red_black", "-st", "debug", "-me", "random-dfs", "-g", "freecell", "firsttest.txt", NULL};
static const char * const Test24[] = { "", "-sam", "-s", "-dd", "-stsr", "red_black", "-sksr", "red_black", "-st", "debug", "-me", "a-star", "-g", "freecell", "firsttest.txt", NULL};
static const char * const Test25[] = { "", "-sam", "-s", "-dd", "-stsr", "red_black", "-sksr", "red_black", "-st", "debug", "-me", "bfs", "-g", "freecell", "firsttest.txt", NULL};

// run through each solving algorithm type in debug state type, 
// optimization on, red-black tree storage and stack type, on the board in firsttest.txt
static const char * const Test26[] = { "", "-sam", "-s", "-dd", "-stsr", "red_black", "-sksr", "avl", "-st", "debug", "-opt", "-me", "dfs", "-g", "freecell", "firsttest.txt", NULL};
static const char * const Test27[] = { "", "-sam", "-s", "-dd", "-stsr", "red_black", "-sksr", "avl", "-st", "debug", "-opt", "-me", "soft-dfs", "-g", "freecell", "firsttest.txt", NULL};
static const char * const Test28[] = { "", "-sam", "-s", "-dd", "-stsr", "red_black", "-sksr", "avl", "-st", "debug", "-opt", "-me", "random-dfs", "-g", "freecell", "firsttest.txt", NULL};
static const char * const Test29[] = { "", "-sam", "-s", "-dd", "-stsr", "red_black", "-sksr", "avl", "-st", "debug", "-opt", "-me", "a-star", "-g", "freecell", "firsttest.txt", NULL};
static const char * const Test30[] = { "", "-sam", "-s", "-dd", "-stsr", "red_black", "-sksr", "avl", "-st", "debug", "-opt", "-me", "bfs", "-g", "freecell", "firsttest.txt", NULL};

// run through each solving algorithm type in debug state type, 
// no optimization, glib tree storage type, on the board in firsttest.txt
static const char * const Test31[] = { "", "-sam", "-s", "-dd", "-stsr", "glib_tree", "-sksr", "glib_tree", "-st", "debug", "-me", "dfs", "-g", "freecell", "firsttest.txt", NULL};
static const char * const Test32[] = { "", "-sam", "-s", "-dd", "-stsr", "glib_tree", "-sksr", "glib_tree", "-st", "debug", "-me", "soft-dfs", "-g", "freecell", "firsttest.txt", NULL};
static const char * const Test33[] = { "", "-sam", "-s", "-dd", "-stsr", "glib_tree", "-sksr", "glib_tree", "-st", "debug", "-me", "random-dfs", "-g", "freecell", "firsttest.txt", NULL};
static const char * const Test34[] = { "", "-sam", "-s", "-dd", "-stsr", "glib_tree", "-sksr", "glib_tree", "-st", "debug", "-me", "a-star", "-g", "freecell", "firsttest.txt", NULL};
static const char * const Test35[] = { "", "-sam", "-s", "-dd", "-stsr", "glib_tree", "-sksr", "glib_tree", "-st", "debug", "-me", "bfs", "-g", "freecell", "firsttest.txt", NULL};

// run through each solving algorithm type in debug state type, 
// optimization on, glib tree storage and stack type, on the board in firsttest.txt
static const char * const Test36[] = { "", "-sam", "-s", "-dd", "-stsr", "glib_tree", "-sksr", "glib_tree", "-st", "debug", "-opt", "-me", "dfs", "-g", "freecell", "firsttest.txt", NULL};
static const char * const Test37[] = { "", "-sam", "-s", "-dd", "-stsr", "glib_tree", "-sksr", "glib_tree", "-st", "debug", "-opt", "-me", "soft-dfs", "-g", "freecell", "firsttest.txt", NULL};
static const char * const Test38[] = { "", "-sam", "-s", "-dd", "-stsr", "glib_tree", "-sksr", "glib_tree", "-st", "debug", "-opt", "-me", "random-dfs", "-g", "freecell", "firsttest.txt", NULL};
static const char * const Test39[] = { "", "-sam", "-s", "-dd", "-stsr", "glib_tree", "-sksr", "glib_tree", "-st", "debug", "-opt", "-me", "a-star", "-g", "freecell", "firsttest.txt", NULL};
static const char * const Test40[] = { "", "-sam", "-s", "-dd", "-stsr", "glib_tree", "-sksr", "glib_tree", "-st", "debug", "-opt", "-me", "bfs", "-g", "freecell", "firsttest.txt", NULL};

// run through each solving algorithm type in debug state type, 
// no optimization, glib hash storage and stack type, on the board in firsttest.txt
static const char * const Test41[] = { "", "-sam", "-s", "-dd", "-stsr", "glib_hash", "-sksr", "glib_hash", "-st", "debug", "-me", "dfs", "-g", "freecell", "firsttest.txt", NULL};
static const char * const Test42[] = { "", "-sam", "-s", "-dd", "-stsr", "glib_hash", "-sksr", "glib_hash", "-st", "debug", "-me", "soft-dfs", "-g", "freecell", "firsttest.txt", NULL};
static const char * const Test43[] = { "", "-sam", "-s", "-dd", "-stsr", "glib_hash", "-sksr", "glib_hash", "-st", "debug", "-me", "random-dfs", "-g", "freecell", "firsttest.txt", NULL};
static const char * const Test44[] = { "", "-sam", "-s", "-dd", "-stsr", "glib_hash", "-sksr", "glib_hash", "-st", "debug", "-me", "a-star", "-g", "freecell", "firsttest.txt", NULL};
static const char * const Test45[] = { "", "-sam", "-s", "-dd", "-stsr", "glib_hash", "-sksr", "glib_hash", "-st", "debug", "-me", "bfs", "-g", "freecell", "firsttest.txt", NULL};

// run through each solving algorithm type in debug state type, 
// optimization on, glib hash storage and stack type, on the board in firsttest.txt
static const char * const Test46[] = { "", "-sam", "-s", "-dd", "-stsr", "glib_hash", "-sksr", "glib_hash", "-st", "debug", "-opt", "-me", "dfs", "-g", "freecell", "firsttest.txt", NULL};
static const char * const Test47[] = { "", "-sam", "-s", "-dd", "-stsr", "glib_hash", "-sksr", "glib_hash", "-st", "debug", "-opt", "-me", "soft-dfs", "-g", "freecell", "firsttest.txt", NULL};
static const char * const Test48[] = { "", "-sam", "-s", "-dd", "-stsr", "glib_hash", "-sksr", "glib_hash", "-st", "debug", "-opt", "-me", "random-dfs", "-g", "freecell", "firsttest.txt", NULL};
static const char * const Test49[] = { "", "-sam", "-s", "-dd", "-stsr", "glib_hash", "-sksr", "glib_hash", "-st", "debug", "-opt", "-me", "a-star", "-g", "freecell", "firsttest.txt", NULL};
static const char * const Test50[] = { "", "-sam", "-s", "-dd", "-stsr", "glib_hash", "-sksr", "glib_hash", "-st", "debug", "-opt", "-me", "bfs", "-g", "freecell", "firsttest.txt", NULL};

// run through each solving algorithm type in debug state type, 
// no optimization, internal storage and stack type, on the board in firsttest.txt
static const char * const Test51[] = { "", "-sam", "-s", "-dd", "-stsr", "internal", "-sksr", "internal", "-st", "debug", "-me", "dfs", "-g", "freecell", "firsttest.txt", NULL};
static const char * const Test52[] = { "", "-sam", "-s", "-dd", "-stsr", "internal", "-sksr", "internal", "-st", "debug", "-me", "soft-dfs", "-g", "freecell", "firsttest.txt", NULL};
static const char * const Test53[] = { "", "-sam", "-s", "-dd", "-stsr", "internal", "-sksr", "internal", "-st", "debug", "-me", "random-dfs", "-g", "freecell", "firsttest.txt", NULL};
static const char * const Test54[] = { "", "-sam", "-s", "-dd", "-stsr", "internal", "-sksr", "internal", "-st", "debug", "-me", "a-star", "-g", "freecell", "firsttest.txt", NULL};
static const char * const Test55[] = { "", "-sam", "-s", "-dd", "-stsr", "internal", "-sksr", "internal", "-st", "debug", "-me", "bfs", "-g", "freecell", "firsttest.txt", NULL};

// run through each solving algorithm type in debug state type, 
// optimization on, internal storage and stack type, on the board in firsttest.txt
static const char * const Test56[] = { "", "-sam", "-s", "-dd", "-stsr", "internal", "-sksr", "internal", "-st", "debug", "-opt", "-me", "dfs", "-g", "freecell", "firsttest.txt", NULL};
static const char * const Test57[] = { "", "-sam", "-s", "-dd", "-stsr", "internal", "-sksr", "internal", "-st", "debug", "-opt", "-me", "soft-dfs", "-g", "freecell", "firsttest.txt", NULL};
static const char * const Test58[] = { "", "-sam", "-s", "-dd", "-stsr", "internal", "-sksr", "internal", "-st", "debug", "-opt", "-me", "random-dfs", "-g", "freecell", "firsttest.txt", NULL};
static const char * const Test59[] = { "", "-sam", "-s", "-dd", "-stsr", "internal", "-sksr", "internal", "-st", "debug", "-opt", "-me", "a-star", "-g", "freecell", "firsttest.txt", NULL};
static const char * const Test60[] = { "", "-sam", "-s", "-dd", "-stsr", "internal", "-sksr", "internal", "-st", "debug", "-opt", "-me", "bfs", "-g", "freecell", "firsttest.txt", NULL};



// run through each solving algorithm type in compact state type, 
// no optimization, avl-tree storage and stack type, on the board in firsttest.txt
static const char * const Test101[] = { "", "-sam", "-s", "-dd", "-stsr", "avl", "-sksr", "avl", "-st", "compact", "-me", "dfs", "-g", "freecell", "firsttest.txt", ""};
static const char * const Test102[] = { "", "-sam", "-s", "-dd", "-stsr", "avl", "-sksr", "avl", "-st", "compact", "-me", "soft-dfs", "-g", "freecell", "firsttest.txt", ""};
static const char * const Test103[] = { "", "-sam", "-s", "-dd", "-stsr", "avl", "-sksr", "avl", "-st", "compact", "-me", "random-dfs", "-g", "freecell", "firsttest.txt", ""};
static const char * const Test104[] = { "", "-sam", "-s", "-dd", "-stsr", "avl", "-sksr", "avl", "-st", "compact", "-me", "a-star", "-g", "freecell", "firsttest.txt", ""};
static const char * const Test105[] = { "", "-sam", "-s", "-dd", "-stsr", "avl", "-sksr", "avl", "-st", "compact", "-me", "bfs", "-g", "freecell", "firsttest.txt", ""};

// run through each solving algorithm type in compact state type, 
// optimization on, avl-tree storage and stack type, on the board in firsttest.txt
static const char * const Test106[] = { "", "-sam", "-s", "-dd", "-stsr", "avl", "-sksr", "avl", "-st", "compact", "-opt", "-me", "dfs", "-g", "freecell", "firsttest.txt", ""};
static const char * const Test107[] = { "", "-sam", "-s", "-dd", "-stsr", "avl", "-sksr", "avl", "-st", "compact", "-opt", "-me", "soft-dfs", "-g", "freecell", "firsttest.txt", ""};
static const char * const Test108[] = { "", "-sam", "-s", "-dd", "-stsr", "avl", "-sksr", "avl", "-st", "compact", "-opt", "-me", "random-dfs", "-g", "freecell", "firsttest.txt", ""};
static const char * const Test109[] = { "", "-sam", "-s", "-dd", "-stsr", "avl", "-sksr", "avl", "-st", "compact", "-opt", "-me", "a-star", "-g", "freecell", "firsttest.txt", ""};
static const char * const Test110[] = { "", "-sam", "-s", "-dd", "-stsr", "avl", "-sksr", "avl", "-st", "compact", "-opt", "-me", "bfs", "-g", "freecell", "firsttest.txt", ""};

// run through each solving algorithm type in compact state type, 
// no optimization, avl red-black tree storage and stack type, on the board in firsttest.txt
static const char * const Test111[] = { "", "-sam", "-s", "-dd", "-stsr", "avl_rb", "-sksr", "avl_rb", "-st", "compact", "-me", "dfs", "-g", "freecell", "firsttest.txt", NULL};
static const char * const Test112[] = { "", "-sam", "-s", "-dd", "-stsr", "avl_rb", "-sksr", "avl_rb", "-st", "compact", "-me", "soft-dfs", "-g", "freecell", "firsttest.txt", NULL};
static const char * const Test113[] = { "", "-sam", "-s", "-dd", "-stsr", "avl_rb", "-sksr", "avl_rb", "-st", "compact", "-me", "random-dfs", "-g", "freecell", "firsttest.txt", NULL};
static const char * const Test114[] = { "", "-sam", "-s", "-dd", "-stsr", "avl_rb", "-sksr", "avl_rb", "-st", "compact", "-me", "a-star", "-g", "freecell", "firsttest.txt", NULL};
static const char * const Test115[] = { "", "-sam", "-s", "-dd", "-stsr", "avl_rb", "-sksr", "avl_rb", "-st", "compact", "-me", "bfs", "-g", "freecell", "firsttest.txt", NULL};

// run through each solving algorithm type in compact state type, 
// optimization on, avl red-black tree storage and stack type, on the board in firsttest.txt
static const char * const Test116[] = { "", "-sam", "-s", "-dd", "-stsr", "avl_rb", "-sksr", "avl_rb", "-st", "compact", "-opt", "-me", "dfs", "-g", "freecell", "firsttest.txt", NULL};
static const char * const Test117[] = { "", "-sam", "-s", "-dd", "-stsr", "avl_rb", "-sksr", "avl_rb", "-st", "compact", "-opt", "-me", "soft-dfs", "-g", "freecell", "firsttest.txt", NULL};
static const char * const Test118[] = { "", "-sam", "-s", "-dd", "-stsr", "avl_rb", "-sksr", "avl_rb", "-st", "compact", "-opt", "-me", "random-dfs", "-g", "freecell", "firsttest.txt", NULL};
static const char * const Test119[] = { "", "-sam", "-s", "-dd", "-stsr", "avl_rb", "-sksr", "avl_rb", "-st", "compact", "-opt", "-me", "a-star", "-g", "freecell", "firsttest.txt", NULL};
static const char * const Test120[] = { "", "-sam", "-s", "-dd", "-stsr", "avl_rb", "-sksr", "avl_rb", "-st", "compact", "-opt", "-me", "bfs", "-g", "freecell", "firsttest.txt", NULL};

// run through each solving algorithm type in compact state type, 
// no optimization, red-black tree storage and stack type, on the board in firsttest.txt
static const char * const Test121[] = { "", "-sam", "-s", "-dd", "-stsr", "red_black", "-sksr", "red_black", "-st", "compact", "-me", "dfs", "-g", "freecell", "firsttest.txt", NULL};
static const char * const Test122[] = { "", "-sam", "-s", "-dd", "-stsr", "red_black", "-sksr", "red_black", "-st", "compact", "-me", "soft-dfs", "-g", "freecell", "firsttest.txt", NULL};
static const char * const Test123[] = { "", "-sam", "-s", "-dd", "-stsr", "red_black", "-sksr", "red_black", "-st", "compact", "-me", "random-dfs", "-g", "freecell", "firsttest.txt", NULL};
static const char * const Test124[] = { "", "-sam", "-s", "-dd", "-stsr", "red_black", "-sksr", "red_black", "-st", "compact", "-me", "a-star", "-g", "freecell", "firsttest.txt", NULL};
static const char * const Test125[] = { "", "-sam", "-s", "-dd", "-stsr", "red_black", "-sksr", "red_black", "-st", "compact", "-me", "bfs", "-g", "freecell", "firsttest.txt", NULL};

// run through each solving algorithm type in compact state type, 
// optimization on, red-black tree storage and stack type, on the board in firsttest.txt
static const char * const Test126[] = { "", "-sam", "-s", "-dd", "-stsr", "red_black", "-sksr", "avl", "-st", "compact", "-opt", "-me", "dfs", "-g", "freecell", "firsttest.txt", NULL};
static const char * const Test127[] = { "", "-sam", "-s", "-dd", "-stsr", "red_black", "-sksr", "avl", "-st", "compact", "-opt", "-me", "soft-dfs", "-g", "freecell", "firsttest.txt", NULL};
static const char * const Test128[] = { "", "-sam", "-s", "-dd", "-stsr", "red_black", "-sksr", "avl", "-st", "compact", "-opt", "-me", "random-dfs", "-g", "freecell", "firsttest.txt", NULL};
static const char * const Test129[] = { "", "-sam", "-s", "-dd", "-stsr", "red_black", "-sksr", "avl", "-st", "compact", "-opt", "-me", "a-star", "-g", "freecell", "firsttest.txt", NULL};
static const char * const Test130[] = { "", "-sam", "-s", "-dd", "-stsr", "red_black", "-sksr", "avl", "-st", "compact", "-opt", "-me", "bfs", "-g", "freecell", "firsttest.txt", NULL};

// run through each solving algorithm type in compact state type, 
// no optimization, glib tree storage type, on the board in firsttest.txt
static const char * const Test131[] = { "", "-sam", "-s", "-dd", "-stsr", "glib_tree", "-sksr", "glib_tree", "-st", "compact", "-me", "dfs", "-g", "freecell", "firsttest.txt", NULL};
static const char * const Test132[] = { "", "-sam", "-s", "-dd", "-stsr", "glib_tree", "-sksr", "glib_tree", "-st", "compact", "-me", "soft-dfs", "-g", "freecell", "firsttest.txt", NULL};
static const char * const Test133[] = { "", "-sam", "-s", "-dd", "-stsr", "glib_tree", "-sksr", "glib_tree", "-st", "compact", "-me", "random-dfs", "-g", "freecell", "firsttest.txt", NULL};
static const char * const Test134[] = { "", "-sam", "-s", "-dd", "-stsr", "glib_tree", "-sksr", "glib_tree", "-st", "compact", "-me", "a-star", "-g", "freecell", "firsttest.txt", NULL};
static const char * const Test135[] = { "", "-sam", "-s", "-dd", "-stsr", "glib_tree", "-sksr", "glib_tree", "-st", "compact", "-me", "bfs", "-g", "freecell", "firsttest.txt", NULL};

// run through each solving algorithm type in compact state type, 
// optimization on, glib tree storage and stack type, on the board in firsttest.txt
static const char * const Test136[] = { "", "-sam", "-s", "-dd", "-stsr", "glib_tree", "-sksr", "glib_tree", "-st", "compact", "-opt", "-me", "dfs", "-g", "freecell", "firsttest.txt", NULL};
static const char * const Test137[] = { "", "-sam", "-s", "-dd", "-stsr", "glib_tree", "-sksr", "glib_tree", "-st", "compact", "-opt", "-me", "soft-dfs", "-g", "freecell", "firsttest.txt", NULL};
static const char * const Test138[] = { "", "-sam", "-s", "-dd", "-stsr", "glib_tree", "-sksr", "glib_tree", "-st", "compact", "-opt", "-me", "random-dfs", "-g", "freecell", "firsttest.txt", NULL};
static const char * const Test139[] = { "", "-sam", "-s", "-dd", "-stsr", "glib_tree", "-sksr", "glib_tree", "-st", "compact", "-opt", "-me", "a-star", "-g", "freecell", "firsttest.txt", NULL};
static const char * const Test140[] = { "", "-sam", "-s", "-dd", "-stsr", "glib_tree", "-sksr", "glib_tree", "-st", "compact", "-opt", "-me", "bfs", "-g", "freecell", "firsttest.txt", NULL};

// run through each solving algorithm type in compact state type, 
// no optimization, glib hash storage and stack type, on the board in firsttest.txt
static const char * const Test141[] = { "", "-sam", "-s", "-dd", "-stsr", "glib_hash", "-sksr", "glib_hash", "-st", "compact", "-me", "dfs", "-g", "freecell", "firsttest.txt", NULL};
static const char * const Test142[] = { "", "-sam", "-s", "-dd", "-stsr", "glib_hash", "-sksr", "glib_hash", "-st", "compact", "-me", "soft-dfs", "-g", "freecell", "firsttest.txt", NULL};
static const char * const Test143[] = { "", "-sam", "-s", "-dd", "-stsr", "glib_hash", "-sksr", "glib_hash", "-st", "compact", "-me", "random-dfs", "-g", "freecell", "firsttest.txt", NULL};
static const char * const Test144[] = { "", "-sam", "-s", "-dd", "-stsr", "glib_hash", "-sksr", "glib_hash", "-st", "compact", "-me", "a-star", "-g", "freecell", "firsttest.txt", NULL};
static const char * const Test145[] = { "", "-sam", "-s", "-dd", "-stsr", "glib_hash", "-sksr", "glib_hash", "-st", "compact", "-me", "bfs", "-g", "freecell", "firsttest.txt", NULL};

// run through each solving algorithm type in compact state type, 
// optimization on, glib hash storage and stack type, on the board in firsttest.txt
static const char * const Test146[] = { "", "-sam", "-s", "-dd", "-stsr", "glib_hash", "-sksr", "glib_hash", "-st", "compact", "-opt", "-me", "dfs", "-g", "freecell", "firsttest.txt", NULL};
static const char * const Test147[] = { "", "-sam", "-s", "-dd", "-stsr", "glib_hash", "-sksr", "glib_hash", "-st", "compact", "-opt", "-me", "soft-dfs", "-g", "freecell", "firsttest.txt", NULL};
static const char * const Test148[] = { "", "-sam", "-s", "-dd", "-stsr", "glib_hash", "-sksr", "glib_hash", "-st", "compact", "-opt", "-me", "random-dfs", "-g", "freecell", "firsttest.txt", NULL};
static const char * const Test149[] = { "", "-sam", "-s", "-dd", "-stsr", "glib_hash", "-sksr", "glib_hash", "-st", "compact", "-opt", "-me", "a-star", "-g", "freecell", "firsttest.txt", NULL};
static const char * const Test150[] = { "", "-sam", "-s", "-dd", "-stsr", "glib_hash", "-sksr", "glib_hash", "-st", "compact", "-opt", "-me", "bfs", "-g", "freecell", "firsttest.txt", NULL};

// run through each solving algorithm type in compact state type, 
// no optimization, internal storage and stack type, on the board in firsttest.txt
static const char * const Test151[] = { "", "-sam", "-s", "-dd", "-stsr", "internal", "-sksr", "internal", "-st", "compact", "-me", "dfs", "-g", "freecell", "firsttest.txt", NULL};
static const char * const Test152[] = { "", "-sam", "-s", "-dd", "-stsr", "internal", "-sksr", "internal", "-st", "compact", "-me", "soft-dfs", "-g", "freecell", "firsttest.txt", NULL};
static const char * const Test153[] = { "", "-sam", "-s", "-dd", "-stsr", "internal", "-sksr", "internal", "-st", "compact", "-me", "random-dfs", "-g", "freecell", "firsttest.txt", NULL};
static const char * const Test154[] = { "", "-sam", "-s", "-dd", "-stsr", "internal", "-sksr", "internal", "-st", "compact", "-me", "a-star", "-g", "freecell", "firsttest.txt", NULL};
static const char * const Test155[] = { "", "-sam", "-s", "-dd", "-stsr", "internal", "-sksr", "internal", "-st", "compact", "-me", "bfs", "-g", "freecell", "firsttest.txt", NULL};

// run through each solving algorithm type in compact state type, 
// optimization on, internal storage and stack type, on the board in firsttest.txt
static const char * const Test156[] = { "", "-sam", "-s", "-dd", "-stsr", "internal", "-sksr", "internal", "-st", "compact", "-opt", "-me", "dfs", "-g", "freecell", "firsttest.txt", NULL};
static const char * const Test157[] = { "", "-sam", "-s", "-dd", "-stsr", "internal", "-sksr", "internal", "-st", "compact", "-opt", "-me", "soft-dfs", "-g", "freecell", "firsttest.txt", NULL};
static const char * const Test158[] = { "", "-sam", "-s", "-dd", "-stsr", "internal", "-sksr", "internal", "-st", "compact", "-opt", "-me", "random-dfs", "-g", "freecell", "firsttest.txt", NULL};
static const char * const Test159[] = { "", "-sam", "-s", "-dd", "-stsr", "internal", "-sksr", "internal", "-st", "compact", "-opt", "-me", "a-star", "-g", "freecell", "firsttest.txt", NULL};
static const char * const Test160[] = { "", "-sam", "-s", "-dd", "-stsr", "internal", "-sksr", "internal", "-st", "compact", "-opt", "-me", "bfs", "-g", "freecell", "firsttest.txt", NULL};



// run through each solving algorithm type in indirect state type, 
// no optimization, avl-tree storage and stack type, on the board in firsttest.txt
static const char * const Test201[] = { "", "-sam", "-s", "-dd", "-stsr", "avl", "-sksr", "avl", "-st", "indirect", "-me", "dfs", "-g", "freecell", "firsttest.txt", NULL};
static const char * const Test202[] = { "", "-sam", "-s", "-dd", "-stsr", "avl", "-sksr", "avl", "-st", "indirect", "-me", "soft-dfs", "-g", "freecell", "firsttest.txt", NULL};
static const char * const Test203[] = { "", "-sam", "-s", "-dd", "-stsr", "avl", "-sksr", "avl", "-st", "indirect", "-me", "random-dfs", "-g", "freecell", "firsttest.txt", NULL};
static const char * const Test204[] = { "", "-sam", "-s", "-dd", "-stsr", "avl", "-sksr", "avl", "-st", "indirect", "-me", "a-star", "-g", "freecell", "firsttest.txt", NULL};
static const char * const Test205[] = { "", "-sam", "-s", "-dd", "-stsr", "avl", "-sksr", "avl", "-st", "indirect", "-me", "bfs", "-g", "freecell", "firsttest.txt", NULL};

// run through each solving algorithm type in indirect state type, 
// optimization on, avl-tree storage and stack type, on the board in firsttest.txt
static const char * const Test206[] = { "", "-sam", "-s", "-dd", "-stsr", "avl", "-sksr", "avl", "-st", "indirect", "-opt", "-me", "dfs", "-g", "freecell", "firsttest.txt", NULL};
static const char * const Test207[] = { "", "-sam", "-s", "-dd", "-stsr", "avl", "-sksr", "avl", "-st", "indirect", "-opt", "-me", "soft-dfs", "-g", "freecell", "firsttest.txt", NULL};
static const char * const Test208[] = { "", "-sam", "-s", "-dd", "-stsr", "avl", "-sksr", "avl", "-st", "indirect", "-opt", "-me", "random-dfs", "-g", "freecell", "firsttest.txt", NULL};
static const char * const Test209[] = { "", "-sam", "-s", "-dd", "-stsr", "avl", "-sksr", "avl", "-st", "indirect", "-opt", "-me", "a-star", "-g", "freecell", "firsttest.txt", NULL};
static const char * const Test210[] = { "", "-sam", "-s", "-dd", "-stsr", "avl", "-sksr", "avl", "-st", "indirect", "-opt", "-me", "bfs", "-g", "freecell", "firsttest.txt", NULL};

// run through each solving algorithm type in indirect state type, 
// no optimization, avl red-black tree storage and stack type, on the board in firsttest.txt
static const char * const Test211[] = { "", "-sam", "-s", "-dd", "-stsr", "avl_rb", "-sksr", "avl_rb", "-st", "indirect", "-me", "dfs", "-g", "freecell", "firsttest.txt", NULL};
static const char * const Test212[] = { "", "-sam", "-s", "-dd", "-stsr", "avl_rb", "-sksr", "avl_rb", "-st", "indirect", "-me", "soft-dfs", "-g", "freecell", "firsttest.txt", NULL};
static const char * const Test213[] = { "", "-sam", "-s", "-dd", "-stsr", "avl_rb", "-sksr", "avl_rb", "-st", "indirect", "-me", "random-dfs", "-g", "freecell", "firsttest.txt", NULL};
static const char * const Test214[] = { "", "-sam", "-s", "-dd", "-stsr", "avl_rb", "-sksr", "avl_rb", "-st", "indirect", "-me", "a-star", "-g", "freecell", "firsttest.txt", NULL};
static const char * const Test215[] = { "", "-sam", "-s", "-dd", "-stsr", "avl_rb", "-sksr", "avl_rb", "-st", "indirect", "-me", "bfs", "-g", "freecell", "firsttest.txt", NULL};

// run through each solving algorithm type in indirect state type, 
// optimization on, avl red-black tree storage and stack type, on the board in firsttest.txt
static const char * const Test216[] = { "", "-sam", "-s", "-dd", "-stsr", "avl_rb", "-sksr", "avl_rb", "-st", "indirect", "-opt", "-me", "dfs", "-g", "freecell", "firsttest.txt", NULL};
static const char * const Test217[] = { "", "-sam", "-s", "-dd", "-stsr", "avl_rb", "-sksr", "avl_rb", "-st", "indirect", "-opt", "-me", "soft-dfs", "-g", "freecell", "firsttest.txt", NULL};
static const char * const Test218[] = { "", "-sam", "-s", "-dd", "-stsr", "avl_rb", "-sksr", "avl_rb", "-st", "indirect", "-opt", "-me", "random-dfs", "-g", "freecell", "firsttest.txt", NULL};
static const char * const Test219[] = { "", "-sam", "-s", "-dd", "-stsr", "avl_rb", "-sksr", "avl_rb", "-st", "indirect", "-opt", "-me", "a-star", "-g", "freecell", "firsttest.txt", NULL};
static const char * const Test220[] = { "", "-sam", "-s", "-dd", "-stsr", "avl_rb", "-sksr", "avl_rb", "-st", "indirect", "-opt", "-me", "bfs", "-g", "freecell", "firsttest.txt", NULL};

// run through each solving algorithm type in indirect state type, 
// no optimization, red-black tree storage and stack type, on the board in firsttest.txt
static const char * const Test221[] = { "", "-sam", "-s", "-dd", "-stsr", "red_black", "-sksr", "red_black", "-st", "indirect", "-me", "dfs", "-g", "freecell", "firsttest.txt", NULL};
static const char * const Test222[] = { "", "-sam", "-s", "-dd", "-stsr", "red_black", "-sksr", "red_black", "-st", "indirect", "-me", "soft-dfs", "-g", "freecell", "firsttest.txt", NULL};
static const char * const Test223[] = { "", "-sam", "-s", "-dd", "-stsr", "red_black", "-sksr", "red_black", "-st", "indirect", "-me", "random-dfs", "-g", "freecell", "firsttest.txt", NULL};
static const char * const Test224[] = { "", "-sam", "-s", "-dd", "-stsr", "red_black", "-sksr", "red_black", "-st", "indirect", "-me", "a-star", "-g", "freecell", "firsttest.txt", NULL};
static const char * const Test225[] = { "", "-sam", "-s", "-dd", "-stsr", "red_black", "-sksr", "red_black", "-st", "indirect", "-me", "bfs", "-g", "freecell", "firsttest.txt", NULL};

// run through each solving algorithm type in indirect state type, 
// optimization on, red-black tree storage and stack type, on the board in firsttest.txt
static const char * const Test226[] = { "", "-sam", "-s", "-dd", "-stsr", "red_black", "-sksr", "avl", "-st", "indirect", "-opt", "-me", "dfs", "-g", "freecell", "firsttest.txt", NULL};
static const char * const Test227[] = { "", "-sam", "-s", "-dd", "-stsr", "red_black", "-sksr", "avl", "-st", "indirect", "-opt", "-me", "soft-dfs", "-g", "freecell", "firsttest.txt", NULL};
static const char * const Test228[] = { "", "-sam", "-s", "-dd", "-stsr", "red_black", "-sksr", "avl", "-st", "indirect", "-opt", "-me", "random-dfs", "-g", "freecell", "firsttest.txt", NULL};
static const char * const Test229[] = { "", "-sam", "-s", "-dd", "-stsr", "red_black", "-sksr", "avl", "-st", "indirect", "-opt", "-me", "a-star", "-g", "freecell", "firsttest.txt", NULL};
static const char * const Test230[] = { "", "-sam", "-s", "-dd", "-stsr", "red_black", "-sksr", "avl", "-st", "indirect", "-opt", "-me", "bfs", "-g", "freecell", "firsttest.txt", NULL};

// run through each solving algorithm type in indirect state type, 
// no optimization, glib tree storage type, on the board in firsttest.txt
static const char * const Test231[] = { "", "-sam", "-s", "-dd", "-stsr", "glib_tree", "-sksr", "glib_tree", "-st", "indirect", "-me", "dfs", "-g", "freecell", "firsttest.txt", NULL};
static const char * const Test232[] = { "", "-sam", "-s", "-dd", "-stsr", "glib_tree", "-sksr", "glib_tree", "-st", "indirect", "-me", "soft-dfs", "-g", "freecell", "firsttest.txt", NULL};
static const char * const Test233[] = { "", "-sam", "-s", "-dd", "-stsr", "glib_tree", "-sksr", "glib_tree", "-st", "indirect", "-me", "random-dfs", "-g", "freecell", "firsttest.txt", NULL};
static const char * const Test234[] = { "", "-sam", "-s", "-dd", "-stsr", "glib_tree", "-sksr", "glib_tree", "-st", "indirect", "-me", "a-star", "-g", "freecell", "firsttest.txt", NULL};
static const char * const Test235[] = { "", "-sam", "-s", "-dd", "-stsr", "glib_tree", "-sksr", "glib_tree", "-st", "indirect", "-me", "bfs", "-g", "freecell", "firsttest.txt", NULL};

// run through each solving algorithm type in indirect state type, 
// optimization on, glib tree storage and stack type, on the board in firsttest.txt
static const char * const Test236[] = { "", "-sam", "-s", "-dd", "-stsr", "glib_tree", "-sksr", "glib_tree", "-st", "indirect", "-opt", "-me", "dfs", "-g", "freecell", "firsttest.txt", NULL};
static const char * const Test237[] = { "", "-sam", "-s", "-dd", "-stsr", "glib_tree", "-sksr", "glib_tree", "-st", "indirect", "-opt", "-me", "soft-dfs", "-g", "freecell", "firsttest.txt", NULL};
static const char * const Test238[] = { "", "-sam", "-s", "-dd", "-stsr", "glib_tree", "-sksr", "glib_tree", "-st", "indirect", "-opt", "-me", "random-dfs", "-g", "freecell", "firsttest.txt", NULL};
static const char * const Test239[] = { "", "-sam", "-s", "-dd", "-stsr", "glib_tree", "-sksr", "glib_tree", "-st", "indirect", "-opt", "-me", "a-star", "-g", "freecell", "firsttest.txt", NULL};
static const char * const Test240[] = { "", "-sam", "-s", "-dd", "-stsr", "glib_tree", "-sksr", "glib_tree", "-st", "indirect", "-opt", "-me", "bfs", "-g", "freecell", "firsttest.txt", NULL};

// run through each solving algorithm type in indirect state type, 
// no optimization, glib hash storage and stack type, on the board in firsttest.txt
static const char * const Test241[] = { "", "-sam", "-s", "-dd", "-stsr", "glib_hash", "-sksr", "glib_hash", "-st", "indirect", "-me", "dfs", "-g", "freecell", "firsttest.txt", NULL};
static const char * const Test242[] = { "", "-sam", "-s", "-dd", "-stsr", "glib_hash", "-sksr", "glib_hash", "-st", "indirect", "-me", "soft-dfs", "-g", "freecell", "firsttest.txt", NULL};
static const char * const Test243[] = { "", "-sam", "-s", "-dd", "-stsr", "glib_hash", "-sksr", "glib_hash", "-st", "indirect", "-me", "random-dfs", "-g", "freecell", "firsttest.txt", NULL};
static const char * const Test244[] = { "", "-sam", "-s", "-dd", "-stsr", "glib_hash", "-sksr", "glib_hash", "-st", "indirect", "-me", "a-star", "-g", "freecell", "firsttest.txt", NULL};
static const char * const Test245[] = { "", "-sam", "-s", "-dd", "-stsr", "glib_hash", "-sksr", "glib_hash", "-st", "indirect", "-me", "bfs", "-g", "freecell", "firsttest.txt", NULL};

// run through each solving algorithm type in indirect state type, 
// optimization on, glib hash storage and stack type, on the board in firsttest.txt
static const char * const Test246[] = { "", "-sam", "-s", "-dd", "-stsr", "glib_hash", "-sksr", "glib_hash", "-st", "indirect", "-opt", "-me", "dfs", "-g", "freecell", "firsttest.txt", NULL};
static const char * const Test247[] = { "", "-sam", "-s", "-dd", "-stsr", "glib_hash", "-sksr", "glib_hash", "-st", "indirect", "-opt", "-me", "soft-dfs", "-g", "freecell", "firsttest.txt", NULL};
static const char * const Test248[] = { "", "-sam", "-s", "-dd", "-stsr", "glib_hash", "-sksr", "glib_hash", "-st", "indirect", "-opt", "-me", "random-dfs", "-g", "freecell", "firsttest.txt", NULL};
static const char * const Test249[] = { "", "-sam", "-s", "-dd", "-stsr", "glib_hash", "-sksr", "glib_hash", "-st", "indirect", "-opt", "-me", "a-star", "-g", "freecell", "firsttest.txt", NULL};
static const char * const Test250[] = { "", "-sam", "-s", "-dd", "-stsr", "glib_hash", "-sksr", "glib_hash", "-st", "indirect", "-opt", "-me", "bfs", "-g", "freecell", "firsttest.txt", NULL};

// run through each solving algorithm type in indirect state type, 
// no optimization, internal storage and stack type, on the board in firsttest.txt
static const char * const Test251[] = { "", "-sam", "-s", "-dd", "-stsr", "internal", "-sksr", "internal", "-st", "indirect", "-me", "dfs", "-g", "freecell", "firsttest.txt", NULL};
static const char * const Test252[] = { "", "-sam", "-s", "-dd", "-stsr", "internal", "-sksr", "internal", "-st", "indirect", "-me", "soft-dfs", "-g", "freecell", "firsttest.txt", NULL};
static const char * const Test253[] = { "", "-sam", "-s", "-dd", "-stsr", "internal", "-sksr", "internal", "-st", "indirect", "-me", "random-dfs", "-g", "freecell", "firsttest.txt", NULL};
static const char * const Test254[] = { "", "-sam", "-s", "-dd", "-stsr", "internal", "-sksr", "internal", "-st", "indirect", "-me", "a-star", "-g", "freecell", "firsttest.txt", NULL};
static const char * const Test255[] = { "", "-sam", "-s", "-dd", "-stsr", "internal", "-sksr", "internal", "-st", "indirect", "-me", "bfs", "-g", "freecell", "firsttest.txt", NULL};

// run through each solving algorithm type in indirect state type, 
// optimization on, internal storage and stack type, on the board in firsttest.txt
static const char * const Test256[] = { "", "-sam", "-s", "-dd", "-stsr", "internal", "-sksr", "internal", "-st", "indirect", "-opt", "-me", "dfs", "-g", "freecell", "firsttest.txt", NULL};
static const char * const Test257[] = { "", "-sam", "-s", "-dd", "-stsr", "internal", "-sksr", "internal", "-st", "indirect", "-opt", "-me", "soft-dfs", "-g", "freecell", "firsttest.txt", NULL};
static const char * const Test258[] = { "", "-sam", "-s", "-dd", "-stsr", "internal", "-sksr", "internal", "-st", "indirect", "-opt", "-me", "random-dfs", "-g", "freecell", "firsttest.txt", NULL};
static const char * const Test259[] = { "", "-sam", "-s", "-dd", "-stsr", "internal", "-sksr", "internal", "-st", "indirect", "-opt", "-me", "a-star", "-g", "freecell", "firsttest.txt", NULL};
static const char * const Test260[] = { "", "-sam", "-s", "-dd", "-stsr", "internal", "-sksr", "internal", "-st", "indirect", "-opt", "-me", "bfs", "-g", "freecell", "firsttest.txt", NULL};

// run through each solving algorithm type in indirect state type, 
// no optimization, internal storage and stack type, on the board in firsttest.txt
static const char * const Test261[] = { "", "-sam", "-s", "-dd", "-stsr", "indirect", "-sksr", "internal", "-st", "indirect", "-me", "dfs", "-g", "freecell", "firsttest.txt", NULL};
static const char * const Test262[] = { "", "-sam", "-s", "-dd", "-stsr", "indirect", "-sksr", "internal", "-st", "indirect", "-me", "soft-dfs", "-g", "freecell", "firsttest.txt", NULL};
static const char * const Test263[] = { "", "-sam", "-s", "-dd", "-stsr", "indirect", "-sksr", "internal", "-st", "indirect", "-me", "random-dfs", "-g", "freecell", "firsttest.txt", NULL};
static const char * const Test264[] = { "", "-sam", "-s", "-dd", "-stsr", "indirect", "-sksr", "internal", "-st", "indirect", "-me", "a-star", "-g", "freecell", "firsttest.txt", NULL};
static const char * const Test265[] = { "", "-sam", "-s", "-dd", "-stsr", "indirect", "-sksr", "internal", "-st", "indirect", "-me", "bfs", "-g", "freecell", "firsttest.txt", NULL};

// run through each solving algorithm type in indirect state type, 
// optimization on, internal storage and stack type, on the board in firsttest.txt
static const char * const Test266[] = { "", "-sam", "-s", "-dd", "-stsr", "indirect", "-sksr", "internal", "-st", "indirect", "-opt", "-me", "dfs", "-g", "freecell", "firsttest.txt", NULL};
static const char * const Test267[] = { "", "-sam", "-s", "-dd", "-stsr", "indirect", "-sksr", "internal", "-st", "indirect", "-opt", "-me", "soft-dfs", "-g", "freecell", "firsttest.txt", NULL};
static const char * const Test268[] = { "", "-sam", "-s", "-dd", "-stsr", "indirect", "-sksr", "internal", "-st", "indirect", "-opt", "-me", "random-dfs", "-g", "freecell", "firsttest.txt", NULL};
static const char * const Test269[] = { "", "-sam", "-s", "-dd", "-stsr", "indirect", "-sksr", "internal", "-st", "indirect", "-opt", "-me", "a-star", "-g", "freecell", "firsttest.txt", NULL};
static const char * const Test270[] = { "", "-sam", "-s", "-dd", "-stsr", "indirect", "-sksr", "internal", "-st", "indirect", "-opt", "-me", "bfs", "-g", "freecell", "firsttest.txt", NULL};



// All the tests run
// Right now, I'm commenting out BFS Tests becuase they take too long with Freecell
static const char** const AllTests[] = 
{
	//this is here so test number and array offset match
	NULL
	,(const char **const)Test1
	,(const char **const)Test2 
	,(const char **const)Test3
	,(const char **const)Test4 
//	,(const char **const)Test5 
	,(const char **const)Test6
	,(const char **const)Test7 
	,(const char **const)Test8 
	,(const char **const)Test9
//	,(const char **const)Test10
	,(const char **const)Test11
	,(const char **const)Test12 
	,(const char **const)Test13
	,(const char **const)Test14 
//	,(const char **const)Test15 
	,(const char **const)Test16
	,(const char **const)Test17 
	,(const char **const)Test18 
	,(const char **const)Test19
//	,(const char **const)Test20
	,(const char **const)Test21
	,(const char **const)Test22 
	,(const char **const)Test23
	,(const char **const)Test24 
//	,(const char **const)Test25 
	,(const char **const)Test26
	,(const char **const)Test27 
	,(const char **const)Test28 
	,(const char **const)Test29
//	,(const char **const)Test30
	,(const char **const)Test31
	,(const char **const)Test32 
	,(const char **const)Test33
	,(const char **const)Test34 
//	,(const char **const)Test35 
	,(const char **const)Test36
	,(const char **const)Test37 
	,(const char **const)Test38 
	,(const char **const)Test39
//	,(const char **const)Test40
	,(const char **const)Test41
	,(const char **const)Test42 
	,(const char **const)Test43
	,(const char **const)Test44 
//	,(const char **const)Test45 
	,(const char **const)Test46
	,(const char **const)Test47 
	,(const char **const)Test48 
	,(const char **const)Test49
//	,(const char **const)Test50
	,(const char **const)Test51
	,(const char **const)Test52 
	,(const char **const)Test53
	,(const char **const)Test54 
//	,(const char **const)Test55 
	,(const char **const)Test56
	,(const char **const)Test57 
	,(const char **const)Test58 
	,(const char **const)Test59
//	,(const char **const)Test60


	,(const char **const)Test101
	,(const char **const)Test102 
	,(const char **const)Test103
	,(const char **const)Test104 
//	,(const char **const)Test105 
	,(const char **const)Test106
	,(const char **const)Test107 
	,(const char **const)Test108 
	,(const char **const)Test109
//	,(const char **const)Test110
	,(const char **const)Test111
	,(const char **const)Test112 
	,(const char **const)Test113
	,(const char **const)Test114 
//	,(const char **const)Test115 
	,(const char **const)Test116
	,(const char **const)Test117 
	,(const char **const)Test118 
	,(const char **const)Test119
//	,(const char **const)Test120
	,(const char **const)Test121
	,(const char **const)Test122 
	,(const char **const)Test123
	,(const char **const)Test124 
//	,(const char **const)Test125 
	,(const char **const)Test126
	,(const char **const)Test127 
	,(const char **const)Test128 
	,(const char **const)Test129
//	,(const char **const)Test130
	,(const char **const)Test131
	,(const char **const)Test132 
	,(const char **const)Test133
	,(const char **const)Test134 
//	,(const char **const)Test135 
	,(const char **const)Test136
	,(const char **const)Test137 
	,(const char **const)Test138 
	,(const char **const)Test139
//	,(const char **const)Test140
	,(const char **const)Test141
	,(const char **const)Test142 
	,(const char **const)Test143
	,(const char **const)Test144 
//	,(const char **const)Test145 
	,(const char **const)Test146
	,(const char **const)Test147 
	,(const char **const)Test148 
	,(const char **const)Test149
//	,(const char **const)Test150
	,(const char **const)Test151
	,(const char **const)Test152 
	,(const char **const)Test153
	,(const char **const)Test154 
//	,(const char **const)Test155 
	,(const char **const)Test156
	,(const char **const)Test157 
	,(const char **const)Test158 
	,(const char **const)Test159
//	,(const char **const)Test160


	,(const char **const)Test201
	,(const char **const)Test202 
	,(const char **const)Test203
	,(const char **const)Test204 
//	,(const char **const)Test205 
	,(const char **const)Test206
	,(const char **const)Test207 
	,(const char **const)Test208 
	,(const char **const)Test209
//	,(const char **const)Test210
	,(const char **const)Test211
	,(const char **const)Test212 
	,(const char **const)Test213
	,(const char **const)Test214 
//	,(const char **const)Test215 
	,(const char **const)Test216
	,(const char **const)Test217 
	,(const char **const)Test218 
	,(const char **const)Test219
//	,(const char **const)Test220
	,(const char **const)Test221
	,(const char **const)Test222 
	,(const char **const)Test223
	,(const char **const)Test224 
//	,(const char **const)Test225 
	,(const char **const)Test226
	,(const char **const)Test227 
	,(const char **const)Test228 
	,(const char **const)Test229
//	,(const char **const)Test230
	,(const char **const)Test231
	,(const char **const)Test232 
	,(const char **const)Test233
	,(const char **const)Test234 
//	,(const char **const)Test235 
	,(const char **const)Test236
	,(const char **const)Test237 
	,(const char **const)Test238 
	,(const char **const)Test239
//	,(const char **const)Test240
	,(const char **const)Test241
	,(const char **const)Test242 
	,(const char **const)Test243
	,(const char **const)Test244 
//	,(const char **const)Test245 
	,(const char **const)Test246
	,(const char **const)Test247 
	,(const char **const)Test248 
	,(const char **const)Test249
//	,(const char **const)Test250
	,(const char **const)Test251
	,(const char **const)Test252 
	,(const char **const)Test253
	,(const char **const)Test254 
//	,(const char **const)Test255 
	,(const char **const)Test256
	,(const char **const)Test257 
	,(const char **const)Test258 
	,(const char **const)Test259
//	,(const char **const)Test260
};

void TestAllSolvingAlgorithms()
{
	FCSFreecellSolvingAlgorithm* SolvingAlgorithm = NULL;
	FCCommandLineArguments *CommandLineArguments;
	FCSStateWithLocations *InitialState,
						  *DuplicateInitialState;
	FCSStateSolvingReturnCodes SolveValue;
	int Count;

	for (int i = 1;i<(sizeof(AllTests)/sizeof(char*));i++)
	{
		Count = 0;
		while (AllTests[i][Count] != NULL)
			Count++;
	
		CommandLineArguments = new FCCommandLineArguments();

		if (!CommandLineArguments->Parse(Count, (char**)AllTests[i]))
		{
			cout << "Couldn't parse test parameters!" << endl;
			return;
		}

		if (!CommandLineArguments->Verify())
		{
			cout << "Couldn't verify test parameters!" << endl;
			return;
		}

		SolvingAlgorithm = FCSFreecellSolvingAlgorithm::Create(CommandLineArguments);

		InitialState = CommandLineArguments->GetInitialState();

		DuplicateInitialState = CreateStateWithLocations();
		DuplicateInitialState->Copy(InitialState);

		InitialState->CanonizeState(CommandLineArguments->GetNumberOfFreecells(), 
									CommandLineArguments->GetNumberOfStacks());

		if (InitialState != NULL)
		{
			SolveValue = SolvingAlgorithm->Solve(InitialState, 0);
		}

		while (SolveValue == FCS_STATE_SUSPEND_PROCESS)
		{
			SolvingAlgorithm->IncreaseMaxNumberOfCheckedStates();
			SolveValue = SolvingAlgorithm->Resume(0);
		}
		if (SolveValue == FCS_STATE_WAS_SOLVED)
		{
			cout << "This game is solveable." << endl;
			if (CommandLineArguments->GetOptimizeSolution())
			{
				cout << "Now trying to optimize solution." << endl;

				SolvingAlgorithm->CleanData();
				FCSOptimizeSolvingAlgorithm<FCSFreecellSolvingAlgorithm>*  OptimizeSolution;
				OptimizeSolution = new FCSOptimizeSolvingAlgorithm<FCSFreecellSolvingAlgorithm>(SolvingAlgorithm);
				OptimizeSolution->Solve(InitialState, 0);
				OptimizeSolution->TraceSolution();
				OptimizeSolution->ShowSolution(InitialState, DuplicateInitialState);
				delete OptimizeSolution;
			}
			else
			{
				SolvingAlgorithm->TraceSolution();
				SolvingAlgorithm->ShowSolution(InitialState, DuplicateInitialState);
			}
		}a
		else
		{
			cout << "I could not solve this game." << endl;
		}

		cout << "Total number of states checked is " << SolvingAlgorithm->GetNumberOfCheckedStates() << "." << endl;
		cout << "This scan generated " << SolvingAlgorithm->GetNumberOfStatesInCollection() <<  " states." << endl << endl;
		cout << endl << "TEST DONE!" << endl << endl;

		delete InitialState;
		DuplicateInitialState->CleanState();
		delete DuplicateInitialState;
		delete SolvingAlgorithm;
		delete CommandLineArguments;
	}
}
*/
static const char * const AllGames[] = { "freecell" , /*"simple_simon","klondike" */};
/*
"bakers_dozen", "bakers_game", "beleaguered_castle", "casino_klondike", "citadel", "cruel",
"der_katzenschwanz", "der_katz", "die_schlange", "eight_off", "fan", "forecell", "freecell",
"gnome_freecell", "good_measure", "gypsy", "kings_only_bakers_game", "klondike", "klondike_by_threes",
"ko_bakers_game", "microsoft_freecell", "pysol_bakers_game", "pysol_beleaguered_castle", 
"pysol_casino_klondike", "pysol_eight_off", "pysol_freecell", "pysol_kings_only_bakers_game", 
"pysol_klondike", "pysol_klondike_by_threes", "pysol_ko_bakers_game", "pysol_relaxed_freecell",
"pysol_relaxed_seahaven", "pysol_relaxed_seahaven_towers", "pysol_seahaven", "pysol_seahaven_towers",
"pysol_streets_and_alleys", "pysol_thumb_and_pouch", "pysol_vegas_klondike", "relaxed_freecell",
"relaxed_seahaven_towers", "relaxed_seahaven", "seahaven_towers", "simple_simon", "small_harp",
"streets_and_alleys", "thumb_and_pouch", "vegas_klondike", "yukon", "whitehead",
*/
//static const char * const GameParameters[] = { "", "-stsr", "indirect", "-sksr", "internal", "-st", "indirect", "-opt", "-me", "dfs", "-g", "gamestring123456789012345678901234567890", "GameBoardTest.txt"};

///Maximum number of command line parameters
#define NUMBER_OF_COMMAND_LINE_PARAMETERS		16
///Largest string size a command line parameter can have
#define MAX_LENGTH_OF_COMMAND_LINE_PARAMETER	40
///what board number to start testing at
#define TEST_GAME_START							1
///what board number to end testing at
#define TEST_GAME_END							1001

void TestEachGame()
{
	AFreecellGameBoard* GameBoard;
	char GameBoardString[1024];
	char** GameParameters;
	FCSFreecellSolvingAlgorithm* SolvingAlgorithm = NULL;
	FCSSimpleSimonSolvingAlgorithm* SimpleSimonSolvingAlgorithm = NULL;
	FCSTalonSolvingAlgorithm* TalonSolvingAlgorithm = NULL;
	FCCommandLineArguments* CommandLineArguments;
	FCSStateWithLocations *InitialState,
						  *DuplicateInitialState;
	FCSStateSolvingReturnCodes SolveValue;
	FCSPresetID	GamePreset;
	std::ofstream Write;
	int i;
	int NumberOfUnsolvableGames = 0;

	//create "command-line"
	GameParameters = new char*[NUMBER_OF_COMMAND_LINE_PARAMETERS];
	for (i = 0; i < NUMBER_OF_COMMAND_LINE_PARAMETERS; i++)
		GameParameters[i] = new char[MAX_LENGTH_OF_COMMAND_LINE_PARAMETER];
	strcpy(GameParameters[0], "");
	strcpy(GameParameters[1], "-stsr");
	strcpy(GameParameters[2], "indirect");
	strcpy(GameParameters[3], "-sksr");
	strcpy(GameParameters[4], "internal");
	strcpy(GameParameters[5], "-st");
	strcpy(GameParameters[6], "indirect");
	strcpy(GameParameters[7], "-opt");
	strcpy(GameParameters[8], "-me");
	strcpy(GameParameters[9], "a-star");
	strcpy(GameParameters[10], "-g");
	strcpy(GameParameters[11], "");
	strcpy(GameParameters[12], "-mi");
	strcpy(GameParameters[13], "10000");
	
	strcpy(GameParameters[14], "GameBoardTest.txt");

	//Number of games to play
	for (int Seed = TEST_GAME_START; Seed < TEST_GAME_END; Seed++)
	{
		//run through all the games
		for (int Game = 0; Game < (sizeof(AllGames)/sizeof(char*)); Game++)
		{
			if ( Seed == 4 )
			{
				int i = 1;
			}
			
			cout << endl << "Game: " << AllGames[Game] << "\tSeed: " << Seed << endl << endl;

			//Create the game board
			GameBoard = CreateAFreecellGameBoard((char*)AllGames[Game], false);
			GameBoardString[0] = NULL;
			GameBoard->Shuffle(Seed);
			GameBoard->Deal(GameBoardString);
			Write.open("GameBoardTest.txt");
			Write << GameBoardString << endl;
			Write.close();
	
			//modify "command-line" to account for the game
			strcpy((char*)GameParameters[11], (char*)AllGames[Game]);			

			//modify commandline to reset game parameters
			strcpy(GameParameters[12], "-mi");
			strcpy(GameParameters[13], "10000");
			
			strcpy(GameParameters[14], "GameBoardTest.txt");

			CommandLineArguments = new FCCommandLineArguments();

			if (!CommandLineArguments->Parse(15, GameParameters))
			{
				cerr << "Couldn't solve " << AllGames[Game] << " board# " << Seed << endl
					 << "Command line didn't parse." << endl;
				break;
			}
			if (!CommandLineArguments->Verify())
			{
				cerr << "Couldn't solve " << AllGames[Game] << " board# " << Seed << endl
					 << "Command line didn't verify." << endl;
				break;
			}

			//first try to solve the game using a-star
			GamePreset = FCSPresetName::GetPresetID(CommandLineArguments->GetGameName());
			MainCreate(GamePreset, CommandLineArguments, 
						&SolvingAlgorithm, &SimpleSimonSolvingAlgorithm, &TalonSolvingAlgorithm);
			InitialState = CommandLineArguments->GetInitialState();
			DuplicateInitialState = CreateStateWithLocations();
			DuplicateInitialState->Copy(InitialState);
			InitialState->CanonizeState(CommandLineArguments->GetNumberOfFreecells(), 
										CommandLineArguments->GetNumberOfStacks());

			if (InitialState != NULL)
			{
				SolveValue = MainSolve(GamePreset, InitialState, 
										SolvingAlgorithm, SimpleSimonSolvingAlgorithm, TalonSolvingAlgorithm);
			}

			if (SolveValue == FCS_STATE_IS_NOT_SOLVEABLE)
			{
				cout << "This game is not solveable." << endl;
				NumberOfUnsolvableGames++;
			}
			else if (SolveValue != FCS_STATE_WAS_SOLVED)
			{
				//now try to solve the game using dfs
				DuplicateInitialState->CleanState();
				delete DuplicateInitialState;
	
				delete InitialState;
				delete SolvingAlgorithm;
				delete SimpleSimonSolvingAlgorithm;
				delete TalonSolvingAlgorithm;
				delete CommandLineArguments;

				//change solving method to dfs
				strcpy(GameParameters[9], "dfs");

				//reparse commandline
				CommandLineArguments = new FCCommandLineArguments();

				if (!CommandLineArguments->Parse(15, GameParameters))
				{
					cerr << "Couldn't solve " << AllGames[Game] << " board# " << Seed << endl
						 << "Command line didn't parse." << endl;
					break;
				}
				if (!CommandLineArguments->Verify())
				{
					cerr << "Couldn't solve " << AllGames[Game] << " board# " << Seed << endl
						 << "Command line didn't verify." << endl;
					break;
				}

				MainCreate(GamePreset, CommandLineArguments, 
							&SolvingAlgorithm, &SimpleSimonSolvingAlgorithm, &TalonSolvingAlgorithm);
				InitialState = CommandLineArguments->GetInitialState();
				DuplicateInitialState = CreateStateWithLocations();
				DuplicateInitialState->Copy(InitialState);
				InitialState->CanonizeState(CommandLineArguments->GetNumberOfFreecells(), 
											CommandLineArguments->GetNumberOfStacks());

				if (InitialState != NULL)
				{
					SolveValue = MainSolve(GamePreset, InitialState, 
								SolvingAlgorithm, SimpleSimonSolvingAlgorithm, TalonSolvingAlgorithm);
				}

				if (SolveValue == FCS_STATE_IS_NOT_SOLVEABLE)
				{
					cout << "This game is not solveable." << endl;
					NumberOfUnsolvableGames++;
				}
				else if (SolveValue != FCS_STATE_WAS_SOLVED)
				{
					//try a-star again, but don't put a higher limit on the iterations
					DuplicateInitialState->CleanState();
					delete DuplicateInitialState;
		
					delete InitialState;
					delete SolvingAlgorithm;
					delete SimpleSimonSolvingAlgorithm;
					delete TalonSolvingAlgorithm;
					delete CommandLineArguments;

					//change solving method to back to a-star
					strcpy(GameParameters[9], "a-star");
					//up the limit of iterations
					strcpy(GameParameters[13], "500000");

					//reparse commandline
					CommandLineArguments = new FCCommandLineArguments();

					if (!CommandLineArguments->Parse(15, GameParameters))
					{
						cerr << "Couldn't solve " << AllGames[Game] << " board# " << Seed << endl
							 << "Command line didn't parse." << endl;
						break;
					}
					if (!CommandLineArguments->Verify())
					{
						cerr << "Couldn't solve " << AllGames[Game] << " board# " << Seed << endl
							 << "Command line didn't verify." << endl;
						break;
					}

					MainCreate(GamePreset, CommandLineArguments, 
						&SolvingAlgorithm, &SimpleSimonSolvingAlgorithm, &TalonSolvingAlgorithm);
					InitialState = CommandLineArguments->GetInitialState();
					DuplicateInitialState = CreateStateWithLocations();
					DuplicateInitialState->Copy(InitialState);
					InitialState->CanonizeState(CommandLineArguments->GetNumberOfFreecells(), 
												CommandLineArguments->GetNumberOfStacks());

					if (InitialState != NULL)
					{
						SolveValue = MainSolve(GamePreset, InitialState, 
							SolvingAlgorithm, SimpleSimonSolvingAlgorithm, TalonSolvingAlgorithm);
					}

					if (SolveValue != FCS_STATE_WAS_SOLVED)
					{
						cout << "This game is not solveable." << endl;
						NumberOfUnsolvableGames++;
					}
					else
					{
						cout << "This game is solveable." << endl;
						if (CommandLineArguments->GetOptimizeSolution())
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
				}
				else
				{
					cout << "This game is solveable." << endl;
					if (CommandLineArguments->GetOptimizeSolution())
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
			}
			else
			{
				cout << "This game is solveable." << endl;
				if (CommandLineArguments->GetOptimizeSolution())
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

			MainStats(GamePreset, SolvingAlgorithm, SimpleSimonSolvingAlgorithm, TalonSolvingAlgorithm);
	
			DuplicateInitialState->CleanState();
			delete DuplicateInitialState;
	
			delete InitialState;
			delete SolvingAlgorithm;
			delete SimpleSimonSolvingAlgorithm;
			delete TalonSolvingAlgorithm;
			delete CommandLineArguments;

			delete GameBoard;
		}
	}

	cout << endl << "Number of unsolvable games: " << NumberOfUnsolvableGames << endl;

	//delete "command-line"
	for (i = 0; i < NUMBER_OF_COMMAND_LINE_PARAMETERS; i++)
		delete [] GameParameters[i];
	delete [] GameParameters;

}