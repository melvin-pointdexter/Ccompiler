#define _CRT_SECURE_NO_WARNINGS
#ifndef AST_TOOLS_FILE
#define AST_TOOLS_FILE

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define ADD_TAB 0
#define NODE_PROGRAM 1
#define NODE_FUNC 2
#define NODE_PROC 3
#define NODE_EXPR 4
#define NODE_CODEBLOCK 5
#define NODE_STATEMENT 6
#define NODE_FUNCSTATEMENT 7
#define NODE_PROCSTATEMENT 8
#define NODE_IF 9
#define NODE_IFELSE 10
#define NODE_WHILE 11
#define NODE_TERMINAL 12
#define NODE_FUNC_PROD_LIST 13
#define DONT_ADD_TAB 14
typedef struct node {
	char* token;
	int sons;
	struct node** nodes;
	int NODETYPE;
} node;

//#define YYSTYPE struct node *		//define the $ type

struct node* mknode(char* token, int sizeArr, int nodeType);
void printAst(struct node* tree, int tabs);

struct node* mknode(char* token, int sizeArr, int nodeType) {
	struct node* newNode = (node*)malloc(sizeof(node));
	newNode->token = (char*)malloc(sizeof(token) + 1);
	strcpy(newNode->token, token);
	newNode->nodes = (node**)malloc(sizeof(node*)*sizeArr);
	newNode->sons = sizeArr;
	newNode->NODETYPE = nodeType;
	return newNode;
}
void printAst(struct node* tree, int tabs) {
	printf("\n");
	//print current node with enough tabs
	for (int i = 0; i < tabs; i++)
		printf("\t");
	printf("(%s ", tree->token);

	//decide how many tabs there are in the other nodes
	int newTabs = tabs;
	switch (tree->NODETYPE) {
	case NODE_PROGRAM:
	case NODE_FUNC:
	case NODE_PROC:
	case NODE_CODEBLOCK:
	case ADD_TAB:
		newTabs += 1;
		break;
	default:
		break;
	}

	for (int i = 0; i < tree->sons; i++) {
		printAst(tree->nodes[i],newTabs);
		
	}
	if (tree->sons!=0){
		printf("\n");
		for (int i = 0; i < tabs; i++)
			printf("\t");
	}
	printf(")");
}

#endif