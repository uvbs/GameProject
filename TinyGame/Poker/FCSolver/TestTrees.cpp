////////////////////////////////////////////////
///\file TestTrees.cpp
///\brief This file contains the implementation of functions to help test the tree classes
///\author Michael Mann
///\version 1.0
///\date August 2002
////////////////////////////////////////////////

#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <stdio.h>

#include <afx.h>

#include "TestTrees.h"
#include "RedBlackTree.h"
#include "AVLRedBlackTree.h"
#include "TestAVLRedBlackTree.h"
#include "AVLTree.h"
#include "GLIBTree.h"


using std::endl;
using std::cout;

void TestRedBlackTree()
{
	int *ptr;
	int *val, *ret;
	bool IsInsert;

	RedBlackTree<int, int>* RBTree;
	SimpleCompareNodesAlgorithm<int, int> CompareAlgorithm;

/**********************
* Example 
***********************/
	RBTree = new RedBlackTree<int, int>(&CompareAlgorithm);

	//initialize randomizer
	srand( (unsigned)time( NULL ) );

	for (int i = 0; i < 12; i++)
	{
		ptr = new int;
		*ptr = i;//rand()&0xff;
		val = RBTree->Search(ptr, &IsInsert);
		if(val != NULL)
		{
			cout << "ERROR" << endl;
		}
	}

	//display all values
	for(val=RBTree->Lookup(RBFIRST, NULL); val!=NULL; val=RBTree->Lookup(RBNEXT, val))
		cout << *val << endl;

	//delete tree
	for(val=RBTree->Lookup(RBFIRST, NULL); val!=NULL; val=RBTree->Lookup(RBFIRST, val))
	{
		ret = RBTree->Delete(val);
		delete (int*) ret;
	}
	
	delete RBTree;

/**********************
* Example 1
***********************/
	
	RBTree = new RedBlackTree<int, int>(&CompareAlgorithm);

	for (int j = 200; j > 0; j--)
	{
		ptr = new int;
		*ptr = j;
		val = RBTree->Search(ptr, &IsInsert);
		if(val != NULL)
		{
			cout << "ERROR" << endl;
		}
	}

	//display tree
	for(val=RBTree->Lookup(RBFIRST, NULL); val!=NULL; val=RBTree->Lookup(RBNEXT, val))
		cout << *val << endl;

	//delete tree
	for(val=RBTree->Lookup(RBFIRST, NULL); val!=NULL; val=RBTree->Lookup(RBFIRST, val))
	{
		ret = RBTree->Delete(val);
		delete (int*) ret;
	}
	
	delete RBTree;

/**********************
* Example 2
***********************/

	RBTree = new RedBlackTree<int, int>(&CompareAlgorithm);

	for (int k = 400; k > 0; k--)
	{
		ptr = new int;
		*ptr = k;
		val = RBTree->Search(ptr, &IsInsert);
		if(val != NULL)
		{
			cout << "ERROR" << endl;
		}
	}

/*
	printf("Minimum branch length: %d\n", minleaf);
	printf("Maximum branch length: %d\n", maxleaf);
*/	
	for (int k1 = 400; k1 > 0; k1--)
	{
		val = RBTree->Delete(&k1);
		if(val == NULL)
		{
			cout << "ERROR" << endl;
		}

		delete (int*) val;
	}

	delete RBTree;

/**********************
* Test Find
***********************/
	
	RBTree = new RedBlackTree<int, int>(&CompareAlgorithm);

	for (int m = 200; m > 0; m--)
	{
		ptr = new int;
		*ptr = m;
		val = RBTree->Find(ptr);
		if(val != NULL)
		{
			cout << "ERROR" << endl;
		}

		//ptr shouldn't be added to the tree, so just delete it
		delete ptr;
	}

	//display tree (shouldn't be one)
	for(val=RBTree->Lookup(RBFIRST, NULL); val!=NULL; val=RBTree->Lookup(RBFIRST, val))
	{
		cout << *val << endl;
		ret = RBTree->Delete(val);
		delete (int*) ret;
	}
	
	delete RBTree;
}

///Number of nodes on the tree
#define TREE_SIZE 16
///Number of iterations to test
#define N_ITERATIONS 1024

