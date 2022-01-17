#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#define ElementType int
#define Max 30
#define MAX(a,b) (a>b)?a:b


// data structure used to organize tree
struct treeNode {
	ElementType val; // store node value
	bool isRed;		 // mark node color
	bool isValid;	 // mark node validity
	struct treeNode* leftT;  // link left child
	struct treeNode* rightT; // link right child
}; 
int results[Max]; // store results


struct treeNode* BuildT( ElementType data, struct treeNode *tree );
struct treeNode* NewNode( ElementType data, struct treeNode* node );

int SearchT( struct treeNode* tree );
int RBTJudge( struct treeNode *tree );
int NodeColor( struct treeNode* node );
int BlackNode( struct treeNode* tree );
int BlackNodeNum( struct treeNode* node );



/*---- Part 1: Main Function ----*/
/* Get input and Output results  */
int main() {

	// init and read input
	int cases; // store number of cases
	scanf("%d", &cases);
	int i;

	// read in cases and judge whether it's a Red-Black Tree
	for ( i = 0; i < cases; i++ ) {
		int num; // store number of nodes
		scanf("%d", &num); // read in number of nodes

		struct treeNode* tree = NULL;
		int j;
		for ( j = 0; j < num; j++ ) {	// read in nodes' value
			ElementType node; // store nodes' value
			scanf("%d", &node);
			// create balanced binary search tree by preorder sequence
			tree = BuildT( node, tree);
		}

		// judge
		results[i] = RBTJudge(tree);

	}
	// output results
	for ( i = 0; i < cases; i++ ) {
		if ( results[i] ) 
			printf ("Yes\n");
		else
			printf("No\n");
	}

	return 0;
}



/*---- Part 2: Build Tree ----*/
/*	Create a Binary Search Tree
	by Preorder sequence      */
struct treeNode* BuildT( ElementType data, struct treeNode* tree ) {
	// if tree is null, init it
	if ( !tree ) {
		tree = NewNode( data, tree);
		return tree;
	}
	// judge if input is valid
	if ( tree->val == abs(data) ) {
		tree->isValid = false;
	}
	// insert node in right position
	else if ( tree->val > abs(data) ) {
		tree->leftT = BuildT( data, tree->leftT );
	}
	else if ( tree->val < abs(data) ) {
		tree->rightT = BuildT( data, tree->rightT );
	}

	return tree;
}


struct treeNode* NewNode( ElementType data, struct treeNode* node ){
	// allocate new node space
	node = (struct treeNode *)malloc(sizeof(struct treeNode));
	// init new node and store data in
	node->leftT = NULL;
	node->rightT = NULL;
	node->val = abs(data);
	// judge if input is valid
	if ( !data ) // '0' is invalid
		node->isValid = false;
	else {
		// judge node's color
		node->isValid = true;
		if ( data < 0 ) 
			node->isRed = true;
		else
			node->isRed = false;	
	}

	return node;
}



/*---- Part 3: Judge Tree ----*/
/*	Judge if the tree is a RBT
	by certain steps          */
int RBTJudge( struct treeNode *tree ) {

	// 1st, null tree is RedBlack Tree
	if ( !tree )
		return 1; 

	// 2nd, it should be a Search Tree
	if ( !SearchT(tree) )
		return 0;

	// 3rd, root color should be Black
	if ( tree->isRed )
		return 0;

	// 4th, Recursively judge whether Numbers of Black Nodes are correct
	if ( !BlackNode(tree) )
		return 0;

	// 5th, Recursively judge whether Nodes' Colors are correct
	if ( !NodeColor(tree) )
		return 0;

	return 1;
}


// Judge Whether it's a Search Tree
int SearchT( struct treeNode* tree ) {
	struct treeNode* curT = tree; // point to Current Node
	if ( !curT )
		return 1;
	if ( !curT->isValid )
		return 0;
	// recursively traverse left child and right child
	return ( SearchT( curT->leftT) && SearchT(curT->rightT) );
}


// Judge whether Numbers of Black Nodes are Correct
int BlackNode( struct treeNode* tree ) {
	if ( !tree )
		return 1;
	// calculate Number of Black Nodes
	int leftNum, rightNum;
	leftNum = BlackNodeNum(tree->leftT);
	rightNum = BlackNodeNum(tree->rightT);
	// Compare
	if ( leftNum != rightNum ) 
		return 0;
	// Recurse
	return BlackNode(tree->leftT) && BlackNode(tree->rightT);
}


// Compute the Number of Black Nodes
int BlackNodeNum( struct treeNode* node ) {
	// Recursive export
	if ( !node ) 
		return 0;
	// Recurse to count Black Nodes
	int leftBlackNum, rightBlackNum;
	leftBlackNum = BlackNodeNum(node->leftT);
	rightBlackNum = BlackNodeNum(node->rightT);
	int blackNodeNum = MAX(leftBlackNum, rightBlackNum);
	// count current node
	if ( node->isRed )
		return blackNodeNum;
	return (1 + blackNodeNum);
}


// Detect if there're consecutive red nodes
int NodeColor( struct treeNode* node ) {
	// leaf(null) is black
	if ( !node )
		return 1;
	// if node ir red, then detect its' children
	if ( node->isRed ) {
		// if it has child and its child's node is red, invalid RBT
		if ( (node->leftT && node->leftT->isRed)
			 || (node->rightT && node->rightT->isRed)) 
			return 0;
	}
	// Recurse left and right child
	return (NodeColor(node->leftT) && NodeColor(node->rightT));
}