void TestAVLRedBlackTree(int argc, char **argv)
{
	int array[TREE_SIZE];
	int seed;
	bool IsDone = false;
	bool IsInsert;

	if (argc == 2)
		seed = atoi(argv[1]);
	else
		seed = time(0) *257%32768;

	SimpleCompareNodesAlgorithm<int, int> CompareAlgorithm;

	for (int iteration = 1; iteration <= N_ITERATIONS; iteration++)
	{
		AVLRedBlackTree<int, int> *tree;
		int i;

		printf("Iteration %4d/%4d: seed=%5d", iteration, N_ITERATIONS, seed);
		fflush(stdout);

		srand(seed++);

		for (i = 0; i < TREE_SIZE; i++)
			array[i] = i;
		Shuffle(array, TREE_SIZE);

		tree = new AVLRedBlackTree<int, int>(&CompareAlgorithm);
		for (i = 0; i< TREE_SIZE; i++)
			tree->Search(&(array[i]), &IsInsert);
		
//		if (tree->Verify(&IsDone))
//			return;

		int copy_array[TREE_SIZE];
		for (i = 0; i<TREE_SIZE; i++)
			copy_array[i] = array[i];

		Shuffle(copy_array, TREE_SIZE);
		for(i = 0; i < TREE_SIZE; i++)
		{
			tree->Delete(&copy_array[i]);
//			if (tree->Verify(&IsDone))
//				return;
		
			AVLRedBlackTree<int, int> *copy;
			copy = tree->Copy();
//			if (tree->Verify(&IsDone))
//				return;
			
			bool same = tree->Compare(copy);
			delete copy;

			if (!same)
				return;
		}

		if (i%128 == 0)
		{
			printf(".");
		}
		
		printf("\ttest good.\n");
		fflush(stdout);

		delete tree;
	}
}

void TestAVLTree(int argc, char **argv)
{
	int array[TREE_SIZE];
	int seed;
	bool IsInsert;

	if (argc == 2)
		seed = atoi(argv[1]);
	else
		seed = time(0) *257%32768;

	SimpleCompareNodesAlgorithm<int, int> CompareAlgorithm;

	for (int iteration = 1; iteration <= N_ITERATIONS; iteration++)
	{
		AVLTree<int, int> *tree;
		int i;

		printf("Iteration %4d/%4d: seed=%5d", iteration, N_ITERATIONS, seed);
		fflush(stdout);

		srand(seed++);

		for (i = 0; i < TREE_SIZE; i++)
			array[i] =  i;

		Shuffle(array, TREE_SIZE);

		tree = new AVLTree<int, int>(&CompareAlgorithm);
		for (i = 0; i< TREE_SIZE; i++)
			tree->Search(&(array[i]), &IsInsert);
		
//		if (tree->Verify(&IsDone))
//			return;

		for(i = 0; i < TREE_SIZE; i++)
		{
			tree->Delete(&(array[i]));

//			if (tree->Verify(&IsDone))
//				return;
		}

		printf("\ttest good.\n");
		fflush(stdout);

		delete tree;
	}
}

void TestGLIBTree(int argc, char **argv)
{
	int array[TREE_SIZE];
	int seed;
	bool IsInsert;

	if (argc == 2)
		seed = atoi(argv[1]);
	else
		seed = time(0) *257%32768;

	SimpleCompareNodesAlgorithm<int, int> CompareAlgorithm;

	for (int iteration = 1; iteration <= N_ITERATIONS; iteration++)
	{
		GLIBTree<int, int> *tree;
		int i;

		printf("Iteration %4d/%4d: seed=%5d", iteration, N_ITERATIONS, seed);
		fflush(stdout);

		srand(seed++);

		for (i = 0; i < TREE_SIZE; i++)
			array[i] =  i;

		Shuffle(array, TREE_SIZE);

		tree = new GLIBTree<int, int>(&CompareAlgorithm);
		for (i = 0; i< TREE_SIZE; i++)
			tree->Search(&(array[i]), &IsInsert);
		
//		if (tree->Verify(&IsDone))
//			return;

		for(i = 0; i < TREE_SIZE; i++)
		{
			tree->Delete(&(array[i]));

//			if (tree->Verify(&IsDone))
//				return;
		}

		printf("\ttest good.\n");
		fflush(stdout);

		delete tree;
	}
}